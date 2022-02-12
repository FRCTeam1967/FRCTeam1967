// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.XboxController;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorController;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.motorcontrol.PWMVictorSPX;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.math.controller.PIDController;
//import edu.wpi.first.wpilibj2.command.Command;
import org.janksters.CommonClassesThisYear.*;
import edu.wpi.first.wpilibj.ADXRS450_Gyro;
import edu.wpi.first.wpilibj.SPI;
import edu.wpi.first.wpilibj.ADIS16470_IMU.IMUAxis;
import edu.wpi.first.wpilibj.ADIS16470_IMU;

import edu.wpi.first.wpilibj.AnalogGyro;



/**
 * The VM is configured to automatically run this class, and to call the functions corresponding to
 * each mode, as described in the TimedRobot documentation. If you change the name of this class or
 * the package after creating this project, you must also update the build.gradle file in the
 * project.
 */
public class Robot extends TimedRobot {

  //AutoDriveSubsystem m_drive;
  private static final String kDefaultAuto = "Default";
  private static final String kCustomAuto = "My Auto";
  private String m_autoSelected;
  private final SendableChooser<String> m_chooser = new SendableChooser<>();

  private WPI_TalonSRX m_leftLeader = new WPI_TalonSRX(0);
  private MotorController m_leftFollower = new PWMVictorSPX(3);
  private WPI_TalonSRX m_rightLeader = new WPI_TalonSRX(1);
  private MotorController m_rightFollower = new PWMVictorSPX(4);
  private MotorControllerGroup m_left = new MotorControllerGroup(m_leftLeader, m_leftFollower);
  private MotorControllerGroup m_right = new MotorControllerGroup(m_rightLeader, m_rightFollower);
  public DifferentialDrive m_myRobot = new DifferentialDrive(m_left,m_right);
  PIDController pidDistance = new PIDController(0.05, 0, 0);
  PIDController pidAngle = new PIDController(0.01, 0, 0);

  private XboxController m_Controller = new XboxController(2);
  private boolean m_LimelightHasValidTarget = false;

  private Joystick m_leftStick = new Joystick(0);
  private Joystick m_rightStick = new Joystick(1);

  //final PWMVictorSPX flmotor = new PWMVictorSPX(4);
  WPI_TalonSRX rlmotor = new WPI_TalonSRX(0);
  WPI_TalonSRX frmotor = new WPI_TalonSRX(1);
  //final PWMVictorSPX rrmotor = new PWMVictorSPX(3);

  // Auto delayForZero = new Auto(frmotor, rlmotor, m_myRobot, 0);
  // Command delayZero = delayForZero;

  // Auto delayForOne = new Auto(frmotor, rlmotor, m_myRobot, 1);
  // Command delayOne = delayForOne;

  // Auto delayForTwo = new Auto(frmotor, rlmotor, m_myRobot, 2);
  // Command delayTwo = delayForTwo;

  // Auto delayForThree = new Auto(frmotor, rlmotor, m_myRobot, 3);
  // Command delayThree = delayForThree;


  

  Auto generalAuto = new Auto(frmotor, rlmotor, m_myRobot);
  AutoSelector selector =  new AutoSelector();
  AutoSelector pathSelector = new AutoSelector();

  public ADIS16470_IMU m_gyro;

  int autoCaseNum;
  


  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_gyro = new ADIS16470_IMU();
    autoCaseNum = 0;
    SmartDashboard.putString("hello", "hello");
    // m_gyro.calibrate();
    m_gyro.reset();
    // m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    // m_chooser.addOption("My Auto", kCustomAuto);
    // SmartDashboard.putData("Auto choices", m_chooser);

    // CHASSIS ENCODERS
    rlmotor.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute, 0, 0);
    rlmotor.setSelectedSensorPosition(0, 0, 10);
    rlmotor.getSensorCollection().setQuadraturePosition(0,10);
    frmotor.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute, 0, 0);
    frmotor.setSelectedSensorPosition(0, 0, 10);
    frmotor.getSensorCollection().setQuadraturePosition(0,10);

  
    selector.DisplayAutoOptions();
    pathSelector.DisplayActualAutoOptions();
    
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

    // selector.getAutoMode();
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
    m_myRobot.tankDrive(0, 0);
    SmartDashboard.putString("hello auto init", "it works");

  

    
    
    Auto.delay = selector.getAutoMode();

    // if (selector.getAutoMode() == 0) {
    //   Auto.delay = 0;
    // } else if (selector.getAutoMode() == 1) {
    //   Auto.delay = 1;
    // } else if (selector.getAutoMode() == 2) {
    //   Auto.delay = 2;
    // } else if (selector.getAutoMode() == 3) {
    //   Auto.delay = 3;
    // } else if (selector.getAutoMode() == 4) {
    //   Auto.delay = 4;
    // } else if (selector.getAutoMode() == 5) {
    //   Auto.delay = 5;
    // } 

   
    Auto.autoPathFinal = pathSelector.getActualAutoMode();


    // if (pathSelector.getActualAutoMode() == 0) {
    //   Auto.autoPathFinal = 0;
    // } else if (pathSelector.getActualAutoMode() == 1) {
    //   Auto.autoPathFinal = 1;
    // } else if (pathSelector.getActualAutoMode() == 2) {
    //   Auto.autoPathFinal = 2;
    // }

   
  
    autoCaseNum = 0;

    SmartDashboard.putNumber("initial auto delay: ", Auto.delay);
    SmartDashboard.putNumber("initial auto path selected", Auto.autoPathFinal);

    generalAuto.resetDelayTimer();
    m_gyro.reset();


    
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {

    SmartDashboard.putNumber("triaxis angle" , m_gyro.getAngle());
    SmartDashboard.putNumber("triaxis rate" , m_gyro.getRate());
    
  
    if (Auto.autoPathFinal == 0){
      generalAuto.AutoSequenceSimple(m_myRobot,  selector.getAutoMode(), m_gyro, 0);
    }
    else if (Auto.autoPathFinal == 1){
      generalAuto.AutoSequenceStandard(m_myRobot,  selector.getAutoMode(), m_gyro, 0);
    }else if (Auto.autoPathFinal == 2){
      generalAuto.autoTurn180(m_myRobot, m_gyro);
    } else{
      m_myRobot.tankDrive(0, 0);
    }

  }

  public int getCaseNum(){
    return autoCaseNum;
  }



  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {
    
  }

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {
  
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
