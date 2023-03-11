package frc.robot;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXInvertType;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;

/**
 * SimpleChassis Joystick Button Mapping
 * getRawButton(1) L or R- drive straight
 * getRawButton(2) L or R- slow&straight mode
 * getRawButton(9) L or R- brake mode
 * getRawButton(10) L or R- coast mode
 */

public class SimpleChassis implements DriveSystem {
    //motors
    private WPI_TalonFX leftLeader, leftFollower, rightLeader, rightFollower;
    private MotorControllerGroup leftGroup, rightGroup;
    private DifferentialDrive myRobot;

    //brake vs coast mode boolean
    private boolean inBrakeMode;

    /**
     * Constructor for Simple Chassis
     * <p> Defining and configuring motors, motor controller groups, and differential drive class
     */
    public SimpleChassis() {
        leftLeader = new WPI_TalonFX(Constants.Chassis.LEFT_LEADER_ID);
        leftFollower = new WPI_TalonFX(Constants.Chassis.LEFT_FOLLOWER_ID);
        rightLeader = new WPI_TalonFX(Constants.Chassis.RIGHT_LEADER_ID);
        rightFollower = new WPI_TalonFX(Constants.Chassis.RIGHT_FOLLOWER_ID);
        
        leftLeader.setNeutralMode(NeutralMode.Coast);
        leftFollower.setNeutralMode(NeutralMode.Coast);
        rightLeader.setNeutralMode(NeutralMode.Coast);
        rightFollower.setNeutralMode(NeutralMode.Coast);

        leftLeader.setInverted(TalonFXInvertType.Clockwise);
        leftFollower.setInverted(TalonFXInvertType.Clockwise);

        leftGroup = new MotorControllerGroup(leftLeader, leftFollower);
        rightGroup = new MotorControllerGroup(rightLeader, rightFollower);
        
        myRobot = new DifferentialDrive(leftGroup, rightGroup);
    }

    public void configDashboard(ShuffleboardTab tab){
        tab.addBoolean("Is Chassis in Brake Mode?", () -> inBrakeMode);
    }
    
    /**
     * Standard tank drive with joystick inputs raised to a power (default is 1)
     * @param leftJoystick - double value between -1.0 and 1.0
     * @param rightJoystick - double value between -1.0 and 1.0
     */
    public void drive(double leftJoystick, double rightJoystick){
        myRobot.tankDrive(Math.pow(leftJoystick,Constants.Chassis.JOYSTICK_EXP), Math.pow(rightJoystick,Constants.Chassis.JOYSTICK_EXP));
    }
    
    /**
     * Averages left and right joystick inputs + raising inputs to a power (default = 1)
     * @param leftJoystick - double value between -1.0 and 1.0
     * @param rightJoystick - double value between -1.0 and 1.0
     */
    public void driveStraight(double leftJoystick, double rightJoystick){
        double joystickAvg = (leftJoystick + rightJoystick)/2;
        myRobot.tankDrive(Math.pow(joystickAvg,Constants.Chassis.JOYSTICK_EXP), Math.pow(joystickAvg,Constants.Chassis.JOYSTICK_EXP));
    }
    
    /** 
     * Averages left and right joystick inputs
     * Same as regular tank drive, joystick inputs are multiplied by SLOW_MODE_FACTOR
     * @param leftJoystick - double value between -1.0 and 1.0
     * @param rightJoystick - double value between -1.0 and 1.0
     */
    public void slowMode(double leftJoystick, double rightJoystick){
        double joystickAvg = (leftJoystick + rightJoystick)/2;
        myRobot.tankDrive(Math.pow((joystickAvg*Constants.Chassis.SLOW_MODE_FACTOR), Constants.Chassis.JOYSTICK_EXP), Math.pow((joystickAvg*Constants.Chassis.SLOW_MODE_FACTOR), Constants.Chassis.JOYSTICK_EXP));
    }
    
    /**
     * Activated with joystick buttons when manually engaging on charge station
     * @param isBrake - true means set to brake mode, false set to coast mode
     */
    public void setBrakeMode(boolean isBrake){
        if (isBrake){
            leftLeader.setNeutralMode(NeutralMode.Brake);
            rightLeader.setNeutralMode(NeutralMode.Brake);
            leftFollower.setNeutralMode(NeutralMode.Brake);
            rightFollower.setNeutralMode(NeutralMode.Brake);

            leftLeader.configNeutralDeadband(0.001);
            rightLeader.configNeutralDeadband(0.001);
            leftFollower.configNeutralDeadband(0.001);
            rightFollower.configNeutralDeadband(0.001);

            inBrakeMode = true;

        } else {
            leftLeader.setNeutralMode(NeutralMode.Coast);
            rightLeader.setNeutralMode(NeutralMode.Coast);
            leftFollower.setNeutralMode(NeutralMode.Coast);
            rightFollower.setNeutralMode(NeutralMode.Coast);

            inBrakeMode = false;
        }
    }

    public void autoAlign(){}
}
