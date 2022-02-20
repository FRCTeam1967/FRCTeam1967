//climbing code with manual stop button options in state machine

package org.janksters.CommonClassesThisYear;
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.PneumaticsModuleType;

import javax.naming.ConfigurationException;

import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX; //potential issue since it's not the one on WPILib
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.PS4Controller.Button;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import org.janksters.jankyLib.*;

public class Climb2 extends JankyStateMachine{
    private WPI_TalonFX winchMotorL; //left
    private WPI_TalonFX winchMotorR; //right
    
    private Solenoid midBarLatchL;
    private Solenoid midBarLatchR;

    private Timer climbTimer;
    private Timer climbTestTimer;

    private jankyXboxJoystick XboxController;

    //constants
    private final double WINCH_MOTOR_SPEED = 0.3;
    private final double WINCH_MOTOR_STOP = 0.0;
    private final double WINCH_MOTOR_GENTLE = 0.1;
    private final double PULSES_PER_REVOLUTION = 2048; //2048 for falcon integrated, 4096 for MAG
    private final double LATCH_TIME = 1.0; //check value - in seconds
    private final double TESTING_TIME = 7.0;
    private final double WINCH_UP_DISTANCE = 28; //in inches
    private final int WINCH_FINAL_DISTANCE = 2; //in inches
    private final double WINCH_ENCODER_DIFFERENCE = 2000;
    private final double WINCH_ENCODER_DOWN = 1000;
    private final double HOOKED_ON_CURRENT = 1;
    private final double TOO_FAR_DOWN = 26;
    private final double POS_FACTOR = 10;
    private final double GEAR_RATIO = 34/66;  //motor to winch

    //pid
    private final int kTimeoutMs = 1;
    private final int kPIDLoopIdx = 0;

    private final double winchDownKP = 0.15;
    private final double winchDownKI = 0.0;
    private final double winchDownKD = 1.0;
    private final double winchDownKF = 0.0;

    private final double manualWinchKP = 0.15;
    private final double manualWinchKI = 0.0;
    private final double manualWinchKD = 1.0;
    private final double manualWinchKF = 0.0;

    //states
    private final int IDLE = 0; //keep bar down
    private final int LIFTER_DOWN = 1;
    private final int READY_TO_CLIMB = 2;
    private final int LATCH_MID_BAR = 3;
    private final int WINCH_UP = 4;
    private final int RELEASE = 5;
    private final int MANUAL_WINCH = 6;
    private final int UNLATCH_MID_BAR = 7; 
    private final int WINCH_FINAL = 8;
    private final int WINCH_DOWN = 9;
    private final int UNLATCH_MID_BAR_TO_EXIT = 10;
    private final int DO_NOTHING = 11;

    boolean climbYAxisUp;
    boolean climbYAxisWasUp = false;
    boolean climbYAxisDown;
    boolean climbYAxisWasDown = false;
    boolean xButtonPressed;
    boolean xButtonWasPressed = false;
    boolean yButtonPressed;
    boolean yButtonWasPressed = false;
    boolean aButtonPressed;
    boolean aButtonWasPressed = false;
    boolean bButtonPressed;
    boolean bButtonWasPressed = false;
    boolean startButtonPressed;
    boolean startButtonWasPressed = false;

    boolean runClimbStateMachine = false; 

    boolean testing = true;
    double targetPositionRotations = 0; 
    
