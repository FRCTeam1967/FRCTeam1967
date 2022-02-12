package com.janksters.robotcode;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableInstance;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.XboxController;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorController;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.motorcontrol.PWMVictorSPX;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.math.controller.PIDController;



public class PID {

    boolean m_LimelightHasValidTarget; 
    int pidDistance;
    int pidAngle;
    DifferentialDrive m_myRobot = m_myRobot; 

    public void manualLimelightAdjust(){
        NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
    
        double tx = table.getEntry("tx").getDouble(0);
        double ty = table.getEntry("ty").getDouble(0);
        double ta = table.getEntry("ta").getDouble(0);
        double distance = 156.15637194655 - (36.746837632824 * Math.log(ta));
        
        if (tx < -10){
          // turn right
            m_myRobot.tankDrive (-0.5, 0);
          }  
          else if (tx > 10){
          // turn left
            m_myRobot.tankDrive (0,-0.5);
          }
          else {
            if (distance > 70 && distance < 1000) {
              m_myRobot.tankDrive(-0.5, -0.5);
              SmartDashboard.putBoolean("distance > 40", true);
            }
            else if (distance < 60) {
              m_myRobot.tankDrive(0.5, 0.5);
              SmartDashboard.putBoolean("distance > 40", false);
            }
            else {
              m_myRobot.tankDrive(0,0);
              SmartDashboard.putBoolean("distance > 40", false);
            }
          }
        }

    public double pidCalcDistance(){
        NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
        double tx = table.getEntry("tx").getDouble(0);
        double ta = table.getEntry("ta").getDouble(0);
        double distance = 156.15637194655 - (36.746837632824 * Math.log(ta)); //Unit- centimeters 
  
        return pidDistance.calculate(distance, 100); //Current distance, setpoint (desired distance from target)
      }
  
      public double pidCalcAngle(){
        NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
        double tx = table.getEntry("tx").getDouble(0);
  
        return pidAngle.calculate(tx, 0);
      }

      public void updateLimelightTracking(){
        NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
        
        /**
          * tv- Whether the limelight has any valid targets (0 or 1)
          * tx- Horizontal offset from crosshair to target (LL2 -29.8 to 29.8)
          * ty- Vertical offset from crosshair to target (LL2 -24.85 to 24.85)
          * ta- Target area (0% of image to 100% of image)
        */
        double tv = table.getEntry("tv").getDouble(0);
        double tx = table.getEntry("tx").getDouble(0);
        double ty = table.getEntry("ty").getDouble(0);
        double ta = table.getEntry("ta").getDouble(0);
    
        /** Condition returns true when target is not detected- tv is 0 */
        
        if (tv < 1.0)
        {
          m_LimelightHasValidTarget = false;
          return;
        } else {
          m_LimelightHasValidTarget = true;
        }
      }
    
}
