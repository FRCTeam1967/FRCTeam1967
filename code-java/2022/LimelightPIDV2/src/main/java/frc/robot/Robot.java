// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

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

/**
 * The VM is configured to automatically run this class, and to call the functions corresponding to
 * each mode, as described in the TimedRobot documentation. If you change the name of this class or
 * the package after creating this project, you must also update the build.gradle file in the
 * project.
 */
public class Robot extends TimedRobot {
  private static final String kDefaultAuto = "Default";
  private static final String kCustomAuto = "My Auto";
  private String m_autoSelected;
  private final SendableChooser<String> m_chooser = new SendableChooser<>();

  private WPI_TalonSRX m_leftLeader = new WPI_TalonSRX(0);
  private MotorController m_leftFollower = new PWMVictorSPX(3);
  private WPI_TalonSRX m_rightLeader = new WPI_TalonSRX(1);
  private MotorController m_rightFollower = new PWMVictorSPX(4);
  private MotorControllerGroup m_left = new MotorControllerGroup(m_leftLeader, m_leftFollower);
  private MotorControllerGroup m_right = new MotorControllerGroup(m_rightLeader, m_rightFollower);
  private DifferentialDrive m_myRobot = new DifferentialDrive(m_left,m_right);
  PIDController pidDistance = new PIDController(0.1, 0, 0);
  PIDController pidAngle = new PIDController(0.1, 0, 0);

  private XboxController m_Controller = new XboxController(2);
  private boolean m_LimelightHasValidTarget = false;

  private Joystick m_leftStick = new Joystick(0);
  private Joystick m_rightStick = new Joystick(1);

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);

    m_rightLeader.setInverted(true); //Is this causing me issues?
  }

  /**
   * This function is called every robot packet, no matter the mode. Use this for items like
   * diagnostics that you want ran during disabled, autonomous, teleoperated and test.
   *
   * <p>This runs after the mode specific periodic functions, but before LiveWindow and
   * SmartDashboard integrated updating.
   */
  @Override
  public void robotPeriodic() {}

  /**
   * This autonomous (along with the chooser code above) shows how to select between different
   * autonomous modes using the dashboard. The sendable chooser code works with the Java
   * SmartDashboard. If you prefer the LabVIEW Dashboard, remove all of the chooser code and
   * uncomment the getString line to get the auto name from the text box below the Gyro
   *
   * <p>You can add additional auto modes by adding additional comparisons to the switch structure
   * below with additional strings. If using the SendableChooser make sure to add them to the
   * chooser code above as well.
   */
  @Override
  public void autonomousInit() {
    m_autoSelected = m_chooser.getSelected();
    // m_autoSelected = SmartDashboard.getString("Auto Selector", kDefaultAuto);
    System.out.println("Auto selected: " + m_autoSelected);
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {
    switch (m_autoSelected) {
      case kCustomAuto:
        // Put custom auto code here
        break;
      case kDefaultAuto:
      default:
        // Put default auto code here
        break;
    }
  }

  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {}

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() 
  {
    updateLimelightTracking();
    
    NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
    double tx = table.getEntry("tx").getDouble(0);
    double ty = table.getEntry("ty").getDouble(0);
    double ta = table.getEntry("ta").getDouble(0);
    double distance = 156.15637194655 - (36.746837632824 * Math.log(ta));

    boolean auto = m_Controller.getAButton();
    SmartDashboard.putBoolean("Limelight Has Valid Target", m_LimelightHasValidTarget);
    SmartDashboard.putNumber("Limelight Horizontal offset", tx);
    SmartDashboard.putNumber("Limelight Vertical offset ", ty);
    SmartDashboard.putNumber("Limelight Distance ", distance);

    SmartDashboard.putNumber("left Y", m_leftStick.getY());
    SmartDashboard.putNumber("right Y", m_rightStick.getY());
    
    if (auto)
    {
      if (m_LimelightHasValidTarget)
      {
        //Get distance, get angle 
        //distance-angle, distance+angle 
        //m_myRobot.tankDrive(-pidCalcAngle(), pidCalcAngle());
        m_myRobot.tankDrive(pidCalcDistance(), pidCalcDistance()); //Call pidCalcAngle() subtract from one side, add to another
      } else {
        m_myRobot.tankDrive(0.0, 0.0); //Try Seeking here 
      } 
    } else {
      /** Use joysticks to drive if A not pressed */
      m_myRobot.tankDrive(1.0 * m_leftStick.getY(), 1.0 * m_rightStick.getY());
    }

  }

  /** This function is called once when the robot is disabled. */
  @Override
  public void disabledInit() {}

  /** This function is called periodically when disabled. */
  @Override
  public void disabledPeriodic() {}

  /** This function is called once when test mode is enabled. */
  @Override
  public void testInit() {}

  /** This function is called periodically during test mode. */
  @Override
  public void testPeriodic() {}

  /**
   * This function implements a simple method of updating tracking data from a limelight camera.
  */
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

  /**
   * This function manually adjusts drivetrain to target using limelight 
  */
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
    
    /**
     * This function adjusts drivetrain to target using limelight and PID 
     * (both drive and steering adjust into one adjustment value)
    */
    public void driveAssistPID(){
      double KpAim = -0.07;
      double KpDistance = -0.1;
      double min_aim_command = 0.05;
      
      NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
      double tx = table.getEntry("tx").getDouble(0);
      double ty = table.getEntry("ty").getDouble(0);

      double heading_error = -tx;
      double distance_error = -ty;
      double steering_adjust = 0.0;

      double left_command = 0.0;
      double right_command = 0.0;

      if (tx > 1.0){
        steering_adjust = KpAim*heading_error - min_aim_command;
      } else if (tx < 1.0) {
        steering_adjust = KpAim*heading_error + min_aim_command;
      }

      double distance_adjust = KpDistance * distance_error;

      left_command += steering_adjust+distance_adjust;
      right_command -= steering_adjust+distance_adjust;

      m_myRobot.tankDrive(left_command, right_command);
    }

     /**
     * This function adjusts drivetrain to target using limelight and PID 
     * (only distance adjust)
    */
    public void distAssistPID(boolean forwardDrive){
      double KpDistance = -0.1;
      double left_command = 0.0;
      double right_command = 0.0;

      NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
      double ty = table.getEntry("ty").getDouble(0);

      double distance_adjust = KpDistance * ty;

      left_command += distance_adjust;
      right_command += distance_adjust;
      
      if (forwardDrive){
        m_myRobot.tankDrive(-left_command, -right_command);
      } else {
        m_myRobot.tankDrive(left_command, right_command);
      }
    }

    public void combineAssistPID(){
      NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");

      double tx = table.getEntry("tx").getDouble(0);
      double ty = table.getEntry("ty").getDouble(0);
      double ta = table.getEntry("ta").getDouble(0);
      double distance = 156.15637194655 - (36.746837632824 * Math.log(ta));

      if (tx < 0){
        // turn right
          driveAssistPID();
        }  
        else if (tx > 15){ // why is this 15 instead of greater than
        // turn left
          driveAssistPID();
        }
        else {
          if (distance > 80 && distance < 1000) {
            distAssistPID(true);
            SmartDashboard.putBoolean("distance > 40", true);
          }
          else if (distance < 60) {
            distAssistPID(false);
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
  }
