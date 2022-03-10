// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
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
import edu.wpi.first.wpilibj.motorcontrol.PWMTalonFX;


public class Robot extends TimedRobot {

  PIDController pidDistance = new PIDController(0.05, 0, 0);
  PIDController pidAngle = new PIDController(0.01, 0, 0);

  public ADIS16470_IMU m_gyro = new ADIS16470_IMU();
  
  AutoStateMachine autoSM;
  AutoSelector delaySelector =  new AutoSelector();
  AutoSelector pathSelector = new AutoSelector();


  int autoCaseNum;



  @Override
  public void robotInit() {
    delaySelector.DisplayDelayOptions();
    pathSelector.DisplayPathOptions();
    autoCaseNum = 0;
  }

  @Override
  public void robotPeriodic() {
  }

  @Override
  public void autonomousInit() {
    autoCaseNum = 0;
    
    autoSM = new AutoStateMachine(delaySelector.getDelaySelected(),pathSelector.getPathSelected(), m_gyro);
  }

  @Override
  public void autonomousPeriodic() {
    
    autoSM.displayCurrentState();
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
