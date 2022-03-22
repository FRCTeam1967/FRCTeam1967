// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import edu.wpi.first.wpilibj.util.Color;

import edu.wpi.first.wpilibj.Timer;

import com.revrobotics.ColorMatchResult;

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

  // LED panel configuration
  private static final int m_ledWidth = 48;
  private static final int m_ledHeight = 16;
  private static final int m_ledPWMPin = 0;
  private LEDPanel m_ledPanel;

  // Color sensor configuration
  private ColorSensorPair m_colorSensor;
  private static final double m_ConfidenceThreshold = 0.75;

  // MDS HACK
  private Timer m_timer;

  // Average of 3 samples we observed using real game pieces
  private final Color kRedBallTarget = new Color((0.5825 + 0.57764 + 0.5712) / 3,
    (0.3091 + 0.3122 + 0.3188) / 3,
    (0.1086 + 0.1108 + 0.1104) / 3);

  private final Color kBlueBallTarget = new Color((0.1323 + 0.1232 + 0.1323) / 3,
    (0.3621 + 0.3613 + 0.3618)/3,
    (0.5059 + 0.5068 + 0.5061)/3);

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);

    // Init LED subsystem
    m_ledPanel = new LEDPanel(m_ledWidth, m_ledHeight, m_ledPWMPin);

    // Init color sensor. You can create only one of these.
    m_colorSensor = new ColorSensorPair();
    Color colorsToMatch[] = {kRedBallTarget, kBlueBallTarget};
    m_colorSensor.setColorMatches(colorsToMatch);
    m_colorSensor.setConfidenceThreshold(m_ConfidenceThreshold);

    m_timer = new Timer();
    m_timer.start();
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
    // Paint the whole panel the match color
    m_ledPanel.setColor(Color.kBlack);

    // We have 16 rows to the display to work with. Let's do:
    // 0  - T Proximity
    // 1  - T Confidence
    // 2  - T Observed Red
    // 3  - T Observed Green
    // 4  - T Observed Blue
    // 5  - Match color (if any)
    // 6  - Match color (if any)
    // 7  - (blank)
    // 8  - (blank)
    // 9  - B Proximity
    // 10 - B Confidence
    // 11 - B Observed Red
    // 12 - B Observed Green
    // 13 - B Observed Blue
    // 14 - Match color (if any)
    // 15 - Match color (if any)

    if (m_colorSensor != null && m_colorSensor.isSensor0Connected()) {
      ColorMatchResult match = m_colorSensor.match0();
      Color observedColor = m_colorSensor.getColor0();
      int proximity = m_colorSensor.getProximity0();
      Color matchDisplayColor = match != null ? match.color : null;
      double confidence = match != null ? match.confidence : 0.0;
      setSensorDisplay(0, confidence, observedColor, matchDisplayColor, proximity, "T");
    }

    if (m_colorSensor != null && m_colorSensor.isSensor1Connected()) {
      ColorMatchResult match = m_colorSensor.match1();
      Color observedColor = m_colorSensor.getColor1();
      int proximity = m_colorSensor.getProximity1();
      double confidence = match != null ? match.confidence : 0.0;
      Color matchDisplayColor = match != null ? match.color : null;
      setSensorDisplay(0, confidence, observedColor, matchDisplayColor, proximity, "B");
    }

    int sensors = 0;
    if (m_colorSensor.isSensor0Connected()) sensors++;
    if (m_colorSensor.isSensor1Connected()) sensors++;
    SmartDashboard.putNumber("Sensors detected", sensors);

    m_ledPanel.commit();
  }

  public void setSensorDisplay(int startRow, double confidence, Color observedColor, Color matchedColor, double proximity, String label) {
    // Write out proximity, which appears to scale from ~100 to 2048
    double proximityPercent = (double)proximity / 2048;
    setOutputMeter(startRow, 1, proximityPercent, Color.kPurple);

    // Write out a confidence
    setOutputMeter(startRow + 1, 1, confidence, Color.kWhite);

    setOutputMeter(startRow + 2, 1, observedColor.red, Color.kRed);
    setOutputMeter(startRow + 3, 1, observedColor.green, Color.kGreen);
    setOutputMeter(startRow + 4, 1, observedColor.blue, Color.kBlue);

    // Pass 1.0 so the whole row is colored. 
    if (matchedColor != null) { 
      if (matchedColor == kRedBallTarget) {
        setOutputMeter(startRow + 5, 2, 1.0, Color.kRed);
      } else if (matchedColor == kBlueBallTarget) {
        setOutputMeter(startRow + 5, 2, 1.0, Color.kBlue);
      }
    }

    // Update SmartDashboard with stuff.
    SmartDashboard.putNumber(label + " Proximity", proximity);
    SmartDashboard.putNumber(label + " Confidence", confidence);
    SmartDashboard.putNumber(label + " Observed red", observedColor.red);
    SmartDashboard.putNumber(label + " Observed green", observedColor.green);
    SmartDashboard.putNumber(label + " Observed blue", observedColor.blue);

    if (matchedColor == kBlueBallTarget) {
      SmartDashboard.putString(label + " Color Match", "Blue");
    } else if (matchedColor == kRedBallTarget) {
      SmartDashboard.putString(label + " Color Match", "Red");
    } else {
      SmartDashboard.putString(label + " Color Match", "None");
    }

    if (m_timer.hasElapsed(1.0)) {
      System.out.println(label + " Proximity " + proximity);
      System.out.println(label + " Confidence " + confidence);
      System.out.println(label + " Observed red " + observedColor.red);
      System.out.println(label + " Observed green " + observedColor.green);
      System.out.println(label + " Observed blue " + observedColor.blue);
  
      if (matchedColor == kBlueBallTarget) {
        System.out.println(label + " sees Blue");
      } else if (matchedColor == kRedBallTarget) {
        System.out.println(label+ " sees Red");
      } else {
        System.out.println(label + " didn't match");
      }
  
      m_timer.reset();
    }
  }

  public void setOutputMeter(int startRow, int meterHeight, double fraction, Color meterColor) {
    int meterValue = (int)(fraction * m_ledWidth + 0.5);
    for (int col = 0; col < m_ledWidth; col++) {
      for (int meterRow = 0; meterRow < meterHeight; meterRow++) {
        m_ledPanel.setPixelByXY(col, startRow + meterRow, col < meterValue ? meterColor : Color.kBlack);
      }
    }
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
  public void teleopInit() {
  }

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {}

  /** This function is called once when the robot is disabled. */
  @Override
  public void disabledInit() {
  }

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
