//updated pivot code --> using motion magic for smoother control

package org.janksters.CommonClassesThisYear;

import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.StatusFrameEnhanced;

import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
// import edu.wpi.first.wpilibj.DigitalInput;

import org.janksters.jankyLib.JankyStateMachine;
import org.janksters.jankyLib.jankyXboxJoystick;

public class PivotMagic extends JankyStateMachine {
    private WPI_TalonFX pivotMotor;
    private jankyXboxJoystick XboxController;
    private Timer timer;
    private double lifterTargetValue = 0.0;
    //private DigitalInput bottomLimitSwitch;

    // Declaring states
    private final int IDLE = 0; // homing sequence
    private final int STARTING_CONFIG = 1; // keeps at 0 position
    private final int INTAKE_CONFIG = 2; // lowers arm
    private final int SHOOTER_CONFIG = 3; // raises arm
    private final int SHOOTER_CONFIG_ACHIEVED = 4; // completed shooter config
    private final int CLIMB_CONFIG = 5; // lowers arm to climbing position
    private final int CLIMB_CONFIG_ACHIEVED = 6; // completed climbing config

    // Flags
    boolean goIntakeConfig = false;
    boolean goShooterConfig = false;
    boolean goClimbConfig = false;

    public PivotMagic() {
        pivotMotor = new WPI_TalonFX(Constants.PIVOT_MOTOR_ID);
        XboxController = new jankyXboxJoystick(Constants.PIVOT_CONTROLLER_PORT_ID);
        //bottomLimitSwitch = new DigitalInput(Constants.PIVOT_MOTOR_ID);
        timer = new Timer();
        
        setUpPivot();

        SetMachineName("PivotMagic");
        SetName(IDLE, "Idle");
        SetName(STARTING_CONFIG, "Starting Config");
        SetName(INTAKE_CONFIG, "Intake Config");
        SetName(SHOOTER_CONFIG, "Shooter Config");
        SetName(SHOOTER_CONFIG_ACHIEVED, "Shooter Config Achieved");
        SetName(CLIMB_CONFIG, "Climb Config");
        SetName(CLIMB_CONFIG_ACHIEVED, "Climb Config Achieved");

        start();
    }

