//climbing code with manual stop button options in state machine

package org.janksters.CommonClassesThisYear;
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.PneumaticsModuleType;

import com.ctre.phoenix.ErrorCode;
//import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX; //potential issue since it's not the one on WPILib
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import org.janksters.jankyLib.*;

public class Climb extends JankyStateMachine{
    private JankyTalonFX winchMotorL; //left
    private JankyTalonFX winchMotorR; //right
    
    private Solenoid midBarLatchL;
    private Solenoid midBarLatchR;

    private Timer climbTimer;
    private Timer climbTestTimer;

    private jankyXboxJoystick XboxController;
    private Pivot pivot;

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

    boolean climbYAxisUp, climbYAxisDown, xButtonPressed, yButtonPressed, aButtonPressed, bButtonPressed, startButtonPressed;
    boolean climbYAxisWasUp=false, climbYAxisWasDown=false, xButtonWasPressed=false, yButtonWasPressed=false, 
    aButtonWasPressed=false, bButtonWasPressed=false, startButtonWasPressed=false;

    boolean runClimbStateMachine = false; 
    boolean testing = true;
    double targetPositionRotations = 0; 

    double previousCountL;
    double previousCountR;
    
    public Climb(int winchMotorChannelL, int winchMotorChannelR,
    int PCMChannel, int midLatchChannelL, int midLatchChannelR, Pivot _pivot){
        winchMotorL = new JankyTalonFX(winchMotorChannelL);
        winchMotorR = new JankyTalonFX(winchMotorChannelR);

        midBarLatchL = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, midLatchChannelL);
        midBarLatchR = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, midLatchChannelR);
        climbTimer = new Timer();
        climbTestTimer = new Timer();

        XboxController= new jankyXboxJoystick(2);

        pivot = _pivot;

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

    private void raiseWinchString(double winchSpeed){
        winchMotorL.set(winchSpeed * Constants.WINCH_MOTOR_UP_FACTOR);
        winchMotorR.set(winchSpeed * Constants.WINCH_MOTOR_UP_FACTOR);
    }

    private void lowerWinchString(double winchSpeed){ //pass in joystick value
        winchMotorL.set(winchSpeed * Constants.WINCH_MOTOR_DOWN_FACTOR);
        winchMotorR.set(winchSpeed * Constants.WINCH_MOTOR_DOWN_FACTOR);
    }

    private void stopWinchString(){ //make sure this is when you actually want to stop, not hold
        //winchMotorL.set(Constants.WINCH_MOTOR_STOP);
        //winchMotorR.set(Constants.WINCH_MOTOR_STOP);
        targetPositionRotations = getAvgWinchEncoderCount();
        winchMotorL.set(TalonFXControlMode.Position, targetPositionRotations);
        winchMotorR.set(TalonFXControlMode.Position, targetPositionRotations);
    }

    private void releaseWinchString(){
        winchMotorL.set(Constants.WINCH_MOTOR_GENTLE);
        winchMotorR.set(Constants.WINCH_MOTOR_GENTLE);
    }

    /* public void raiseWinchStringManual(){
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
    } */
    
    private void latchMidBar(){
        midBarLatchL.set(true);
        midBarLatchR.set(true);
    }

    private void unlatchMidBar(){
        midBarLatchL.set(false);
        midBarLatchR.set(false);
    }

    private void setUpWinchEncoder(){
        winchMotorL.setUpJankyTalonFX();
        winchMotorR.setUpJankyTalonFX();

        /* //left setup
        winchMotorL.configFactoryDefault();
        winchMotorL.configSelectedFeedbackSensor(FeedbackDevice.IntegratedSensor, Constants.kPIDLoopIdx, Constants.kTimeoutMs);
        winchMotorL.setSensorPhase(true);
        winchMotorL.setInverted(false);

        //right setup
        winchMotorR.configFactoryDefault();
        winchMotorR.configSelectedFeedbackSensor(FeedbackDevice.IntegratedSensor, Constants.kPIDLoopIdx, Constants.kTimeoutMs);
        winchMotorR.setSensorPhase(true);
        winchMotorR.setInverted(false); //to check

        //left min/max
        winchMotorL.configNominalOutputForward(0, Constants.kTimeoutMs);
        winchMotorL.configNominalOutputReverse(0, Constants.kTimeoutMs);
        winchMotorL.configPeakOutputForward(1, Constants.kTimeoutMs);
		winchMotorL.configPeakOutputReverse(-1, Constants.kTimeoutMs);
        
        //right min/max
        winchMotorR.configNominalOutputForward(0, Constants.kTimeoutMs);
        winchMotorR.configNominalOutputReverse(0, Constants.kTimeoutMs);
        winchMotorR.configPeakOutputForward(1, Constants.kTimeoutMs);
		winchMotorR.configPeakOutputReverse(-1, Constants.kTimeoutMs);
        */

        /* allowable error */
        winchMotorL.configAllowableClosedloopError(0, Constants.kPIDLoopIdx, Constants.kTimeoutMs);
        winchMotorR.configAllowableClosedloopError(0, Constants.kPIDLoopIdx, Constants.kTimeoutMs);

        /* left PID config */
        winchMotorL.config_kF(Constants.kPIDLoopIdx, Constants.winchDownKF, Constants.kTimeoutMs);
		winchMotorL.config_kP(Constants.kPIDLoopIdx, Constants.winchDownKP, Constants.kTimeoutMs);
		winchMotorL.config_kI(Constants.kPIDLoopIdx, Constants.winchDownKI, Constants.kTimeoutMs);
		winchMotorL.config_kD(Constants.kPIDLoopIdx, Constants.winchDownKD, Constants.kTimeoutMs);

        /* right PID config */
        winchMotorR.config_kF(Constants.kPIDLoopIdx, Constants.winchDownKF, Constants.kTimeoutMs);
		winchMotorR.config_kP(Constants.kPIDLoopIdx, Constants.winchDownKP, Constants.kTimeoutMs);
		winchMotorR.config_kI(Constants.kPIDLoopIdx, Constants.winchDownKI, Constants.kTimeoutMs);
		winchMotorR.config_kD(Constants.kPIDLoopIdx, Constants.winchDownKD, Constants.kTimeoutMs);
    }

    private void configurePIDForManualClimb(){
        winchMotorL.config_kF(Constants.kPIDLoopIdx, Constants.manualWinchKF, Constants.kTimeoutMs);
		winchMotorL.config_kP(Constants.kPIDLoopIdx, Constants.manualWinchKP, Constants.kTimeoutMs);
		winchMotorL.config_kI(Constants.kPIDLoopIdx, Constants.manualWinchKI, Constants.kTimeoutMs);
		winchMotorL.config_kD(Constants.kPIDLoopIdx, Constants.manualWinchKD, Constants.kTimeoutMs);

        winchMotorR.config_kF(Constants.kPIDLoopIdx, Constants.manualWinchKF, Constants.kTimeoutMs);
		winchMotorR.config_kP(Constants.kPIDLoopIdx, Constants.manualWinchKP, Constants.kTimeoutMs);
		winchMotorR.config_kI(Constants.kPIDLoopIdx, Constants.manualWinchKI, Constants.kTimeoutMs);
		winchMotorR.config_kD(Constants.kPIDLoopIdx, Constants.manualWinchKD, Constants.kTimeoutMs);
    }

    private double getWinchEncoderCountL(){
        return winchMotorL.getSensorCollection().getIntegratedSensorPosition();
    }

    private double getWinchEncoderCountR(){
        return winchMotorR.getSensorCollection().getIntegratedSensorPosition();
    }

    private void resetWinchEncoders(){
        ErrorCode leftWinchEncoderError = winchMotorL.getSensorCollection().setIntegratedSensorPosition(0, 10);
        if(leftWinchEncoderError != ErrorCode.OK){
            SmartDashboard.putString("LWinch Encoder Error", leftWinchEncoderError.name());
        }
        ErrorCode rightWinchEncoderError = winchMotorR.getSensorCollection().setIntegratedSensorPosition(0, 10);
        if(rightWinchEncoderError != ErrorCode.OK){
            SmartDashboard.putString("RWinch Encoder Error", rightWinchEncoderError.name());
        }
    }

    private double getAvgWinchEncoderCount(){
        if (Math.abs(getWinchEncoderCountL() - getWinchEncoderCountR()) > Constants.WINCH_ENCODER_DIFFERENCE) {
            SmartDashboard.putBoolean("climb winch encoders working?", false);
            SmartDashboard.putNumber("left climb encoders", getWinchEncoderCountL());
            SmartDashboard.putNumber("right climb encoders", getWinchEncoderCountR());

            //if one of the encoders gets stuck, disregard it
            if (previousCountL == getWinchEncoderCountL()) {
                return getWinchEncoderCountR();
            } else if (previousCountR == getWinchEncoderCountR()) {
                return getWinchEncoderCountL();
            }
        } else {
            SmartDashboard.putBoolean("climb winch encoders working?", true);
        }
        previousCountL = getWinchEncoderCountL();
        previousCountR = getWinchEncoderCountR();
        return (getWinchEncoderCountL() + getWinchEncoderCountR()) / 2;
    }

    private double getAvgWinchEncoderDistance(){ //in inches
        if (testing) {
            return getAvgWinchEncoderCount() / (Constants.PULSES_PER_REVOLUTION);
        }
        return getAvgWinchEncoderCount() * Constants.GEAR_RATIO / (Constants.PULSES_PER_REVOLUTION);
    }

    private double winchDistanceToEncoderCount(double distance){ //in inches
        if (testing) {
            return distance * (Constants.PULSES_PER_REVOLUTION);
        }
        return distance / Constants.GEAR_RATIO * (Constants.PULSES_PER_REVOLUTION);
        
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
        //to do-remove unnecessary SmartDashBoard print
        SmartDashboard.putNumber("climb state", GetCurrentState());
        SmartDashboard.putNumber("climb timer", climbTimer.get());
        SmartDashboard.putNumber("climb test timer", climbTestTimer.get());
        SmartDashboard.putNumber("climb target position", targetPositionRotations);
        SmartDashboard.putNumber("climb encoder actual", getAvgWinchEncoderCount());
        SmartDashboard.putNumber("climb encoder position", getAvgWinchEncoderDistance());
        
        /* SmartDashboard.putNumber("winchDownKP", Constants.winchDownKP);
        SmartDashboard.putNumber("winchDownKI", Constants.winchDownKI);
        SmartDashboard.putNumber("winchDownKD", Constants.winchDownKD);
        SmartDashboard.putNumber("winchDownKF", Constants.winchDownKF);
        SmartDashboard.putNumber("error", (targetPositionRotations - getAvgWinchEncoderCount()));  */
        
        if(XboxController.GetButtonB()) { //stop button pressed
            runClimbStateMachine=false;
        }
        switch (curState){
            case IDLE:
                if(onStateEntered){
                    resetWinchEncoders();
                }
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
                pivot.flagClimbConfig();
                if(pivot.checkClimbConfigAchieved() && !testing){
                    NewState(READY_TO_CLIMB, "lifter is down");
                }
                if(testing && climbTestTimer.get()>=Constants.TESTING_TIME){
                    NewState(READY_TO_CLIMB, "testing - lifter timer is done");
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
                    raiseWinchString(XboxController.GetRightYAxis()); 
                    climbYAxisWasUp = true;
                } else if (!climbYAxisUp && climbYAxisWasUp) {
                    stopWinchString();
                    climbYAxisWasUp = false;
                }
                
                /*** joystick down ***/
                climbYAxisDown = XboxController.GetRightYAxis() < -0.2;
                if (climbYAxisDown && !climbYAxisWasDown) { 
                    lowerWinchString(XboxController.GetRightYAxis());
                    climbYAxisWasDown = true;
                } else if (!climbYAxisDown && climbYAxisWasDown) {
                    stopWinchString();
                    climbYAxisWasDown = false;
                }  

                /*** mid bar latch ***/
                xButtonPressed = XboxController.GetButtonX();
                if (xButtonPressed && !xButtonWasPressed) {
                    latchMidBar();
                    xButtonWasPressed = true;
                } else if (!xButtonPressed && xButtonWasPressed) {
                    xButtonWasPressed = false;
                }
 
                /*** mid bar unlatch ***/
                yButtonPressed = XboxController.GetButtonY();
                if (yButtonPressed && !yButtonWasPressed) {
                    unlatchMidBar();
                    yButtonWasPressed = true;
                } else if (!yButtonPressed && yButtonWasPressed) {
                    yButtonWasPressed = false;
                }
                
                /* start sequence */
                if (XboxController.GetButtonA()){
                    runClimbStateMachine=true; 
                    NewState(LATCH_MID_BAR,"x button pressed to start sequence");
                }

                if (XboxController.GetButtonB()){
                    NewState(IDLE, "stop button pressed in ready to climb");
                }
                break;

            case LATCH_MID_BAR:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                latchMidBar();
                if (!runClimbStateMachine) {
                    NewState(UNLATCH_MID_BAR_TO_EXIT,"no longer going to winch up");
                }
                if (climbTimer.get()>=Constants.LATCH_TIME){
                    climbTimer.stop();
                    resetWinchEncoders();
                    NewState(WINCH_UP,"mid bar latch timer finished");
                }
                break;

            case UNLATCH_MID_BAR_TO_EXIT:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                unlatchMidBar();
                if (climbTimer.get()>=Constants.LATCH_TIME){
                    climbTimer.stop();
                    NewState(READY_TO_CLIMB,"done with unlatching after stop button");
                }
                break;

            case WINCH_UP:
                if (onStateEntered){ 
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                raiseWinchString(Constants.ROBOT_UP_SPEED_SEQUENCE);                
                if (!runClimbStateMachine) {
                    stopWinchString();
                    NewState(WINCH_DOWN,"no longer going to winch up");
                }
                if (getAvgWinchEncoderDistance() >= Constants.WINCH_UP_DISTANCE && !testing){ 
                    stopWinchString();
                    NewState(RELEASE,"reached full winch distance");
                } else if(testing && climbTestTimer.get()>=1){ //used to be Constants.TESTING_TIME
                    stopWinchString();
                    NewState(RELEASE,"testing - climbTestTimer is done");
                }
                if(XboxController.GetButtonX()){
                    stopWinchString();
                    NewState(MANUAL_WINCH,"decided to move to manual");
                }
    
                break;

            case WINCH_DOWN:
                if(onStateEntered){
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                lowerWinchString(Constants.ROBOT_DOWN_SPEED_SEQUENCE);
                if (getAvgWinchEncoderDistance() <= Constants.WINCH_ENCODER_DOWN && !testing){ //unspool a bit more than necessary, or make manual
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR_TO_EXIT,"reached full winch down distance");
                } else if(testing && climbTestTimer.get()>=Constants.TESTING_TIME){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR_TO_EXIT,"testing - climbTestTimer is done");
                }
                break;

            case RELEASE:
                if(onStateEntered){
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                releaseWinchString();
                if (getWinchStatorCurrent() < Constants.HOOKED_ON_CURRENT && !testing){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR,"current is < hooked on current");
                } else if(testing && climbTestTimer.get()>=Constants.TESTING_TIME){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR,"testing = climbTestTimer is done");
                }
                if (getAvgWinchEncoderDistance() < Constants.TOO_FAR_DOWN && !testing){
                    stopWinchString();
                    NewState(WINCH_UP,"distance is too far down");
                } else if(testing && XboxController.GetButtonStart()){ //just testing
                    stopWinchString();
                    NewState(WINCH_UP,"pressed button start while testing");
                }
                if(XboxController.GetButtonX()){
                    NewState(MANUAL_WINCH,"decided to move to manual");
                }
                break;

            case MANUAL_WINCH:
                if (onStateEntered){
                    configurePIDForManualClimb();
                    targetPositionRotations = getAvgWinchEncoderCount();
                }

                if(XboxController.GetRightYAxis() >= 0.2 || XboxController.GetRightYAxis() <= -0.2){
                    targetPositionRotations += winchDistanceToEncoderCount(XboxController.GetRightYAxis() * Constants.POS_FACTOR);  //make POS_FACTOR editable from SmartDashboard
                }
                
                /** just for testing */
                if(XboxController.GetButtonStart() && testing){
                    targetPositionRotations = winchDistanceToEncoderCount(100);
                }

                winchMotorL.set(TalonFXControlMode.Position, targetPositionRotations);
                winchMotorR.set(TalonFXControlMode.Position, targetPositionRotations);

                //manual control with PID
                if(XboxController.GetButtonY()){
                    NewState(UNLATCH_MID_BAR, "done with manual winching");
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
                if (climbTimer.get()>=Constants.LATCH_TIME){
                    climbTimer.stop();
                    NewState(WINCH_FINAL,"mid bar unlatch timer finished");
                }
                break;

            case WINCH_FINAL:
                if (onStateEntered){
                    resetWinchEncoders();
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                raiseWinchString(Constants.ARM_UP_SPEED_SEQUENCE);
                if (!runClimbStateMachine) {
                    stopWinchString();
                    NewState(DO_NOTHING,"stopping sequence");
                }
                if (getAvgWinchEncoderDistance()>=Constants.WINCH_FINAL_DISTANCE && !testing){
                    stopWinchString();
                    NewState(DO_NOTHING,"slightly pulling up winch finished");
                } else if (testing && climbTestTimer.get()>=Constants.TESTING_TIME){
                    stopWinchString();
                    NewState(DO_NOTHING,"testing - climbTestTimer is done");
                }
                break;

            case DO_NOTHING:
                break;
        }
    }  
}