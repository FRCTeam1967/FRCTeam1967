// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import org.janksters.jankyLib.jankyXboxJoystick;

import edu.wpi.first.wpilibj.ADIS16470_IMU;
import edu.wpi.first.wpilibj.DriverStation;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.shuffleboard.BuiltInWidgets;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.util.Color;

/**
 * The VM is configured to automatically run this class, and to call the functions corresponding to
 * each mode, as described in the TimedRobot documentation. If you change the name of this class or
 * the package after creating this project, you must also update the build.gradle file in the
 * project.
 */
 
public class Robot extends TimedRobot {
  //joystick objects
  private jankyXboxJoystick xboxController = new jankyXboxJoystick(Constants.Controllers.XBOX_PORT);
  private Joystick leftJoystick = new Joystick(Constants.Controllers.LEFT_JOYSTICK_PORT);
  private Joystick rightJoystick = new Joystick(Constants.Controllers.RIGHT_JOYSTICK_PORT);
  public double curLeftJsY; //deadband for left joystick (jack)
  public double curRightJsY; //deadband for left joystick (jill)

  //declaring mechanism objects
  private Auto m_auto = null;
  private Intake m_intake;
  private DriveSystem m_chassis;
  private Arm m_arm;
  //private Limelight m_limelight;

  //leds
  private LED m_led;
  
  //gyro defs
  public ADIS16470_IMU m_gyro = new ADIS16470_IMU();
  public double gyroAngle;

  //shuffleboard
  public ShuffleboardTab m_matchTab;
  
  //auto config dashboard
  SendableChooser<Integer> autoDelayChooser = new SendableChooser<Integer>();
  SendableChooser<Integer> autoPathChooser = new SendableChooser<Integer>(); 
  int delay, path;

  /**
   * This function is run when the robot is first started up and should be used for any initialization code.
   */
  @Override
  public void robotInit() {
    //defining mechanism objects
    m_intake = new Intake();
    m_arm = new Arm();
    m_led = new LED(Constants.LED.WIDTH, Constants.LED.LENGTH, Constants.LED.PWM_PORT);
    //m_limelight = new Limelight();
    
    m_arm.initEncoder();
    m_arm.armHoming();

    //shuffleboard
    m_matchTab = Shuffleboard.getTab("Match");
    m_intake.configDashboard(m_matchTab);
    m_arm.configDashboard(m_matchTab);
    //m_limelight.configDashboard(m_matchTab);
    
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
    autoPathChooser.addOption("Two cube", Constants.Auto.TWO_CUBE_AUTOPATH);
    autoPathChooser.addOption("Charge station", Constants.Auto.CHARGE_STATION);
    autoPathChooser.addOption("Deluxe", Constants.Auto.DELUXE_CHARGE_STATION);
    m_matchTab.add("Auto Path", autoPathChooser).withWidget(BuiltInWidgets.kComboBoxChooser);

    //default led sequnce- chasing red
    m_led.setChasingColors(Color.kRed, Color.kBlack, 10, 0.005);
  }

