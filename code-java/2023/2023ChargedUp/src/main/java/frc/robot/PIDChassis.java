package frc.robot;

import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXInvertType;
import com.ctre.phoenix.motorcontrol.can.TalonFXConfiguration;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

/**
 * PIDChassis Joystick Button Mapping
 * getRawButton(1) L or R- drive straight
 * getRawButton(2) L or R- slow mode
 */

public class PIDChassis implements DriveSystem {
    //motors
    private WPI_TalonFX leftLeader, leftFollower, rightLeader, rightFollower;
    
    /**
     * Constructor for PID Chassis
     * Defining and configuring motors
     */
    public PIDChassis(){
        leftLeader = new WPI_TalonFX(Constants.Chassis.LEFT_LEADER_ID);
        leftFollower = new WPI_TalonFX(Constants.Chassis.LEFT_FOLLOWER_ID);
        rightLeader = new WPI_TalonFX(Constants.Chassis.RIGHT_LEADER_ID);
        rightFollower = new WPI_TalonFX(Constants.Chassis.RIGHT_FOLLOWER_ID);

        configMotors(leftLeader);
        configMotors(leftFollower);
        configMotors(rightLeader);
        configMotors(rightFollower);

        leftLeader.setInverted(TalonFXInvertType.Clockwise);
        leftFollower.setInverted(TalonFXInvertType.Clockwise);
    }

    /**
     * Configures kP, kI, kD values for each Falcon
     * @param motor - falcon object 
     */
    private void configMotors(WPI_TalonFX motor){
        TalonFXConfiguration config = new TalonFXConfiguration();
        motor.configAllSettings(config);

        motor.config_kP(Constants.Chassis.kPIDLoopIdx, Constants.Chassis.kP, Constants.Chassis.K_TIMEOUT_MS);
        motor.config_kI(Constants.Chassis.kPIDLoopIdx, Constants.Chassis.kI, Constants.Chassis.K_TIMEOUT_MS);
        motor.config_kD(Constants.Chassis.kPIDLoopIdx, Constants.Chassis.kD, Constants.Chassis.K_TIMEOUT_MS);
        motor.config_kF(Constants.Chassis.kPIDLoopIdx, Constants.Chassis.kF, Constants.Chassis.K_TIMEOUT_MS);

        motor.configNeutralDeadband(0.001);
    }

    /**
     * Calculates desired velocity from joystick inputs and sets to falcons
     * @param leftJoystick - double value between -1.0 and 1.0
     * @param rightJoystick - double value between -1.0 and 1.0
     */
    public void drive(double leftJoystick, double rightJoystick){
        double targetVelocityL = leftJoystick * Constants.Chassis.MAX_RPM_NORMAL * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR;
        double targetVelocityR = rightJoystick * Constants.Chassis.MAX_RPM_NORMAL * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR;
       
        leftLeader.set(TalonFXControlMode.Velocity, targetVelocityL);
        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);

        rightLeader.set(TalonFXControlMode.Velocity, targetVelocityR);
        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
    }
    
    /**
     * Averages left and right joystick inputs and sets calculated desired velocity to falcons
     * @param leftJoystick - double value between -1.0 and 1.0
     * @param rightJoystick - double value between -1.0 and 1.0
     */
    public void driveStraight(double leftJoystick, double rightJoystick){
        double joystickAvg = (leftJoystick + rightJoystick)/2;
        double targetVelocity = joystickAvg * Constants.Chassis.MAX_RPM_NORMAL * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR;
        leftLeader.set(TalonFXControlMode.Velocity, targetVelocity);
        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);

        rightLeader.set(TalonFXControlMode.Velocity, targetVelocity);
        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
    }

    /**
     * Calculates desired velocity (with a slower max RPM)
     * @param leftJoystick - double value between -1.0 and 1.0
     * @param rightJoystick - double value between -1.0 and 1.0
     */
    public void slowMode(double leftJoystick, double rightJoystick){
        double targetVelocityL = leftJoystick * Constants.Chassis.MAX_RPM_SLOW * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR;
        double targetVelocityR = rightJoystick * Constants.Chassis.MAX_RPM_SLOW * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR;
       
        leftLeader.set(TalonFXControlMode.Velocity, targetVelocityL);
        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);

        rightLeader.set(TalonFXControlMode.Velocity, targetVelocityR);
        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
    }

    public void autoAlign(){}     
}