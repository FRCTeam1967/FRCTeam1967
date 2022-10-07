//old pivot code --> backup for updated pivot magic

package org.janksters.CommonClassesThisYear;

import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;

import edu.wpi.first.wpilibj.Timer;
// import edu.wpi.first.wpilibj.DigitalInput;

import org.janksters.jankyLib.JankyStateMachine;
import org.janksters.jankyLib.jankyXboxJoystick;

//8 values are TBD

public class Pivot extends JankyStateMachine {
    private WPI_TalonFX pivotMotor;
    private jankyXboxJoystick XboxController;
    //private DigitalInput topLimitSwitch, bottomLimitSwitch;

    // Declaring states
    private final int IDLE = 0; // homing sequence
    private final int STARTING_CONFIG = 1; // keeps at 0 position
    private final int INTAKE_CONFIG = 2; // lowers arm
    private final int SHOOTER_CONFIG = 3; // raises arm
    private final int SHOOTER_CONFIG_ACHIEVED = 4; // completed shooter config
    private final int CLIMB_CONFIG = 5; // lowers arm to climbing position
    private final int CLIMB_CONFIG_ACHIEVED = 6; // ☑ completed climbing config

    // Flags
    boolean startingConfigAchieved = false;
    boolean goIntakeConfig = false;
    boolean intakeConfigAchieved = false;
    boolean goShooterConfig = false;
    boolean shooterConfigAchieved = false;
    boolean goClimbConfig = false;
    boolean climbConfigAchieved = false;

    private Timer timer;

    public Pivot() {
        pivotMotor = new WPI_TalonFX(Constants.PIVOT_MOTOR_ID);
        XboxController = new jankyXboxJoystick(Constants.PIVOT_CONTROLLER_PORT_ID);
        // DigitalInput topLimitSwitch = new DigitalInput(Constants.PIVOT_TOP_LIMIT_SWITCH_ID);
        // DigitalInput bottomLimitSwitch = new DigitalInput(Constants.PIVOT_BOTTOM_LIMIT_SWITCH_ID);

        setUpPivot();

        SetMachineName("Pivot");
        SetName(IDLE, "Idle");
        SetName(STARTING_CONFIG, "Starting Config");
        SetName(INTAKE_CONFIG, "Intake Config");
        SetName(SHOOTER_CONFIG, "Shooter Config");
        SetName(SHOOTER_CONFIG_ACHIEVED, "Shooter Config Achieved");
        SetName(CLIMB_CONFIG, "Climb Config");
        SetName(CLIMB_CONFIG_ACHIEVED, "Climb Config Achieved");

        timer = new Timer();

        start();
    }

