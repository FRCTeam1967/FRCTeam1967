// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;
import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.StatorCurrentLimitConfiguration;
import com.ctre.phoenix.motorcontrol.StatusFrameEnhanced;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.revrobotics.ColorMatchResult;
import com.revrobotics.CANSparkMax.ControlType;

import org.janksters.CommonClassesThisYear.AutoConstants;
import org.janksters.CommonClassesThisYear.AutoSelector;
import org.janksters.CommonClassesThisYear.AutoStateMachine;
import org.janksters.CommonClassesThisYear.ClimbMBR;
import org.janksters.CommonClassesThisYear.ColorSensor;
import org.janksters.CommonClassesThisYear.Constants;
import org.janksters.CommonClassesThisYear.LED;
import org.janksters.CommonClassesThisYear.PivotMagic;
import org.janksters.CommonClassesThisYear.Shooter;
import org.janksters.CommonClassesThisYear.VisionSubsystem;
import org.janksters.jankyLib.jankyXboxJoystick;

import edu.wpi.first.cameraserver.CameraServer;
import edu.wpi.first.cscore.UsbCamera;
//import edu.wpi.first.cameraserver.CameraServer;
import edu.wpi.first.wpilibj.ADIS16470_IMU;
import edu.wpi.first.wpilibj.I2C;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.PowerDistribution;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
// import edu.wpi.first.wpilibj.motorcontrol.MotorController;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
// import edu.wpi.first.wpilibj.motorcontrol.PWMVictorSPX;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.util.Color;

public class Robot extends TimedRobot {
  private UsbCamera camera;

  private int autoDelaySelected;
  private final SendableChooser<Integer> autoDelayChooser = new SendableChooser<>();

  //private PowerDistribution pdp;

  public int ledCounter = 0;

  private Joystick leftJoystick;
  private Joystick rightJoystick;
  private jankyXboxJoystick XboxController;

  double averageSpeed;

  //Chassis in a Day - Lazlo
    /*private WPI_TalonSRX leftLeader = new WPI_TalonSRX(2);
    private PWMVictorSPX leftFollower = new PWMVictorSPX(3);
    private WPI_TalonFX rightLeader = new WPI_TalonFX(1);
    private PWMVictorSPX rightFollower = new PWMVictorSPX(0);*/

  //2022 Real Bot (wifi is bob)
    private WPI_TalonFX leftLeader = new WPI_TalonFX(Constants.CHASSIS_L_LEADER_CHANNEL);//m2
    private WPI_TalonFX leftFollower = new WPI_TalonFX(Constants.CHASSIS_L_FOLLOWER_CHANNEL);//m3
    private WPI_TalonFX rightLeader = new WPI_TalonFX(Constants.CHASSIS_R_LEADER_CHANNEL);//m1
    private WPI_TalonFX rightFollower = new WPI_TalonFX(Constants.CHASSIS_R_FOLLOWER_CHANNEL);//m0

  //2022 Janky (wifi is C1Day)
    // private WPI_TalonSRX leftLeader = new WPI_TalonSRX(2);//m2
    // private WPI_TalonFX leftFollower = new WPI_TalonFX(3);//m3
    // private WPI_TalonFX rightLeader = new WPI_TalonFX(1);//m1
    // private WPI_TalonSRX rightFollower = new WPI_TalonSRX(0);//m0

  private MotorControllerGroup left;
  private MotorControllerGroup right;
  private DifferentialDrive myRobot;

  // Color sensor configuration
  private ColorSensor m_colorSensorRight;
  private ColorSensor m_colorSensorLeft;
  private static final double m_ConfidenceThreshold = 0.85;
  private Timer ColorSensorTimer = new Timer();

  //Average 3 samples
  private final Color kRedBallTarget = new Color((0.5825 + 0.57764 + 0.5712) / 3,
    (0.3091 + 0.3122 + 0.3188) / 3,
    (0.1086 + 0.1108 + 0.1104) / 3);

  private final Color kBlueBallTarget = new Color((0.1323 + 0.1232 + 0.1323) / 3,
    (0.3621 + 0.3613 + 0.3618)/3,
    (0.5059 + 0.5068 + 0.5061)/3);

  LED led = new LED(1, 500, 9); 

  Shooter shooter = new Shooter();
  //Pivot pivot = new Pivot();
  PivotMagic pivotmagic = null;

  //AUTO
  AutoStateMachine autoSM = null;
  public ADIS16470_IMU m_gyro = new ADIS16470_IMU();
  AutoSelector delaySelector =  new AutoSelector();  

  private ClimbMBR climbMech = null;

