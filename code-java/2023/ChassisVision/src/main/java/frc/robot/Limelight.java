package frc.robot;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableEntry;
import edu.wpi.first.networktables.NetworkTableInstance;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.Joystick;


public class Limelight{
    private NetworkTable limelightTable;
    private NetworkTableEntry tx, ty, ta; 
    private double x, y, area;

    
    public Limelight(){
        limelightTable = NetworkTableInstance.getDefault().getTable("limelight");
    }

    public void getLimelightValues(){
        tx = limelightTable.getEntry("tx");
        ty = limelightTable.getEntry("ty");
        ta = limelightTable.getEntry("ta");

        x = tx.getDouble(0.0);
        y = ty.getDouble(0.0);
        area = ta.getDouble(0.0);

        SmartDashboard.putNumber("Limelight X-offset", x);
        SmartDashboard.putNumber("Limelight Y-offset", y);
        SmartDashboard.putNumber("Limelight Area", area);
    }

    public void trackingLimelight (DifferentialDrive myRobot, Joystick joystick) {
        getLimelightValues();

        double KpAiming = -0.1;
        double KpDistance = -0.1;
        double minCommand = 0.1;
        double leftCommand = 0.0;
        double rightCommand = 0.0; 
        double leftSteeringAdjust = 0.0;
        double rightSteeringAdjust = 0.0;

        if (joystick.getRawButton(4)) {
            double headingError = -x;
            double steeringAdjust = 0.0f;
            double distanceError = -y;
            if (x > 1.4) {
                leftSteeringAdjust = KpAiming*headingError + minCommand;
            } else if (x < 1.4) {
                rightSteeringAdjust = KpAiming*headingError - minCommand;
            }

            double distanceAdjust = KpDistance * distanceError;
            leftCommand += leftSteeringAdjust;
            rightCommand += rightSteeringAdjust;
            
            myRobot.tankDrive(leftCommand, rightCommand);
        } 
    } 
    

}