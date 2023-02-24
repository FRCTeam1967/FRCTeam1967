package frc.robot;
import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableEntry;
import edu.wpi.first.networktables.NetworkTableInstance;
//import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.math.kinematics.DifferentialDriveKinematics;
import edu.wpi.first.math.util.Units;
import edu.wpi.first.wpilibj.ADIS16470_IMU; 
import edu.wpi.first.math.estimator.DifferentialDrivePoseEstimator; 



public class Limelight{
    private NetworkTable limelightTable;
    private NetworkTableEntry tx, ty, ta, tshort, tlong, thor, tvert; 
    private double x, y, shortLength, longLength, area, horLength, verLength;
    private double leftCommand, rightCommand;

    private ADIS16470_IMU gyro = new ADIS16470_IMU();
    private DifferentialDrivePoseEstimator poseEstimator;

    public Limelight(){
        limelightTable = NetworkTableInstance.getDefault().getTable("limelight");
    }


    // poseEstimator = new DifferentialDrivePoseEstimator (
    //     DifferentialDriveKinematics Constants.Chassis.DRIVE_KINEMATICS, 
    //     Rotation2d gyro.getAngle(), 
    //     double 0.4, 
    //     double 0.4, 
    //     new Pose2d);

    
    public void getLimelightValues(){
        tx = limelightTable.getEntry("tx");
        ty = limelightTable.getEntry("ty");
        ta = limelightTable.getEntry("ta");

        tshort = limelightTable.getEntry("tshort");
        tlong = limelightTable.getEntry("tlong");
        thor = limelightTable.getEntry("thor");
        tvert = limelightTable.getEntry("tvert");

        x = tx.getDouble(0.0);
        y = ty.getDouble(0.0);
        shortLength = tshort.getDouble(0.0);
        longLength = tlong.getDouble(0.0);
        area = ta.getDouble(0.0);
        horLength = thor.getDouble(0.0);
        verLength = tvert.getDouble(0.0);

        SmartDashboard.putNumber("Limelight X-offset", x);
        SmartDashboard.putNumber("Limelight Y-offset", y);
        SmartDashboard.putNumber("Limelight Area", area);
        SmartDashboard.putNumber("Limelight length of shortest side", shortLength);
        SmartDashboard.putNumber("Limelight length of longest side", longLength);
        SmartDashboard.putNumber("Limelight length of horizontal side", horLength);
        SmartDashboard.putNumber("Limelight length of vertical side", verLength);

    }
   

    public double getRatio(){
        //boolean targetRatio = false; //if target ratio is reached/=true, robot will stop moving
        getLimelightValues();

        double ratio = verLength/horLength *100; //height / width

        //optimal & minimum ratio: 1 
        //size of ratio is will tell robot how much to swoop: bigger ratio, need to swoop more
        //x-offset will tell in which direction to turn: negative --> right, positive --> left
        //after camera is aligned with target, then it checks it for y value
        //ratios: (head on values) --> 0.97 - 1.02 

        SmartDashboard.putNumber("ratio", ratio);
        SmartDashboard.putNumber("height", verLength);
        SmartDashboard.putNumber("width", horLength);
        return ratio;
    }

    public void trackingLimelight () {
        getLimelightValues();
       // getRatio();

        double KpAiming = -0.05;
        double KpDistance = -0.05;

        double minCommand = 0.06;
        double steeringAdjust = 0.0;
        double distanceAdjust = 0.0;
        double headingError = -x;
        double distanceError = -y;

        leftCommand = 0.0;
        rightCommand = 0.0;

        if (x > 1.0){
            steeringAdjust = KpAiming*headingError - minCommand;
            leftCommand = -steeringAdjust;
            rightCommand = steeringAdjust;
        } else if (x < -1.0) {
            steeringAdjust = KpAiming*headingError + minCommand;
            leftCommand = -steeringAdjust;
            rightCommand = steeringAdjust;
        } else {
            distanceAdjust = KpDistance * distanceError;
            leftCommand = -distanceAdjust;
            rightCommand = distanceAdjust;
        }


        /*distanceAdjust = KpDistance * distanceError;

        leftCommand -= steeringAdjust + distanceAdjust;
        rightCommand += steeringAdjust + distanceAdjust;*/
    } 

    public double getLeftCommand(){
        return leftCommand;
    }

    public double getRightCommand(){
        return rightCommand;
    }

    public double getDist(){  //from limelight documentation, finding distance from limelight to goal.
        NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
        NetworkTableEntry ty = table.getEntry("ty");
        double targetOffsetAngle_Vertical = ty.getDouble(0.0);

        // how many degrees back is your limelight rotated from perfectly vertical?
        double limelightMountAngleDegrees = 15.0;

        // distance from the center of the Limelight lens to the floor
        double limelightLensHeightInches = 14.0;

        // distance from the target to the floor
        double goalHeightInches = 14.0;

        double angleToGoalDegrees = limelightMountAngleDegrees + targetOffsetAngle_Vertical;
        double angleToGoalRadians = angleToGoalDegrees * (3.14159 / 180.0);

        //calculate distance
        double distanceFromLimelightToGoalInches = (goalHeightInches - limelightLensHeightInches)/Math.tan(angleToGoalRadians);
        return distanceFromLimelightToGoalInches;
    }

   

}