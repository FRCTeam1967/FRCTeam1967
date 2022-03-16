//climbing code with manual stop button options in state machine
package org.janksters.CommonClassesThisYear;
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.PneumaticsModuleType;
// import java.util.Set;
import com.ctre.phoenix.ErrorCode;
import com.ctre.phoenix.motorcontrol.NeutralMode;
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
    
    private Solenoid midBarLatch;

    private Timer climbTimer;
    private Timer climbTestTimer;
    private Timer climbSmartDashboardTimer;

    private jankyXboxJoystick XboxController;
    private Pivot pivot;
    private LED led;

    //states
    private final int IDLE = 0; //keep bar down
    private final int LIFTER_DOWN = 1;
    private final int READY_TO_CLIMB = 2;
    private final int LATCH_MID_BAR = 3;
    private final int WINCH_UP = 4;
    private final int GET_ON_HIGH_BAR = 5;
    private final int UNLATCH_MID_BAR = 6; 
    private final int GET_OFF_MID_BAR = 7;
    private final int DO_NOTHING = 8;
    private final int MANUAL_WINCH = 9;
    private final int RESET_ENCODER_FOR_WINCH_UP = 10;
    private final int RESET_ENCODER_FOR_WINCH_FINAL = 11;

    boolean climbYAxisUp, climbYAxisDown, xButtonPressed, yButtonPressed, aButtonPressed, bButtonPressed, startButtonPressed;
    boolean climbYAxisWasUp=false, climbYAxisWasDown=false, xButtonWasPressed=false, yButtonWasPressed=false, 
    aButtonWasPressed=false, bButtonWasPressed=false, startButtonWasPressed=false;

    boolean testing = false;
    double targetPositionRotations = 0; 

    double previousCountL = 0;
    double previousCountR = 0;
    
    public Climb(int winchMotorChannelL, int winchMotorChannelR, int PCMChannel, int midLatchChannel, Pivot _pivot, LED _led){
        winchMotorL = new JankyTalonFX(winchMotorChannelL);
        winchMotorR = new JankyTalonFX(winchMotorChannelR);
        
        midBarLatch = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, midLatchChannel);
        climbTimer = new Timer();
        climbTestTimer = new Timer();
        climbSmartDashboardTimer = new Timer();

        XboxController= new jankyXboxJoystick(2);

        pivot = _pivot;
        led = _led;

        setUpWinchEncoder();

        SetMachineName("JankyStateMachineClimb");
        SetName(IDLE,"Idle");
        SetName(LIFTER_DOWN, "LifterDown");
        SetName(READY_TO_CLIMB, "ReadyToClimb");
        SetName(LATCH_MID_BAR,"LatchMidBar");
        SetName(WINCH_UP,"WinchUp");
        SetName(GET_ON_HIGH_BAR,"GetOnHighBar");
        SetName(UNLATCH_MID_BAR,"UnlatchMidBar");
        SetName(GET_OFF_MID_BAR,"GetOffMidBar");
        SetName(DO_NOTHING,"DoNothing");
        SetName(MANUAL_WINCH, "Manual winch");
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
        winchMotorL.set(Constants.CLIMB_WINCH_MOTOR_STOP);
        winchMotorR.set(Constants.CLIMB_WINCH_MOTOR_STOP);
    }

    private void stopWinchStringPID(){
        winchMotorL.set(TalonFXControlMode.Position, getWinchEncoderCountL());
        winchMotorR.set(TalonFXControlMode.Position, getWinchEncoderCountR());
    }
    
    private void latchMidBar(){
        midBarLatch.set(true);
    }

    private void unlatchMidBar(){
        midBarLatch.set(false);
    }

    private void setUpWinchEncoder(){
        winchMotorL.configFactoryDefault();
        winchMotorL.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_K_TIMEOUT_MS);
        //winchMotorL.setSensorPhase(true);
        winchMotorL.setInverted(TalonFXInvertType.CounterClockwise);
        winchMotorL.configNominalOutputForward(0, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorL.configNominalOutputReverse(0, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorL.configPeakOutputForward(1, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.configPeakOutputReverse(-1, Constants.CLIMB_K_TIMEOUT_MS);

        winchMotorR.configFactoryDefault();
        winchMotorR.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_K_TIMEOUT_MS);
        //winchMotorR.setSensorPhase(false);
        winchMotorR.setInverted(TalonFXInvertType.Clockwise);
        winchMotorR.configNominalOutputForward(0, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorR.configNominalOutputReverse(0, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorR.configPeakOutputForward(1, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.configPeakOutputReverse(-1, Constants.CLIMB_K_TIMEOUT_MS);

        // allowable error
        winchMotorL.configAllowableClosedloopError(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_ALLOWABLE_ERROR, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorR.configAllowableClosedloopError(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_ALLOWABLE_ERROR, Constants.CLIMB_K_TIMEOUT_MS);

        // left PID config
        winchMotorL.config_kF(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KF, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.config_kP(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KP, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.config_kI(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KI, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.config_kD(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KD, Constants.CLIMB_K_TIMEOUT_MS);

        // right PID config
        winchMotorR.config_kF(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KF, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.config_kP(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KP, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.config_kI(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KI, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.config_kD(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_WINCH_DOWN_KD, Constants.CLIMB_K_TIMEOUT_MS);

        winchMotorL.setNeutralMode(NeutralMode.Brake);
        winchMotorR.setNeutralMode(NeutralMode.Brake);
    }

    private void configurePIDForRobotWeight(){
        winchMotorL.configPeakOutputForward(Constants.CLIMB_ROBOT_WEIGHT_PEAK, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.configPeakOutputReverse(-1.0 * Constants.CLIMB_ROBOT_WEIGHT_PEAK, Constants.CLIMB_K_TIMEOUT_MS);
        winchMotorR.configPeakOutputForward(Constants.CLIMB_ROBOT_WEIGHT_PEAK, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.configPeakOutputReverse(-1.0 * Constants.CLIMB_ROBOT_WEIGHT_PEAK, Constants.CLIMB_K_TIMEOUT_MS);

        winchMotorL.config_kF(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_ROBOT_WEIGHT_KF, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.config_kP(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_ROBOT_WEIGHT_KP, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.config_kI(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_ROBOT_WEIGHT_KI, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorL.config_kD(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_ROBOT_WEIGHT_KD, Constants.CLIMB_K_TIMEOUT_MS);

        winchMotorR.config_kF(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_ROBOT_WEIGHT_KF, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.config_kP(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_ROBOT_WEIGHT_KP, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.config_kI(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_ROBOT_WEIGHT_KI, Constants.CLIMB_K_TIMEOUT_MS);
		winchMotorR.config_kD(Constants.CLIMB_K_PID_LOOP_IDX, Constants.CLIMB_ROBOT_WEIGHT_KD, Constants.CLIMB_K_TIMEOUT_MS);
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
        /*
        ErrorCode leftWinchEncoderError = winchMotorL.getSensorCollection().setIntegratedSensorPosition(0, 10);
        SmartDashboard.putString("LWinch Encoder Error", leftWinchEncoderError.name());
        previousCountL = 0;
        
        ErrorCode rightWinchEncoderError = winchMotorR.getSensorCollection().setIntegratedSensorPosition(0, 10);
        SmartDashboard.putString("RWinch Encoder Error", rightWinchEncoderError.name());
        previousCountR = 0;
        */
        winchMotorL.setSelectedSensorPosition(0);
        winchMotorR.setSelectedSensorPosition(0);

    }

    private double getAvgWinchEncoderCount(){
        double currentCountL = getWinchEncoderCountL();
        double currentCountR = getWinchEncoderCountR();
        SmartDashboard.putNumber("left climb encoders", currentCountL);
        SmartDashboard.putNumber("right climb encoders", currentCountR);
        if (Math.abs(currentCountL - currentCountR) >= Constants.CLIMB_WINCH_ENCODER_DIFFERENCE) {
            SmartDashboard.putBoolean("climb winch encoders working?", false);
            // SmartDashboard.putNumber("left climb encoders", currentCountL);
            // SmartDashboard.putNumber("right climb encoders", currentCountR);

            //if one of the encoders gets stuck, disregard it
            if (previousCountL == currentCountL) {
                return currentCountR;
            } else if (previousCountR == currentCountR) {
                return currentCountL;
            }
        } else {
            SmartDashboard.putBoolean("climb winch encoders working?", true);
        }
        previousCountL = currentCountL;
        previousCountR = currentCountR;
        return (currentCountL + currentCountR) / 2;
    }

    private double getAvgWinchEncoderDistance(){ //in inches
        if (testing) {
            return getAvgWinchEncoderCount() / (Constants.FALCON_PULSES_PER_REVOLUTION);
        }
        return getAvgWinchEncoderCount() * Constants.CLIMB_GEAR_RATIO * Constants.WINCH_CIRCUMFERENCE / Constants.FALCON_PULSES_PER_REVOLUTION;
    }

    private void resetForClimb(){
        unlatchMidBar();
        stopWinchString();
    }

    private double winchDistanceToEncoderCount(double distance){ //in inches
        if (testing) {
            return distance * (Constants.FALCON_PULSES_PER_REVOLUTION);
        }
        return ((distance / Constants.CLIMB_GEAR_RATIO) * Constants.FALCON_PULSES_PER_REVOLUTION) / Constants.WINCH_CIRCUMFERENCE;
    }
    
    private double getWinchStatorCurrent(){
        double leftMotorCurrent = winchMotorL.getStatorCurrent();
        double rightMotorCurrent = winchMotorR.getStatorCurrent();
        return (leftMotorCurrent+rightMotorCurrent)/2; 
    }

    public void killStateMachine(){
        Terminate();
    }

    public boolean isInIdle(){
        return GetCurrentState() == IDLE;
    }

    public void StateEngine(int curState, boolean onStateEntered){
        // TODO - remove unnecessary SmartDashBoard print
        if (climbSmartDashboardTimer.get() >= 0.25) {
            SmartDashboard.putNumber("climb state", GetCurrentState());
            SmartDashboard.putNumber("climb timer", climbTimer.get());
            SmartDashboard.putNumber("climb test timer", climbTestTimer.get());
            SmartDashboard.putNumber("climb target position", targetPositionRotations);
            SmartDashboard.putNumber("climb encoder actual", getAvgWinchEncoderCount());
            SmartDashboard.putNumber("climb encoder position", getAvgWinchEncoderDistance());
            SmartDashboard.putNumber("stator current", getWinchStatorCurrent());
            SmartDashboard.putNumber("left encoder error", winchMotorL.getClosedLoopError(Constants.CLIMB_K_PID_LOOP_IDX));
            SmartDashboard.putNumber("right encoder error", winchMotorR.getClosedLoopError(Constants.CLIMB_K_PID_LOOP_IDX));
            climbSmartDashboardTimer.reset();
            climbSmartDashboardTimer.start();
        }

        /* //editable values to tune PID
        SmartDashboard.putNumber("CLIMB_ROBOT_WEIGHT_KP", Constants.CLIMB_ROBOT_WEIGHT_KP);
        SmartDashboard.putNumber("CLIMB_ROBOT_WEIGHT_KI", Constants.CLIMB_ROBOT_WEIGHT_KI);
        SmartDashboard.putNumber("CLIMB_ROBOT_WEIGHT_KD", Constants.CLIMB_ROBOT_WEIGHT_KD);
        SmartDashboard.putNumber("CLIMB_ROBOT_WEIGHT_KF", Constants.CLIMB_ROBOT_WEIGHT_KF);
        
        Constants.CLIMB_ROBOT_WEIGHT_KP = SmartDashboard.getNumber("CLIMB_ROBOT_WEIGHT_KP", Constants.CLIMB_ROBOT_WEIGHT_KP);
        Constants.CLIMB_ROBOT_WEIGHT_KI = SmartDashboard.getNumber("CLIMB_ROBOT_WEIGHT_KI", Constants.CLIMB_ROBOT_WEIGHT_KI);
        Constants.CLIMB_ROBOT_WEIGHT_KD = SmartDashboard.getNumber("CLIMB_ROBOT_WEIGHT_KD", Constants.CLIMB_ROBOT_WEIGHT_KD);
        Constants.CLIMB_ROBOT_WEIGHT_KF = SmartDashboard.getNumber("CLIMB_ROBOT_WEIGHT_KF", Constants.CLIMB_ROBOT_WEIGHT_KF); */
        
        switch (curState){
            case IDLE:
                if(onStateEntered){
                    resetWinchEncoders();
                    climbSmartDashboardTimer.start();
                }
                if (XboxController.GetRightStickButton()){
                    NewState(LIFTER_DOWN, "right stick button was pressed");
                }
                break;

            case LIFTER_DOWN:
                if(onStateEntered){
                    climbTestTimer.reset();
                    climbTestTimer.start();
                    led.setColor(0, 255, 0); //makes it green 
                    //TODO: make sure other groups don't use at this point
                }
                //pivot.flagClimbConfig(); //TODO: uncomment once we want to use lifter
                //if(!testing && pivot.checkClimbConfigAchieved()){
                    NewState(READY_TO_CLIMB, "lifter is down");
                //}
                if(testing && climbTestTimer.get()>=Constants.CLIMB_TESTING_TIME){ //make else if
                    NewState(READY_TO_CLIMB, "testing - lifter timer is done");
                }
                else if (XboxController.GetButtonB()){
                    NewState(IDLE, "reset button pressed");
                }
                break;
                
            case READY_TO_CLIMB:
                if (onStateEntered){
                    resetForClimb();
                }
                
                // joystick up and down
                double rightYAxis = XboxController.GetRightYAxis();
                if(rightYAxis > Constants.CLIMB_DEADBAND){
                    raiseWinchString(-1.0 * rightYAxis); 
                } else if(rightYAxis < -1.0 * Constants.CLIMB_DEADBAND){
                    lowerWinchString(rightYAxis);
                } else{
                    stopWinchString();
                }

                xButtonPressed = XboxController.GetButtonX(); //mid bar latch
                if (xButtonPressed && !xButtonWasPressed) {
                    latchMidBar();
                    xButtonWasPressed = true;
                } else if (!xButtonPressed && xButtonWasPressed) {
                    xButtonWasPressed = false;
                }

                yButtonPressed = XboxController.GetButtonY(); //mid bar unlatch
                if (yButtonPressed && !yButtonWasPressed) {
                    unlatchMidBar();
                    yButtonWasPressed = true;
                } else if (!yButtonPressed && yButtonWasPressed) {
                    yButtonWasPressed = false;
                }

                if (XboxController.GetButtonA()){  //start sequence
                    NewState(LATCH_MID_BAR, "x button pressed to start sequence");
                } else if (XboxController.GetButtonB()){
                    NewState(IDLE, "reset button pressed in ready to climb");
                }
                break;

            case LATCH_MID_BAR:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                    latchMidBar();
                }
                if (XboxController.GetButtonB()) {
                    NewState(MANUAL_WINCH, "pressed button for manual control");
                }
                else if (climbTimer.get()>=Constants.CLIMB_LATCH_TIME){
                    climbTimer.stop();
                    NewState(RESET_ENCODER_FOR_WINCH_UP, "mid bar latch timer finished");
                }
                break;
           
            case RESET_ENCODER_FOR_WINCH_UP:
                if(onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                    resetWinchEncoders();
                }
                if(Math.abs(getAvgWinchEncoderCount()) < Constants.CLIMB_ENCODER_RESET_ALLOWABLE_ERROR){
                    NewState(WINCH_UP, "Encoders reset to 0");
                }
                else if(climbTimer.get() > Constants.CLIMB_ENCODER_RESET_TIMER){
                    NewState(MANUAL_WINCH, "Encoders didn't reset for 1 second - do it yourself smile"); 
                }
                break;

            case WINCH_UP:
                if (onStateEntered){ 
                    climbTestTimer.reset();
                    climbTestTimer.start();
                    configurePIDForRobotWeight();
                }
                double winchUpPulses = winchDistanceToEncoderCount(Constants.CLIMB_WINCH_UP_DISTANCE);
                winchMotorL.set(TalonFXControlMode.Position, winchUpPulses); 
                winchMotorR.set(TalonFXControlMode.Position, winchUpPulses);  
                //winchMotorL.set(TalonFXControlMode.Position, getWinchEncoderCountL() + winchDistanceToEncoderCount(Constants.CLIMB_WINCH_UP_DISTANCE)); 
                //winchMotorR.set(TalonFXControlMode.Position, getWinchEncoderCountR() + winchDistanceToEncoderCount(Constants.CLIMB_WINCH_UP_DISTANCE));   
                SmartDashboard.putNumber("winch up - climb target location", winchUpPulses);
                
                //raiseWinchString(Constants.CLIMB_ROBOT_UP_SPEED_SEQUENCE);                            
                if (XboxController.GetButtonB()) {
                    stopWinchStringPID();
                    NewState(MANUAL_WINCH, "no longer going to winch up");
                }
                else if (!testing && getAvgWinchEncoderDistance() <= Constants.CLIMB_WINCH_UP_DISTANCE){ 
                    stopWinchStringPID();
                    //NewState(GET_ON_HIGH_BAR,"reached full winch distance");
                } else if(testing && climbTestTimer.get()>=Constants.CLIMB_TESTING_TIME){
                    stopWinchStringPID();
                    NewState(GET_ON_HIGH_BAR,"testing - climbTestTimer is done");
                }
                break;

            case GET_ON_HIGH_BAR:
                if(onStateEntered){
                    climbTestTimer.reset();
                    climbTestTimer.start();
                    winchMotorL.set(TalonFXControlMode.Position, winchDistanceToEncoderCount(Constants.CLIMB_HIGH_BAR_DISTANCE)); 
                    winchMotorR.set(TalonFXControlMode.Position, winchDistanceToEncoderCount(Constants.CLIMB_HIGH_BAR_DISTANCE));  
                }
                if (!testing && getWinchStatorCurrent() < Constants.CLIMB_HOOKED_ON_CURRENT){
                    stopWinchStringPID();
                    NewState(UNLATCH_MID_BAR,"current is < hooked on current");
                } else if(testing && climbTestTimer.get()>=Constants.CLIMB_TESTING_TIME){
                    stopWinchStringPID();
                    NewState(UNLATCH_MID_BAR,"testing = climbTestTimer is done");
                } else if (!testing && getAvgWinchEncoderDistance() > Constants.CLIMB_TOO_FAR_DOWN){
                    stopWinchStringPID();
                    NewState(WINCH_UP,"distance is too far down");
                } else if(testing && XboxController.GetButtonStart()){ 
                    stopWinchStringPID();
                    NewState(WINCH_UP,"pressed button start while testing");
                } else if(XboxController.GetButtonB()){
                    NewState(MANUAL_WINCH,"decided to move to manual");
                }
                break;

            case UNLATCH_MID_BAR:
                if (onStateEntered){
                    climbTimer.reset();
                    climbTimer.start();
                    unlatchMidBar();
                }
                if (XboxController.GetButtonB()) {
                    NewState(MANUAL_WINCH,"stopping sequence");
                }
                else if (climbTimer.get()>=Constants.CLIMB_LATCH_TIME){
                    climbTimer.stop();
                    NewState(RESET_ENCODER_FOR_WINCH_FINAL,"mid bar unlatch timer finished");
                }
                break;

            case RESET_ENCODER_FOR_WINCH_FINAL:
                if(onStateEntered){
                    climbTestTimer.reset();
                    climbTestTimer.start();
                    resetWinchEncoders();
                }
                if(Math.abs(getAvgWinchEncoderCount()) < Constants.CLIMB_ENCODER_RESET_ALLOWABLE_ERROR){
                    NewState(GET_OFF_MID_BAR, "Encoders reset to 0");
                }
                else if(climbTestTimer.get() > Constants.CLIMB_ENCODER_RESET_TIMER){
                    NewState(MANUAL_WINCH, "Encoders didn't reset for 1 second - do it yourself lol"); //so that it's not stuck here
                }
                break;
            
            case GET_OFF_MID_BAR:
                if (onStateEntered){
                    //inchEncoders();
                    climbTestTimer.reset();
                    climbTestTimer.start();
                    winchMotorL.set(TalonFXControlMode.Position, Constants.CLIMB_WINCH_FINAL_DISTANCE);
                    winchMotorR.set(TalonFXControlMode.Position, Constants.CLIMB_WINCH_FINAL_DISTANCE);
                }
                if (XboxController.GetButtonB()) {
                    stopWinchStringPID();
                    NewState(MANUAL_WINCH,"stopping sequence");
                } else if (!testing && getAvgWinchEncoderDistance()>=Constants.CLIMB_WINCH_FINAL_DISTANCE){
                    stopWinchStringPID();
                    NewState(DO_NOTHING,"slightly pulling up winch finished");
                } else if (testing && climbTestTimer.get()>=Constants.CLIMB_TESTING_TIME){
                    stopWinchStringPID();
                    NewState(DO_NOTHING,"testing - climbTestTimer is done");
                }
                break;

            case MANUAL_WINCH:
                if(onStateEntered){
                    stopWinchStringPID();
                    configurePIDForRobotWeight();
                    targetPositionRotations = getAvgWinchEncoderCount();
                    led.setColor(255, 0, 0); //makes it red
                }

                //joystick
                if(Math.abs(XboxController.GetRightYAxis()) >= Constants.CLIMB_DEADBAND){
                    //TODO: add timer to check joystick less or tune CLIMB_POS_FACTOR
                    targetPositionRotations += winchDistanceToEncoderCount(XboxController.GetRightYAxis() * Constants.CLIMB_POS_FACTOR);  //make POS_FACTOR editable from SmartDashboard
                }
                winchMotorL.set(TalonFXControlMode.Position, targetPositionRotations);
                winchMotorR.set(TalonFXControlMode.Position, targetPositionRotations);

                // mid bar latch
                xButtonPressed = XboxController.GetButtonX();
                if (xButtonPressed && !xButtonWasPressed) {
                    latchMidBar();
                    xButtonWasPressed = true;
                } else if (!xButtonPressed && xButtonWasPressed) {
                    xButtonWasPressed = false;
                }
    
                // mid bar unlatch
                yButtonPressed = XboxController.GetButtonY();
                if (yButtonPressed && !yButtonWasPressed) {
                    unlatchMidBar();
                    yButtonWasPressed = true;
                } else if (!yButtonPressed && yButtonWasPressed) {
                    yButtonWasPressed = false;
                }
                break;

            case DO_NOTHING:
                break;
        }
    }  
}