  public void instantiateClimb(){
    climbMech = new ClimbMBR(Constants.CLIMB_WINCH_MOTOR_CHANNEL_L, Constants.CLIMB_WINCH_MOTOR_CHANNEL_R);
  }

  public void instantiatePivotMagic(){
    pivotmagic = new PivotMagic();
  }

  public void setUpChassisMotors(WPI_TalonFX falcon){
    StatorCurrentLimitConfiguration chassisCurrentLimit = new StatorCurrentLimitConfiguration(true, Constants.CHASSIS_CURRENT_LIMIT_MAX,
    Constants.CHASSIS_TRIGGER_CURRENT, Constants.CHASSIS_THRESHOLD_TIME);

    falcon.configFactoryDefault();
    falcon.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_K_TIMEOUT_MS);
    falcon.configNominalOutputForward(0, Constants.CHASSIS_K_TIMEOUT_MS);
    falcon.configNominalOutputReverse(0, Constants.CHASSIS_K_TIMEOUT_MS);
    falcon.configPeakOutputForward(1, Constants.CHASSIS_K_TIMEOUT_MS);
    falcon.configPeakOutputReverse(-1, Constants.CHASSIS_K_TIMEOUT_MS);
    falcon.config_kF(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kF, Constants.CHASSIS_K_TIMEOUT_MS);
    falcon.config_kP(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kP, Constants.CHASSIS_K_TIMEOUT_MS);
    falcon.config_kI(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kI, Constants.CHASSIS_K_TIMEOUT_MS);
    falcon.config_kD(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kD, Constants.CHASSIS_K_TIMEOUT_MS);
    falcon.configStatorCurrentLimit(chassisCurrentLimit);
    falcon.setNeutralMode(NeutralMode.Brake);

