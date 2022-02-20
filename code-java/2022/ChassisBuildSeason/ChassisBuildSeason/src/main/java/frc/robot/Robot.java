// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorController;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.motorcontrol.PWMVictorSPX;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import edu.wpi.first.wpilibj.GenericHID;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;


import edu.wpi.first.wpilibj.Joystick;

import edu.wpi.first.wpilibj.drive.DifferentialDrive;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;

import edu.wpi.first.wpilibj.XboxController;
import com.ctre.phoenix.sensors.CANCoder;


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
  Joystick m_arcadeJoystick;
  Joystick m_arcadeJoystick2;

  private WPI_TalonSRX m_leftLeader = new WPI_TalonSRX(0);
    MotorController m_leftFollower = new PWMVictorSPX(3);
    WPI_TalonSRX m_rightLeader = new WPI_TalonSRX(1);
    MotorController m_rightFollower = new PWMVictorSPX(4);
    MotorControllerGroup m_left = new MotorControllerGroup(m_leftLeader, m_leftFollower);
    MotorControllerGroup m_right = new MotorControllerGroup(m_rightLeader, m_rightFollower);
    DifferentialDrive m_myRobot = new DifferentialDrive(m_left,m_right);

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);

    m_arcadeJoystick = new Joystick(0);
    m_arcadeJoystick2 = new Joystick(1);

  
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
  public void teleopInit() {
  }

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {

  //arcadeDrive
    //this is arcade drive code that works
    // Arcade drive with a given forward and turn rate
    //double moveSpeed = m_arcadeJoystick.getRawAxis(1);
    //double rotateSpeed = m_arcadeJoystick.getRawAxis(0);
    //m_myRobot.arcadeDrive(moveSpeed, rotateSpeed); - working arcade drive

    //instead of this code above,test code below to use steering wheel to change x-axis and y-axis is controlled for
    //m_myRobot.arcadeDrive(-m_arcadeJoystick2.getY(), m_arcadeJoystick.getX());
    //SmartDashboard.putNumber("Steering Wheel", m_arcadeJoystick.getY());
    //SmartDashboard.putNumber("Joystick", m_arcadeJoystick2.getY());
  
  
  //curvatureDrive
    // Curvature drive with a given forward and turn rate, as well as a button for turning in-place.
    m_myRobot.curvatureDrive(-m_arcadeJoystick.getY(), m_arcadeJoystick.getX(), m_arcadeJoystick.getTrigger());
    SmartDashboard.putNumber("Joystick Y-axis", m_arcadeJoystick.getY());
    SmartDashboard.putNumber("Joystick X-axis", m_arcadeJoystick.getX());

  //tankDrive
    //myDrive.tankDrive(-m_arcadeJoystick.getY(), -m_arcadeJoystick2.getY());

  }
  
  //}
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