    public void StateEngine(int curState, boolean onStateEntered) {
        switch (curState) {
            case IDLE:
                if (onStateEntered) {
                    intakeConfigAchieved = false;
                    shooterConfigAchieved = false;
                    climbConfigAchieved = false;
                    timer.reset();
                    timer.start();
                }

                setStartPos();

                //RMW
                //NewState(STARTING_CONFIG, "top limit switch pressed, reached start config");
                
                if (timer.get() >= 1){
                    timer.stop();
                    startingConfigAchieved = true;
                    NewState(STARTING_CONFIG, "top limit switch pressed, reached start config");
                }
                /*
                
                if (topLimitSwitch.get()) {
                    // Switch to Start Config, set current position as 0
                    startingConfigAchieved = true;
                    NewState(STARTING_CONFIG, "top limit switch pressed, reached start config");
                }
                 */
                break;
            case STARTING_CONFIG:
                if (onStateEntered) {
                    //pivotMotor.getSensorCollection().setIntegratedSensorPosition(0, 10); // define position 0
                    intakeConfigAchieved = false;
                    shooterConfigAchieved = false;
                    climbConfigAchieved = false;
                }

                holdStartPos();

                if (XboxController.GetButtonBack()) {
                    //flagIntakeConfig();
                    NewState(INTAKE_CONFIG, "intake config flag is true");
                }

                if (goIntakeConfig) {
                    goIntakeConfig = false;
                    NewState(INTAKE_CONFIG, "intake config flag is true");
                }

                if (XboxController.GetButtonStart()) {
                    //flagShooterConfig();
                    NewState(SHOOTER_CONFIG, "shooter config flag is true");
                }

                if (goShooterConfig) {
                    goShooterConfig = false;
                    NewState(SHOOTER_CONFIG, "shooter config flag is true");
                }

                break;
            case INTAKE_CONFIG:
                if (onStateEntered) {
                    shooterConfigAchieved = false;
                    climbConfigAchieved = false;
                    startingConfigAchieved = false;
                }
                setIntakePos();
                
                if (XboxController.GetButtonStart()) {
                    //flagShooterConfig();
                    NewState(SHOOTER_CONFIG, "intake config flag is true");
                }

                if (goShooterConfig) {
                    goShooterConfig = false;
                    NewState(SHOOTER_CONFIG, "shooter config flag is true");
                }
                if (goClimbConfig) {
                    goClimbConfig = false;
                    NewState(CLIMB_CONFIG, "climb config flag is true");
                }

                if (checkPosition(Constants.PIVOT_INTAKE_ANGLE_PULSES)) {
                    intakeConfigAchieved = true;
                }
                break;
            case SHOOTER_CONFIG:
                intakeConfigAchieved = false;
                climbConfigAchieved = false;
                startingConfigAchieved = false;
                setShooterConfig();
                if (XboxController.GetButtonBack()) {
                    //flagIntakeConfig();
                    NewState(INTAKE_CONFIG, "intake config flag is true");
                }
                if (goIntakeConfig) {
                    goIntakeConfig = false;
                    NewState(INTAKE_CONFIG, "intake config flag is true");
                }
                if (goClimbConfig) {
                    goClimbConfig = false;
                    NewState(CLIMB_CONFIG, "climb config flag is true");
                }

                if (checkPosition(Constants.PIVOT_SHOOTER_ANGLE_PULSES)) {
                    shooterConfigAchieved = true;
                }
                break;
            case CLIMB_CONFIG:
                if (onStateEntered) {
                    intakeConfigAchieved = false;
                    startingConfigAchieved = false;
                    shooterConfigAchieved = false;
                }

                setClimbConfig();

                if (checkPosition(Constants.PIVOT_CLIMB_ANGLE_PULSES) && GetCurrentState() == CLIMB_CONFIG) {
                    climbConfigAchieved = true;
                    NewState(CLIMB_CONFIG_ACHIEVED, "climb config achieved flag set to true");
                }
                break;
            case CLIMB_CONFIG_ACHIEVED:
                setClimbConfig(); // Hold pivot in climb config position
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
    }

    public double getEncoder() {
        return pivotMotor.getSensorCollection().getIntegratedSensorPosition();
    }

    public void setStartPos() {
        pivotMotor.set(TalonFXControlMode.Position, Constants.PIVOT_STARTING_ANGLE_PULSES); // TBD
    }

    public void holdStartPos() {
        pivotMotor.set(TalonFXControlMode.Position, Constants.PIVOT_STARTING_ANGLE_PULSES);
    }

    public void flagIntakeConfig() {
        goIntakeConfig = true;
    }

    public void setIntakePos() {
        pivotMotor.set(TalonFXControlMode.Position, Constants.PIVOT_INTAKE_ANGLE_PULSES);
    }

    public void flagShooterConfig() {
        goShooterConfig = true;
    }
    
    public void setShooterConfig() {
        pivotMotor.set(TalonFXControlMode.Position, Constants.PIVOT_SHOOTER_ANGLE_PULSES);
    }

    // Auto calls this to check if shooter config is reached
    public boolean checkShooterFlag() {
        return shooterConfigAchieved;
    }

    public boolean checkIntakeFlag() {
        return intakeConfigAchieved;
    }

    // Climb mech calls this to trigger pivot into climb config state
    public void flagClimbConfig() {
        goClimbConfig = true;
    }

    public void setClimbConfig() {
        pivotMotor.set(TalonFXControlMode.Position, Constants.PIVOT_CLIMB_ANGLE_PULSES);
    }

    // Used by climb mech to check if pivot has entered CLIMB_CONFIG_ACHIEVED state
    public boolean checkClimbConfigAchieved() {
        return GetCurrentState() == CLIMB_CONFIG_ACHIEVED;
    }

    public boolean checkPosition(double desiredPos) {
        return (Math.abs(getEncoder() - desiredPos) < 8); // returns true if in range
    }

}