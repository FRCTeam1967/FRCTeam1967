package frc.robot;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
public class KitBotChassis {
    private static final WPI_TalonSRX leftLeader = new WPI_TalonSRX(Constants.LEFT_LEADER);
    private static final WPI_TalonSRX leftFollower = new WPI_TalonSRX(Constants.LEFT_FOLLOWER);
    private static final WPI_TalonSRX rightLeader = new WPI_TalonSRX(Constants.RIGHT_LEADER);
    private static final WPI_TalonSRX rightFollower = new WPI_TalonSRX(Constants.RIGHT_FOLLOWER);
    private static final MotorControllerGroup leftGroup = new MotorControllerGroup(leftFollower, leftLeader);
    private static final MotorControllerGroup rightGroup = new MotorControllerGroup(rightFollower, rightLeader);
    private static final DifferentialDrive myRobot = new DifferentialDrive(leftGroup, rightGroup);
    private Joystick leftJoystick, rightJoystick;
    private double averageSpeed;
    public void driveTank () {
        if (leftJoystick.getRawButton(1) || rightJoystick.getRawButton(1)) {
            averageSpeed = (leftJoystick.getY() + rightJoystick.getY())/2;
            myRobot.tankDrive(averageSpeed, -averageSpeed);
        }else{
            myRobot.tankDrive(-Math.pow(leftJoystick.getY(), 3), Math.pow(rightJoystick.getY(), 3));
        }
    }
}