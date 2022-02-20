package org.janksters.CommonClassesThisYear;
import org.janksters.jankyLib.JankyTask;

import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableInstance;
import edu.wpi.first.math.controller.PIDController;

public class VisionSubsystem extends JankyTask{

    PIDController pidDistance = new PIDController(0.1, 0, 0);
    PIDController pidAngle = new PIDController(0.1, 0, 0);

    private boolean m_LimelightHasValidTarget = false;
    private double distance;
    private double angle;
    private NetworkTable table;
    private double tx;
    private double ty;
    private double tv;
    private double ta;


    public VisionSubsystem(){
        table = NetworkTableInstance.getDefault().getTable("limelight");
        tx = 0.0;
        ty = 0.0;
        tv = 0.0;
        ta = 0.0;
        distance = 0.0;
        start();
    }

    public void Run(){
        tx = table.getEntry("tx").getDouble(0);
        ty = table.getEntry("ty").getDouble(0);
        
        updateLimelightTracking();

        distance = pidCalcDistance();
        angle = pidCalcAngle();

        SmartDashboard.putBoolean("Limelight Has Valid Target", m_LimelightHasValidTarget);
        SmartDashboard.putNumber("Limelight Horizontal offset", tx);
        SmartDashboard.putNumber("Limelight Vertical offset ", ty);
        SmartDashboard.putNumber("Limelight Distance ", distance);        
    }

    private void updateLimelightTracking(){        
        /*
        * tv- Whether the limelight has any valid targets (0 or 1)
        * tx- Horizontal offset from crosshair to target (LL2 -29.8 to 29.8)
        * ty- Vertical offset from crosshair to target (LL2 -24.85 to 24.85)
        * ta- Target area (0% of image to 100% of image)
        */

        tv = table.getEntry("tv").getDouble(0);

        if (tv < 1.0) {
        m_LimelightHasValidTarget = false;
        } else {
        m_LimelightHasValidTarget = true;
        }
    }
    private double pidCalcAngle(){
        tx = table.getEntry("tx").getDouble(0);
        return pidAngle.calculate(tx, 0);
      }

    private double pidCalcDistance(){
        ta = table.getEntry("ta").getDouble(0);
        distance = 156.15637194655 - (36.746837632824 * Math.log(ta)); //Unit- centimeters 
        System.out.println("distance" + distance);
        return pidDistance.calculate(distance, 100); //Current distance, setpoint (desired distance from target)
        //get rid of pidcontrol in Vision
    }

    public double getDistance(){
        return distance;
    }
    public double getAngle(){
        return angle;
    }
    public double xOffset(){
        return tx;
    }
    public boolean targetValid(){
        return m_LimelightHasValidTarget;
    }
    
}
