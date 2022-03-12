// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;
import org.janksters.CommonClassesThisYear.*;


import edu.wpi.first.cscore.UsbCamera;
import edu.wpi.first.cameraserver.CameraServer;

import edu.wpi.first.wpilibj.ADIS16470_IMU;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
// import edu.wpi.first.wpilibj.motorcontrol.MotorController;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
// import edu.wpi.first.wpilibj.motorcontrol.PWMVictorSPX;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import com.ctre.phoenix.motorcontrol.can.WPI_VictorSPX;
import com.ctre.phoenix.motorcontrol.ControlMode;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.TalonSRXControlMode;

import edu.wpi.first.wpilibj.motorcontrol.PWMMotorController;
import edu.wpi.first.wpilibj.motorcontrol.PWMVictorSPX;
import javax.swing.text.AbstractDocument.LeafElement;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import edu.wpi.first.math.controller.PIDController;

public class Robot extends TimedRobot {

  private static final int CAMERA_DEV_NUMBER = 0;
  private UsbCamera camera;


  private static final String kTankDrive = "Tank Drive";
  private static final String kArcadeDrive = "Arcade Drive";
  private static final String kCurvatureDrive = "Curvature Drive";
  private static final String kSteeringWheel = "Steering Wheel";

  private String DriveSelected;
  private final SendableChooser<String> driveChooser = new SendableChooser<>();

  public int ledCounter = 0;
  Joystick leftJoystick;
  Joystick rightJoystick;
  double averageSpeed;

  //Chassis in a Day - Lazlo
    /*private WPI_TalonSRX leftLeader = new WPI_TalonSRX(2);
    private PWMVictorSPX leftFollower = new PWMVictorSPX(3);
    private WPI_TalonFX rightLeader = new WPI_TalonFX(1);
    private PWMVictorSPX rightFollower = new PWMVictorSPX(0);*/

  //2022 Real Bot (wifi is bob)
    private WPI_TalonFX leftLeader = new WPI_TalonFX(2);//m2
    private WPI_TalonFX leftFollower = new WPI_TalonFX(3);//m3
    private WPI_TalonFX rightLeader = new WPI_TalonFX(1);//m1
    private WPI_TalonFX rightFollower = new WPI_TalonFX(0);//m0


  //2022 Janky (wifi is C1Day)
    /*private WPI_TalonSRX leftLeader = new WPI_TalonSRX(2);//m2
    private WPI_TalonFX leftFollower = new WPI_TalonFX(3);//m3
    private WPI_TalonFX rightLeader = new WPI_TalonFX(1);//m1
    private WPI_TalonSRX rightFollower = new WPI_TalonSRX(0);//m0*/

  //private MotorControllerGroup left = new MotorControllerGroup(leftLeader, leftFollower);
  //private MotorControllerGroup right = new MotorControllerGroup(rightLeader, rightFollower);
  //DifferentialDrive myRobot = new DifferentialDrive(left,right);

  PIDController pidDistance = new PIDController(0.1, 0, 0);
  PIDController pidAngle = new PIDController(0.1, 0, 0);

  VisionSubsystem limeLight = new VisionSubsystem();

  Shooter shooter = new Shooter();
  Pivot pivot = new Pivot();

  AutoStateMachine autoSM = null;
  public ADIS16470_IMU m_gyro = new ADIS16470_IMU();

  AutoSelector delaySelector =  new AutoSelector();
  AutoSelector pathSelector = new AutoSelector();

  // LED led = new LED(1, 500, 9); 
  //private Climb climbMech;

  //AUTO
  /*
  AutoStateMachine autoSM;
  AutoSelector pathSelector = new AutoSelector();
  int autoCaseNum;
  */

