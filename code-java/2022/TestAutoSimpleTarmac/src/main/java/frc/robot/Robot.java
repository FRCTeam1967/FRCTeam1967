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
import org.janksters.CommonClassesThisYear.*;
import edu.wpi.first.wpilibj.ADXRS450_Gyro;
import edu.wpi.first.wpilibj.SPI;
import edu.wpi.first.wpilibj.ADIS16470_IMU.IMUAxis;
import edu.wpi.first.wpilibj.ADIS16470_IMU;

import edu.wpi.first.wpilibj.AnalogGyro;


public class Robot extends TimedRobot {

  //idk when we use this either
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

  WPI_TalonSRX rlmotor = new WPI_TalonSRX(0);
  WPI_TalonSRX frmotor = new WPI_TalonSRX(1);

  

  Auto generalAuto = new Auto(frmotor, rlmotor, m_myRobot);
  AutoSelector selector =  new AutoSelector();
  AutoSelector pathSelector = new AutoSelector();

  public ADIS16470_IMU m_gyro;

  int autoCaseNum;
  


  @Override
  public void robotInit() {
    //initializing the gyro --> 4 seconds don't touch the robot bc callibration.
    m_gyro = new ADIS16470_IMU();
    autoCaseNum = 0;
    m_gyro.reset();
    rlmotor.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute, 0, 0);
    rlmotor.setSelectedSensorPosition(0, 0, 10);
    rlmotor.getSensorCollection().setQuadraturePosition(0,10);
    frmotor.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute, 0, 0);
    frmotor.setSelectedSensorPosition(0, 0, 10);
    frmotor.getSensorCollection().setQuadraturePosition(0,10);
    selector.DisplayAutoOptions();
    pathSelector.DisplayActualAutoOptions();
    
}

  @Override
  public void robotPeriodic() {
  }

  @Override
  public void autonomousInit() {
    
    Auto.delay = selector.getAutoMode();

   //making the path selector
    Auto.autoPathFinal = pathSelector.getActualAutoMode();
   
    //idk where we use this but we are keeping it just in case
    autoCaseNum = 0;
  
    SmartDashboard.putNumber("initial auto delay: ", Auto.delay);
    SmartDashboard.putNumber("initial auto path selected", Auto.autoPathFinal);
    //making sure delay timer = 0
    generalAuto.resetDelayTimer();
    m_gyro.reset();
    
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {
//figuring out what the gyro angle
    SmartDashboard.putNumber("triaxis angle" , m_gyro.getAngle());
    SmartDashboard.putNumber("triaxis rate" , m_gyro.getRate());
    
//which auto path we should run based on what is selected
    if (Auto.autoPathFinal == 0){
      //just moving forward
      generalAuto.AutoSequenceSimple(m_myRobot,  selector.getAutoMode(), m_gyro, 0);
    }
    else if (Auto.autoPathFinal == 1){
      //moving forward, getting ball, moving back, shooting
      generalAuto.AutoSequenceStandard(m_myRobot,  selector.getAutoMode(), m_gyro, 0);
    }else if (Auto.autoPathFinal == 2){
      //just for testing purposes, turning
      generalAuto.autoTurn180(m_myRobot, m_gyro);
    } else{
      m_myRobot.tankDrive(0, 0);
    }

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
