// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.shuffleboard.BuiltInLayouts;
import edu.wpi.first.wpilibj.shuffleboard.BuiltInWidgets;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardLayout;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.util.Color;
import frc.robot.Constants.LED;

import java.util.Map;

import com.ctre.phoenix.sensors.Pigeon2Configuration;
import com.ctre.phoenix.sensors.WPI_Pigeon2;

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

  private WPI_Pigeon2 m_pigeon2 = new WPI_Pigeon2(Constants.Gyroscope.canID);

  private double m_rawGyro[] = new double[3];
  private double m_yaw;
  private double m_pitch;
  private double m_roll;
  private double m_heading;
  private Rotation2d m_rotation;

  private LEDPanel m_panel;
  private HorizontalMeter m_hMeter;

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);

    Pigeon2Configuration pigeonConfig = new Pigeon2Configuration();
    m_pigeon2.configAllSettings(pigeonConfig);
    m_pigeon2.configMountPose(Constants.Gyroscope.forwardAxis, Constants.Gyroscope.upAxis);

    ShuffleboardTab tab = Shuffleboard.getTab("Gyro");
    tab.addDoubleArray("Raw Values", () -> m_rawGyro);
    tab.addDouble("Heading", () -> m_heading);

    tab.add(m_pigeon2);

    ShuffleboardLayout yprList = tab.getLayout("Graphs", BuiltInLayouts.kList)
      .withProperties(Map.of("Label position", "TOP"));
    yprList.addDouble("Yaw", () -> m_yaw)
      .withWidget(BuiltInWidgets.kGraph)
      .withSize(3,1);
    yprList.addDouble("Pitch", () -> m_pitch)
      .withWidget(BuiltInWidgets.kGraph)
      .withSize(3,1);
    yprList.addDouble("Roll", () -> m_roll)
      .withWidget(BuiltInWidgets.kGraph)
      .withSize(3,1);

      m_panel = new LEDPanel(Constants.LED.width, Constants.LED.height, Constants.LED.pwmPin);
      m_hMeter = new HorizontalMeter(m_panel, 7);
  }

  /**
   * This function is called every 20 ms, no matter the mode. Use this for items like diagnostics
   * that you want ran during disabled, autonomous, teleoperated and test.
   *
   * <p>This runs after the mode specific periodic functions, but before LiveWindow and
   * SmartDashboard integrated updating.
   */
  @Override
  public void robotPeriodic() {
    m_pigeon2.getRawGyro(m_rawGyro);
    m_yaw = m_pigeon2.getYaw();
    m_pitch = m_pigeon2.getPitch();
    m_roll = m_pigeon2.getRoll();
    m_heading = m_pigeon2.getAngle();
    m_rotation = m_pigeon2.getRotation2d();

    // Range is -180 to 180, but we're can't turn GerryRig upside down, so reduce pitch/roll to +/- 90
    m_hMeter.setCenterOutputChannel(0, m_pitch / 90.0, Color.kRed, Color.kGreen);
    m_hMeter.setCenterOutputChannel(1, m_roll / 90.0, Color.kRed, Color.kGreen);
    m_hMeter.setCenterOutputChannel(2, m_yaw / 180.0, Color.kRed, Color.kGreen);

    // Leave channel 3 unused as a separator

    for (int axis = 0; axis < 3; axis++) {
      // Raw gyro is in degrees/second
      m_hMeter.setCenterOutputChannel(4 + axis, m_rawGyro[axis] / 45.0, Color.kRed, Color.kGreen);
    }

    m_panel.commit();
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
  public void teleopPeriodic() {}

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

  /** This function is called once when the robot is first started up. */
  @Override
  public void simulationInit() {}

  /** This function is called periodically whilst in simulation. */
  @Override
  public void simulationPeriodic() {}
}
