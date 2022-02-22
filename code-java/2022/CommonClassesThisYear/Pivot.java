package org.janksters.CommonClassesThisYear;

import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;

import edu.wpi.first.wpilibj.DigitalInput;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import org.janksters.jankyLib.JankyStateMachine;
import org.janksters.jankyLib.jankyXboxJoystick;

//8 values are TBD

public class Pivot extends JankyStateMachine {
    private WPI_TalonFX pivotMotor;
    private jankyXboxJoystick XboxController;
    private DigitalInput topLimitSwitch, bottomLimitSwitch;

    // Declaring states
    private final int IDLE = 0; // homing sequence
    private final int STARTING_CONFIG = 1; // keeps at 0 position
    private final int INTAKE_CONFIG = 2; // lowers arm
    private final int SHOOTER_CONFIG = 3; // raises arm
    private final int SHOOTER_CONFIG_ACHIEVED = 4; // completed shooter config
    private final int CLIMB_CONFIG = 5; // lowers arm to climbing position
    private final int CLIMB_CONFIG_ACHIEVED = 6; // ☑ completed climbing config

    private double targetPositionRotations = 0;

    // Flags
    boolean startingConfigAchieved = false;
    boolean goIntakeConfig = false;
    boolean intakeConfigAchieved = false;
    boolean goShooterConfig = false;
    boolean shooterConfigAchieved = false;
    boolean goClimbConfig = false;
    boolean climbConfigAchieved = false;

    public Pivot() {
        pivotMotor = new WPI_TalonFX(Constants.MOTOR_ID);
        XboxController = new jankyXboxJoystick(Constants.CONTROLLER_PORT_ID);
        DigitalInput topLimitSwitch = new DigitalInput(Constants.TOP_LIMIT_SWITCH_ID);
        DigitalInput bottomLimitSwitch = new DigitalInput(Constants.BOTTOM_LIMIT_SWITCH_ID);

        setUpPivot();

        SetMachineName("Pivot");
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
        switch (curState) {
            case IDLE:
                intakeConfigAchieved = false;
                shooterConfigAchieved = false;
                climbConfigAchieved = false;
                setStartPos();
                if (topLimitSwitch.get()) {
                    // Switch to Start Config, set current position as 0
                    startingConfigAchieved = true;
                    pivotMotor.getSensorCollection().setIntegratedSensorPosition(0, 10);
                    NewState(STARTING_CONFIG, "top limit switch pressed, reached start config");
                }
                break;
            case STARTING_CONFIG:
                intakeConfigAchieved = false;
                shooterConfigAchieved = false;
                climbConfigAchieved = false;
                holdStartPos(); 
                if (XboxController.GetButtonBack()){
                    flagIntakeConfig();
                }
                if (goIntakeConfig) {
                    goIntakeConfig = false;
                    NewState(INTAKE_CONFIG, "intake config flag is true");
                }
                if (XboxController.GetButtonStart()){
                    flagShooterConfig();
                }
                if (goShooterConfig) {
                    goShooterConfig = false;
                    NewState(SHOOTER_CONFIG, "shooter config flag is true");
                }
                //TODO use encoder values or limit switch to check if true
                startingConfigAchieved = true;
                break;
            case INTAKE_CONFIG:
                shooterConfigAchieved = false;
                climbConfigAchieved = false;
                startingConfigAchieved = false;
                setIntakePos();
                if (XboxController.GetButtonStart()){
                    flagShooterConfig();
                }
                if (goShooterConfig) {
                    goShooterConfig = false;
                    NewState(SHOOTER_CONFIG, "shooter config flag is true");
                }
                if (goClimbConfig){
                    goClimbConfig = false;
                    NewState(CLIMB_CONFIG, "climb config flag is true");
                }
                //TODO use encoder values or limit switch to check if true
                intakeConfigAchieved = true;
                break;
            case SHOOTER_CONFIG:
                intakeConfigAchieved = false;
                climbConfigAchieved = false;
                startingConfigAchieved = false;
                setShooterConfig();
                if (XboxController.GetButtonBack()){
                    flagIntakeConfig();
                }
                if (goIntakeConfig){
                    goIntakeConfig = false;
                    NewState(INTAKE_CONFIG, "intake config flag is true");
                }
                if (goClimbConfig){
                    goClimbConfig = false;
                    NewState(CLIMB_CONFIG, "climb config flag is true");
                }
                //TODO use encoder values to check if true
                shooterConfigAchieved = true;
                break;
            case CLIMB_CONFIG:
                intakeConfigAchieved = false;
                startingConfigAchieved = false;                
                shooterConfigAchieved = false;
                setClimbConfig(); 
                //TODO use encoder values to check if true
                climbConfigAchieved = true;
                if (climbConfigAchieved){
                    NewState(CLIMB_CONFIG_ACHIEVED, "climb config achieved flag set to true");
                }
                break;
            case CLIMB_CONFIG_ACHIEVED:
                setClimbConfig();
                break;
        }
    }

    private void setUpPivot() {
        /* setup */
        pivotMotor.configFactoryDefault();
        pivotMotor.configSelectedFeedbackSensor(FeedbackDevice.IntegratedSensor, Constants.kPIDLoopIdx,
                Constants.kTimeoutMs);
        pivotMotor.setSensorPhase(true);
        pivotMotor.setInverted(false); // TBD

        /* min/max */
        pivotMotor.configNominalOutputForward(0, Constants.kTimeoutMs);
        pivotMotor.configNominalOutputReverse(0, Constants.kTimeoutMs);
        pivotMotor.configPeakOutputForward(1, Constants.kTimeoutMs);
        pivotMotor.configPeakOutputReverse(-1, Constants.kTimeoutMs);

        /* allowable error */
        pivotMotor.configAllowableClosedloopError(0, Constants.kPIDLoopIdx, Constants.kTimeoutMs);

        /* PID config */
        pivotMotor.config_kF(Constants.kPIDLoopIdx, Constants.kF, Constants.kTimeoutMs);
        pivotMotor.config_kP(Constants.kPIDLoopIdx, Constants.kP, Constants.kTimeoutMs);
        pivotMotor.config_kI(Constants.kPIDLoopIdx, Constants.kI, Constants.kTimeoutMs);
        pivotMotor.config_kD(Constants.kPIDLoopIdx, Constants.kD, Constants.kTimeoutMs);
    }

    public void setStartPos() {
        // Input is speed, value between -1 and 1
        pivotMotor.set(8); // TBD
    }

    public void holdStartPos() {
        targetPositionRotations = 0;
        pivotMotor.set(TalonFXControlMode.Position, targetPositionRotations);
    }

    public void flagIntakeConfig() {
        goIntakeConfig = true;
    }

    public void setIntakePos() {
        targetPositionRotations = 8; // TBD
        pivotMotor.set(TalonFXControlMode.Position, targetPositionRotations);
    }

    public void flagShooterConfig(){
        goShooterConfig = true;
    }

    public void setShooterConfig(){
        targetPositionRotations = 8; // TBD
        pivotMotor.set(TalonFXControlMode.Position, targetPositionRotations);
    }
    
    public void flagClimbConfig(){
        goClimbConfig = true;
    }

    public void setClimbConfig(){
        targetPositionRotations = 8; //TBD
        pivotMotor.set(TalonFXControlMode.Position, targetPositionRotations);
    }

}