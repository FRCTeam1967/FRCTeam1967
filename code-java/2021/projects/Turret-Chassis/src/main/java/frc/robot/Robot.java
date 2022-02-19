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

import com.janksters.robotcode.TestTurret;

import edu.wpi.first.wpilibj.XboxController;
import com.ctre.phoenix.sensors.CANCoder;

import com.ctre.phoenix.motorcontrol.ControlMode;
import com.ctre.phoenix.motorcontrol.can.TalonSRX;
import com.ctre.phoenix.motorcontrol.can.VictorSPX;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import com.ctre.phoenix.motorcontrol.can.MotControllerJNI;
import edu.wpi.first.wpilibj.SpeedControllerGroup;

import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;


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

  // Turret
  int turretChannelNumber = 6;
  int lowerBound = -5;
  int upperBound = 1;
  int badData = -1000;
  double turretSpeedManual = 0.4;
  double turretUsingVision = 0.1;

  // Ports of joysticks & sensors
  int leftJoystickChannel = 0;
  int rightJoystickChannel = 1;
  int xboxChannelNumber = 2;

  //Chassis (prog bot)
  int frontLeftMotorChannel = 1;
  int rearLeftMotorChannel = 2;
  int frontRightMotorChannel = 3;
  int rearRightMotorChannel = 4;

  //Chassis (janky bot)
  int shootingLeftMotorChannel = 4;
  int intakeLeftMotorChannel = 0;
  int shootingRightMotorChannel = 1;
  int intakeRightMotorChannel = 3;

  // Vision
  String visionDistance = "Distance to Tape ";
  String visionOffset = "Offset";
  int noDataDefault = -1000;
  int CTRE_MagEncoder_Absolute = 8;

  //Janky Drivestick ports
  int buttonTrigPort = 1;
  int button2Port = 2;
  int button3Port = 3;
  int button4Port = 4;
  int button5Port = 5;
  int button6Port = 6;

  //encoders
  final int PULSES_PER_REVOLUTION = 4096;
  final double WHEEL_DIAMETER = 6; // 0.1524 meters, 6 inches
  final double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * Math.PI;

  //Turret
  private XboxController xbox = new XboxController(xboxChannelNumber);
  private TestTurret turret = new TestTurret(turretChannelNumber); 

  private CANCoder _coder = new CANCoder(1);
  private int _loopCount = 0;

  TalonSRX _talon0 = new TalonSRX(0); // Change '0' to match device ID in Tuner.  Use VictorSPX for Victor SPXs
  //Joystick _joystick = new Joystick(0);


  //Chassis:
  PWMVictorSPX flmotor;
  WPI_TalonSRX rlmotor;
  WPI_TalonSRX frmotor;
  PWMVictorSPX rrmotor;
  SpeedControllerGroup leftDrive;
  SpeedControllerGroup rightDrive;
  DifferentialDrive drive;

  //Chassis joysticks
  Joystick leftJoystick;
  Joystick rightJoystick;

  boolean shootingSideFront = true;



  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    /*
    SpeedController m_leftLeader = new WPI_TalonSRX(0);
    SpeedController m_leftFollower = new PWMVictorSPX(2);
    SpeedControllerGroup m_left = new SpeedControllerGroup(m_leftLeader, m_leftFollower);
    
    SpeedController m_rightLeader = new WPI_TalonSRX(1);
    SpeedController m_rightFollower = new PWMVictorSPX(3);
    SpeedControllerGroup m_right = new SpeedControllerGroup(m_rightLeader, m_rightFollower);
    

    m_myRobot = new DifferentialDrive(m_left, m_right);
    m_leftStick = new Joystick(0);
    m_rightStick = new Joystick(1);
    */
      //Chassis
    flmotor = new PWMVictorSPX(shootingLeftMotorChannel);
    rlmotor = new WPI_TalonSRX(intakeLeftMotorChannel);
    frmotor = new WPI_TalonSRX(shootingRightMotorChannel);
    rrmotor = new PWMVictorSPX(intakeRightMotorChannel);

    leftDrive = new SpeedControllerGroup(flmotor, rlmotor);
    rightDrive = new SpeedControllerGroup(frmotor, rrmotor);
    drive = new DifferentialDrive(leftDrive, rightDrive);

    leftJoystick= new Joystick(leftJoystickChannel);
    rightJoystick = new Joystick(rightJoystickChannel);

    drive.setSafetyEnabled(false);

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
    //chassis encoders
    rlmotor.configFactoryDefault();
    rlmotor.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute,0,0);
    rlmotor.setSelectedSensorPosition(0, 10, 0);
    frmotor.configFactoryDefault();
    frmotor.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute,0,0);
    frmotor.setSelectedSensorPosition(0, 10, 0);
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
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {

  }

  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {  }

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {
    //chassis
    boolean buttonPressed = false;
    boolean drivingToggle = leftJoystick.getRawButton(4);

    if (drivingToggle && shootingSideFront)
    {
      shootingSideFront = false;
      buttonPressed = true;
    }
    else if(drivingToggle && !buttonPressed && !shootingSideFront)
    {
      shootingSideFront = true;
      buttonPressed = true;
    }
    else if (!drivingToggle && buttonPressed)
    {
      buttonPressed = false;
  }

    if (shootingSideFront){
      drive.tankDrive(-1.0 * leftJoystick.getY(), -1.0 * rightJoystick.getY());
    }
    else if (!shootingSideFront) {
      drive.tankDrive(rightJoystick.getY(), leftJoystick.getY());
    }

    SmartDashboard.putNumber("Left Joystick", leftJoystick.getY());
    SmartDashboard.putNumber("Right Joystick", rightJoystick.getY());

    boolean driveStraightButton = rightJoystick.getRawButton(5);
    if(driveStraightButton && shootingSideFront) {
      double avg = ((leftJoystick.getY()) + (rightJoystick.getY())) / 2;
      drive.tankDrive(-avg, -avg);
    }
    else if (driveStraightButton && !shootingSideFront) {
      double avg = ((leftJoystick.getY()) + (rightJoystick.getY())) / 2;
      drive.tankDrive(avg, avg);
    }

  
    //chassis encoder:
    SmartDashboard.putNumber("left Encoder teleop", rlmotor.getSensorCollection().getQuadraturePosition());
    SmartDashboard.putNumber("right Encoder teleop", frmotor.getSensorCollection().getQuadraturePosition());
    double rlMotorEncoder=rlmotor.getSensorCollection().getQuadraturePosition();
    double rlMotorEncoderInches=rlMotorEncoder * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION);
    double frMotorEncoder=-1.0*frmotor.getSensorCollection().getQuadraturePosition();
    double frMotorEncoderInches=frMotorEncoder *  (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION);

    SmartDashboard.putNumber("Left Distance in Inches: ", rlMotorEncoderInches);
    SmartDashboard.putNumber("Right Distance in Inches: ", frMotorEncoderInches);


    // Turret
    Double offsetFromVisionTarget = SmartDashboard.getNumber(visionOffset, noDataDefault);

    boolean buttonStart = xbox.getStartButton();
    boolean buttonBack = xbox.getBackButton();

    if (buttonStart) {
      turret.turretRight(0.2);
    }
    else if (buttonBack) {
      turret.turretLeft(0.2);
    }
    else if (offsetFromVisionTarget == badData){
      turret.stopTurret();
    }
    else if ((offsetFromVisionTarget < lowerBound) && (xbox.getYButton())){
      turret.turretRight(turretUsingVision);
    }
    else if ((offsetFromVisionTarget > upperBound) && (xbox.getYButton())){
      turret.turretLeft(turretUsingVision);
    }
    else {
      turret.stopTurret();
    }
    
    // Chassis
        // driving with 2 joysticks on janky chassis
        //boolean drivingToggle = 
    /*
    if(_loopCount++ > 10){
      _loopCount = 0;
      int degrees = _coder.getPosition();
      System.out.println("CANCoder position is: " + degrees);
    }

    double stick = _joystick.getRawAxis(1);
    _talon0.set(ControlMode.PercentOutput, stick); // from https://docs.ctre-phoenix.com/en/latest/ch13_MC.html
    
    */
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