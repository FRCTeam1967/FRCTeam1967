//climbing code with manual stop button options in state machine

package org.janksters.CommonClassesThisYear;
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.PneumaticsModuleType;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX; //potential issue since it's not the one on WPILib
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.PS4Controller.Button;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import org.janksters.jankyLib.*;

public class Climb2 extends JankyStateMachine{
    private WPI_TalonFX winchMotorL; //left
    private WPI_TalonFX winchMotorR; //right
    
    private Solenoid midBarLatchL;
    private Solenoid midBarLatchR;
    private Solenoid highBarLatchL;
    private Solenoid highBarLatchR;

    private Solenoid pivotHookSwingL;
    private Solenoid pivotHookSwingR;

    private Timer climbTimer;

    private jankyXboxJoystick XboxController;

    //constants
    private final double WINCH_MOTOR_SPEED = 0.3;
    private final double WINCH_MOTOR_STOP = 0.0;
    private final double WINCH_MOTOR_GENTLE = 0.1;
    private final double PULSES_PER_REVOLUTION = 2048; //2048 for falcon integrated, 4096 for MAG
    private final double LATCH_TIME = 7.0; //check value - in seconds
    private final double HOOK_TIME = 1.0; //check value - in seconds
    private final double WINCH_UP_DISTANCE = 28; //in inches
    private final int WINCH_FINAL_DISTANCE = 2; //in inches
    private final int kTimeoutMs = 1;
    private final double WINCH_ENCODER_DIFFERENCE = 2000;
    private final double WINCH_ENCODER_DOWN = 1000;
    private final double HOOKED_ON_CURRENT = 1;
    private final double TOO_FAR_DOWN = 26;

    //states
    private final int IDLE = 0; //keep bar down
    private final int READY_TO_CLIMB = 1;
    private final int LATCH_MID_BAR = 2;
    private final int WINCH_UP = 3;
    private final int RELEASE = 4;
    private final int UNLATCH_MID_BAR = 5; 
    private final int WINCH_FINAL = 6;
    private final int WINCH_DOWN = 7;
    private final int UNLATCH_MID_BAR_TO_EXIT = 8;
    private final int DO_NOTHING = 9;

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

    boolean runClimbStateMachine = false; //note: this variable is useful if xbox is in robot.java, 
                                          //but not as useful when controls is inside state machine

    boolean testing = true;
    
