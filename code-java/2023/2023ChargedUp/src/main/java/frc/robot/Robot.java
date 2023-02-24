// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

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
  private Joystick leftJoystick = new Joystick(Constants.Controllers.LEFT_JOYSTICK_PORT);
  private Joystick rightJoystick = new Joystick(Constants.Controllers.RIGHT_JOYSTICK_PORT);

  //declaring mechanism objects
  private Intake m_intake;
  private DriveSystem m_chassis;
  private Arm m_arm;

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

    //defining mechanism objects
    m_intake = new Intake();
    m_arm = new Arm();

    m_arm.initEncoder();
    m_arm.armHoming();

    if (Constants.Chassis.USE_SIMPLE_CHASSIS){
      m_chassis = new SimpleChassis();
    } else {
      m_chassis = new PIDChassis();
    }

    //shuffleboard
    m_matchTab = Shuffleboard.getTab("Match");
    m_intake.configDashboard(m_matchTab);
    m_arm.configDashboard(m_matchTab);
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

    m_arm.armHoming();
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
    m_arm.armHoming();
  }

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {
    //chassis button triggers
    if (leftJoystick.getRawButton(1) || rightJoystick.getRawButton(1)) {
      m_chassis.driveStraight(leftJoystick.getY(), rightJoystick.getY());

    } else if (leftJoystick.getRawButton(2) || rightJoystick.getRawButton(2)) {
       m_chassis.slowMode(leftJoystick.getY(), rightJoystick.getY());

    } else {
       m_chassis.drive(leftJoystick.getY(), rightJoystick.getY());
    }

    //intake button triggers
    if (xboxController.GetLeftThrottle()==1 || xboxController.GetRightThrottle()==1) {
      m_intake.runIntake();

    } else if (xboxController.getPOV() == 0){
      m_intake.runLowEject();

    } else if (xboxController.getPOV() == 90){
      m_intake.runMiddleEject();

    } else if(xboxController.getPOV() == 180){
      m_intake.runHighEject();

    } else {
      m_intake.setMotorsToZero();
    }

    //arm button triggers
    if(xboxController.GetButtonRB()){
      m_arm.setDesiredPosition(Constants.Arm.INTAKE_ANGLE);
      System.out.println("Intake button pressed");
      
    } else if(xboxController.GetButtonA() && !xboxController.GetButtonX() && !xboxController.GetButtonY() && !xboxController.GetButtonB()){
      m_arm.setDesiredPosition(Constants.Arm.fMIDDLE_ANGLE);
      System.out.println("Front Middle button pressed");

    } else if(xboxController.GetButtonY() && !xboxController.GetButtonX() && !xboxController.GetButtonA() && !xboxController.GetButtonB()){
      m_arm.setDesiredPosition(Constants.Arm.fTOP_ANGLE);
      System.out.println("Front Top button pressed");
      
    } else if(xboxController.GetButtonLB()){
      m_arm.setDesiredPosition(Constants.Arm.SAFE_ANGLE);
      System.out.println("Safe button pressed");
      
    } else if(xboxController.GetButtonB() && !xboxController.GetButtonX() && !xboxController.GetButtonY() && !xboxController.GetButtonA()){
      m_arm.setDesiredPosition(Constants.Arm.bMIDDLE_ANGLE);
      System.out.println("Back Middle button pressed");
      
    } else if(xboxController.GetButtonX() && !xboxController.GetButtonY() && !xboxController.GetButtonA() && !xboxController.GetButtonB()){
      m_arm.setDesiredPosition(Constants.Arm.bTOP_ANGLE);
      System.out.println("Back Top button pressed");
      
    } else if (xboxController.GetButtonStart()){
      m_arm.armHoming(); //if arm slips during match, press the start button to re-home arm
      System.out.println("Start button pressed");
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
  public void testInit() {
    m_arm.armHoming();
  }

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