    if(Constants.SUPER_CHASSIS){
      falcon.setStatusFramePeriod(StatusFrameEnhanced.Status_13_Base_PIDF0, Constants.CAN_STATUS_FRAME_PERIOD, Constants.CHASSIS_K_TIMEOUT_MS);
    } else{
      //falcon.setStatusFramePeriod(0x1240, Constants.CAN_STATUS_FRAME_PERIOD);
      falcon.setStatusFramePeriod(StatusFrameEnhanced.Status_12_Feedback1, Constants.CAN_STATUS_FRAME_PERIOD, Constants.CHASSIS_K_TIMEOUT_MS);
    }
  }

  public void updateColorSensor(){
    if(ColorSensorTimer.get() >= 0.5){
      ColorSensorTimer.reset();

      int proximityRight = m_colorSensorRight.getProximity();
      int proximityLeft = m_colorSensorLeft.getProximity();

      // Use .match() to get a match or null if nothing 
      // matches. Use .closestMatch() to force a match
      ColorMatchResult closestMatchRight = m_colorSensorRight.match();
      Color displayColorRight = new Color(0, 0, 0);
      double confidenceRight = 0.0;

      ColorMatchResult closestMatchLeft = m_colorSensorLeft.match();
      Color displayColorLeft = new Color(0, 0, 0);
      double confidenceLeft = 0.0;

      if (closestMatchRight != null) { 
        Color colorRight = closestMatchRight.color;
        confidenceRight = closestMatchRight.confidence;

        if (colorRight == kBlueBallTarget) {
          System.out.println("Blue");
          SmartDashboard.putString("Right Color Match", "Blue");
          displayColorRight = Color.kBlue;
        } else if (colorRight == kRedBallTarget) {
          System.out.println("Red");
          SmartDashboard.putString("Right Color Match", "Red");
          displayColorRight = Color.kRed;
        } else {
          System.out.println("No Match");
          SmartDashboard.putString("Right Color Match", "None"); 
        }
      }

      if (closestMatchLeft != null) { 
        Color colorLeft = closestMatchLeft.color;
        confidenceLeft = closestMatchLeft.confidence;

        if (colorLeft == kBlueBallTarget) {
          System.out.println("Blue");
          SmartDashboard.putString("Left Color Match", "Blue");
          displayColorLeft = Color.kBlue;
        } else if (colorLeft == kRedBallTarget) {
          System.out.println("Red");
          SmartDashboard.putString("Left Color Match", "Red");
          displayColorLeft = Color.kRed;
        } else {
          System.out.println("No Match");
          SmartDashboard.putString("Left Color Match", "None"); 
        }
      }

      SmartDashboard.putNumber("Right Color Sensor Proximity", proximityRight);
      SmartDashboard.putNumber("Right Color Sensor Confidence", confidenceRight);
      SmartDashboard.putNumber("Left Color Sensor Proximity", proximityLeft);
      SmartDashboard.putNumber("Left Color Sensor Confidence", confidenceLeft);
    }
  }

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    //JOYSTICKS
    leftJoystick = new Joystick(0);  //whatever is in port 0
    rightJoystick = new Joystick(1); //whatever is in port 0
    XboxController = new jankyXboxJoystick(2); 

    //CHASSIS
    //pdp = new PowerDistribution();

    setUpChassisMotors(leftLeader);
    setUpChassisMotors(leftFollower);
    setUpChassisMotors(rightLeader);
    setUpChassisMotors(rightFollower);

    if(!Constants.SUPER_CHASSIS){
      left = new MotorControllerGroup(leftLeader, leftFollower);
      right = new MotorControllerGroup(rightLeader, rightFollower);
      myRobot = new DifferentialDrive(left,right);

      myRobot.setMaxOutput(0.7); //default is 1, also does current limit
    }

    //COLOR SENSOR
    // Init ColorSensor subsystem
    if (Constants.SUPER_COLOR_SENSOR){
      Color colorsToMatch[] = {kRedBallTarget, kBlueBallTarget};

      m_colorSensorRight = new ColorSensor(I2C.Port.kOnboard);
      m_colorSensorRight.setColorMatches(colorsToMatch);
      m_colorSensorRight.setConfidenceThreshold(m_ConfidenceThreshold);

      m_colorSensorLeft = new ColorSensor(I2C.Port.kMXP);
      m_colorSensorLeft.setColorMatches(colorsToMatch);
      m_colorSensorLeft.setConfidenceThreshold(m_ConfidenceThreshold);

      ColorSensorTimer.reset();
      ColorSensorTimer.start();
    }

    camera = CameraServer.startAutomaticCapture(Constants.CAMERA_DEV_NUMBER);
    //camera.setResolution(160, 120);
    camera.setResolution(80, 60);
    //camera.setFPS(5);
    camera.setFPS(3);

    //AUTO 
    autoDelayChooser.setDefaultOption("0", AutoConstants.ZeroDelay);
    autoDelayChooser.addOption("1", AutoConstants.OneDelay); 
    autoDelayChooser.addOption("2", AutoConstants.TwoDelay);
    autoDelayChooser.addOption("3", AutoConstants.ThreeDelay);
    autoDelayChooser.addOption("4", AutoConstants.FourDelay);
    autoDelayChooser.addOption("5", AutoConstants.FiveDelay);
    SmartDashboard.putData("Auto Delay Chooser", autoDelayChooser);

    // //PIVOT
    // if (pivotmagic == null){
    //   instantiatePivotMagic();
    // }

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
    if(Constants.SUPER_COLOR_SENSOR){
      updateColorSensor();
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
    if (pivotmagic == null){
      instantiatePivotMagic();
    }

    autoDelaySelected = autoDelayChooser.getSelected();
    //autoDelaySelected = AutoConstants.ZeroDelay;
    SmartDashboard.putNumber("Auto Delay Selected", autoDelaySelected);

    autoSM = new AutoStateMachine(autoDelaySelected, AutoConstants.ONE_BALL_AUTOPATH, m_gyro, shooter, pivotmagic);
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {
    autoSM.displayCurrentState();
  }

  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {
    if (pivotmagic == null){
       instantiatePivotMagic();
    }

    if (autoSM != null) {
      autoSM.endStateMachine();
      autoSM = null;
    }

    if (climbMech == null){
      instantiateClimb();
    }

    if(!Constants.SUPER_SHOOTER){
      shooter.removePID();
    }

    leftLeader.setNeutralMode(NeutralMode.Coast);
    leftFollower.setNeutralMode(NeutralMode.Coast);
    rightLeader.setNeutralMode(NeutralMode.Coast);
    rightFollower.setNeutralMode(NeutralMode.Coast);
  }

  /** This function is called periodically during operator control. :) */
  @Override
  public void teleopPeriodic() {
    // ledCounter = led.setRainbow(ledCounter);
    // led.commit();

    //SHOOTER
    if(!Constants.SUPER_SHOOTER){
      double YAxisValue = XboxController.GetLeftYAxis();
      if(YAxisValue >= Constants.SIMPLE_SHOOT_INTAKE_DEADBAND) {
        shooter.simpleShoot(Constants.SIMPLE_SHOOT_SPEED);
      } else if (YAxisValue <= -Constants.SIMPLE_SHOOT_INTAKE_DEADBAND) {
        shooter.simpleIntake(Constants.SIMPLE_INTAKE_SPEED);
      } //TODO: add in set to 0 with stopShooter() after proving stopVelocityPID works
    }
    shooter.displayCurrentState();

    //CHASSIS
    SmartDashboard.putBoolean("Super Chassis", Constants.SUPER_CHASSIS);
    if(Constants.SUPER_CHASSIS){
      if (leftJoystick.getRawButton(1) || rightJoystick.getRawButton(1)) { // drive straight, was button 3
        averageSpeed = (leftJoystick.getY() + rightJoystick.getY()) / 2;
        double chassisTargetVelocity = averageSpeed * Constants.CHASSIS_RPM * Constants.FALCON_PULSES_PER_REVOLUTION / 600.0; //units per 100 seconds
        leftLeader.set(TalonFXControlMode.Velocity, -1.0 * chassisTargetVelocity);
        leftFollower.set(TalonFXControlMode.Velocity, -1.0 * chassisTargetVelocity);
        rightLeader.set(TalonFXControlMode.Velocity, chassisTargetVelocity);
        rightFollower.set(TalonFXControlMode.Velocity, chassisTargetVelocity);
      } else {
        //change sensitivity by cubing values from joystick
        double origLeftY = leftJoystick.getY();
        double origRightY = rightJoystick.getY();
        
        //keep exponent odd (or make leftY/rightY negative if origLeftY/origRightY is negative)
        double leftY = Math.pow(origLeftY, 3);
        double rightY = Math.pow(origRightY, 3);
        
        double chassisTargetVelocityLeft = -1.0 * leftY * Constants.CHASSIS_RPM * Constants.FALCON_PULSES_PER_REVOLUTION / 600.0; //units per 100 seconds
        double chassisTargetVelocityRight = rightY * Constants.CHASSIS_RPM * Constants.FALCON_PULSES_PER_REVOLUTION / 600.0;
        
        if(Math.abs(origLeftY) < Constants.CHASSIS_DEADBAND){
          chassisTargetVelocityLeft = 0;
        }
        if(Math.abs(origRightY) < Constants.CHASSIS_DEADBAND){
          chassisTargetVelocityRight = 0;
        }
        
        leftLeader.set(TalonFXControlMode.Velocity, chassisTargetVelocityLeft);
        leftFollower.set(TalonFXControlMode.Velocity, chassisTargetVelocityLeft);
        rightLeader.set(TalonFXControlMode.Velocity, chassisTargetVelocityRight);
        rightFollower.set(TalonFXControlMode.Velocity, chassisTargetVelocityRight);

        // SmartDashboard.putNumber("chassis left target velocity", chassisTargetVelocityLeft);
        // SmartDashboard.putNumber("chassis right target velocity", chassisTargetVelocityRight);
      }
    } 
    else{
      if (leftJoystick.getRawButton(1) || rightJoystick.getRawButton(1)) { 
        averageSpeed = (leftJoystick.getY() + rightJoystick.getY()) / 2;
        myRobot.tankDrive(-averageSpeed, averageSpeed);
      } else{
        myRobot.tankDrive(-leftJoystick.getY(), rightJoystick.getY());
      }
    }
   
    // SmartDashboard.putNumber("total current", pdp.getTotalCurrent());
    // SmartDashboard.putNumber("left leader current", pdp.getCurrent(Constants.CHASSIS_L_LEADER_CIRCUIT));
    // SmartDashboard.putNumber("left follower current", pdp.getCurrent(Constants.CHASSIS_L_FOLLOWER_CIRCUIT));
    // SmartDashboard.putNumber("right leader current", pdp.getCurrent(Constants.CHASSIS_R_LEADER_CIRCUIT));
    // SmartDashboard.putNumber("right follower current", pdp.getCurrent(Constants.CHASSIS_R_FOLLOWER_CIRCUIT));
    
    // SmartDashboard.putNumber("left joystick", leftJoystick.getY());
    // SmartDashboard.putNumber("right joystick", rightJoystick.getY());
    // SmartDashboard.putNumber("Left leader motor output", leftLeader.getMotorOutputPercent());
    // SmartDashboard.putNumber("Left follower motor output", leftFollower.getMotorOutputPercent());
    // SmartDashboard.putNumber("right leader motor output", rightLeader.getMotorOutputPercent());
    // SmartDashboard.putNumber("right follower motor output", rightFollower.getMotorOutputPercent());   
  }
  
  /** This function is called once when the robot is disabled. */
  @Override
  public void disabledInit() {
    leftLeader.setNeutralMode(NeutralMode.Coast);
    leftFollower.setNeutralMode(NeutralMode.Coast);
    rightLeader.setNeutralMode(NeutralMode.Coast);
    rightFollower.setNeutralMode(NeutralMode.Coast);

  }

  /** This function is called periodically when disabled. */
  @Override
  public void disabledPeriodic() {
  }

  /** This function is called once when test mode is enabled. */
  @Override
  public void testInit() {}

  /** This function is called periodically during test mode. */
  @Override
  public void testPeriodic() {}
}