  //move forward
  final int simplePath = 0;
  //move forward, get ball, turn, move, shoot
  final int standardPath = 1;
  //turn 180 degrees
  final int turnPath = 2;

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {

    camera = CameraServer.startAutomaticCapture(CAMERA_DEV_NUMBER);
    camera.setResolution(160, 120);
    camera.setFPS(5);
    
    // leftLeader.setNeutralMode(NeutralMode.Brake);
    // leftFollower.setNeutralMode(NeutralMode.Brake);
    // rightLeader.setNeutralMode(NeutralMode.Brake);
    // rightFollower.setNeutralMode(NeutralMode.Brake);

    //AUTO 
    /*
    pathSelector.DisplayActualAutoOptions();
    autoCaseNum = 0;
    */

    //CHASSIS
    driveChooser.setDefaultOption("Tank Drive", kTankDrive);
    driveChooser.addOption("Arcade Drive", kArcadeDrive);
    driveChooser.addOption("Curvature Drive", kCurvatureDrive);
    driveChooser.addOption("Steering Wheel", kSteeringWheel);
    SmartDashboard.putData("Drive choices", driveChooser);

    leftJoystick = new Joystick(0);  //whatever is in port 0
    rightJoystick = new Joystick(1); //whatever is in port 0

    leftLeader.configFactoryDefault();
    leftLeader.configNeutralDeadband(0.001, Constants.CHASSIS_K_TIMEOUT_MS);
    leftLeader.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_K_TIMEOUT_MS);
    leftLeader.configNominalOutputForward(0, Constants.CHASSIS_K_TIMEOUT_MS);
    leftLeader.configNominalOutputReverse(0, Constants.CHASSIS_K_TIMEOUT_MS);
    leftLeader.configPeakOutputForward(1, Constants.CHASSIS_K_TIMEOUT_MS);
    leftLeader.configPeakOutputReverse(-1, Constants.CHASSIS_K_TIMEOUT_MS);
    leftLeader.config_kF(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kF, Constants.CHASSIS_K_TIMEOUT_MS);
    leftLeader.config_kP(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kP, Constants.CHASSIS_K_TIMEOUT_MS);
    leftLeader.config_kI(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kI, Constants.CHASSIS_K_TIMEOUT_MS);
    leftLeader.config_kD(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kD, Constants.CHASSIS_K_TIMEOUT_MS);  

