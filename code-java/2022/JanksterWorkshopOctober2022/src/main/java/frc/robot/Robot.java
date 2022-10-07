// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.Joystick;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.StatusFrameEnhanced;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;

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
  /* Hardware */
  WPI_TalonFX _talonL = new WPI_TalonFX(4, "rio");
  WPI_TalonFX _talonR = new WPI_TalonFX(13, "rio");
  Joystick _joy = new Joystick(0);

  // LED stuff
  private static final int m_ledWidth = 48;
  private static final int m_ledHeight = 16;
  private static final int m_ledPWMPin = 0;
  private static final int m_meterColumnStart = 32;
  private static final double m_brightness = 0.10;
  private LEDPanel m_ledPanel;
  private HorizontalMeter m_hMeter;
  private int m_rainbowFirstPixelHue = 0;

  // Color sensor configuration
  private ColorSensor m_colorSensor; // Onboard
  private static final double m_ConfidenceThreshold = 0.85;  

  // Do we show images, or rainbow?
  private SendableChooser<Boolean> m_showsRainbow = new SendableChooser<>();

  // Average of 3 samples we observed using real game pieces
  private final Color kRedBallTarget = new Color((0.5825 + 0.57764 + 0.5712) / 3,
    (0.3091 + 0.3122 + 0.3188) / 3,
    (0.1086 + 0.1108 + 0.1104) / 3);

  private final Color kBlueBallTarget = new Color((0.1323 + 0.1232 + 0.1323) / 3,
    (0.3621 + 0.3613 + 0.3618)/3,
    (0.5059 + 0.5068 + 0.5061)/3);

  // Images
  private BufferedImage m_image;  // current image
  BufferedImage m_image_blackAndWhite;
  BufferedImage m_image_black;
  BufferedImage m_image_gray;
  BufferedImage m_image_redGreen;
  BufferedImage m_image_original;

  // Hack to change images
  private int m_tickCount = 0;
  static int m_imageNum = 0;

  //Position stuff
  private int button_state = 0;
  private double kp_pos = .001;
  private double ki_pos = 0;
  private double kd_pos = 0;
  private int max_tik_tocK_count = 100; // 500/.02 seconds = 10 second cycle
  private int tik = 0;
  private double abs_pos_1 = 768;
  private double abs_pos_2 = 1280;
  private double target_pos = 0;

      
  /** 
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    // Let the dashboard figure out what we show
    m_showsRainbow.setDefaultOption("Rainbow", true);
    m_showsRainbow.addOption("Image", false);
    SmartDashboard.putData("LED Mode", m_showsRainbow);

    // Init LED subsystem
    m_ledPanel = new LEDPanel(m_ledWidth, m_ledHeight, m_ledPWMPin);
    m_ledPanel.m_brightness = m_brightness;
    m_ledPanel.setColor(Color.kBlack);

    // Init meter LED subsystem
    m_hMeter = new HorizontalMeter(m_ledPanel, 6);
    m_hMeter.setStartColumn(m_meterColumnStart);

    // Init sensor
    sensorInit();

    // Init hardware
    motorInit();

    // Load our images
    imageInit();

    m_image = m_image_redGreen;
    updateLEDs();
  }

  public void imageInit() {
    m_image_original = loadImage("48x16 Logo.png");
    m_image_blackAndWhite = loadImage("48x16 Logo B&W.png");
    m_image_black = loadImage("48x16 Logo Black.png");
    m_image_gray = loadImage("48x16 Logo Gray.png");
    m_image_redGreen = loadImage("48x16 Logo.png");
  }


  public void motorInit() {
    /* Factory Default all hardware to prevent unexpected behaviour */
    _talonL.configFactoryDefault();
    _talonR.configFactoryDefault();
    
    /* Config neutral deadband to be the smallest possible */
    _talonL.configNeutralDeadband(0.001);
    _talonR.configNeutralDeadband(0.001);

    /* Config sensor used for Primary PID [Velocity] */
    _talonL.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor,
                                        Constants.kPIDLoopIdx, 
                                        Constants.kTimeoutMs);
    _talonR.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor,
                                        Constants.kPIDLoopIdx, 
                                        Constants.kTimeoutMs);
                                                                                          
    /* Config the peak and nominal outputs */
    _talonL.configNominalOutputForward(0, Constants.kTimeoutMs);
    _talonL.configNominalOutputReverse(0, Constants.kTimeoutMs);
    _talonL.configPeakOutputForward(.1, Constants.kTimeoutMs); //100%
    _talonL.configPeakOutputReverse(-1, Constants.kTimeoutMs);//100%

    //Motion magic
    _talonL.configMotionCruiseVelocity(1500, Constants.kTimeoutMs);
    _talonL.configMotionAcceleration(10000, Constants.kTimeoutMs);
    //zero sensor
    _talonL.setSelectedSensorPosition(0, Constants.kPIDLoopIdx, Constants.kTimeoutMs);


    _talonR.configNominalOutputForward(0, Constants.kTimeoutMs);
    _talonR.configNominalOutputReverse(0, Constants.kTimeoutMs);
    _talonR.configPeakOutputForward(1, Constants.kTimeoutMs);
    _talonR.configPeakOutputReverse(-1, Constants.kTimeoutMs);

    /* Config the Velocity closed loop gains in slot0 */
    _talonL.config_kF(Constants.kPIDLoopIdx, 0, Constants.kTimeoutMs);
    _talonL.config_kP(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kP, Constants.kTimeoutMs);
    _talonL.config_kI(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kI, Constants.kTimeoutMs);
    _talonL.config_kD(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kD, Constants.kTimeoutMs);

    _talonL.setNeutralMode(NeutralMode.Brake);

    _talonR.config_kF(Constants.kPIDLoopIdx, 0, Constants.kTimeoutMs);
    _talonR.config_kP(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kP, Constants.kTimeoutMs);
    _talonR.config_kI(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kI, Constants.kTimeoutMs);
    _talonR.config_kD(Constants.kPIDLoopIdx, Constants.kGains_Velocit.kD, Constants.kTimeoutMs);
    
    _talonL.setStatusFramePeriod(StatusFrameEnhanced.Status_13_Base_PIDF0, 200);
    _talonL.setStatusFramePeriod(StatusFrameEnhanced.Status_10_Targets, 200);
    
    _talonR.setStatusFramePeriod(StatusFrameEnhanced.Status_13_Base_PIDF0, 200);
    _talonR.setStatusFramePeriod(StatusFrameEnhanced.Status_10_Targets, 200);
    
    /*
      * Talon FX does not need sensor phase set for its integrated sensor
      * This is because it will always be correct if the selected feedback device is integrated sensor (default value)
      * and the user calls getSelectedSensor* to get the sensor's position/velocity.
      * 
      * https://phoenix-documentation.readthedocs.io/en/latest/ch14_MCSensor.html#sensor-phase
      */
    // _talon.setSensorPhase(true);
    SmartDashboard.putNumber("kP",kp_pos);
    SmartDashboard.putNumber("kI",ki_pos);
    SmartDashboard.putNumber("kD",kd_pos);
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
    // Update the LED panel buffer
    updateLEDs();

    // This will clobber part of the image
    updateSensorDisplay();

    // Update the motors
    updateMotors();
    
    // Set the LEDs (commit)
    m_ledPanel.commit();
  }

  public void updateLEDs() {
    m_tickCount++;
    if (m_showsRainbow.getSelected()) {
      updateRainbow();
    } else {
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
    }

  }

  private void updateRainbow() {
    // For every pixel up to the start of the horizotal meter
    int ledStringLength = m_ledHeight * m_meterColumnStart;
    for (var i = 0; i < ledStringLength; i++) {
      // Calculate the hue - hue is easier for rainbows because the color
      // shape is a circle so only one value needs to precess
      final var hue = (m_rainbowFirstPixelHue + (int)(i * 180.0 / ledStringLength)) % 180;
      // Set the value
      m_ledPanel.setPixelHSVByID(i, hue, 255, 255);
    }
    // Increase by to make the rainbow "move"
    m_rainbowFirstPixelHue += 3;
    // Check bounds
    m_rainbowFirstPixelHue %= 180;
  }


  public void updateMotors() {
    /* Get gamepad axis */
    double leftYstick = -1 * _joy.getY();

    /* Get Talon/Victor's current output percentage */
    double motorOutputL = _talonL.getMotorOutputPercent();
    double motorOutputR = _talonR.getMotorOutputPercent();

    /** 
     * When button 1 is held, start and run Velocity Closed loop.
     * Velocity Closed Loop is controlled by joystick position x500 RPM, [-500, 500] RPM
     */
    if (_joy.getRawButton(1)) {
      /* Velocity Closed Loop */

      /**
       * Convert 2000 RPM to units / 100ms.
       * 2048 Units/Rev * 2000 RPM / 600 100ms/min in either direction:
       * velocity setpoint is in units/100ms
       */
      double targetVelocity_UnitsPer100ms = leftYstick * 2000.0 * 2048.0 / 600.0;

      /* 2000 RPM in either direction */
      _talonL.set(TalonFXControlMode.MotionMagic, target_pos);
      //_talonR.set(TalonFXControlMode.Velocity, targetVelocity_UnitsPer100ms * -1);
    } else {
      /* Percent Output */
      

      if(_joy.getRawButton(11)){
        _talonL.set(TalonFXControlMode.MotionMagic, abs_pos_1);
      }
      else if(_joy.getRawButton(9)){
        _talonL.set(TalonFXControlMode.MotionMagic, abs_pos_2);
      }
      //_talonR.set(TalonFXControlMode.PercentOutput, leftYstick * -1);
    }

    /* put data on smart dashboard*/
    SmartDashboard.putNumber("L_Pos",_talonL.getSelectedSensorPosition(0));
    SmartDashboard.putNumber("R_Pos",_talonR.getSelectedSensorPosition(0));
    double dat = SmartDashboard.getNumber("L_Pos", 0);
    SmartDashboard.putNumber("Data", dat);

    /* increment count */
    if (tik >= max_tik_tocK_count){
      tik = 0;
      double _kf = SmartDashboard.getNumber("kF", 0 );
      double _kp = SmartDashboard.getNumber("kP", kp_pos );
      double _ki = SmartDashboard.getNumber("kI", ki_pos );
      double _kd = SmartDashboard.getNumber("kD", kd_pos );
      _talonL.config_kP(Constants.kPIDLoopIdx, _kp, Constants.kTimeoutMs);
      _talonL.config_kI(Constants.kPIDLoopIdx, _ki, Constants.kTimeoutMs);
      _talonL.config_kD(Constants.kPIDLoopIdx, _kd, Constants.kTimeoutMs);
    }
    else{
      tik++;
    }

    /*update position*/
    if( tik < (max_tik_tocK_count/2)){
      target_pos = abs_pos_1;
    }
    else{
      target_pos = abs_pos_2;
    }

    SmartDashboard.putNumber("Target Position", target_pos);
    SmartDashboard.putNumber("L_pos_num", _talonL.getSelectedSensorPosition(0));
    SmartDashboard.putNumber("error",_talonL.getClosedLoopError(Constants.kPIDLoopIdx));

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

    // Images write to the whole display (potentially), so we need to tell the
    // meter to clear any unused area
    m_hMeter.clearUnusedArea();
  }

  @Override
  public void autonomousInit() {}

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {}

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