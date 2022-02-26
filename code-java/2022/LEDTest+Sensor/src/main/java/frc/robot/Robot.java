// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.I2C;

import edu.wpi.first.wpilibj.util.Color;

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
  private ColorSensor m_colorSensor;
  private final Color kBlueTarget = new Color(0.143, 0.427, 0.429);
  private final Color kGreenTarget = new Color(0.197, 0.561, 0.240);
  private final Color kRedTarget = new Color(0.561, 0.232, 0.114);
  private final Color kYellowTarget = new Color(0.361, 0.524, 0.113);
  private final Color kBlackTarget = new Color (0.000,0.000,0.000);
  private final Color kBlack1Target = new Color (0.5,0.5,0.0);
  private final Color kGrayTarget = new Color(0.38, 0.41, 0.21);

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

    // Init ColorSensor subsystem
    I2C.Port i2cPort = I2C.Port.kOnboard;
    m_colorSensor = new ColorSensor(i2cPort);
    Color colorsToMatch[] = {kBlueTarget, kGreenTarget, kRedTarget, kYellowTarget, kBlack1Target,
    kBlackTarget, kGrayTarget};
    m_colorSensor.setColorMatches(colorsToMatch);
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
    int proximity = m_colorSensor.getProximity();

    // Use .match() to get a match or null if nothing 
    // matches. Use .closestMatch() to force a match
    ColorMatchResult closestMatch = m_colorSensor.closestMatch();
    Color displayColor = new Color(0.1, 0.1, 0.1);
    double confidence = 0.0;

    if (closestMatch != null) { 
      Color color = closestMatch.color;
      confidence = closestMatch.confidence;

      if (color == kBlueTarget) {
        System.out.println("Blue");
        SmartDashboard.putString("Color Match", "Blue");
        displayColor = Color.kBlue;
      } else if (color == kRedTarget) {
        System.out.println("Red");
        SmartDashboard.putString("Color Match", "Red");
        displayColor = Color.kRed;
      } else if (color == kGreenTarget) {
        SmartDashboard.putString("Color Match", "Green");
        System.out.println("Green");
      } else if (color == kYellowTarget) {
        System.out.println("Yellow");
        SmartDashboard.putString("Color Match", "Yellow");
        displayColor = Color.kYellow;
      } else if (color == kBlackTarget || color == kBlack1Target) {
        System.out.println("Black");
        SmartDashboard.putString("Color Match", "Black");
        displayColor = Color.kBlack;
      } else if (color == kGrayTarget) {
        System.out.println("Gray");
        SmartDashboard.putString("Color Match", "Gray");
        displayColor = Color.kSlateGray;
      } else {
        System.out.println("No Match");
        SmartDashboard.putString("Color Match", "None");
      }
    }

    // Paint the whole panel the match color
    m_ledPanel.setColor(displayColor);

    // Write out a confidence
    setOutputMeter(0, 2, confidence, Color.kWhite);

    // Write out the individual RGB values we see
    Color observedColor = m_colorSensor.rawColor();
    setOutputMeter(2, 2, observedColor.red, Color.kRed);
    setOutputMeter(4, 2, observedColor.green, Color.kGreen);
    setOutputMeter(6, 2, observedColor.blue, Color.kBlue);

    // Write out proximity, which appears to scale from ~100 to 2048
    double proximityPercent = (double)m_colorSensor.getProximity() / 2048;
    setOutputMeter(8, 2, proximityPercent, Color.kPurple);

    SmartDashboard.putNumber("Proximity", proximity);
    SmartDashboard.putNumber("Confidence", confidence);

    m_ledPanel.commit();
  }

  public void setOutputMeter(int startRow, int meterHeight, double fraction, Color meterColor) {
    int meterValue = (int)(fraction * m_ledWidth + 0.5);
    for (int col = 0; col < m_ledWidth; col++) {
      for (int meterRow = 0; meterRow < meterHeight; meterRow++) {
        m_ledPanel.setPixelByXY(col, startRow + meterRow, col < meterValue ? meterColor : Color.kBlack);
      }
    }
  }

  public void setConfidenceMeter(double confidence) {

  }

  public void setColorMeter(Color color) {

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
}
