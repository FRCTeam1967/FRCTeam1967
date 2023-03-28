// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.motorcontrol.NeutralMode;

import edu.wpi.first.wpilibj.ADIS16470_IMU;
import edu.wpi.first.wpilibj.Joystick;

import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import org.janksters.jankyLib.jankyXboxJoystick;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;


public class Robot extends TimedRobot {
  private int autoDelaySelected, autoPathSelected;
  private final SendableChooser<Integer> autoDelayChooser = new SendableChooser<>();
  private final SendableChooser<Integer> autoPathChooser = new SendableChooser<>();

  AutoStateMachine autoSM = null;

  private Joystick leftJoystick;
  private Joystick rightJoystick;
  private jankyXboxJoystick XboxController;
  
  private WPI_TalonFX leftLeader = new WPI_TalonFX(4);//m2
  private WPI_TalonFX rightLeader = new WPI_TalonFX(5);
  //private WPI_TalonFX leftFollower = new WPI_TalonFX(6);//m2
  //private WPI_TalonFX rightFollower = new WPI_TalonFX(7);

  private MotorControllerGroup left, right;
  private DifferentialDrive myRobot;
  double averageSpeed;
  

  public ADIS16470_IMU m_gyro = new ADIS16470_IMU();
  public double gyroAngle;
  public ShuffleboardTab AutoPIDTestingTab;


  public void setUpChassisMotors(WPI_TalonFX falcon){
    falcon.configFactoryDefault();

    falcon.configNominalOutputForward(0, 75);
    falcon.configNominalOutputReverse(0, 75);
    falcon.configPeakOutputForward(1, 75);
    falcon.configPeakOutputReverse(-1, 75);

    //needs to be tuned - default variables
    falcon.config_kP(0, 0.1, 75);
    falcon.config_kI(0, 0.0, 75);
    falcon.config_kD(0, 0.0, 75);
    falcon.configClosedloopRamp(0.7); 

    falcon.setNeutralMode(NeutralMode.Brake);
  }


  @Override
  public void robotInit() {

    autoDelayChooser.setDefaultOption("0", AutoConstants.ZeroDelay);
    autoDelayChooser.addOption("1", AutoConstants.OneDelay); 
    autoDelayChooser.addOption("2", AutoConstants.TwoDelay);
    autoDelayChooser.addOption("3", AutoConstants.ThreeDelay);
    autoDelayChooser.addOption("4", AutoConstants.FourDelay);
    autoDelayChooser.addOption("5", AutoConstants.FiveDelay);
    SmartDashboard.putData("Auto Delay Chooser", autoDelayChooser);

    autoPathChooser.setDefaultOption("Community", AutoConstants.SIMPLE_AUTOPATH);
    autoPathChooser.addOption("One cube", AutoConstants.ONE_CUBE_AUTOPATH);
    autoPathChooser.addOption("Charge station", AutoConstants.CHARGE_STATION);
    SmartDashboard.putData("Auto Path Chooser", autoPathChooser);

    leftJoystick = new Joystick(0);
    rightJoystick = new Joystick(1);
    XboxController = new jankyXboxJoystick(2); 

    setUpChassisMotors(leftLeader);
    //setUpChassisMotors(leftFollower);
    setUpChassisMotors(rightLeader);
    //setUpChassisMotors(rightFollower);

    left = new MotorControllerGroup(leftLeader);
    right = new MotorControllerGroup(rightLeader);
    myRobot = new DifferentialDrive(left,right);

    AutoPIDTestingTab = Shuffleboard.getTab("Auto PID Testing");
    AutoPIDTestingTab.addDouble("Gyro angle auto", () -> gyroAngle);

  }

  @Override
  public void robotPeriodic() {}

  @Override
  public void autonomousInit() {
    autoDelaySelected = autoDelayChooser.getSelected();
    SmartDashboard.putNumber("Auto Delay Selected", autoDelaySelected);

    autoPathSelected = autoPathChooser.getSelected();
    SmartDashboard.putNumber("Auto Path Selected", autoPathSelected);

    autoSM = new AutoStateMachine(autoDelaySelected, 2, m_gyro);
  }

  @Override
  public void autonomousPeriodic() {
    SmartDashboard.putNumber("Pitch Gyro Angle", (m_gyro.getYComplementaryAngle()));
    
    autoSM.displayCurrentState();

    //SmartDashboard.putNumber("Left leader % output", leftLeader.getMotorOutputPercent());
    //SmartDashboard.putNumber("Right leader % output", rightLeader.getMotorOutputPercent());

    //SmartDashboard.putNumber("Left motor encoder:", leftLeader.getSelectedSensorPosition());
    //SmartDashboard.putNumber("Right motor encoder:", rightLeader.getSelectedSensorPosition());
  }

  @Override
  public void teleopInit() {
    leftLeader.setNeutralMode(NeutralMode.Coast);
    rightLeader.setNeutralMode(NeutralMode.Coast);
  }

  @Override
  public void teleopPeriodic() {
    SmartDashboard.putNumber("Left motor encoder:", leftLeader.getSelectedSensorPosition());
    SmartDashboard.putNumber("Right motor encoder:", rightLeader.getSelectedSensorPosition());

    SmartDashboard.putNumber("Pitch Gyro Angle", (m_gyro.getYComplementaryAngle()));

    //testing velocity in teleop
    averageSpeed = (leftJoystick.getY() + rightJoystick.getY()) / 2;
    double targetVelocity_UnitsPer100ms = averageSpeed * 2000.0 * 2048.0 / 600.0;
    rightLeader.set(TalonFXControlMode.Velocity, targetVelocity_UnitsPer100ms);
    leftLeader.set(TalonFXControlMode.Velocity, -targetVelocity_UnitsPer100ms);

   //myRobot.tankDrive(-leftJoystick.getY(), rightJoystick.getY());
  
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
