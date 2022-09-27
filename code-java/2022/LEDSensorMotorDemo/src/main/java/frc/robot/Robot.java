// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.util.Color;

import java.io.File;
import javax.imageio.ImageIO;
import java.awt.image.*;

import edu.wpi.first.wpilibj.AddressableLED;
import edu.wpi.first.wpilibj.AddressableLEDBuffer;
import edu.wpi.first.wpilibj.Filesystem;
import edu.wpi.first.wpilibj.I2C;

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

  // LED stuff
  private static final int m_ledWidth = 48;
  private static final int m_ledHeight = 16;
  private static final int m_ledPWMPin = 0;
  private static final int m_meterColumnStart = 32;
  private LEDPanel m_ledPanel;
  private HorizontalMeter m_hMeter;
  private BufferedImage m_image;
  private static final double m_brightness = 0.04;

  // Color sensor configuration
  private ColorSensor m_colorSensor; // Onboard
  private static final double m_ConfidenceThreshold = 0.85;  

    // Average of 3 samples we observed using real game pieces
  private final Color kRedBallTarget = new Color((0.5825 + 0.57764 + 0.5712) / 3,
    (0.3091 + 0.3122 + 0.3188) / 3,
    (0.1086 + 0.1108 + 0.1104) / 3);

  private final Color kBlueBallTarget = new Color((0.1323 + 0.1232 + 0.1323) / 3,
    (0.3621 + 0.3613 + 0.3618)/3,
    (0.5059 + 0.5068 + 0.5061)/3);



  // Images
  BufferedImage m_image_blackAndWhite;
  BufferedImage m_image_black;
  BufferedImage m_image_gray;
  BufferedImage m_image_redGreen;
  BufferedImage m_image_original;

  // Hack to change images
  private int m_tickCount = 0;
  static int m_imageNum = 0;

      
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
    m_ledPanel.m_brightness = m_brightness;
    m_ledPanel.setColor(Color.kBlack);

    // Init meter LED subsystem
    m_hMeter = new HorizontalMeter(m_ledPanel, 6);
    m_hMeter.setStartColumn(m_meterColumnStart);

    // Init sensor
    sensorInit();

    // Load our images
    m_image_original = loadImage("48x16 Logo.png");
    m_image_blackAndWhite = loadImage("48x16 Logo B&W.png");
    m_image_black = loadImage("48x16 Logo Black.png");
    m_image_gray = loadImage("48x16 Logo Gray.png");
    m_image_redGreen = loadImage("48x16 Logo.png");
    m_image = m_image_redGreen;

    pushLoadedImage();
  }


  public void sensorInit() {
    // Init ColorSensor subsystem
    Color colorsToMatch[] = {kRedBallTarget, kBlueBallTarget};

    // Setup the "top" sensor using the on-board i2c port
    I2C.Port i2cPort = I2C.Port.kOnboard;
    m_colorSensor = new ColorSensor(i2cPort);
    m_colorSensor.setColorMatches(colorsToMatch);
    m_colorSensor.setConfidenceThreshold(m_ConfidenceThreshold);
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
    // Fill the buffer with a rainbow
    // updateRainbow();
    // pushLoadedImage();
    // setColor(0, 0, 255);

    m_tickCount++;

    if ((m_tickCount % 200) == 0) {
      m_imageNum++;
      if (m_imageNum > 4) {
        m_imageNum = 0;
      }

      System.out.println("Switching to image: " + m_imageNum);

      switch (m_imageNum) {
        case 4:
          m_image = m_image_blackAndWhite;
          break;
        case 3:
          m_image = m_image_black;
          break;
        case 2:
         m_image = m_image_gray;
         break;
        case 1:
          m_image = m_image_redGreen;
          break;
        case 0:
          m_image = m_image_original;
          break;
      }
      pushLoadedImage();
    }

    // This will clobber part of the image
    updateSensorDisplay();
    
    // Set the LEDs (commit)
    m_ledPanel.commit();
  }

  public void updateSensorDisplay() {
    // Use .match() to get a match or null if nothing 
    // matches. Use .closestMatch() to force a match
    Color observedColorT = m_colorSensor.rawColor();
    ColorMatchResult closestMatch = m_colorSensor.match();
    int proximityT = m_colorSensor.getProximity();
    double confidenceT = closestMatch != null ? closestMatch.confidence : 0.0;
    Color matchColorT = closestMatch != null ? closestMatch.color : null;

    setSensorDisplay(0, confidenceT, observedColorT, matchColorT, proximityT);
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

  private BufferedImage loadImage(String fileName) {
    File deployDirectory = Filesystem.getDeployDirectory();
    File imageFile = new File(deployDirectory, fileName);
    System.out.println("loadImage() called with fn: " + fileName);

    BufferedImage loadedImage;

    try {
      loadedImage = ImageIO.read(imageFile);
       System.out.println("Successfully loaded image file!");
       System.out.println("Image dimensions are: " + loadedImage.getWidth() + " x " + loadedImage.getHeight());
    }
    catch(Exception e) {
      System.out.println("Couldn't load image: " + imageFile.getPath());
      return null;
    }

    return loadedImage;

  }

  private void pushLoadedImage() {
    // We might want to be more efficient about this, but if we're changing images,
    // we need to blank the pixels the new image isn't drawing directly.
    m_ledPanel.setColor(Color.kBlack);
    m_ledPanel.setImage(m_image);
  }

  public void setSensorDisplay(int startRow, double confidence, Color observedColor, Color matchedColor, double proximity) {
    // Write out proximity, which appears to scale from ~100 to 2048
    double proximityPercent = (double)proximity / 2048;
    m_hMeter.setOutputChannel(0, proximityPercent, Color.kPurple);

    // Write out a confidence
    m_hMeter.setOutputChannel(1, confidence, Color.kWhite);

    m_hMeter.setOutputChannel(2, observedColor.red, Color.kRed);
    m_hMeter.setOutputChannel(3, observedColor.green, Color.kGreen);
    m_hMeter.setOutputChannel(4, observedColor.blue, Color.kBlue);

    // Pass 1.0 so the whole row is colored. 
    if (matchedColor != null) { 
      if (matchedColor == kRedBallTarget) {
        m_hMeter.setOutputChannel(5, 1.0, Color.kRed);
      } else if (matchedColor == kBlueBallTarget) {
        m_hMeter.setOutputChannel(5, 1.0, Color.kBlue);
      }
    } else {
      m_hMeter.setOutputChannel(5, 1.0, Color.kBlack);
    }

    // Update SmartDashboard with stuff.
    SmartDashboard.putNumber("Proximity", proximity);
    SmartDashboard.putNumber("Confidence", confidence);
    SmartDashboard.putNumber("Observed red", observedColor.red);
    SmartDashboard.putNumber("Observed green", observedColor.green);
    SmartDashboard.putNumber("Observed blue", observedColor.blue);

    if (matchedColor == kBlueBallTarget) {
      // System.out.println(label + " sees Blue");
      SmartDashboard.putString("Color Match", "Blue");
    } else if (matchedColor == kRedBallTarget) {
      // System.out.println(label+ " sees Red");
      SmartDashboard.putString("Color Match", "Red");
    } else {
      // System.out.println(label + " didn't match");
      SmartDashboard.putString("Color Match", "None");
    }
  }

}