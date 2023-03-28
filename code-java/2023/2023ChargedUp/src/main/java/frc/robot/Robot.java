// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import org.janksters.CommonClassesThisYear.*;
import org.janksters.jankyLib.*;

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

  //joystick objects
  private jankyXboxJoystick xboxController = new jankyXboxJoystick(Constants.Controllers.XBOX_PORT);

  //declaring mechanism objects
  private Intake m_intake;

  //shuffleboard
  public ShuffleboardTab m_matchTab;

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);

    //shuffleboard
    m_matchTab = Shuffleboard.getTab("Match");

    //defining mechanism objects
    m_intake = new Intake();
        
    //configure shuffleboard
    m_intake.configDashboard(m_matchTab);
<<<<<<< Updated upstream
=======
    m_arm.configDashboard(m_matchTab);
    

    // m_matchTab.addCamera("Limelight Camera", "m_limelight", "http://10.19.67.11:5800/");
    

    //AUTO SELECTORS
    //delay timer chooser
    autoDelayChooser.setDefaultOption("0", Constants.Auto.ZERO_DELAY);
    autoDelayChooser.addOption("1", Constants.Auto.ONE_DELAY); 
    autoDelayChooser.addOption("2", Constants.Auto.TWO_DELAY);
    autoDelayChooser.addOption("3", Constants.Auto.THREE_DELAY);
    autoDelayChooser.addOption("4", Constants.Auto.FOUR_DELAY);
    autoDelayChooser.addOption("5", Constants.Auto.FIVE_DELAY);
    m_matchTab.add("Auto Delay", autoDelayChooser).withWidget(BuiltInWidgets.kComboBoxChooser);

    //auto path chooser
    autoPathChooser.setDefaultOption("Community", Constants.Auto.SIMPLE_AUTOPATH);
    autoPathChooser.addOption("One cube", Constants.Auto.ONE_CUBE_AUTOPATH);
    autoPathChooser.addOption("Charge station", Constants.Auto.CHARGE_STATION);
    autoPathChooser.addOption("Charge station + Intake", Constants.Auto.DELUXE_CHARGE_STATION);
    m_matchTab.add("Auto Path", autoPathChooser).withWidget(BuiltInWidgets.kComboBoxChooser);

    //default led sequnce- chasing red
    m_led.setChasingColors(Color.kRed, Color.kBlack, 10, 0.005);
>>>>>>> Stashed changes
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
  public void teleopInit() {}

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {

    //intake button triggers
    if (xboxController.GetButtonX() && !xboxController.GetButtonA() && !xboxController.GetButtonB() && !xboxController.GetButtonY()) {
      m_intake.runIntake();

    } else if (xboxController.GetButtonA() && !xboxController.GetButtonX() && !xboxController.GetButtonB() && !xboxController.GetButtonY()){
      m_intake.runLowEject();

    } else if (xboxController.GetButtonB() && !xboxController.GetButtonA() && !xboxController.GetButtonX() && !xboxController.GetButtonY()){
      m_intake.runMiddleEject();

    } else if(xboxController.GetButtonY() && !xboxController.GetButtonA() && !xboxController.GetButtonB() && !xboxController.GetButtonX()){
      m_intake.runHighEject();

    } else {
      m_intake.setMotorsToZero();
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
