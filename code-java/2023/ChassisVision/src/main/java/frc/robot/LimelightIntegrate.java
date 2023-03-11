package frc.robot;

import edu.wpi.first.cameraserver.CameraServer;
import edu.wpi.first.cscore.HttpCamera;
import edu.wpi.first.cscore.HttpCamera.HttpCameraKind;
import edu.wpi.first.networktables.GenericEntry;
import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableEntry;
import edu.wpi.first.networktables.NetworkTableInstance;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;

public class LimelightIntegrate{

    //limelight
    private HttpCamera limelightCamera;
    private NetworkTable limelightTable;
    private NetworkTableEntry tx, ty;
    private double x, y;

    //adjustment fields
    private double angleError, distanceError;
    private double KpAiming, KpDistance;
    private double minCommand;
    private double steeringAdjust, distanceAdjust;
    private double leftCommand, rightCommand;

    //shuffleboard
    GenericEntry lime_KpAiming, lime_KpDistance, lime_minCommand;

    public LimelightIntegrate(){
        //define limelight network table + get offset values
        limelightTable = NetworkTableInstance.getDefault().getTable("limelight");
        tx = limelightTable.getEntry("tx");
        ty = limelightTable.getEntry("ty");

        x = tx.getDouble(0.0);
        y = ty.getDouble(0.0);
    }

    public void configLimelightTab(ShuffleboardTab limelightTab){        
        limelightCamera = new HttpCamera("Limelight Camera", "http://10.19.67.11:5800/stream.mjpg", HttpCameraKind.kMJPGStreamer);
        //limelightCamera.setVideoMode(PixelFormat.kMJPEG,320, 240, 45);

        limelightCamera.setResolution(320, 240);
        limelightCamera.setFPS(30);

        //CameraServer.addCamera(limelightCamera);
        CameraServer.startAutomaticCapture(limelightCamera);

        //test two different methods to hopefully add stream to Shuffleboard
        limelightTab.add("Limelight Cam 1", limelightCamera);
        limelightTab.addCamera("Limelight Cam 2","limelight", "http://10.19.67.11:5800/stream.mjpg");
        
        //other Limelight Tab Values
        limelightTab.addDouble("Limelight tx", () -> x);
        limelightTab.addDouble("Limelight ty", () -> y);

        //adjust limelight vals for testing
        lime_KpAiming = limelightTab.add("Kp Aiming", -0.02).getEntry();
        lime_KpDistance = limelightTab.add("Kp Distance", -0.02).getEntry();
        lime_minCommand = limelightTab.add("Min Command (Angle)", 0.2).getEntry();
    }

    public void getShuffleboardValues(){
        KpAiming = lime_KpAiming.getDouble(-0.02); 
        KpDistance = lime_KpDistance.getDouble(-0.02);
        minCommand = lime_minCommand.getDouble(0.2);
    }

    /**
     * X-offset range = -29.8 to 29.8 degrees
     * Y-offset range = -24.85 to 24.85 degrees
     * 
     * minCommand = If the error is larger than some threshold, 
     * just add a constant to your motor command which roughly 
     * represents the minimum amount of power needed for the robot to actually move 
     * minCommand = 0.15 is amount of power needed for bot to move (not 0.1)
     * 
     * Max kP = -0.033 (to not have x-offset heading error exceed value of 0.1)
     * 
     * Braindump when brain is mush:
     * Robot to the left = -x offset = + heading error = left command needed to move forward, right to move back
     * Robot to the right = +x offset = - heading error = right command forward, left backwards
     */

    public void angleAdjust(){
        angleError = -x;
        // KpAiming = -0.02;
        // minCommand = 0.2;
        steeringAdjust = 0.0;

        leftCommand = 0.0;
        rightCommand = 0.0; 

        steeringAdjust = KpAiming * angleError;

        if (x < -0.5){ //heading error is positive, steering adjust will be positive, need to turn right, left has to move forward, right backwards
            steeringAdjust -= minCommand;
            leftCommand += steeringAdjust;
            rightCommand -= steeringAdjust;
        } else if (x > 0.5) { // heading error is negative, steering adjust will be negative, right has to move forward, left back
            steeringAdjust += minCommand; 
            rightCommand += steeringAdjust;
            leftCommand -= steeringAdjust; // assuming no motors are inverted
        }
    }

    public void distanceAdjust(){
        distanceError = -y;
        // KpDistance = -0.02;

        distanceAdjust = 0.0;

        leftCommand = 0.0;
        rightCommand = 0.0;
        
        distanceAdjust = KpDistance * distanceError;

        leftCommand = distanceAdjust;
        rightCommand = -distanceAdjust;
    }

    public double getLeftCommand(){
        return leftCommand;
    }

    public double getRightCommand(){
        return rightCommand;
    }


}
