// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.
package frc.robot;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.PneumaticsModuleType;
import edu.wpi.first.wpilibj.PneumaticsControlModule;
import org.janksters.jankyLib.*;
/**
 * The VM is configured to automatically run this class, and to call the functions corresponding to
 * each mode, as described in the TimedRobot documentation. If you change the name of this class or
 * the package after creating this project, you must also update the build.gradle file in the
 * project.
 */
public class Robot extends TimedRobot {
  private final int PCMChannel = 12;
  private final int SolChannel = 0;
  private final int falconChannelL = 5;
  private final int falconChannelR = 6;
  private Solenoid solenoid1; //= new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, SolChannel);
  //private final Solenoid solenoid2 = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, SolChannel);
  private final JankyTalonFX falconL = new JankyTalonFX(falconChannelL);
  private final JankyTalonFX falconR = new JankyTalonFX(falconChannelR);
  /* hi hi hi hihihihihihiihihihihihihii */
  private jankyXboxJoystick XboxController = new jankyXboxJoystick(2);
  private PneumaticsControlModule pcm;
  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    solenoid1 = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, SolChannel);
    pcm = new PneumaticsControlModule(PCMChannel);
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
  }
  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {

  }
  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {
    falconL.setUpJankyTalonFX();
    falconR.setUpJankyTalonFX();
    /* allowable error
    falconL.configAllowableClosedloopError(0, Constants.kPIDLoopIdx, Constants.kTimeoutMs);
    falconR.configAllowableClosedloopError(0, Constants.kPIDLoopIdx, Constants.kTimeoutMs);
    /* left PID config
    falconL.config_kF(Constants.kPIDLoopIdx, Constants.winchDownKF, Constants.kTimeoutMs);
    falconL.config_kP(Constants.kPIDLoopIdx, Constants.winchDownKP, Constants.kTimeoutMs);
    falconL.config_kI(Constants.kPIDLoopIdx, Constants.winchDownKI, Constants.kTimeoutMs);
    falconL.config_kD(Constants.kPIDLoopIdx, Constants.winchDownKD, Constants.kTimeoutMs);
    /* right PID config
    falconR.config_kF(Constants.kPIDLoopIdx, Constants.winchDownKF, Constants.kTimeoutMs);
    falconR.config_kP(Constants.kPIDLoopIdx, Constants.winchDownKP, Constants.kTimeoutMs);
    falconR.config_kI(Constants.kPIDLoopIdx, Constants.winchDownKI, Constants.kTimeoutMs);
    falconR.config_kD(Constants.kPIDLoopIdx, Constants.winchDownKD, Constants.kTimeoutMs); */
  }
  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {
    SmartDashboard.putNumber("falconL encoder count", falconL.getSensorCollection().getIntegratedSensorPosition());
    SmartDashboard.putNumber("falconR encoder count", falconR.getSensorCollection().getIntegratedSensorPosition());

    SmartDashboard.putNumber("pcm solenoids", pcm.getSolenoids());
    SmartDashboard.putNumber("compressor current", pcm.getCompressorCurrent());
    SmartDashboard.putBoolean("compressor?", pcm.getCompressor());
    
    if(XboxController.GetButtonA()){
      solenoid1.set(true);
      //solenoid2.set(true);
    } else if (XboxController.GetButtonY()){
      solenoid1.set(false);
      //solenoid2.set(false);
    }
    if (XboxController.GetRightYAxis() > 0.2 || XboxController.GetRightYAxis() < -0.2) {
      falconL.set(XboxController.GetRightYAxis());
      falconR.set(XboxController.GetRightYAxis());
    } else {
      falconL.set(0);
      falconR.set(0);
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