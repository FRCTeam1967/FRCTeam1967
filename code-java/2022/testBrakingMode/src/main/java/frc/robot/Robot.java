// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.DriverStation;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.Timer;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.StatorCurrentLimitConfiguration;

import org.janksters.jankyLib.*;


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

  private WPI_TalonFX falcon = null;
  private Timer teleTimer = null;
  private Timer disTimer = null;

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);
    falcon = new WPI_TalonFX(1);
    falcon.configStatorCurrentLimit(new StatorCurrentLimitConfiguration(true, 30, 35, 1.0));
//    falcon.configStatorCurrentLimit(new StatorCurrentLimitConfiguration(true, 20, 25, 1.0));
//    falcon.configStatorCurrentLimit(new StatorCurrentLimitConfiguration(true, 2, 3, 1.0));
//    falcon.enableCurrentLimit();
    teleTimer = new Timer();
    disTimer = new Timer();
    teleTimer.reset();
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
    teleTimer.reset();
    teleTimer.start();
  }

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {
    SmartDashboard.putNumber("timer:", teleTimer.get());
    if (teleTimer.get()<7) {
      SmartDashboard.putString("status", "Falcon Spinning");
      falcon.set(0.25);
    }
    else {
      falcon.set(0.0);
      SmartDashboard.putString("status", "Braking Mode.");
      falcon.setNeutralMode(NeutralMode.Brake);
    }
  }

  /** This function is called once when the robot is disabled. */
  @Override
  public void disabledInit() {
    disTimer.reset();
    disTimer.start();
  }

  /** This function is called periodically when disabled. */
  @Override
  public void disabledPeriodic() {
    SmartDashboard.putNumber("timer:", disTimer.get());
  
    if (teleTimer.get() > 0) {
      if (disTimer.get() < 5) {
        SmartDashboard.putString("status", "Robot Disabled+BRAKING.");
        falcon.setNeutralMode(NeutralMode.Brake);
      }
      else {
        SmartDashboard.putString("status", "Robot Disabled+COAST Mode.");
        falcon.setNeutralMode(NeutralMode.Coast);
      }
    }
    else {
      SmartDashboard.putString("status", "Robot Disabled+ERROR.");
    }


  }

  /** This function is called once when test mode is enabled. */
  @Override
  public void testInit() {}

  /** This function is called periodically during test mode. */
  @Override
  public void testPeriodic() {}
}
