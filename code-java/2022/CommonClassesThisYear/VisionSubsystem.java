package org.janksters.CommonClassesThisYear;
import org.janksters.jankyLib.JankyTask;

import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableInstance;
import edu.wpi.first.math.controller.PIDController;

public class VisionSubsystem extends JankyTask{

    PIDController pidDistance = new PIDController(0.1, 0, 0);
    PIDController pidAngle = new PIDController(0.1, 0, 0);

    private boolean limelightHasValidTarget = false;
    private double distance;
    private double distanceInches;
    private double angle;
    private NetworkTable table;
    private double tx;
    private double ty;
    private double tv;
    private double ta;
    private double distanceError;

    public VisionSubsystem(){
        table = NetworkTableInstance.getDefault().getTable("limelight");
        tx = 0.0;
        ty = 0.0;
        tv = 0.0;
        ta = 0.0;
        distance = 0.0;
        distanceInches = 0.0;
        distanceError = 0.0;
        start();
    }

    public void Run(){
        //tx = table.getEntry("tx").getDouble(0);
        //ty = table.getEntry("ty").getDouble(0);
        SmartDashboard.putString("Run", "Run started");
        updateLimelightTracking();
        distanceInches = calcRealDistance();
        distance = pidCalcDistance(); //returns the power the robot needs to reach setpoint
        angle = pidCalcAngle();
        distanceError = distanceInches - 20;

        SmartDashboard.putBoolean("Limelight Has Valid Target", limelightHasValidTarget);
        SmartDashboard.putNumber("Limelight Horizontal offset", tx);
        SmartDashboard.putNumber("Limelight Vertical offset ", ty);
        SmartDashboard.putNumber("Limelight Distance ", getDistance()); 
    }

     /*
        * tv- Whether the limelight has any valid targets (0 or 1)
        * tx- Horizontal offset from crosshair to target (LL2 -29.8 to 29.8)
        * ty- Vertical offset from crosshair to target (LL2 -24.85 to 24.85)
        * ta- Target area (0% of image to 100% of image)
        */

    private void updateLimelightTracking(){        
        //checks whether the limelight has a valid target

        tv = table.getEntry("tv").getDouble(0);

        if (tv < 1.0) {
        limelightHasValidTarget = false;
        } else {
        limelightHasValidTarget = true;
        }
    }
    private double pidCalcAngle(){
        tx = table.getEntry("tx").getDouble(0);
        return pidAngle.calculate(tx, 0);
      }

    private double pidCalcDistance(){
        return pidDistance.calculate(distanceInches, 39.3701); //Current distance, setpoint (desired distance from target)
        //get rid of pidcontrol in Vision
    }

    private double calcRealDistance(){
        //calculates robot's distance to target
        ta = table.getEntry("ta").getDouble(0);
        distanceInches = (40.4 * Math.pow(ta, -0.795));
        return distanceInches;
    }

    public double getDistance(){
        return distance;
    }
    public double getAngle(){
        return angle;
    }
    public double xOffset(){
        double offset = table.getEntry("tx").getDouble(0);
        return offset;
    }
    public boolean targetValid(){
        return limelightHasValidTarget;
    }

    public double getDistanceInches(){
        return distanceInches;
    }
  
    public double getDistanceError(){
        return distanceError;
    }
 
    
}