    public void StateEngine(int curState, boolean onStateEntered) {
        //setShooterAngle();
        //SmartDashboard.putBoolean("Bottom Limit Switch", bottomLimitSwitch.get());
        //SmartDashboard.putNumber("pivot magic state", curState);
        switch (curState) {
            case IDLE:
                if (onStateEntered) {
                    pivotMotor.setSelectedSensorPosition(0);
                    //pivotMotor.getSensorCollection().setIntegratedSensorPosition(0, 10);
                    timer.reset();
                }

                SmartDashboard.putNumber("pivotMotor Actual Pos", getEncoder());
                timer.start();
                pivotMotor.set(TalonFXControlMode.MotionMagic, 0);
                
                if (timer.get() >= 0.5){ //timer tbd
                    timer.stop();
                    NewState(STARTING_CONFIG, "top limit switch pressed, reached start config");
                }
                
                break;
            case STARTING_CONFIG:
                setStartPos();
                
                SmartDashboard.putNumber("pivotMotor Actual Pos", getEncoder());
                SmartDashboard.putNumber("pivotMotor Target Value", Constants.PIVOT_STARTING_ANGLE_PULSES);

                if (XboxController.GetButtonBack()) {
                    NewState(INTAKE_CONFIG, "button back was pressed");
                }
                else if (goIntakeConfig) {
                    goIntakeConfig = false;
                    NewState(INTAKE_CONFIG, "intake config flag is true");
                }
                else if (XboxController.GetButtonStart()) {
                    NewState(SHOOTER_CONFIG, "shooter config flag is true");
                }
                else if (goShooterConfig) {
                    goShooterConfig = false;
                    NewState(SHOOTER_CONFIG, "shooter config flag is true");
                }

                break;
            case INTAKE_CONFIG:
                if (onStateEntered) {
                    goIntakeConfig = false;
                    pivotMotor.config_kP(Constants.PIVOT_kPIDLoopIdx, Constants.PIVOT_kP_INTAKE, Constants.PIVOT_K_TIMEOUT_MS);
                    setIntakePos();
                }

                SmartDashboard.putNumber("pivotMotor Actual Pos", getEncoder());
                SmartDashboard.putNumber("pivotMotor Target Value", Constants.PIVOT_INTAKE_ANGLE_PULSES);
                
                if (XboxController.GetButtonStart()) {
                    NewState(SHOOTER_CONFIG, "button start was pressed");
                }

                else if (goShooterConfig) {
                    goShooterConfig = false;
                    NewState(SHOOTER_CONFIG, "shooter config flag is true");
                }
                else if (goClimbConfig) {
                    goClimbConfig = false;
                    NewState(CLIMB_CONFIG, "climb config flag is true");
                }

                break;
            case SHOOTER_CONFIG:
                if (onStateEntered) {
                    goShooterConfig = false;
                    pivotMotor.config_kP(Constants.PIVOT_kPIDLoopIdx, Constants.PIVOT_kP_SHOOTING, Constants.PIVOT_K_TIMEOUT_MS);
                    setShooterConfig();
                }
                
                SmartDashboard.putNumber("pivotMotor Actual Pos", getEncoder());
                SmartDashboard.putNumber("pivotMotor Target Value", (lifterTargetValue/ 360) * Constants.PIVOT_GEAR_RATIO * Constants.FALCON_PULSES_PER_REVOLUTION);
                
                if (XboxController.GetButtonBack()) {
                    NewState(INTAKE_CONFIG, "button back was pressed");
                }
                else if (goIntakeConfig) {
                    goIntakeConfig = false;
                    NewState(INTAKE_CONFIG, "intake config flag is true");
                }
                else if (goClimbConfig) {
                    goClimbConfig = false;
                    NewState(CLIMB_CONFIG, "climb config flag is true");
                }
                break;
            case CLIMB_CONFIG:
                if(onStateEntered){
                    goClimbConfig = false;
                    setClimbConfig();
                }
                SmartDashboard.putNumber("pivotMotor Actual Pos", getEncoder());
                SmartDashboard.putNumber("pivotMotor Target Value", Constants.PIVOT_CLIMB_ANGLE_PULSES);

                if (isClimbConfigAchieved()) {
                    NewState(CLIMB_CONFIG_ACHIEVED, "climb config achieved flag set to true");
                }
                break;
            case CLIMB_CONFIG_ACHIEVED:
                setClimbConfig(); // Hold pivot in climb config position
                SmartDashboard.putNumber("pivotMotor Actual Pos", getEncoder());
                SmartDashboard.putNumber("pivotMotor Target Value", Constants.PIVOT_CLIMB_ANGLE_PULSES);
                break;
        }
    }

    private void setUpPivot() {
        /* setup */
        pivotMotor.configFactoryDefault();
        pivotMotor.configSelectedFeedbackSensor(FeedbackDevice.IntegratedSensor, Constants.PIVOT_kPIDLoopIdx,
                Constants.PIVOT_K_TIMEOUT_MS);
        pivotMotor.setSensorPhase(true);
        pivotMotor.setInverted(false); // TBD
        
        /* set deadband to super small 0.001 (0.1 %).
			The default deadband is 0.04 (4 %) */
        pivotMotor.configNeutralDeadband(0.001, Constants.PIVOT_K_TIMEOUT_MS);

        pivotMotor.setStatusFramePeriod(StatusFrameEnhanced.Status_13_Base_PIDF0, Constants.CAN_STATUS_FRAME_PERIOD, Constants.PIVOT_K_TIMEOUT_MS);
		pivotMotor.setStatusFramePeriod(StatusFrameEnhanced.Status_10_MotionMagic, Constants.CAN_STATUS_FRAME_PERIOD, Constants.PIVOT_K_TIMEOUT_MS);

        /* min/max */
        pivotMotor.configNominalOutputForward(0, Constants.PIVOT_K_TIMEOUT_MS);
        pivotMotor.configNominalOutputReverse(0, Constants.PIVOT_K_TIMEOUT_MS);
        pivotMotor.configPeakOutputForward(1, Constants.PIVOT_K_TIMEOUT_MS); //1
        pivotMotor.configPeakOutputReverse(-1, Constants.PIVOT_K_TIMEOUT_MS); //-1

        /* allowable error */
        pivotMotor.configAllowableClosedloopError(0, Constants.PIVOT_kPIDLoopIdx, Constants.PIVOT_K_TIMEOUT_MS);

        /* PID config */
        pivotMotor.config_kF(Constants.PIVOT_kPIDLoopIdx, Constants.PIVOT_kF, Constants.PIVOT_K_TIMEOUT_MS);
        pivotMotor.config_kP(Constants.PIVOT_kPIDLoopIdx, Constants.PIVOT_kP_SHOOTING, Constants.PIVOT_K_TIMEOUT_MS);
        pivotMotor.config_kI(Constants.PIVOT_kPIDLoopIdx, Constants.PIVOT_kI, Constants.PIVOT_K_TIMEOUT_MS);
        pivotMotor.config_kD(Constants.PIVOT_kPIDLoopIdx, Constants.PIVOT_kD, Constants.PIVOT_K_TIMEOUT_MS);

        pivotMotor.configMotionCruiseVelocity(22500, Constants.PIVOT_K_TIMEOUT_MS); //15000
		pivotMotor.configMotionAcceleration(13500, Constants.PIVOT_K_TIMEOUT_MS); //6000
    }

