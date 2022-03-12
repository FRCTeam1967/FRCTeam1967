//climbing code with manual stop button options in state machine

package org.janksters.CommonClassesThisYear;
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.PneumaticsModuleType;

import com.ctre.phoenix.ErrorCode;
//import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX; //potential issue since it's not the one on WPILib
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import org.janksters.jankyLib.*;
import com.ctre.phoenix.motorcontrol.TalonFXInvertType;


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
    private final int RESET_ENCODER_FOR_WINCH_UP = 12;
    private final int RESET_ENCODER_FOR_WINCH_FINAL = 13;

    boolean climbYAxisUp, climbYAxisDown, xButtonPressed, yButtonPressed, aButtonPressed, bButtonPressed, startButtonPressed;
    boolean climbYAxisWasUp=false, climbYAxisWasDown=false, xButtonWasPressed=false, yButtonWasPressed=false, 
    aButtonWasPressed=false, bButtonWasPressed=false, startButtonWasPressed=false;

    boolean runClimbStateMachine = false; 
    boolean testing = false;
    double targetPositionRotations = 0; 

    double previousCountL;
    double previousCountR;
    
    public Climb(int winchMotorChannelL, int winchMotorChannelR,
    int PCMChannel, int midLatchChannelL, int midLatchChannelR, Pivot _pivot){
        winchMotorL = new JankyTalonFX(winchMotorChannelL);
        winchMotorR = new JankyTalonFX(winchMotorChannelR);
        
        //TO DO - ONLY GOING TO USE ONE SOLENOID SO WE NEED TO COMBINE THIS
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
        SetName(RESET_ENCODER_FOR_WINCH_UP, "ResetEncoderForWinchUp");
        SetName(RESET_ENCODER_FOR_WINCH_FINAL, "ResetEncoderForWinchFinal");
        
        
        start();
    } 

    private void raiseWinchString(double winchSpeed){
        winchMotorL.set(winchSpeed * Constants.CLIMB_WINCH_MOTOR_UP_FACTOR); //MAY NEED TO BE POSITIVE DEPENDING ON MECH
        winchMotorR.set(winchSpeed * Constants.CLIMB_WINCH_MOTOR_UP_FACTOR); //MAY NEED TO BE POSITIVE DEPENDING ON MECH
    }

    private void lowerWinchString(double winchSpeed){ //pass in joystick value
        winchMotorL.set(winchSpeed * Constants.CLIMB_WINCH_MOTOR_DOWN_FACTOR);
        winchMotorR.set(winchSpeed * Constants.CLIMB_WINCH_MOTOR_DOWN_FACTOR);
    }

    private void stopWinchString(){ //make sure this is when you want to hold, not actually stop
        //winchMotorL.set(Constants.WINCH_MOTOR_STOP);
        //winchMotorR.set(Constants.WINCH_MOTOR_STOP);
        targetPositionRotations = getAvgWinchEncoderCount();
        winchMotorL.set(TalonFXControlMode.Position, targetPositionRotations);
        winchMotorR.set(TalonFXControlMode.Position, targetPositionRotations);
    }

    private void releaseWinchString(){
        winchMotorL.set(Constants.CLIMB_WINCH_MOTOR_GENTLE);
        winchMotorR.set(Constants.CLIMB_WINCH_MOTOR_GENTLE);
    }
    
    private void latchMidBar(){
        midBarLatchL.set(true);
        midBarLatchR.set(true);
    }

    private void unlatchMidBar(){
        midBarLatchL.set(false);
        midBarLatchR.set(false);
    }

    private void setUpWinchEncoder(){
        //winchMotorL.setUpJankyTalonFX();
        //winchMotorR.setUpJankyTalonFX();
        winchMotorL.configFactoryDefault();
        winchMotorL.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorL.setSensorPhase(true);
        winchMotorL.setInverted(TalonFXInvertType.CounterClockwise);
        winchMotorL.configNominalOutputForward(0, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorL.configNominalOutputReverse(0, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorL.configPeakOutputForward(1, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.configPeakOutputReverse(-1, Constants.CLIMB_K_TIMEOUT_MS);

        winchMotorR.configFactoryDefault();
        winchMotorR.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorR.setSensorPhase(false);
        winchMotorR.setInverted(TalonFXInvertType.Clockwise);
        winchMotorR.configNominalOutputForward(0, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorR.configNominalOutputReverse(0, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorR.configPeakOutputForward(1, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.configPeakOutputReverse(-1, Constants.CLIMB_K_TIMEOUT_MS);

        /* allowable error */
        winchMotorL.configAllowableClosedloopError(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_ALLOWABLE_ERROR, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorR.configAllowableClosedloopError(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_ALLOWABLE_ERROR, Constants.CLIMB_K_TIMEOUT_MS);

        /* left PID config */
        winchMotorL.config_kF(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KF, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.config_kP(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KP, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.config_kI(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KI, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.config_kD(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KD, Constants.CLIMB_K_TIMEOUT_MS);

        /* right PID config */
        winchMotorR.config_kF(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KF, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.config_kP(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KP, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.config_kI(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KI, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.config_kD(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KD, Constants.CLIMB_K_TIMEOUT_MS);
    }

    private void configurePIDForManualClimb(){
        winchMotorL.config_kF(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_MANUAL_WINCH_KF, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.config_kP(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_MANUAL_WINCH_KP, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.config_kI(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_MANUAL_WINCH_KI, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.config_kD(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_MANUAL_WINCH_KD, Constants.CLIMB_K_TIMEOUT_MS);

        winchMotorR.config_kF(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_MANUAL_WINCH_KF, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.config_kP(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_MANUAL_WINCH_KP, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.config_kI(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_MANUAL_WINCH_KI, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.config_kD(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_MANUAL_WINCH_KD, Constants.CLIMB_K_TIMEOUT_MS);
    }

    private double getWinchEncoderCountL(){
        //return winchMotorL.getSensorCollection().getIntegratedSensorPosition();
        return winchMotorL.getSelectedSensorPosition();
    }

    private double getWinchEncoderCountR(){
        //return -1.0 * winchMotorR.getSensorCollection().getIntegratedSensorPosition();
        return winchMotorR.getSelectedSensorPosition();
    }

    private void resetWinchEncoders(){
        ErrorCode leftWinchEncoderError = winchMotorL.getSensorCollection().setIntegratedSensorPosition(0, 10);
        // if(leftWinchEncoderError != ErrorCode.OK){
        //     SmartDashboard.putString("LWinch Encoder Error", leftWinchEncoderError.name());
        // }
        SmartDashboard.putString("LWinch Encoder Error", leftWinchEncoderError.name());
        
        ErrorCode rightWinchEncoderError = winchMotorR.getSensorCollection().setIntegratedSensorPosition(0, 10);
        // if(rightWinchEncoderError != ErrorCode.OK){
        //     SmartDashboard.putString("RWinch Encoder Error", rightWinchEncoderError.name());
        // }
        SmartDashboard.putString("LWinch Encoder Error", rightWinchEncoderError.name());
    }

    private double getAvgWinchEncoderCount(){
        if (Math.abs(getWinchEncoderCountL() - getWinchEncoderCountR()) > Constants.CLIMB_WINCH_ENCODER_DIFFERENCE) {
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
            return getAvgWinchEncoderCount() / (Constants.FALCON_PULSES_PER_REVOLUTION);
        }
        return getAvgWinchEncoderCount() * Constants.CLIMB_GEAR_RATIO / (Constants.FALCON_PULSES_PER_REVOLUTION) * Constants.WINCH_CURCUMFERENCE;
    }

    private double winchDistanceToEncoderCount(double distance){ //in inches
        if (testing) {
            return distance * (Constants.FALCON_PULSES_PER_REVOLUTION);
        }
        return distance / Constants.CLIMB_GEAR_RATIO * (Constants.FALCON_PULSES_PER_REVOLUTION) / Constants.WINCH_CURCUMFERENCE;
        
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
        SmartDashboard.putNumber("stator current", getWinchStatorCurrent());
        
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
                //pivot.flagClimbConfig();
                //if(pivot.checkClimbConfigAchieved() && !testing){
                    NewState(READY_TO_CLIMB, "lifter is down");
                //}
                if(testing && climbTestTimer.get()>=Constants.CLIMB_TESTING_TIME){
                    NewState(READY_TO_CLIMB, "testing - lifter timer is done");
                }
                if (XboxController.GetButtonB()){
                    NewState(IDLE, "stop button pressed");
                }
                break;
                
            case READY_TO_CLIMB:
                if (onStateEntered){
                    targetPositionRotations = getAvgWinchEncoderCount();
                    resetForClimb();
                }
                
                // /*** joystick up ***/
                // climbYAxisUp = XboxController.GetRightYAxis() > 0.2;
                // if (climbYAxisUp && !climbYAxisWasUp) { 
                //     raiseWinchString(XboxController.GetRightYAxis()); 
                //     climbYAxisWasUp = true;
                // } else if (!climbYAxisUp && climbYAxisWasUp) {
                //     stopWinchString();
                //     climbYAxisWasUp = false;
                // }
                
                // /*** joystick down ***/
                // climbYAxisDown = XboxController.GetRightYAxis() < -0.2;
                // if (climbYAxisDown && !climbYAxisWasDown) { 
                //     lowerWinchString(XboxController.GetRightYAxis());
                //     climbYAxisWasDown = true;
                // } else if (!climbYAxisDown && climbYAxisWasDown) {
                //     stopWinchString();
                //     climbYAxisWasDown = false;
                // }  
                

                if(Math.abs(XboxController.GetRightYAxis()) >= 0.2){
                    targetPositionRotations += winchDistanceToEncoderCount(XboxController.GetRightYAxis() * Constants.CLIMB_POS_FACTOR);  //make POS_FACTOR editable from SmartDashboard
                }
                winchMotorL.set(TalonFXControlMode.Position, targetPositionRotations);
                winchMotorR.set(TalonFXControlMode.Position, targetPositionRotations);

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
                    NewState(LATCH_MID_BAR, "x button pressed to start sequence");
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
                if (climbTimer.get()>=Constants.CLIMB_LATCH_TIME){
                    climbTimer.stop();
                    NewState(RESET_ENCODER_FOR_WINCH_UP,"mid bar latch timer finished");
                }
                break;

            case UNLATCH_MID_BAR_TO_EXIT:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                }
                unlatchMidBar();
                if (climbTimer.get()>=Constants.CLIMB_LATCH_TIME){
                    climbTimer.stop();
                    NewState(READY_TO_CLIMB,"done with unlatching after stop button");
                }
                break;

            case WINCH_UP:
                if (onStateEntered){ 
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                raiseWinchString(Constants.CLIMB_ROBOT_UP_SPEED_SEQUENCE);                
                if (!runClimbStateMachine) {
                    stopWinchString();
                    NewState(WINCH_DOWN,"no longer going to winch up");
                }
                if (getAvgWinchEncoderDistance() <= Constants.CLIMB_WINCH_UP_DISTANCE && !testing){ 
                    stopWinchString();
                    NewState(RELEASE,"reached full winch distance");
                } else if(testing && climbTestTimer.get()>=Constants.CLIMB_TESTING_TIME){
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
                lowerWinchString(Constants.CLIMB_ROBOT_DOWN_SPEED_SEQUENCE);
                if (getAvgWinchEncoderDistance() <= Constants.CLIMB_WINCH_ENCODER_DOWN && !testing){ //unspool a bit more than necessary, or make manual
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR_TO_EXIT,"reached full winch down distance");
                } else if(testing && climbTestTimer.get()>=Constants.CLIMB_TESTING_TIME){
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
                if (getWinchStatorCurrent() < Constants.CLIMB_HOOKED_ON_CURRENT && !testing){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR,"current is < hooked on current");
                } else if(testing && climbTestTimer.get()>=Constants.CLIMB_TESTING_TIME){
                    stopWinchString();
                    NewState(UNLATCH_MID_BAR,"testing = climbTestTimer is done");
                }
                if (getAvgWinchEncoderDistance() > Constants.CLIMB_TOO_FAR_DOWN && !testing){
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
                    targetPositionRotations += winchDistanceToEncoderCount(XboxController.GetRightYAxis() * Constants.CLIMB_POS_FACTOR);  //make POS_FACTOR editable from SmartDashboard
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
                if (climbTimer.get()>=Constants.CLIMB_LATCH_TIME){
                    climbTimer.stop();
                    NewState(RESET_ENCODER_FOR_WINCH_FINAL,"mid bar unlatch timer finished");
                }
                break;

            case WINCH_FINAL:
                if (onStateEntered){
                    //inchEncoders();
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                raiseWinchString(Constants.CLIMB_ARM_UP_SPEED_SEQUENCE);
                if (!runClimbStateMachine) {
                    stopWinchString();
                    NewState(DO_NOTHING,"stopping sequence");
                }
                if (getAvgWinchEncoderDistance()>=Constants.CLIMB_WINCH_FINAL_DISTANCE && !testing){
                    stopWinchString();
                    NewState(DO_NOTHING,"slightly pulling up winch finished");
                } else if (testing && climbTestTimer.get()>=Constants.CLIMB_TESTING_TIME){
                    stopWinchString();
                    NewState(DO_NOTHING,"testing - climbTestTimer is done");
                }
                break;

            case RESET_ENCODER_FOR_WINCH_UP:
                if(onStateEntered){
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                resetWinchEncoders();
                if(Math.abs(getAvgWinchEncoderCount()) < Constants.CLIMB_ENCODER_RESET_ERROR){
                    NewState(WINCH_UP, "Encoders reset to 0");
                }
                if(climbTestTimer.get() > Constants.CLIMB_ENCODER_RESET_TIMER){
                    NewState(WINCH_UP, "Encoders didn't reset for 1 second"); 
                    //so that it's not stuck here, but take it somewhere else because this could lead to issues?
                }
                break;
                
            case RESET_ENCODER_FOR_WINCH_FINAL:
                if(onStateEntered){
                    climbTestTimer.reset();
                    climbTestTimer.start();
                }
                resetWinchEncoders();
                if(Math.abs(getAvgWinchEncoderCount()) < Constants.CLIMB_ENCODER_RESET_ERROR){
                    NewState(WINCH_FINAL, "Encoders reset to 0");
                }
                if(climbTestTimer.get() > Constants.CLIMB_ENCODER_RESET_TIMER){
                    NewState(WINCH_FINAL, "Encoders didn't reset for 1 second"); 
                    //so that it's not stuck here, but take it somewhere else because this could lead to issues?
                }
                break;

            case DO_NOTHING:
                break;
        }
    }  
}