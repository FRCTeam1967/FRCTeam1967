// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import org.janksters.CommonClassesThisYear.*;

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
  private Climb2 climbMech;
  //private jankyXboxJoystick XboxController;

  //all channel numbers - to edit
  private final int WINCH_MOTOR_CHANNEL_L = 1;
  private final int WINCH_MOTOR_CHANNEL_R = 2;
  private final int PCM_CHANNEL = 0;
  private final int MID_LATCH_CHANNEL_L = 2;
  private final int MID_LATCH_CHANNEL_R = 3;

  //joystick variables
  /*
  boolean climbYAxisUp;
  boolean climbYAxisWasUp = false;
  boolean climbYAxisDown;
  boolean climbYAxisWasDown = false;
  boolean xButtonPressed;
  boolean xButtonWasPressed = false;
  boolean yButtonPressed;
  boolean yButtonWasPressed = false;
  boolean aButtonPressed;
  boolean aButtonWasPressed = false;
  boolean bButtonPressed;
  boolean bButtonWasPressed = false;
  boolean startButtonPressed;
  boolean startButtonWasPressed = false;
  */

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);

    if (climbMech == null){
      climbMech = new Climb2(WINCH_MOTOR_CHANNEL_L, WINCH_MOTOR_CHANNEL_R, PCM_CHANNEL, MID_LATCH_CHANNEL_L, MID_LATCH_CHANNEL_R);

    }
    
    //XboxController = new jankyXboxJoystick(2);
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
    //add manual control to state machine 
    /*
    //--- JOYSTICK UP ---//
    climbYAxisUp = XboxController.GetRightYAxis() > 0.2;
    if (climbYAxisUp && !climbYAxisWasUp) { 
      //raise winch string
      climbMech.raiseWinchString();
      climbYAxisWasUp = true;
    } else if (!climbYAxisUp && climbYAxisWasUp) {
      climbMech.stopWinchString();
      climbYAxisWasUp = false;
    }
    
    //--- JOYSTICK DOWN ---// 
    climbYAxisDown = XboxController.GetRightYAxis() < -0.2;
    if (climbYAxisDown && !climbYAxisWasDown) { 
      //lower winch string
      climbMech.lowerWinchString();
      climbYAxisWasDown = true;
    } else if (!climbYAxisDown && climbYAxisWasDown) {
      climbMech.stopWinchString();
      climbYAxisWasDown = false;
    }  

    //--- MID LATCH ---//
    xButtonPressed = XboxController.GetButtonX();
    if (xButtonPressed && !xButtonWasPressed) {
      climbMech.latchMidBar();
      xButtonWasPressed = true;
    } else if (!xButtonPressed && xButtonWasPressed) {
      xButtonWasPressed = false;
    }

    //--- MID UNLATCH ---//
    bButtonPressed = XboxController.GetButtonB();
    if (bButtonPressed && !bButtonWasPressed) {
      climbMech.latchMidBar();
      bButtonWasPressed = true;
    } else if (!bButtonPressed && bButtonWasPressed) {
      bButtonWasPressed = false;
    }

    //--- HIGH LATCH ---//
    yButtonPressed = XboxController.GetButtonY();
    if (yButtonPressed && !yButtonWasPressed) {
      climbMech.latchHighBar();
      yButtonWasPressed = true;
    } else if (!yButtonPressed && yButtonWasPressed) {
      yButtonWasPressed = false;
    }

    //--- HIGH HOOK PIVOT DOWN ---//
    aButtonPressed = XboxController.GetButtonA();
    if (aButtonPressed && !aButtonWasPressed) {
      climbMech.hookPivotDown();
      aButtonWasPressed = true;
    } else if (!aButtonPressed && aButtonWasPressed) {
      aButtonWasPressed = false;
    }
    
    //--- HIGH HOOK PIVOT UP ---//
    startButtonPressed = XboxController.GetButtonStart();
    if (startButtonPressed && !startButtonWasPressed) {
      climbMech.hookPivotUp();
      startButtonWasPressed = true;
    } else if (!startButtonPressed && startButtonWasPressed) {
      startButtonWasPressed = false;
    }
    /*

    /*
    //--- WHOLE SEQUENCE BUTTON ---//
    xButtonPressed = XboxController.GetButtonX();
    if (xButtonPressed && !xButtonWasPressed) {
      climbMech.startClimbStateMachine();
      xButtonWasPressed = true;
    } else if (!xButtonPressed && xButtonWasPressed) {
      xButtonWasPressed = false;
    }

    //--- STOP BUTTON ---//
    bButtonPressed = XboxController.GetButtonB();
    if (bButtonPressed && !bButtonWasPressed) {
      climbMech.stopClimbStateMachine();
      //climbMech.startClimbStateMachine();
      bButtonWasPressed = true;
    } else if (!bButtonPressed && bButtonWasPressed) {
      bButtonWasPressed = false;
    }
    */
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
