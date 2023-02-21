package frc.robot;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.TalonFXInvertType;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import edu.wpi.first.networktables.GenericEntry;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;

public class SimpleChassis implements DriveSystem {
    //motors
    private WPI_TalonFX leftLeader, leftFollower, rightLeader, rightFollower;
    private MotorControllerGroup leftGroup, rightGroup;
    private DifferentialDrive myRobot;

    //shuffleboard fields
    private ShuffleboardTab m_simpleChassisTab;
    GenericEntry slowModeEntry, joystickEntry;
    private double slowModeFactor;
    private int joystickExp;

    /**
     * Constructor for Simple Chassis
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

    public void checkJoystickDeadband(double leftJoystick, double rightJoystick){
        if (leftJoystick < 0.1 && leftJoystick > -0.1){
            leftJoystick = 0;
        }

        if (rightJoystick < 0.1 && rightJoystick > -0.1){
            rightJoystick = 0;
        }
    }

    public void configDashboard(){
        m_simpleChassisTab = Shuffleboard.getTab("Simple Chassis");
        
        slowModeEntry = m_simpleChassisTab.add("Slow Mode Factor", 0.5).getEntry();
        joystickEntry = m_simpleChassisTab.add("Joystick Exponent", 1).getEntry();
    }

    public void updateTuningValues(){
        slowModeFactor = slowModeEntry.getDouble(0.5);
        joystickExp = (int) joystickEntry.getInteger(1);
    }
        
    public void drive(double leftJoystick, double rightJoystick){
        myRobot.tankDrive(Math.pow(leftJoystick,joystickExp), Math.pow(rightJoystick,joystickExp));
    }

    public void driveStraight(double leftJoystick, double rightJoystick){
        double joystickAvg = (leftJoystick + rightJoystick)/2;
        myRobot.tankDrive(Math.pow(joystickAvg,joystickExp), Math.pow(joystickAvg,joystickExp));
    }

    public void slowMode(double leftJoystick, double rightJoystick){
        myRobot.tankDrive(Math.pow((leftJoystick*slowModeFactor), joystickExp), Math.pow((rightJoystick*slowModeFactor), joystickExp));
    }

    public void fastMode(double leftJoystick, double rightJoystick){
        myRobot.tankDrive(Math.pow(leftJoystick,joystickExp), Math.pow(rightJoystick,joystickExp));
    }

    public void autoAlign(){
        
    }

    
}