    public double getEncoder() {
        return pivotMotor.getSensorCollection().getIntegratedSensorPosition();
    }

    public void setStartPos() {
        pivotMotor.set(TalonFXControlMode.MotionMagic, Constants.PIVOT_STARTING_ANGLE_PULSES); // TBD
    }

    public void holdStartPos() {
        pivotMotor.set(TalonFXControlMode.MotionMagic, Constants.PIVOT_STARTING_ANGLE_PULSES);
    }

    public void flagIntakeConfig() {
        goIntakeConfig = true;
    }

    public void setIntakePos() {
        pivotMotor.set(TalonFXControlMode.MotionMagic, Constants.PIVOT_INTAKE_ANGLE_PULSES);
    }

    public void flagShooterConfig() {
        goShooterConfig = true;
    }

    public void setShooterConfig() {
        pivotMotor.set(TalonFXControlMode.MotionMagic, (lifterTargetValue/ 360) * Constants.PIVOT_GEAR_RATIO * Constants.FALCON_PULSES_PER_REVOLUTION);
    }

    //climb mech calls this to trigger pivot into climb config state
    // public void flagClimbConfig() {
    //     goClimbConfig = true;
    // }

    // Used by climb mech to check if pivot has entered CLIMB_CONFIG_ACHIEVED state
    // public boolean checkClimbConfigAchieved() {
    //     return GetCurrentState() == CLIMB_CONFIG_ACHIEVED;
    // }

    public void setClimbConfig() {
        pivotMotor.set(TalonFXControlMode.MotionMagic, Constants.PIVOT_CLIMB_ANGLE_PULSES);
    }

    public boolean checkPosition(double desiredPos) {
        return (Math.abs(getEncoder() - desiredPos) < 1000); // returns true if in range
    }

    public void displayDpadOutput(){
        SmartDashboard.putNumber("D-pad output", XboxController.getPOV());
    }

    // public void setShooterAngle(){
    //     if (XboxController.getPOV() == 0){
    //         lifterTargetValue = Constants.PIVOT_SHOOTER_ANGLE;
    //     } else if (XboxController.getPOV() == 90){
    //         lifterTargetValue =  Constants.PIVOT_SHOOTER_MIDANGLE;
    //     } else if (XboxController.getPOV() == 180){
    //         lifterTargetValue =  Constants.PIVOT_SHOOTER_LOWANGLE;
    //     } else if (XboxController.getPOV() == 270){
    //         lifterTargetValue = Constants.PIVOT_SHOOTER_LOWESTANGLE;
    //     }
    //     SmartDashboard.putNumber("lifterTargetValue", lifterTargetValue);
    // }

    public boolean isInIdle(){
        return (GetCurrentState() == IDLE);
    }

    public void killStateMachine(){
        Terminate();
    }

    public boolean isShooterConfigAchieved(){
        return (checkPosition((lifterTargetValue/ 360) * Constants.PIVOT_GEAR_RATIO * Constants.FALCON_PULSES_PER_REVOLUTION));
    }

    public boolean isIntakeConfigAchieved(){
        return checkPosition(Constants.PIVOT_INTAKE_ANGLE_PULSES);
    }

    public boolean isClimbConfigAchieved(){
        return (checkPosition(Constants.PIVOT_CLIMB_ANGLE_PULSES) && GetCurrentState() == CLIMB_CONFIG);
    }
}