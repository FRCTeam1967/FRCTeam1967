// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.math.MathUtil;
import edu.wpi.first.wpilibj.Joystick;


import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

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
  Limelight limelight;
  //KitBotChassis m_chassis;
  WPI_TalonFX leftLeader, leftFollower, rightLeader, rightFollower;
  MotorControllerGroup leftGroup, rightGroup;
  DifferentialDrive myRobot;

  Joystick leftJoystick, rightJoystick;


  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);
    
    //m_chassis = new KitBotChassis();
    leftJoystick = new Joystick(0);
    rightJoystick = new Joystick(1);
    leftLeader = new WPI_TalonFX(Constants.LEFT_LEADER);
    leftFollower = new WPI_TalonFX(Constants.LEFT_FOLLOWER);
    rightLeader = new WPI_TalonFX(Constants.RIGHT_LEADER);
    rightFollower = new WPI_TalonFX(Constants.RIGHT_FOLLOWER);

    leftLeader.setNeutralMode(NeutralMode.Brake);
    leftFollower.setNeutralMode(NeutralMode.Brake);
    rightLeader.setNeutralMode(NeutralMode.Brake);
    rightFollower.setNeutralMode(NeutralMode.Brake);
    MotorControllerGroup leftGroup = new MotorControllerGroup(leftLeader,leftFollower);
    MotorControllerGroup rightGroup = new MotorControllerGroup(rightLeader,rightFollower);

    myRobot = new DifferentialDrive(leftGroup, rightGroup);
   
    limelight = new Limelight();
  }
 
  /**
   * This function is called every 20 ms, no matter the mode. Use this for items like diagnostics
   * that you want ran during disabled, autonomous, teleoperated and test.
   *
   * <p>This runs after the mode specific periodic functions, but before LiveWindow and
   * SmartDashboard integrated updating.
   */
  @Override
  public void robotPeriodic() {}

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
  public void teleopPeriodic() {
    if (leftJoystick.getRawButton(1) || rightJoystick.getRawButton(1)){
      double avgSpeed = (leftJoystick.getY() + rightJoystick.getY())/2;
      myRobot.tankDrive(-avgSpeed, avgSpeed);
    } else if (leftJoystick.getRawButton(3) || rightJoystick.getRawButton(3)) {
      limelight.trackingLimelightSteer();
      myRobot.tankDrive(-limelight.getLeftCommand(), limelight.getRightCommand());
    } else if (leftJoystick.getRawButton(4) || rightJoystick.getRawButton(4)) {
      limelight.trackingLimelightDist();
      myRobot.tankDrive(limelight.getLeftCommand(), limelight.getRightCommand());
    } else {
      double cubedLeft = Math.pow(leftJoystick.getY(),3);
      double cubedRight = Math.pow(rightJoystick.getY(),3);
      myRobot.tankDrive(-cubedLeft,cubedRight);
    }
  }

  public double applyDeadband(double val){
    val = MathUtil.applyDeadband(val, 0.1);
    return val;
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

  /** This function is called once when the robot is first started up. */
  @Override
  public void simulationInit() {}

  /** This function is called periodically whilst in simulation. */
  @Override
  public void simulationPeriodic() {}

  
}
