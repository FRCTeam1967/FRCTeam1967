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
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import edu.wpi.first.math.controller.PIDController;


public class Robot extends TimedRobot {
  private static final String kTankDrive = "Tank Drive";
  private static final String kArcadeDrive = "Arcade Drive";
  private static final String kCurvatureDrive = "Curvature Drive";
  private static final String kSteeringWheel = "Steering Wheel";

  private String m_DriveSelected;
  private final SendableChooser<String> m_driveChooser = new SendableChooser<>();

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

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_driveChooser.setDefaultOption("Tank Drive", kTankDrive);
    m_driveChooser.addOption("Arcade Drive", kArcadeDrive);
    m_driveChooser.addOption("Curvature Drive", kCurvatureDrive);
    m_driveChooser.addOption("Steering Wheel", kSteeringWheel);
    SmartDashboard.putData("Drive choices", m_driveChooser);

    m_arcadeJoystickP1 = new Joystick(0);  //whatever is in port 1 - P1 stands for port1
    m_arcadeJoystickP2 = new Joystick(1); //whatever is in port  2

    m_myRobot.setMaxOutput(0.5); //default is 1

    System.out.println("Passed through initialization");
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
    if (m_arcadeJoystickP1.getRawButton(4)&& limeLight.targetValid()){ 
      //m_myRobot.tankDrive(-pidDistance.calculate(limeLight.getDistance(), 100)+limeLight.getAngle(), limeLight.getDistance()-limeLight.getAngle());//Call pidCalcAngle() subtract from one side, add to another 
      SmartDashboard.putNumber("Distance to target:", limeLight.getDistance());
    } else {
        switch (m_DriveSelected) {
          case kArcadeDrive:
            double rotateSpeed = m_arcadeJoystickP1.getRawAxis(1);
            double moveSpeed = m_arcadeJoystickP1.getRawAxis(0);
            m_myRobot.arcadeDrive(moveSpeed, -rotateSpeed); // + move , - rot
            break;
          case kCurvatureDrive:
            // Curvature drive with a given forward and turn rate, as well as a button for turning in-place.
            m_myRobot.curvatureDrive(m_arcadeJoystickP1.getX(), -m_arcadeJoystickP1.getY(), !m_arcadeJoystickP1.getTrigger());//maybe take out negative?
            break;  
          case kSteeringWheel:
            double rSpeed = m_arcadeJoystickP1.getRawAxis(0);//10% works DON'T CHANGE
            double mSpeed = m_arcadeJoystickP2.getRawAxis(1);
            m_myRobot.arcadeDrive(rSpeed, -mSpeed); 
            break;
          default:
            speed = (m_arcadeJoystickP1.getY() + m_arcadeJoystickP2.getY()) / 2;
            if (m_arcadeJoystickP1.getRawButton(3) || m_arcadeJoystickP2.getRawButton(3)) {
              m_myRobot.tankDrive(-speed, speed);
            } else {
              m_myRobot.tankDrive(-m_arcadeJoystickP1.getY(), m_arcadeJoystickP2.getY());
            }
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
}