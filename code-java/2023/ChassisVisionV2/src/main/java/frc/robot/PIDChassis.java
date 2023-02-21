package frc.robot;

import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXInvertType;
import com.ctre.phoenix.motorcontrol.can.TalonFXConfiguration;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import edu.wpi.first.networktables.GenericEntry;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;

public class PIDChassis implements DriveSystem{
    //motors
    private WPI_TalonFX leftLeader, leftFollower, rightLeader, rightFollower;

    private ShuffleboardTab m_pidChassisTab;
    GenericEntry chassis_kP, chassis_kI, chassis_kD, chassis_kF;
    private double kP, kI, kD, kF;

    /**
     * Constructor for PID Chassis
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

    public void checkJoystickDeadband(double leftJoystick, double rightJoystick){
        if (leftJoystick < 0.1 && leftJoystick > -0.1){
            leftJoystick = 0;
        }

        if (rightJoystick < 0.1 && rightJoystick > -0.1){
            rightJoystick = 0;
        }
    }

    public void configDashboard(){
        m_pidChassisTab = Shuffleboard.getTab("PID Chassis");
        
        chassis_kP = m_pidChassisTab.add("kP", 0.061574).getEntry();
        chassis_kI = m_pidChassisTab.add("kI", 0.0).getEntry();
        chassis_kD = m_pidChassisTab.add("kD", 0.0).getEntry();
        chassis_kF = m_pidChassisTab.add("kF", 0.0).getEntry();
    }
    
    private void configMotors(WPI_TalonFX motor){
        TalonFXConfiguration config = new TalonFXConfiguration();
        motor.configAllSettings(config);

        motor.config_kP(Constants.Chassis.kPIDLoopIdx, kP, Constants.Chassis.K_TIMEOUT_MS);
        motor.config_kI(Constants.Chassis.kPIDLoopIdx, kI, Constants.Chassis.K_TIMEOUT_MS);
        motor.config_kD(Constants.Chassis.kPIDLoopIdx, kD, Constants.Chassis.K_TIMEOUT_MS);
        motor.config_kF(Constants.Chassis.kPIDLoopIdx, kF, Constants.Chassis.K_TIMEOUT_MS);

        motor.configNeutralDeadband(0.001);
    }
    
    public void updateTuningValues(){        
        kP = chassis_kP.getDouble(0.061574);
        kI = chassis_kI.getDouble(0.0);
        kD = chassis_kD.getDouble(0.0);
        kF = chassis_kF.getDouble(0.0);
        
        leftLeader.config_kP(Constants.Chassis.kPIDLoopIdx, kP, Constants.Chassis.K_TIMEOUT_MS);
        leftLeader.config_kI(Constants.Chassis.kPIDLoopIdx, kI, Constants.Chassis.K_TIMEOUT_MS);
        leftLeader.config_kD(Constants.Chassis.kPIDLoopIdx, kD, Constants.Chassis.K_TIMEOUT_MS);
        leftLeader.config_kF(Constants.Chassis.kPIDLoopIdx, kF, Constants.Chassis.K_TIMEOUT_MS);

        leftFollower.config_kP(Constants.Chassis.kPIDLoopIdx, kP, Constants.Chassis.K_TIMEOUT_MS);
        leftFollower.config_kI(Constants.Chassis.kPIDLoopIdx, kI, Constants.Chassis.K_TIMEOUT_MS);
        leftFollower.config_kD(Constants.Chassis.kPIDLoopIdx, kD, Constants.Chassis.K_TIMEOUT_MS);
        leftFollower.config_kF(Constants.Chassis.kPIDLoopIdx, kF, Constants.Chassis.K_TIMEOUT_MS);

        rightLeader.config_kP(Constants.Chassis.kPIDLoopIdx, kP, Constants.Chassis.K_TIMEOUT_MS);
        rightLeader.config_kI(Constants.Chassis.kPIDLoopIdx, kI, Constants.Chassis.K_TIMEOUT_MS);
        rightLeader.config_kD(Constants.Chassis.kPIDLoopIdx, kD, Constants.Chassis.K_TIMEOUT_MS);
        rightLeader.config_kF(Constants.Chassis.kPIDLoopIdx, kF, Constants.Chassis.K_TIMEOUT_MS);

        rightFollower.config_kP(Constants.Chassis.kPIDLoopIdx, kP, Constants.Chassis.K_TIMEOUT_MS);
        rightFollower.config_kI(Constants.Chassis.kPIDLoopIdx, kI, Constants.Chassis.K_TIMEOUT_MS);
        rightFollower.config_kD(Constants.Chassis.kPIDLoopIdx, kD, Constants.Chassis.K_TIMEOUT_MS);
        rightFollower.config_kF(Constants.Chassis.kPIDLoopIdx, kF, Constants.Chassis.K_TIMEOUT_MS);
    }

    public void drive(double leftJoystick, double rightJoystick){
        leftLeader.set(TalonFXControlMode.Velocity, leftJoystick * Constants.Chassis.MAX_RPM_NORMAL * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR);
        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);

        rightLeader.set(TalonFXControlMode.Velocity, rightJoystick * Constants.Chassis.MAX_RPM_NORMAL * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR);
        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
    }

    public void driveStraight(double leftJoystick, double rightJoystick){
        double joystickAvg = (leftJoystick + rightJoystick)/2;

        leftLeader.set(TalonFXControlMode.Velocity, joystickAvg * Constants.Chassis.MAX_RPM_NORMAL * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR);
        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);

        rightLeader.set(TalonFXControlMode.Velocity, joystickAvg * Constants.Chassis.MAX_RPM_NORMAL * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR);
        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
    }

    public void slowMode(double leftJoystick, double rightJoystick){
        leftLeader.set(TalonFXControlMode.Velocity, leftJoystick * Constants.Chassis.MAX_RPM_SLOW * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR);
        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);

        rightLeader.set(TalonFXControlMode.Velocity, rightJoystick * Constants.Chassis.MAX_RPM_SLOW * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR);
        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
    }

    public void fastMode(double leftJoystick, double rightJoystick){
        leftLeader.set(TalonFXControlMode.Velocity, leftJoystick * Constants.Chassis.MAX_RPM_FAST * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR);
        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);

        rightLeader.set(TalonFXControlMode.Velocity, rightJoystick * Constants.Chassis.MAX_RPM_FAST * Constants.Chassis.JOYSTICK_TO_UNITSPER100MS_FACTOR);
        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
    }

    public void autoAlign(){
        
    }

}