  /**
   * This function is called every 20 ms, no matter the mode. Use this for items like diagnostics
   * that you want ran during disabled, autonomous, teleoperated and test.
   *
   * <p>This runs after the mode specific periodic functions, but before LiveWindow and SmartDashboard integrated updating.
   */
  @Override
  public void robotPeriodic() {
    m_led.executeSequence();
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
    m_arm.armHoming();
    
    //auto- get selected delay and path
    delay = autoDelayChooser.getSelected();
    path = autoPathChooser.getSelected();

    m_auto = new Auto(delay, path, m_gyro, m_arm, m_intake, m_led);
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {}

  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {
    m_arm.armHoming();

    if (m_auto != null && m_auto.isAlive()) {
      m_auto.terminate();
      m_auto = null;
    }

    //defining m_chassis as simple or pid chassis
    if (Constants.Chassis.USE_SIMPLE_CHASSIS){
      m_chassis = new SimpleChassis();
    } else {
      m_chassis = new PIDChassis();
    }

    m_chassis.configDashboard(m_matchTab);
    m_chassis.setBrakeMode(false);

    m_led.executeSequence();
  }

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {
    curLeftJsY = leftJoystick.getY();
    curRightJsY = rightJoystick.getY();
    
    //ght.setVisionMode(false);

    //CHASSIS BUTTON TRIGGERS
    if (leftJoystick.getRawButton(1) || rightJoystick.getRawButton(1)) {
      m_chassis.driveStraight(leftJoystick.getY(), rightJoystick.getY());
    
    } else if (leftJoystick.getRawButton(2) || rightJoystick.getRawButton(2)) {
       m_chassis.slowMode(leftJoystick.getY(), rightJoystick.getY());

    } else if (leftJoystick.getRawButton(3) || rightJoystick.getRawButton(3)) {
      //m_limelight.setVisionMode(true);
      //m_limelight.alignAngle();
      //m_chassis.drive(m_limelight.getLeftCommand(), m_limelight.getRightCommand());
    
    } else {
      // added for sticky joysticks
      if (Math.abs(curLeftJsY) < Constants.Chassis.JOYSTICK_DEADBAND) {
        curLeftJsY = 0;
      }
      if (Math.abs(curRightJsY) < Constants.Chassis.JOYSTICK_DEADBAND){
        curRightJsY = 0;
      }
      m_chassis.drive(curLeftJsY, curRightJsY);
    }

    if (leftJoystick.getRawButton(9) || rightJoystick.getRawButton(9)) {
      m_chassis.setBrakeMode(true); //set to brake mode
    }
    if (leftJoystick.getRawButton(10) || rightJoystick.getRawButton(10)) {
      m_chassis.setBrakeMode(false); //set to coast mode
    }

    //INTAKE BUTTON TRIGGERS
    if (xboxController.GetLeftThrottle() == 1 || xboxController.GetRightThrottle() == 1) {
      m_intake.runIntake();
      m_led.setColor(Color.kBlueViolet);
      
    } else if (xboxController.getPOV() == 0){
      m_intake.runLowEject();
      m_led.setFlashColors(Color.kPink, Color.kBlue, 0.6);
    
    } else if (xboxController.getPOV() == 90){
      m_intake.runMiddleEject();
      m_led.setFlashColors(Color.kPink, Color.kBlue, 0.6);

    } else if(xboxController.getPOV() == 180){
      m_intake.runHighEject();
      m_led.setFlashColors(Color.kPink, Color.kBlue, 0.6);
      
    } else {
      m_intake.setMotorsToZero();
      if (DriverStation.getMatchTime() > 15){
        m_led.setChasingColors(Color.kRed, Color.kBlack, 10, 0.005);
      } else {
        m_led.setRainbow(0);
      }
    }

    //ARM BUTTON TRIGGERS
    if(xboxController.GetButtonRB()){
      m_arm.setDesiredPosition(Constants.Arm.INTAKE_ANGLE);
      
    } else if(xboxController.GetButtonX() && !xboxController.GetButtonY() && !xboxController.GetButtonA() && !xboxController.GetButtonB()){
      m_arm.setDesiredPosition(Constants.Arm.fTOP_ANGLE);
      
    } else if(xboxController.GetButtonLB()){
      m_arm.setDesiredPosition(Constants.Arm.SAFE_ANGLE);
      
    } else if(xboxController.GetButtonB() && !xboxController.GetButtonX() && !xboxController.GetButtonY() && !xboxController.GetButtonA()){
      m_arm.setDesiredPosition(Constants.Arm.bTOP_ANGLE);
      
    } else if (xboxController.GetButtonStart()){
      m_arm.armHoming(); //if arm slips during match, press the start button to re-home arm
    }

    //turns on brake mode in last 15 sec - coast button should override this
    if (DriverStation.getMatchTime() <= 15){ 
      // m_led.setRainbow(0);
      m_chassis.setBrakeMode(true);
    }
    
  }

  /** This function is called once when the robot is disabled. */
  @Override
  public void disabledInit() {
  }

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