    leftFollower.configFactoryDefault();
    leftFollower.configNeutralDeadband(0.001);
    leftFollower.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_K_TIMEOUT_MS);
    leftFollower.configNominalOutputForward(0, Constants.CHASSIS_K_TIMEOUT_MS);
    leftFollower.configNominalOutputReverse(0, Constants.CHASSIS_K_TIMEOUT_MS);
    leftFollower.configPeakOutputForward(1, Constants.CHASSIS_K_TIMEOUT_MS);
    leftFollower.configPeakOutputReverse(-1, Constants.CHASSIS_K_TIMEOUT_MS);
    leftFollower.config_kF(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kF, Constants.CHASSIS_K_TIMEOUT_MS);
    leftFollower.config_kP(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kP, Constants.CHASSIS_K_TIMEOUT_MS);
    leftFollower.config_kI(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kI, Constants.CHASSIS_K_TIMEOUT_MS);
    leftFollower.config_kD(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kD, Constants.CHASSIS_K_TIMEOUT_MS);  

    rightLeader.configFactoryDefault();
    rightLeader.configNeutralDeadband(0.001);
    rightLeader.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_K_TIMEOUT_MS);
    rightLeader.configNominalOutputForward(0, Constants.CHASSIS_K_TIMEOUT_MS);
    rightLeader.configNominalOutputReverse(0, Constants.CHASSIS_K_TIMEOUT_MS);
    rightLeader.configPeakOutputForward(1, Constants.CHASSIS_K_TIMEOUT_MS);
    rightLeader.configPeakOutputReverse(-1, Constants.CHASSIS_K_TIMEOUT_MS);
    rightLeader.config_kF(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kF, Constants.CHASSIS_K_TIMEOUT_MS);
    rightLeader.config_kP(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kP, Constants.CHASSIS_K_TIMEOUT_MS);
    rightLeader.config_kI(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kI, Constants.CHASSIS_K_TIMEOUT_MS);
    rightLeader.config_kD(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kD, Constants.CHASSIS_K_TIMEOUT_MS);  
    //rightLeader.setInverted(true);
    
    rightFollower.configFactoryDefault();
    rightFollower.configNeutralDeadband(0.001);
    rightFollower.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_K_TIMEOUT_MS);
    rightFollower.configNominalOutputForward(0, Constants.CHASSIS_K_TIMEOUT_MS);
    rightFollower.configNominalOutputReverse(0, Constants.CHASSIS_K_TIMEOUT_MS);
    rightFollower.configPeakOutputForward(1, Constants.CHASSIS_K_TIMEOUT_MS);
    rightFollower.configPeakOutputReverse(-1, Constants.CHASSIS_K_TIMEOUT_MS);
    rightFollower.config_kF(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kF, Constants.CHASSIS_K_TIMEOUT_MS);
    rightFollower.config_kP(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kP, Constants.CHASSIS_K_TIMEOUT_MS);
    rightFollower.config_kI(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kI, Constants.CHASSIS_K_TIMEOUT_MS);
    rightFollower.config_kD(Constants.CHASSIS_K_PID_LOOP_IDX, Constants.CHASSIS_kD, Constants.CHASSIS_K_TIMEOUT_MS);  
    
    /*
    if (climbMech == null){
      climbMech = new Climb(Constants.CLIMB_WINCH_MOTOR_CHANNEL_L, Constants.CLIMB_WINCH_MOTOR_CHANNEL_R,
      Constants.CLIMB_PCM_CHANNEL, Constants.CLIMB_MID_LATCH_CHANNEL_L, Constants.CLIMB_MID_LATCH_CHANNEL_R, pivot);
    }
    */

    //myRobot.setMaxOutput(0.7); //default is 1

    delaySelector.DisplayDelayOptions();
    pathSelector.DisplayPathOptions();

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
    //making the path selector
    /*
    Auto.autoPathFinal = pathSelector.getActualAutoMode();
   
    //idk where we use this but we are keeping it just in case
    autoCaseNum = 0;

    SmartDashboard.putNumber("initial auto path selected", Auto.autoPathFinal);

    autoSM = new AutoStateMachine(pivot, shooter); */

    autoSM = new AutoStateMachine(delaySelector.getDelaySelected(),pathSelector.getPathSelected(), m_gyro, shooter,pivot);

  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {
    autoSM.displayCurrentState();
  }

  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {
    DriveSelected = driveChooser.getSelected();
    System.out.println("Drive selected: " + DriveSelected);

    if (autoSM != null) {
      autoSM.endStateMachine();
    }
  }

  /** This function is called periodically during operator control. :) */
  @Override
  public void teleopPeriodic() {
      // ledCounter = led.setRainbow(ledCounter);
      // led.commit();

    //SHOOTER
    shooter.displayCurrentState();

    //CHASSIS
    if (leftJoystick.getRawButton(4)&& limeLight.targetValid()){ 
      //myRobot.tankDrive(-limeLight.getDistance()-limeLight.getAngle(),limeLight.getDistance() + limeLight.getAngle());//Call limeLight.getAngle() subtract from one side, add to another 
    } else {
        switch (DriveSelected) {
          case kArcadeDrive:
            double rotateSpeed = leftJoystick.getRawAxis(1);
            double moveSpeed = leftJoystick.getRawAxis(0);
            //myRobot.arcadeDrive(moveSpeed, -rotateSpeed); // + move , - rot
            break;
          case kCurvatureDrive:
            // Curvature drive with a given forward and turn rate, as well as a button for turning in-place.
            //myRobot.curvatureDrive(leftJoystick.getX(), -leftJoystick.getY(), leftJoystick.getTrigger());//maybe take out negative?
            break;  
          case kSteeringWheel:
            //put if statement for printing instructions on how to use 
            double rSpeed = leftJoystick.getRawAxis(0);//10% works DON'T CHANGE
            double mSpeed = rightJoystick.getRawAxis(1);
            //myRobot.arcadeDrive(rSpeed, -mSpeed); 
            break;
          default:
            SmartDashboard.putNumber("left joystick", leftJoystick.getY());
            SmartDashboard.putNumber("right joystick", rightJoystick.getY());
            averageSpeed = (leftJoystick.getY() + rightJoystick.getY()) / 2;
            if (leftJoystick.getRawButton(1) || rightJoystick.getRawButton(1)) { // was butto 3
              double chassisTargetVelocity = averageSpeed * Constants.CHASSIS_RPM * Constants.FALCON_PULSES_PER_REVOLUTION / 600.0; //units per 100 seconds
              leftLeader.set(TalonFXControlMode.Velocity, -1.0 * chassisTargetVelocity);
              leftFollower.set(TalonFXControlMode.Velocity, -1.0 * chassisTargetVelocity);
              rightLeader.set(TalonFXControlMode.Velocity, chassisTargetVelocity);
              rightFollower.set(TalonFXControlMode.Velocity, chassisTargetVelocity);
              //myRobot.tankDrive(-averageSpeed, averageSpeed);
            } else {
              //change sensitivity by squaring values from joystick
              double origLeftY = leftJoystick.getY();
              double origRightY = rightJoystick.getY();

              /* boolean leftPositive = false;
              boolean rightPositive = false;
              if (origLeftY > 0 && origLeftY <1) {
                leftPositive = true;
              }
              if (origRightY > 0 && origRightY <1) {
                rightPositive = true;
              }
              
              if (!leftPos) {
                leftY = -1.0 * leftY;
              }
              if (!rightPos) {
                rightY = -1.0 * rightY;
              }*/
              
              double leftY = Math.pow(origLeftY, 3);
              double rightY = Math.pow(origRightY, 3);

              //myRobot.tankDrive(-leftY, rightY);
              double chassisTargetVelocityLeft = -1.0 * leftY * Constants.CHASSIS_RPM * Constants.FALCON_PULSES_PER_REVOLUTION / 600.0; //units per 100 seconds
              double chassisTargetVelocityRight = rightY * Constants.CHASSIS_RPM * Constants.FALCON_PULSES_PER_REVOLUTION / 600.0;
              if(Math.abs(origLeftY) < 0.05){ //deadband
                chassisTargetVelocityLeft = 0;
              }
              if(Math.abs(origRightY) < 0.05){
                chassisTargetVelocityRight = 0;
              }
              leftLeader.set(TalonFXControlMode.Velocity, chassisTargetVelocityLeft);
              leftFollower.set(TalonFXControlMode.Velocity, chassisTargetVelocityLeft);
              rightLeader.set(TalonFXControlMode.Velocity, chassisTargetVelocityRight);
              rightFollower.set(TalonFXControlMode.Velocity, chassisTargetVelocityRight);
              
              SmartDashboard.putNumber("chassis left target velocity", chassisTargetVelocityLeft);
              SmartDashboard.putNumber("chassis right target velocity", chassisTargetVelocityRight);
              SmartDashboard.putNumber("Left leader motor output", leftLeader.getMotorOutputPercent());
              SmartDashboard.putNumber("Left follower motor output", leftFollower.getMotorOutputPercent());
              SmartDashboard.putNumber("right leader motor output", rightLeader.getMotorOutputPercent());
              SmartDashboard.putNumber("right follower motor output", rightFollower.getMotorOutputPercent());
              //myRobot.tankDrive(-leftJoystick.getY(), rightJoystick.getY());
              SmartDashboard.putString("drive being run", DriveSelected);
            }
            break;
        } 
    } 
  }
  /** This function is called once when the robot is disabled. */
  @Override
  public void disabledInit() {
    /*
    leftLeader.setNeutralMode(NeutralMode.Coast);
    leftFollower.setNeutralMode(NeutralMode.Coast);
    rightLeader.setNeutralMode(NeutralMode.Coast);
    rightFollower.setNeutralMode(NeutralMode.Coast);
    */
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