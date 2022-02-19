// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;
import org.janksters.CommonClassesThisYear.VisionSubsystem;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
// import edu.wpi.first.wpilibj.motorcontrol.MotorController;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
// import edu.wpi.first.wpilibj.motorcontrol.PWMVictorSPX;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
// import com.ctre.phoenix.motorcontrol.can.TalonSRX;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
// import edu.wpi.first.wpilibj.GenericHID;
// import edu.wpi.first.wpilibj.TimedRobot;
// import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
// import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
//import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import edu.wpi.first.math.controller.PIDController;
// import edu.wpi.first.wpilibj.XboxController;
// import com.ctre.phoenix.sensors.CANCoder;
// import edu.wpi.first.wpilibj.drive.RobotDriveBase;
// import edu.wpi.first.networktables.NetworkTable;
// import edu.wpi.first.networktables.NetworkTableInstance;
// import edu.wpi.first.networktables.NetworkTableEntry;

/**
 * The VM is configured to automatically run this class, and to call the functions corresponding to
 * each mode, as described in the TimedRobot documentation. If you change the name of this class or
 * the package after creating this project, you must also update the build.gradle file in the
 * project.
 */
public class Robot extends TimedRobot {
  private static final String kTankDrive = "Tank Drive";
  private static final String kArcadeDrive = "arcade Drive";
  private static final String kCurvatureDrive = "Curvature Drive";
  private static final String kSteeringWheel = "Steering Wheel";

  //private static final String kJankyBot2022 = "2022 Janky Bot";
  //private static final String kLazlo = "Chassis In a Day - Lazlo";

  private String m_DriveSelected;
  //private String m_ChassisSelected;
  private final SendableChooser<String> m_driveChooser = new SendableChooser<>();
  //private final SendableChooser<String> m_chassisChooser = new SendableChooser<>();

  Joystick m_arcadeJoystickP1;
  Joystick m_arcadeJoystickP2;
  double speed;

  private boolean m_LimelightHasValidTarget = false;


  //Chassis in a Day - Lazlo
    /*WPI_TalonSRX m_leftLeader = new WPI_TalonSRX(2);
    MotorController m_leftFollower = new PWMVictorSPX(3);
    WPI_TalonFX m_rightLeader = new WPI_TalonFX(1);
    MotorController m_rightFollower = new PWMVictorSPX(0);*/

  //2022 Janky
    private WPI_TalonSRX m_leftLeader = new WPI_TalonSRX(2);//m2
    private WPI_TalonFX m_leftFollower = new WPI_TalonFX(3);//m3
    private WPI_TalonFX m_rightLeader = new WPI_TalonFX(1);//m1
    private WPI_TalonSRX m_rightFollower = new WPI_TalonSRX(0);//m0

    private MotorControllerGroup m_left = new MotorControllerGroup(m_leftLeader, m_leftFollower);
    private MotorControllerGroup m_right = new MotorControllerGroup(m_rightLeader, m_rightFollower);
    DifferentialDrive m_myRobot = new DifferentialDrive(m_left,m_right);
    //DifferentialDrive m_myRobot = new DifferentialDrive(m_leftFollower,m_rightLeader);

  PIDController pidDistance = new PIDController(0.1, 0, 0);
  PIDController pidAngle = new PIDController(0.1, 0, 0);

  VisionSubsystem limeLight = new VisionSubsystem();


  //encoders
  final int PULSES_PER_REVOLUTION = 4096;
  final double WHEEL_DIAMETER = 6; // 0.1524 meters, 6 inches
  final double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * Math.PI;

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    System.out.println("Hello!");
    m_driveChooser.setDefaultOption("Tank Drive", kTankDrive);
    m_driveChooser.addOption("Arcade Drive", kArcadeDrive);
    m_driveChooser.addOption("Curvature Drive", kCurvatureDrive);
    m_driveChooser.addOption("Steering Wheel", kSteeringWheel);

    SmartDashboard.putData("Drive choices", m_driveChooser);

    m_arcadeJoystickP1 = new Joystick(0);  //whatever is in port 1 - P1 stands for port1
    m_arcadeJoystickP2 = new Joystick(1); //whatever is in port  2

    m_myRobot.setMaxOutput(0.5); //default is 1

    // m_rightLeader.configFactoryDefault();
    // m_rightLeader.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute,0,0);
    // m_rightLeader.setSelectedSensorPosition(0, 10, 0);
    // m_leftFollower.configFactoryDefault();
    // m_leftFollower.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute,0,0);
    // m_leftFollower.setSelectedSensorPosition(0, 10, 0);
    System.out.println("Passed through initialization");

    //m_chassisChooser.setDefaultOption("Chassis in a day", kLazlo);
    //m_chassisChooser.addOption("Janky bot 2022", kJankyBot2022);

    //SmartDashboard.putData("Chassis Choices", m_chassisChooser);

