// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import com.ctre.phoenix.motorcontrol.StatorCurrentLimitConfiguration;
import com.ctre.phoenix.motorcontrol.StatusFrameEnhanced;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.motorcontrol.NeutralMode;

import edu.wpi.first.wpilibj.ADIS16470_IMU;
import edu.wpi.first.wpilibj.I2C;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.Timer;


import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import org.janksters.jankyLib.jankyXboxJoystick;

/**
 * The VM is configured to automatically run this class, and to call the functions corresponding to
 * each mode, as described in the TimedRobot documentation. If you change the name of this class or
 * the package after creating this project, you must also update the build.gradle file in the
 * project.
 */
public class Robot extends TimedRobot {
  private int autoDelaySelected, autoPathSelected;
  private final SendableChooser<Integer> autoDelayChooser = new SendableChooser<>();
  private final SendableChooser<Integer> autoPathChooser = new SendableChooser<>();

  AutoStateMachine autoSM = null;

  private Joystick leftJoystick;
  private Joystick rightJoystick;
  private jankyXboxJoystick XboxController;
  
  private WPI_TalonFX leftLeader = new WPI_TalonFX(3);//m2
  private WPI_TalonFX rightLeader = new WPI_TalonFX(1);

  private MotorControllerGroup left, right;
  private DifferentialDrive myRobot;
  double averageSpeed;
  

  public ADIS16470_IMU m_gyro = new ADIS16470_IMU();


  public void setUpChassisMotors(WPI_TalonFX falcon){
    StatorCurrentLimitConfiguration chassisCurrentLimit = new StatorCurrentLimitConfiguration(true, 50,
    45, 0);

    falcon.configFactoryDefault();
    falcon.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 0, 75);
    falcon.configNominalOutputForward(0, 75);
    falcon.configNominalOutputReverse(0, 75);
    falcon.configPeakOutputForward(1, 75);
    falcon.configPeakOutputReverse(-1, 75);
    //falcon.config_kF(0, 0.1074, 75);
    falcon.config_kP(0, 0.48072, 75);
    falcon.config_kI(0, 0.0, 75);
    falcon.config_kD(0, 0.0, 75);
    falcon.configClosedloopRamp(0.7); 
    falcon.configStatorCurrentLimit(chassisCurrentLimit);
    falcon.setNeutralMode(NeutralMode.Brake);
    
    if(false){
      falcon.setStatusFramePeriod(StatusFrameEnhanced.Status_13_Base_PIDF0, 100, 75);
    } else{
      //falcon.setStatusFramePeriod(0x1240, Constants.CAN_STATUS_FRAME_PERIOD);
      falcon.setStatusFramePeriod(StatusFrameEnhanced.Status_12_Feedback1, 100, 75);
    }
  }



  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
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
    setUpChassisMotors(rightLeader);

    left = new MotorControllerGroup(leftLeader);
    right = new MotorControllerGroup(rightLeader);
    myRobot = new DifferentialDrive(left,right);

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
    leftLeader.setNeutralMode(NeutralMode.Brake);
    rightLeader.setNeutralMode(NeutralMode.Brake);

    autoDelaySelected = autoDelayChooser.getSelected();
    SmartDashboard.putNumber("Auto Delay Selected", autoDelaySelected);

    autoPathSelected = autoPathChooser.getSelected();
    SmartDashboard.putNumber("Auto Path Selected", autoPathSelected);

    autoSM = new AutoStateMachine(autoDelaySelected, 2, m_gyro);
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {
    SmartDashboard.putNumber("Pitch Gyro Angle", (m_gyro.getYComplementaryAngle() * -1));
    autoSM.displayCurrentState();
  }

  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {

    leftLeader.setNeutralMode(NeutralMode.Coast);
    rightLeader.setNeutralMode(NeutralMode.Coast);

  }

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {

    SmartDashboard.putNumber("Pitch Gyro Angle", (m_gyro.getYComplementaryAngle()));
   // myRobot.tankDrive(-leftJoystick.getY(), rightJoystick.getY());

    averageSpeed = (leftJoystick.getY() + rightJoystick.getY()) / 2;
    double targetVelocity_UnitsPer100ms = averageSpeed * 2000.0 * 2048.0 / 600.0;
    rightLeader.set(TalonFXControlMode.Velocity, targetVelocity_UnitsPer100ms);
    leftLeader.set(TalonFXControlMode.Velocity, -targetVelocity_UnitsPer100ms);

    // if (leftJoystick.getRawButton(1) || rightJoystick.getRawButton(1)) { 
    //   averageSpeed = (leftJoystick.getY() + rightJoystick.getY()) / 2;
    //   //double targetVelocity_UnitsPer100ms = averageSpeed * 2000.0 * 2048.0 / 600.0;
    //   myRobot.tankDrive(-averageSpeed, averageSpeed);
    //   //rightLeader.set(TalonFXControlMode.Velocity, targetVelocity_UnitsPer100ms);
    //   //leftLeader.set(TalonFXControlMode.Velocity, targetVelocity_UnitsPer100ms);
    // } else{
    //   myRobot.tankDrive(-leftJoystick.getY(), rightJoystick.getY());
    // }
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