    public Climb2(int winchMotorChannelL, int winchMotorChannelR,
    int PCMChannel, int midLatchChannelL, int midLatchChannelR,
    int highLatchChannelL, int highLatchChannelR,
    int pivotHookChannelL, int pivotHookChannelR){
        winchMotorL = new WPI_TalonFX(winchMotorChannelL);
        winchMotorR = new WPI_TalonFX(winchMotorChannelR);

        midBarLatchL = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, midLatchChannelL);
        midBarLatchR = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, midLatchChannelR);
        highBarLatchL = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, highLatchChannelL);
        highBarLatchR = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, highLatchChannelR);

        pivotHookSwingL = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, pivotHookChannelL);
        pivotHookSwingR = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, pivotHookChannelR);
        climbTimer = new Timer();

        XboxController= new jankyXboxJoystick(2);

        setUpWinchEncoder();

        SetMachineName("JankyStateMachineClimb");
        SetName(IDLE,"Idle");
        SetName(READY_TO_CLIMB, "ReadyToClimb");
        SetName(LATCH_MID_BAR,"LatchMidBar");
        SetName(WINCH_UP,"WinchUp");
        SetName(RELEASE,"Release");
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

    private void latchHighBar(){
        highBarLatchL.set(true);
        highBarLatchR.set(true);
    }

    private void unlatchHighBar(){
        highBarLatchL.set(false);
        highBarLatchR.set(false);
    }

    private void hookPivotDown(){
        pivotHookSwingL.set(true);
        pivotHookSwingR.set(true);
    }

    private void hookPivotUp(){
        pivotHookSwingL.set(false);
        pivotHookSwingR.set(false);
    }

    private void setUpWinchEncoder(){
        winchMotorL.configFactoryDefault();
        winchMotorL.configSelectedFeedbackSensor(FeedbackDevice.IntegratedSensor, 0, kTimeoutMs);
        winchMotorL.setSensorPhase(true);

        winchMotorR.configFactoryDefault();
        winchMotorR.configSelectedFeedbackSensor(FeedbackDevice.IntegratedSensor, 0, kTimeoutMs);
        winchMotorR.setSensorPhase(true);
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
        return getAvgWinchEncoderCount() * (PULSES_PER_REVOLUTION); //to do: figure out distance per revolution, gear ratio
    }

    public void startClimbStateMachine() {
        runClimbStateMachine = true;
    }

    public void stopClimbStateMachine(){
        runClimbStateMachine = false;
    }

    public void resetForClimb(){
        hookPivotDown();
        unlatchMidBar();
        unlatchHighBar();
        stopWinchString();
    }

    private void keepArmDown(){
        //to do
    }

    private double getWinchStatorCurrent(){
        double leftMotorCurrent = winchMotorL.getStatorCurrent();
        double rightMotorCurrent = winchMotorR.getStatorCurrent();
        return (leftMotorCurrent+rightMotorCurrent)/2; 
    }

    public void StateEngine(int curState, boolean onStateEntered){
        SmartDashboard.putNumber("climb state", GetCurrentState());
        SmartDashboard.putNumber("climb timer", climbTimer.get());
        if(XboxController.GetButtonB()) { //stop button pressed
            runClimbStateMachine=false;
        }
        switch (curState){
            case IDLE:
                keepArmDown();
                if (XboxController.GetRightStickButton()){
                    NewState(READY_TO_CLIMB, "right stick button was pressed");
                }
                break;

            case READY_TO_CLIMB:
                if (onStateEntered){
                    resetForClimb();
                }
                //--- JOYSTICK UP ---//
                climbYAxisUp = XboxController.GetRightYAxis() > 0.2;
                if (climbYAxisUp && !climbYAxisWasUp) { 
                    //raise winch string
                    raiseWinchString();
                    climbYAxisWasUp = true;
                } else if (!climbYAxisUp && climbYAxisWasUp) {
                    stopWinchString();
                    climbYAxisWasUp = false;
                }
                
                //--- JOYSTICK DOWN ---// 
                climbYAxisDown = XboxController.GetRightYAxis() < -0.2;
                if (climbYAxisDown && !climbYAxisWasDown) { 
                    //lower winch string
                    lowerWinchString();
                    climbYAxisWasDown = true;
                } else if (!climbYAxisDown && climbYAxisWasDown) {
                    stopWinchString();
                    climbYAxisWasDown = false;
                }  

                 //--- MID LATCH ---//
                 xButtonPressed = XboxController.GetButtonX();
                 if (xButtonPressed && !xButtonWasPressed) {
                     latchMidBar();
                     xButtonWasPressed = true;
                 } else if (!xButtonPressed && xButtonWasPressed) {
                     xButtonWasPressed = false;
                 }
 
                 //--- MID UNLATCH ---//
                 bButtonPressed = XboxController.GetButtonB();
                 if (bButtonPressed && !bButtonWasPressed) {
                     latchMidBar();
                     bButtonWasPressed = true;
                 } else if (!bButtonPressed && bButtonWasPressed) {
                     bButtonWasPressed = false;
                 }

                if (XboxController.GetButtonY()){
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
                    climbTimer.start();
                    climbTimer.reset();
                }
                raiseWinchString();

                //stop button pressed
                if (!runClimbStateMachine) {
                    stopWinchString();
                    NewState(WINCH_DOWN,"no longer going to winch up");
                }
                else if (getAvgWinchEncoderDistance()>=WINCH_UP_DISTANCE&&!testing){
                    stopWinchString();
                    NewState(RELEASE,"reached full winch distance");
                }
                else if(testing && climbTimer.get()>=LATCH_TIME){
                    stopWinchString();
                    NewState(RELEASE,"reached full winch distance");
                }
                
                break;

            case WINCH_DOWN:
                if(onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                lowerWinchString();
                if (getAvgWinchEncoderDistance() == WINCH_ENCODER_DOWN && !testing){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR_TO_EXIT,"reached full winch down distance");
                }
                if(testing && climbTimer.get()>=LATCH_TIME){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR_TO_EXIT,"reached full winch down distance");
                }
                break;

            case RELEASE:
                if(onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                releaseWinchString();
                if (!runClimbStateMachine) {
                    stopWinchString();
                    NewState(DO_NOTHING,"stopping sequence");
                }
                if (getWinchStatorCurrent() < HOOKED_ON_CURRENT && !testing){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR," current is < hooked on current");
                }
                if(testing && climbTimer.get()>=LATCH_TIME){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR," current is < hooked on current");
                }
                //if (getAvgWinchEncoderDistance()< TOO_FAR_DOWN){
                if(XboxController.GetButtonStart()){
                    stopWinchString();
                    NewState(WINCH_UP," distance is too far down");
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
                    resetWinchEncoders(); //change to PID and current instead of encoders?
                    climbTimer.reset(); //to test state machine
                    climbTimer.start();
                }
                raiseWinchString();
                if (!runClimbStateMachine) {
                    stopWinchString();
                    NewState(DO_NOTHING,"stopping sequence");
                }
                //if (getAvgWinchEncoderDistance()>=WINCH_FINAL_DISTANCE){
                if (XboxController.GetButtonStart()){
                    climbTimer.stop();
                    stopWinchString();
                    NewState(DO_NOTHING,"slightly pulling up winch finished");
                }
                break;

            case DO_NOTHING:
                break;

        }
    }
    
}
