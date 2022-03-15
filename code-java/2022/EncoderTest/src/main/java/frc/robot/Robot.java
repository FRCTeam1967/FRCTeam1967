// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import org.janksters.jankyLib.jankyXboxJoystick;

import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.TalonFXInvertType;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.ErrorCode;

import edu.wpi.first.wpilibj.RobotController;



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
  private jankyXboxJoystick xbox = null;
  private WPI_TalonFX talon = null;
  private int CLIMB_K_TIMEOUT_MS = 1;
  private int CLIMB_K_PID_LOOP_IDX = 0;
  private double CLIMB_WINCH_DOWN_KP = 0.15;
  private double CLIMB_WINCH_DOWN_KI = 0.0;
  private double CLIMB_WINCH_DOWN_KD = 0.1;
  private double CLIMB_WINCH_DOWN_KF = 0.0;

  private boolean xWasPressed = false;
  private boolean xPressed = false;


  
  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);
    talon = new WPI_TalonFX(4); 
    xbox= new jankyXboxJoystick(2);
    

    talon.configFactoryDefault();
    talon.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, CLIMB_K_PID_LOOP_IDX, CLIMB_K_TIMEOUT_MS);
    //winchMotorL.setSensorPhase(true);
    talon.setInverted(TalonFXInvertType.CounterClockwise);
    talon.configNominalOutputForward(0, CLIMB_K_TIMEOUT_MS);
    talon.configNominalOutputReverse(0, CLIMB_K_TIMEOUT_MS);
    talon.configPeakOutputForward(1, CLIMB_K_TIMEOUT_MS);
    talon.configPeakOutputReverse(-1, CLIMB_K_TIMEOUT_MS);
    talon.config_kF(CLIMB_K_PID_LOOP_IDX, CLIMB_WINCH_DOWN_KF, CLIMB_K_TIMEOUT_MS);
		talon.config_kP(CLIMB_K_PID_LOOP_IDX, CLIMB_WINCH_DOWN_KP, CLIMB_K_TIMEOUT_MS);
		talon.config_kI(CLIMB_K_PID_LOOP_IDX, CLIMB_WINCH_DOWN_KI, CLIMB_K_TIMEOUT_MS);
		talon.config_kD(CLIMB_K_PID_LOOP_IDX, CLIMB_WINCH_DOWN_KD, CLIMB_K_TIMEOUT_MS);

    talon.setNeutralMode(NeutralMode.Brake);
    talon.setNeutralMode(NeutralMode.Brake);
    }
  /**
   * This function is called every robot packet, no matter the mode. Use this for items like
   * diagnostics that you want ran during disabled, autonomous, teleoperated and test.
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
  public void teleopInit() {}

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {
    double yAxis= xbox.GetLeftYAxis();
    if (Math.abs(yAxis)>0.1){
      talon.set(yAxis);
    } else{
      talon.set(0);
    }

    SmartDashboard.putNumber("talon encoder count", talon.getSelectedSensorPosition());

    xPressed = xbox.GetButtonX();
    if(xPressed && !xWasPressed){
      double timeBefore = RobotController.getFPGATime();
      ErrorCode encoderReset = talon.getSensorCollection().setIntegratedSensorPosition(0, 10);
      SmartDashboard.putNumber("Time to reset", RobotController.getFPGATime() - timeBefore); //in microseconds
      SmartDashboard.putString("encoderReset?", encoderReset.name()); 
      xWasPressed = true;
    } else if (!xPressed && xWasPressed){
      xWasPressed = false;
    }
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
