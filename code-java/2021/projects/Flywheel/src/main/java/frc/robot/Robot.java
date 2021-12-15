// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import jdk.dynalink.beans.StaticClass;

import org.janksters.robotcode.Flywheel;
import edu.wpi.first.wpilibj.Joystick;

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
  private static final int FLY_WHEEL_CHANNEL = 5;
  private static Flywheel jankyFlywheel;
  public static Joystick _joy;
  private int shootingZone = 1; //make sure this doesn't change shooting zone for later calls
  private boolean aWasPressed = false;
  private boolean bWasPressed = false;
  private boolean xWasPressed = false;
  private boolean rightXAxisWasPressed = false;


  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);

    jankyFlywheel  = new Flywheel(FLY_WHEEL_CHANNEL);
    _joy = new Joystick(2);
  }

  /**
   * This function is called every robot packet, no matter the mode. Use this for items like
   * diagnostics that you want ran during disabled, autonomous, teleoperated and test.
   *
   * <p>This runs after the mode specific periodic functions, but before LiveWindow and
   * SmartDashboard integrated updating.
   */
  @Override
  public void robotPeriodic() {
    boolean leftButton = _joy.getRawButton(9);
    boolean rightButton = _joy.getRawButton(10);
    double leftThrottle = _joy.getTwist();
    double rightThrottle = _joy.getThrottle();

    if  (rightButton)
    {
      shootingZone = 1;
    }
    else if (leftThrottle > 0.2)
    {
      shootingZone = 2;
    }
    else if (rightThrottle > 0)
    {
      shootingZone = 3;
    }
    else if (leftButton)
    {
      shootingZone = 4;
    }

    jankyFlywheel.SetSelectorVisionDistance(shootingZone);

    SmartDashboard.putNumber("the real shooting zone", shootingZone);
    boolean buttonA = _joy.getRawButton(1);
    double desiredRPM = jankyFlywheel.ReturnDesiredRPM();
    double runningRPM = jankyFlywheel.GetRunningRPM();
    System.out.println("Desired RPM: " + desiredRPM);
    System.out.println("Running RPM: " + runningRPM);

    SmartDashboard.putBoolean("button A was pressed", buttonA);

    if(buttonA) {
      jankyFlywheel.SetRPM();
      aWasPressed = true;
    } else if (aWasPressed){
      jankyFlywheel.StopFlywheel();
      aWasPressed = false;
    }

    //flywheel out
    double rightXAxis = _joy.getRawAxis(4);
    if(rightXAxis >= 0.2 && !rightXAxisWasPressed) { //is it 0.0?
      jankyFlywheel.FlywheelOut();
      rightXAxisWasPressed = true;
    } else if (aWasPressed){
      jankyFlywheel.StopFlywheel();
      rightXAxisWasPressed = false;
    }

    SmartDashboard.putNumber("Encoder Flywheel: ", jankyFlywheel.getFlywheelEncoder());

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
  public void teleopPeriodic() {}

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