    public Climb2(int winchMotorChannelL, int winchMotorChannelR,
    int PCMChannel, int midLatchChannelL, int midLatchChannelR){
        winchMotorL = new WPI_TalonFX(winchMotorChannelL);
        winchMotorR = new WPI_TalonFX(winchMotorChannelR);

        midBarLatchL = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, midLatchChannelL);
        midBarLatchR = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, midLatchChannelR);
        climbTimer = new Timer();
        climbTestTimer = new Timer();

        XboxController= new jankyXboxJoystick(2);

        setUpWinchEncoder();

        SetMachineName("JankyStateMachineClimb");
        SetName(IDLE,"Idle");
        SetName(LIFTER_DOWN, "LifterDown");
        SetName(READY_TO_CLIMB, "ReadyToClimb");
        SetName(LATCH_MID_BAR,"LatchMidBar");
        SetName(WINCH_UP,"WinchUp");
        SetName(RELEASE,"Release");
        SetName(MANUAL_WINCH, "ManualWinch");
        SetName(UNLATCH_MID_BAR,"UnlatchMidBar");
        SetName(WINCH_FINAL,"WinchFinal");
        SetName(WINCH_DOWN,"WinchDown");
        SetName(UNLATCH_MID_BAR_TO_EXIT, "NewUnlatchMidBar");
        SetName(DO_NOTHING,"DoNothing");
        
        start();
    } 

    private void raiseWinchString(){
        winchMotorL.set(WINCH_MOTOR_SPEED);
        winchMotorR.set(WINCH_MOTOR_SPEED);
    }

    private void lowerWinchString(){
        winchMotorL.set(-1.0 * WINCH_MOTOR_SPEED);
        winchMotorR.set(-1.0 * WINCH_MOTOR_SPEED);
    }

    private void stopWinchString(){
        winchMotorL.set(WINCH_MOTOR_STOP);
        winchMotorR.set(WINCH_MOTOR_STOP);
    }

    private void releaseWinchString(){
        winchMotorL.set(WINCH_MOTOR_GENTLE);
        winchMotorR.set(WINCH_MOTOR_GENTLE);
    }
    /*
    public void raiseWinchStringManual(){
        if(GetCurrentState() == IDLE){
            raiseWinchString();
        }
    }

    public void lowerWinchStringManual(){
        if(GetCurrentState() == IDLE){
            lowerWinchString();
        }
    }

    public void stopWinchStringManual(){
        if(GetCurrentState() == IDLE){
            stopWinchString();
        }
    }
    */
    private void latchMidBar(){
        midBarLatchL.set(true);
        midBarLatchR.set(true);
    }

    private void unlatchMidBar(){
        midBarLatchL.set(false);
        midBarLatchR.set(false);
    }

    private void setUpWinchEncoder(){
        /* left setup */
        winchMotorL.configFactoryDefault();
        winchMotorL.configSelectedFeedbackSensor(FeedbackDevice.IntegratedSensor, kPIDLoopIdx, kTimeoutMs);
        winchMotorL.setSensorPhase(true);
        winchMotorL.setInverted(false);

        /* right setup */
        winchMotorR.configFactoryDefault();
        winchMotorR.configSelectedFeedbackSensor(FeedbackDevice.IntegratedSensor, kPIDLoopIdx, kTimeoutMs);
        winchMotorR.setSensorPhase(true);
        winchMotorR.setInverted(false); //to check

        /* left min/max */
        winchMotorL.configNominalOutputForward(0, kTimeoutMs);
        winchMotorL.configNominalOutputReverse(0, kTimeoutMs);
        winchMotorL.configPeakOutputForward(1, kTimeoutMs);
		winchMotorL.configPeakOutputReverse(-1, kTimeoutMs);
        
        /* right min/max */
        winchMotorR.configNominalOutputForward(0, kTimeoutMs);
        winchMotorR.configNominalOutputReverse(0, kTimeoutMs);
        winchMotorR.configPeakOutputForward(1, kTimeoutMs);
		winchMotorR.configPeakOutputReverse(-1, kTimeoutMs);
        
        /* allowable error */
        winchMotorL.configAllowableClosedloopError(0, kPIDLoopIdx, kTimeoutMs);
        winchMotorR.configAllowableClosedloopError(0, kPIDLoopIdx, kTimeoutMs);

        /* left PID config */
        winchMotorL.config_kF(kPIDLoopIdx, winchDownKF, kTimeoutMs);
		winchMotorL.config_kP(kPIDLoopIdx, winchDownKP, kTimeoutMs);
		winchMotorL.config_kI(kPIDLoopIdx, winchDownKI, kTimeoutMs);
		winchMotorL.config_kD(kPIDLoopIdx, winchDownKD, kTimeoutMs);

        /* right PID config */
        winchMotorR.config_kF(kPIDLoopIdx, winchDownKF, kTimeoutMs);
		winchMotorR.config_kP(kPIDLoopIdx, winchDownKP, kTimeoutMs);
		winchMotorR.config_kI(kPIDLoopIdx, winchDownKI, kTimeoutMs);
		winchMotorR.config_kD(kPIDLoopIdx, winchDownKD, kTimeoutMs);
    }

    private void configurePIDForManualClimb(){
        winchMotorL.config_kF(kPIDLoopIdx, manualWinchKF, kTimeoutMs);
		winchMotorL.config_kP(kPIDLoopIdx, manualWinchKD, kTimeoutMs);
		winchMotorL.config_kI(kPIDLoopIdx, manualWinchKI, kTimeoutMs);
		winchMotorL.config_kD(kPIDLoopIdx, manualWinchKD, kTimeoutMs);

        winchMotorR.config_kF(kPIDLoopIdx, manualWinchKF, kTimeoutMs);
		winchMotorR.config_kP(kPIDLoopIdx, manualWinchKP, kTimeoutMs);
		winchMotorR.config_kI(kPIDLoopIdx, manualWinchKI, kTimeoutMs);
		winchMotorR.config_kD(kPIDLoopIdx, manualWinchKD, kTimeoutMs);
    }

    private double getWinchEncoderCountL(){
        return winchMotorL.getSensorCollection().getIntegratedSensorPosition();
    }

    private double getWinchEncoderCountR(){
        return winchMotorR.getSensorCollection().getIntegratedSensorPosition();
    }

    private void resetWinchEncoders(){
        winchMotorL.getSensorCollection().setIntegratedSensorPosition(0, 10);
        winchMotorR.getSensorCollection().setIntegratedSensorPosition(0, 10);
    }

    private double getAvgWinchEncoderCount(){
        //condition for when one is wrong
        if (Math.abs(getWinchEncoderCountL() - getWinchEncoderCountR()) > WINCH_ENCODER_DIFFERENCE) { //change value later
            SmartDashboard.putBoolean("climb winch encoders working?", false);
        } else {
            SmartDashboard.putBoolean("climb winch encoders working?", true);
        }

        return (getWinchEncoderCountL() + getWinchEncoderCountR()) / 2;
    }

    private double getAvgWinchEncoderDistance(){ //in inches
        return getAvgWinchEncoderCount() * GEAR_RATIO / (PULSES_PER_REVOLUTION); //to do: figure out distance per revolution
    }

    private double winchDistanceToEncoderCount(double distance){ //in inches
        return distance / GEAR_RATIO * (PULSES_PER_REVOLUTION); //to do: figure out distance per revolution
    }

    public void startClimbStateMachine() {
        runClimbStateMachine = true;
    }

    public void stopClimbStateMachine(){
        runClimbStateMachine = false;
    }

    public void resetForClimb(){
        unlatchMidBar();
        stopWinchString();
    }

    private void keepArmDown(){
        targetPositionRotations = 0;
        winchMotorL.set(TalonFXControlMode.Position, targetPositionRotations);
        winchMotorR.set(TalonFXControlMode.Position, targetPositionRotations);
    }

    private double getWinchStatorCurrent(){
        double leftMotorCurrent = winchMotorL.getStatorCurrent();
        double rightMotorCurrent = winchMotorR.getStatorCurrent();
        return (leftMotorCurrent+rightMotorCurrent)/2; 
    }

    public void StateEngine(int curState, boolean onStateEntered){
        SmartDashboard.putNumber("climb state", GetCurrentState());
        SmartDashboard.putNumber("climb timer", climbTimer.get());
        SmartDashboard.putNumber("climb test timer", climbTestTimer.get());
        SmartDashboard.putNumber("climb encoder position", getAvgWinchEncoderDistance());
        
        if(XboxController.GetButtonB()) { //stop button pressed
            runClimbStateMachine=false;
        }
        switch (curState){
            case IDLE:
                keepArmDown();
                if (XboxController.GetRightStickButton()){
                    NewState(LIFTER_DOWN, "right stick button was pressed");
                }
                break;

            case LIFTER_DOWN:
                if(onStateEntered){
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                //bring intake shooter down
                //if(intake is down && !testing){
                //NewState(READY_TO_CLIMB, "lifter is down");
                //}
                if(testing && climbTestTimer.get()>=TESTING_TIME){
                    NewState(READY_TO_CLIMB, "lifter is down");
                }
                if (XboxController.GetButtonB()){
                    NewState(IDLE, "stop button pressed");
                }
                break;
                
            case READY_TO_CLIMB:
                if (onStateEntered){
                    resetForClimb();
                }
                /*** joystick up ***/
                climbYAxisUp = XboxController.GetRightYAxis() > 0.2;
                if (climbYAxisUp && !climbYAxisWasUp) { 
                    //raise winch string
                    raiseWinchString();
                    climbYAxisWasUp = true;
                } else if (!climbYAxisUp && climbYAxisWasUp) {
                    stopWinchString();
                    climbYAxisWasUp = false;
                }
                
                /*** joystick down ***/
                climbYAxisDown = XboxController.GetRightYAxis() < -0.2;
                if (climbYAxisDown && !climbYAxisWasDown) { 
                    //lower winch string
                    lowerWinchString();
                    climbYAxisWasDown = true;
                } else if (!climbYAxisDown && climbYAxisWasDown) {
                    stopWinchString();
                    climbYAxisWasDown = false;
                }  

                 /*** mid latch ***/
                 xButtonPressed = XboxController.GetButtonX();
                 if (xButtonPressed && !xButtonWasPressed) {
                     latchMidBar();
                     xButtonWasPressed = true;
                 } else if (!xButtonPressed && xButtonWasPressed) {
                     xButtonWasPressed = false;
                 }
 
                 /*** mid unlatch ***/
                 yButtonPressed = XboxController.GetButtonY();
                 if (yButtonPressed && !yButtonWasPressed) {
                     latchMidBar();
                     yButtonWasPressed = true;
                 } else if (!yButtonPressed && yButtonWasPressed) {
                     yButtonWasPressed = false;
                 }

                if (XboxController.GetButtonA()){
                    runClimbStateMachine=true; 
                }

                if (XboxController.GetButtonB()){
                    NewState(IDLE, "stop button pressed in ready to climb");
                }
                
                if (runClimbStateMachine){
                    NewState(LATCH_MID_BAR,"x button pressed to start sequence");
                }

                break;

            case LATCH_MID_BAR:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                latchMidBar();
                if (!runClimbStateMachine) {  //stop button pressed
                    NewState(UNLATCH_MID_BAR_TO_EXIT,"no longer going to winch up");
                }
                if (climbTimer.get()>=LATCH_TIME){
                    climbTimer.stop();
                    NewState(WINCH_UP,"mid bar latch timer finished");
                }
                break;

            case UNLATCH_MID_BAR_TO_EXIT:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                unlatchMidBar();
                if (climbTimer.get()>=LATCH_TIME){
                    climbTimer.stop();
                    NewState(READY_TO_CLIMB,"done with unlatching after stop button");
                }
                break;

            case WINCH_UP:
                if (onStateEntered){
                    resetWinchEncoders(); //change to PID and current instead of encoders?
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                raiseWinchString();                
                if (!runClimbStateMachine) { //stop button pressed
                    stopWinchString();
                    NewState(WINCH_DOWN,"no longer going to winch up");
                }
                if (getAvgWinchEncoderDistance()>=WINCH_UP_DISTANCE&&!testing){
                    stopWinchString();
                    NewState(RELEASE,"reached full winch distance");
                } else if(testing && climbTestTimer.get()>=TESTING_TIME){
                    stopWinchString();
                    NewState(RELEASE,"reached full winch distance");
                }
                if(XboxController.GetButtonX()){
                    //use PID to hold it at this position
                    targetPositionRotations = getAvgWinchEncoderCount();
                    winchMotorL.set(TalonFXControlMode.Position, targetPositionRotations);
                    winchMotorR.set(TalonFXControlMode.Position, targetPositionRotations);

                    NewState(MANUAL_WINCH,"decided to move to manual");
                }

                if(XboxController.GetButtonX()){

                }
    
                break;

            case WINCH_DOWN:
                if(onStateEntered){
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                lowerWinchString();
                if (getAvgWinchEncoderDistance() == WINCH_ENCODER_DOWN && !testing){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR_TO_EXIT,"reached full winch down distance");
                } else if(testing && climbTestTimer.get()>=TESTING_TIME){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR_TO_EXIT,"reached full winch down distance");
                }

                break;

            case RELEASE:
                if(onStateEntered){
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                releaseWinchString();
                if (!runClimbStateMachine) {
                    stopWinchString();
                    NewState(DO_NOTHING,"stopping sequence");
                }
                if (getWinchStatorCurrent() < HOOKED_ON_CURRENT && !testing){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR," current is < hooked on current");
                } else if(testing && climbTestTimer.get()>=TESTING_TIME){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR," current is < hooked on current");
                }
                if (getAvgWinchEncoderDistance()< TOO_FAR_DOWN && !testing){
                    stopWinchString();
                    NewState(WINCH_UP," distance is too far down");
                } else if(testing && XboxController.GetButtonStart()){ //just testing
                    stopWinchString();
                    NewState(WINCH_UP," pressed button start in testing");
                }
                if(XboxController.GetButtonX()){
                    //keep at speed
                    NewState(MANUAL_WINCH,"decided to move to manual");
                }
                break;

            case MANUAL_WINCH:
                if (onStateEntered){
                    configurePIDForManualClimb();
                    targetPositionRotations = getAvgWinchEncoderCount();
                }
                winchMotorL.set(TalonFXControlMode.Position, targetPositionRotations);
                winchMotorR.set(TalonFXControlMode.Position, targetPositionRotations);

                if(XboxController.GetRightYAxis()>= 0.2){
                    double targetPositionChangeInches = XboxController.GetRightYAxis() * POS_FACTOR;
                    targetPositionRotations += winchDistanceToEncoderCount(targetPositionChangeInches);
                    //targetPositionRotations +=  * POS_FACTOR * PULSES_PER_REVOLUTION;
                }
                if(XboxController.GetRightYAxis()<= -0.2){
                    double targetPositionChangeInches = XboxController.GetRightYAxis() * POS_FACTOR; //inches?
                    targetPositionRotations += winchDistanceToEncoderCount(targetPositionChangeInches);
                    //targetPositionRotations += XboxController.GetRightYAxis() * POS_FACTOR * PULSES_PER_REVOLUTION;
                }
                //manual cotrol with PID
                if(XboxController.GetButtonY()){
                    NewState(UNLATCH_MID_BAR, "done with manual winch");
                }

                if(!runClimbStateMachine){
                    NewState(WINCH_DOWN, "stopping climb state machine");
                }
                break;

            case UNLATCH_MID_BAR:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                unlatchMidBar();
                if (!runClimbStateMachine) {
                    NewState(DO_NOTHING,"stopping sequence");
                }
                if (climbTimer.get()>=LATCH_TIME){
                    climbTimer.stop();
                    NewState(WINCH_FINAL,"mid bar unlatch timer finished");
                }
                break;

            case WINCH_FINAL:
                if (onStateEntered){
                    resetWinchEncoders(); //change to current instead of encoders?
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                raiseWinchString();
                if (!runClimbStateMachine) {
                    stopWinchString();
                    NewState(DO_NOTHING,"stopping sequence");
                }
                if (getAvgWinchEncoderDistance()>=WINCH_FINAL_DISTANCE && !testing){
                    stopWinchString();
                    NewState(DO_NOTHING,"slightly pulling up winch finished");
                } else if (testing && climbTestTimer.get()>=TESTING_TIME){
                    stopWinchString();
                    NewState(DO_NOTHING,"slightly pulling up winch finished");
                }
                break;

            case DO_NOTHING:
                break;

        }
    }
    
}