    /*m_leftLeader = new WPI_TalonSRX(2);
    m_leftFollower = new WPI_TalonFX(3);
    m_rightLeader = new WPI_TalonFX(1);
    m_rightFollower = new WPI_TalonSRX(0);*/

  }

  /**
   * This function is called every robot packet, no matter the mode. Use this for items like
   * diagnostics that you want ran during disabled, autonomous, teleoperated and test.
   *
   * <p>This runs after the mode specific periodic functions, but before LiveWindow and
   * SmartDashboard integrated updating.
   */
  @Override
  public void robotPeriodic() {
    /*m_ChassisSelected = m_chassisChooser.getSelected();
    switch(m_ChassisSelected){
      case kLazlo: 
        m_leftLeader = new WPI_TalonSRX(2);
        m_leftFollower = new WPI_TalonFX(3);
        m_rightLeader = new WPI_TalonFX(1);
        m_rightFollower = new WPI_TalonSRX(0);
        break;
      default: 
        m_leftLeader = new WPI_TalonSRX(0);
        m_leftFollower = new PWMVictorSPX(1);
        m_rightLeader = new WPI_TalonFX(2);
        m_rightFollower = new PWMVictorSPX(3);
        break;
    }*/
        

  }

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
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {
    
  }

  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {
    
    m_DriveSelected = m_driveChooser.getSelected();
    System.out.println("Drive selected: " + m_DriveSelected);

  }

  /** This function is called periodically during operator control. :) */
  @Override
  public void teleopPeriodic() {
    // updateLimelightTracking();
    //put encoder values on Smart Dashboard!!
    // SmartDashboard.putBoolean("Limelight Has Valid Target", m_LimelightHasValidTarget);
    // NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
    // double tx = table.getEntry("tx").getDouble(0);
    // double ty = table.getEntry("ty").getDouble(0);
    // double ta = table.getEntry("ta").getDouble(0);
    
    //boolean m_LimelightHasValidTarget = (table.getEntry("tv").getDouble(0) < 1.0);
    //double distanceFromLimelightToGoalCentimeters = 2.54*((31.125-29.68504)/Math.tan((ty+0.0)*(3.14159 / 180.0)));
    //double distanceFromLimelightToGoalCentimeters = 2.54*((80-29.69904)/Math.tan((ty+0.0)*(3.14159 / 180.0)));

    //double distance = 156.15637194655 - (36.746837632824 * Math.log(ta)); //Change! - this is area of target and we want angle

    // SmartDashboard.putBoolean("Limelight Has Valid Target", m_LimelightHasValidTarget);
    // SmartDashboard.putNumber("Limelight Horizontal offset", tx);
    // SmartDashboard.putNumber("Limelight Vertical offset ", ty);
    // SmartDashboard.putNumber("Limelight Distance ", distance);
    // SmartDashboard.putNumber("Limelight distance using angle", distanceFromLimelightToGoalCentimeters);
        
  /*  if (m_arcadeJoystickP1.getRawButton(3)){// do seeking
     m_myRobot.tankDrive(-0.3, 0.3);
    }
    else if (m_arcadeJoystickP2.getRawButton(3)){
      m_myRobot.tankDrive(0.3, -0.3);
    }*/
    //making an if statement for if button pressed vision driving (for shooting)
    if (m_arcadeJoystickP1.getRawButton(4)&& m_LimelightHasValidTarget){ 
      //m_myRobot.tankDrive(-pidCalcDistance()+pidCalcAngle(), pidCalcDistance()-pidCalcAngle());//Call pidCalcAngle() subtract from one side, add to another
      m_myRobot.tankDrive(-limeLight.getDistance()+limeLight.getAngle(), limeLight.getDistance()-limeLight.getAngle());//Call pidCalcAngle() subtract from one side, add to another 
    } else {
        switch (m_DriveSelected) {
          case kArcadeDrive:
            double rotateSpeed = m_arcadeJoystickP1.getRawAxis(1);
            double moveSpeed = m_arcadeJoystickP1.getRawAxis(0);
            m_myRobot.arcadeDrive(moveSpeed, -rotateSpeed); //- working arcade drive
            //MOVE SPEED SHOULD BE + AND ROTATE SPEED SHOULD BE -
            //System.out.println("Arcade Drive");
            break;
          case kCurvatureDrive:
            // Curvature drive with a given forward and turn rate, as well as a button for turning in-place.
            m_myRobot.curvatureDrive(m_arcadeJoystickP1.getX(), -m_arcadeJoystickP1.getY(), !m_arcadeJoystickP1.getTrigger());//maybe take out negative?
            SmartDashboard.putNumber("wheel Y-axis", m_arcadeJoystickP1.getY());
            SmartDashboard.putNumber("wheel X-axis", m_arcadeJoystickP1.getX());
            SmartDashboard.putNumber("Joystick Y-axis", m_arcadeJoystickP2.getY());
            SmartDashboard.putNumber("Joystick X-axis", m_arcadeJoystickP2.getX());
            //make a drive straight button
            //System.out.println("Curvature Drive");
            break;  
          case kSteeringWheel:
            double rSpeed = m_arcadeJoystickP1.getRawAxis(0);//10% works DON'T CHANGE
            double mSpeed = m_arcadeJoystickP2.getRawAxis(1);
            m_myRobot.arcadeDrive(rSpeed, -mSpeed); //- working arcade drive
            //SmartDashboard.putNumber("Steering Wheel", m_arcadeJoystickP1.getY());
            //SmartDashboard.putNumber("Joystick", m_arcadeJoystickP2.getY());
            SmartDashboard.putNumber("wheel Y-axis", m_arcadeJoystickP1.getY());
            SmartDashboard.putNumber("wheel X-axis", m_arcadeJoystickP1.getX());
            SmartDashboard.putNumber("Joystick Y-axis", m_arcadeJoystickP2.getY());
            SmartDashboard.putNumber("Joystick X-axis", m_arcadeJoystickP2.getX());
            //System.out.println("Steering Wheel");
            //testing going on, robot turns when pushing the joystick
            break;
          default:
            speed = (m_arcadeJoystickP1.getY() + m_arcadeJoystickP2.getY()) / 2;
            if (m_arcadeJoystickP1.getRawButton(3) || m_arcadeJoystickP2.getRawButton(3)) {
              m_myRobot.tankDrive(-speed, speed);
            } else {
              m_myRobot.tankDrive(-m_arcadeJoystickP1.getY(), m_arcadeJoystickP2.getY());
            }

            // if (m_arcadeJoystickP1.getRawButton(2)&&m_LimelightHasValidTarget){
            //   double heading_error = tx;
            //   double steering_adjust = Kp * tx;
            //   if (tx > 1.0)
            //     {
            //             steering_adjust = Kp*heading_error - min_command;
            //     }
            //     else if (tx < 1.0)
            //     {
            //             steering_adjust = Kp*heading_error + min_command;
            //     }
            //   m_myRobot.tankDrive(-m_arcadeJoystickP1.getY()+ steering_adjust, m_arcadeJoystickP2.getY()- steering_adjust);
            // }
            break;
    
        }
       
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

//   public double pidCalcDistance(){
//     NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
//     double tx = table.getEntry("tx").getDouble(0);
//     double ta = table.getEntry("ta").getDouble(0);
//     double distance = 156.15637194655 - (36.746837632824 * Math.log(ta)); //Unit- centimeters 
//     System.out.println("distance" + distance);
//     return pidDistance.calculate(distance, 100); //Current distance, setpoint (desired distance from target)
//   }
//   public double angleDistance() {
//     NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
//     double ty = table.getEntry("ty").getDouble(0);
//     //double targetOffsetAngle_Vertical = ty.getDouble(0.0);
//     double targetOffsetAngle_Vertical =ty;
//     // how many degrees back is your limelight rotated from perfectly vertical?
//     double limelightMountAngleDegrees = 20.0; // in our testing it's zero but real degrees would be 20

//     // distance from the center of the Limelight lens to the floor
//     double limelightLensHeightInches = 29.69904;// 75.4cm 29.68504in for testing
//     //awaiting electronics' height

//     // distance from the target to the floor
//     double goalHeightInches = 80; //the real hight would be  5cm height of target
//     // real hight 104in- 5cm = real target = 104-1.9685=102.0315
//     // our testing height is 31.125in 78.8cm

//     double angleToGoalDegrees = limelightMountAngleDegrees + targetOffsetAngle_Vertical;
//     double angleToGoalRadians = angleToGoalDegrees * (3.14159 / 180.0);

//     //calculate distance
//     double distanceFromLimelightToGoalInches = (goalHeightInches - limelightLensHeightInches)/Math.tan(angleToGoalRadians);
//     double distanceFromLimelightToGoalCentimeters= distanceFromLimelightToGoalInches*2.54;
//     //double distanceFromLimelightToGoalCentimeters = 2.54*((80-29.69904)/Math.tan((ty+0.0)*(3.14159 / 180.0)));

//     return pidDistance.calculate(distanceFromLimelightToGoalCentimeters, 100); //Current distance, setpoint (desired distance from target)
//   }


//   public double pidCalcAngle(){
//     NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
//     double tx = table.getEntry("tx").getDouble(0);

//     return pidAngle.calculate(tx, 0);
//   }

//   public void updateLimelightTracking(){
//     NetworkTable table = NetworkTableInstance.getDefault().getTable("limelight");
    
//     /**
//       * tv- Whether the limelight has any valid targets (0 or 1)
//       * tx- Horizontal offset from crosshair to target (LL2 -29.8 to 29.8)
//       * ty- Vertical offset from crosshair to target (LL2 -24.85 to 24.85)
//       * ta- Target area (0% of image to 100% of image)
//       remove this stuff maybe v
//     */
//     double tv = table.getEntry("tv").getDouble(0);
//     double tx = table.getEntry("tx").getDouble(0);
//     double ty = table.getEntry("ty").getDouble(0);
//     double ta = table.getEntry("ta").getDouble(0);

//     if (tv < 1.0)
//     {
//       m_LimelightHasValidTarget = false;
//       return;

//     } else {
//       m_LimelightHasValidTarget = true;
//     }

// }
}
