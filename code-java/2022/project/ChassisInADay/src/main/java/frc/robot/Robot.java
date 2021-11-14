// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.SpeedController;
import edu.wpi.first.wpilibj.SpeedControllerGroup;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.PWMVictorSPX;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;

/**
 * The VM is configured to automatically run this class, and to call the functions corresponding to
 * each mode, as described in the TimedRobot documentation. If you change the name of this class or
 * the package after creating this project, you must also update the build.gradle file in the
 * project.
 */
public class Robot extends TimedRobot {
  private DifferentialDrive m_myRobot;
  private Joystick m_leftStick;
  private Joystick m_rightStick;
  
  private static final String kDefaultAuto = "Default";
  private static final String kCustomAuto = "My Auto";
  private String m_autoSelected;
  private final SendableChooser<String> m_chooser = new SendableChooser<>();

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {

    //WPI_TalonSRX m_leftLeader = new WPI_TalonSRX(1);
    /*
    SpeedController m_leftFollower = new PWMVictorSPX(2);
    SpeedController m_rightFollower = new PWMVictorSPX(4);
    SpeedControllerGroup m_left = new SpeedControllerGroup(m_rightFollower, m_leftFollower);
    */

    
    SpeedController m_leftLeader = (SpeedController) new WPI_TalonSRX(0);
    SpeedController m_leftFollower = new PWMVictorSPX(2);
    SpeedControllerGroup m_left = new SpeedControllerGroup(m_leftLeader, m_leftFollower);
    
    SpeedController m_rightLeader = (SpeedController) new WPI_TalonSRX(1);
    SpeedController m_rightFollower = new PWMVictorSPX(3);
    SpeedControllerGroup m_right = new SpeedControllerGroup(m_rightLeader, m_rightFollower);
    

    m_myRobot = new DifferentialDrive(m_left, m_right);
    m_leftStick = new Joystick(0);
    m_rightStick = new Joystick(1);

    /*
    //encoders
    m_leftLeader.configFactoryDefault();
    m_rightLeader.configFactoryDefault();
    //follow?
    m_leftLeader.setStatusFramePeriod(StatusFrameEnhanced.Status_2_Feedback0, 1);
    m_rightLeader.setStatusFramePeriod(StatusFrameEnhanced.Status_2_Feedback0, 1);

    m_leftLeader.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Relative);
    m_rightLeader.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Relative);

    //quadrature??
    final int kTimeoutMs = 30;
    m_leftLeader.setQuadraturePosition(-745, kTimeoutMs);
    m_rightLeader.setQuadraturePosition(-745, kTimeoutMs);
    */
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
    /*
    double velocityLeft = m_leftLeader.getSelectedSensorVelocity(0);
		SmartDashboard.putNumber("velLeft", velocityLeft);
    double velocityRight = m_rightLeader.getSelectedSensorVelocity(0);
		SmartDashboard.putNumber("velRight", velocityRight);

    double leftPos = m_leftLeader.getSelectedSensorPosition();
    SmartDashboard.putNumber("leftPos", leftPos);
    double rightPos = m_rightLeader.getSelectedSensorPosition();
    SmartDashboard.putNumber("rightPos", rightPos);
    

    //quadrature??
    m_leftLeader.getQuadraturePosition();
    m_rightLeader.getQuadraturePosition();
    */
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
    //m_autoSelected = m_chooser.getSelected();
    // m_autoSelected = SmartDashboard.getString("Auto Selector", kDefaultAuto);
    //System.out.println("Auto selected: " + m_autoSelected);
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {

  }

  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {}

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {
    m_myRobot.tankDrive(-1.0 * m_leftStick.getY(), -1.0 * m_rightStick.getY());
    SmartDashboard.putNumber("Left Joystick", m_leftStick.getY());
    SmartDashboard.putNumber("Right Joystick", m_rightStick.getY());
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
