// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
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

  private ShooterController shootingController;
  private boolean shootingSideFront = true;
  private jankyXboxJoystick _joy;

  boolean yWasPressed = false;
  boolean xWasPressed = false;
  boolean aWasPressed = false;
  boolean bWasPressed = false;
  boolean rightXAxisWasPressed = false;
  boolean rightYAxisWasPressed = false;

  private int shootingZone = 1; 

  double offsetFromVisionTarget;

  //SETTTINGS:
  //channel;
  private static final int SHOOTING_LEFT_MOTOR_CHANNEL = 4;
  private static final int INTAKE_LEFT_MOTOR_CHANNEL = 0;
  private static final int SHOOTING_RIGHT_MOTOR_CHANNEL = 1;
  private static final int INTAKE_RIGHT_MOTOR_CHANNEL = 3;

  // Vision
  private static final String VISION_DISTANCE = "Distance to Tape";
  private static final String VISION_OFFSET = "Offset";
  private static final double NO_DATA_DEFAULT = -1000;

  //Intake
  private static final int MOTOR_ROLL_CHANNEL = 2; //change the channel accordingly 
  private static final int LEFT_PISTON_CHANNEL = 0;
  private static final int RIGHT_PISTON_CHANNEL = 1;
  //Intake to turret transportation
  private static final int CONVEYOR_BELT_CHANNEL = 7;
  private static final int BRIDGE_CHANNEL = 8;

  // Turret
  private static final int TURRET_MOTOR_CHANNEL = 6;
  private static final int LOWER_BOUND = -5; //was -3
  private static final int UPPER_BOUND = 1; //was 3
  private static final int BAD_DATA = -1000;
  private static final double TURRET_SPEED_MANUAL = 0.4;
  private static final double TURRET_USING_VISION = 0.1;

  // Shooting
  private static final int FLYWHEEL_CHANNEL = 5;

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    shootingController = new ShooterController(CONVEYOR_BELT_CHANNEL, MOTOR_ROLL_CHANNEL, LEFT_PISTON_CHANNEL, RIGHT_PISTON_CHANNEL, FLYWHEEL_CHANNEL, BRIDGE_CHANNEL, TURRET_MOTOR_CHANNEL);
    _joy = new jankyXboxJoystick(2);
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

    offsetFromVisionTarget = SmartDashboard.getNumber(VISION_OFFSET, NO_DATA_DEFAULT); //positive is to the right

    //Intake
    boolean buttonRB = _joy.GetButtonRB(); // rollers in
    boolean buttonLB = _joy.GetButtonLB(); // rollers out
    if(buttonLB){
      shootingController.IntakePistonsUp();
    }
    else if(buttonRB){
      shootingController.IntakePistonsDown();
    }

    // INTAKE TO TURRET TRANSPORTATION
    boolean buttonY = _joy.GetButtonY(); // bridge
    //SmartDashboard::PutBoolean("button Y", buttonY);
    //SmartDashboard::PutBoolean("yWasPressed", yWasPressed);

    // CONVEYOR BELT & BRIDGE
     if (buttonY && !yWasPressed) { 
      // run bridge motor
        shootingController.BridgeForward();
        //  bridgeMotor->Set(1.0);
         //shootingcontroller->StartConveyorBelt();
         yWasPressed = true;
         //SmartDashboard::PutNumber("Bridge running", 0);
     }
     else if (!buttonY && yWasPressed) {
      //SmartDashboard::PutNumber("Bridge running", 1);
      // stop bridge motor
      shootingController.StopBridge();
      // bridgeMotor->Set(0);
      shootingController.StopConveyorBelt();
      yWasPressed = false; //correct verrsion
     }
     
    //bridge out
     double rightYAxis = _joy.GetRightYAxis();//NOTE: currently uses the same as climbing; hard when both uses flywehel and bridge out right axis?
     if (rightYAxis >=0.2 && !rightYAxisWasPressed){
       shootingController.BridgeBackward();
       rightYAxisWasPressed = true;
     } 
     else if (!(rightYAxis >=0.2) && rightYAxisWasPressed){
       shootingController.StopBridge();
       rightYAxisWasPressed = false;
     }
     

    boolean buttonA = _joy.GetButtonA();
    boolean buttonX = _joy.GetButtonX();
    boolean buttonB = _joy.GetButtonB();
    double desiredRPM = shootingController.GetDesiredRPM();
    double runningRPM = shootingController.GetRunningRPM();
    System.out.println("desired rpm: " + desiredRPM);
    System.out.println("running rpm: " + runningRPM);

    // FLYWHEEL
    boolean leftButton = _joy.GetLeftStickButton();
    boolean rightButton = _joy.GetRightStickButton();
    double leftThrottle = _joy.GetLeftThrottle();
    double rightThrottle = _joy.GetRightThrottle();
    
    if (rightButton)
    {
      shootingZone = 1;
    }
    else if (leftThrottle > 0.2)
    {
      shootingZone = 2;
    }
    else if(rightThrottle > 0)
    {
      shootingZone = 3;
    }
    else if(leftButton)
    {
      shootingZone = 4;
    }

    shootingController.SetSelectorVisionDistance(shootingZone);

    if(buttonA) {
      shootingController.Target();
      aWasPressed = true;
    }
    else if (aWasPressed){
      shootingController.StopTarget();
      aWasPressed = false;
    }

    //flywheel out
    
     double rightXAxis = _joy.GetRightXAxis();
     if (rightXAxis >=0.2 && !rightXAxisWasPressed){ //is it 0.0?  
      shootingController.FlywheelOut();
      rightXAxisWasPressed = true; 
     }
     else if (rightXAxis <=0.2 && rightXAxisWasPressed){
       shootingController.StopTarget();
       rightXAxisWasPressed = false;
     }

     
    if(buttonX && !xWasPressed) {
      shootingController.SetDesiredCount(-100); //was -4096
      //shootingcontroller->ConveyorBeltOut(); //not pulsing
      xWasPressed = true;
    }
    else if(buttonB && !bWasPressed) {
      shootingController.SetDesiredCount(100); //was 4096
      //shootingcontroller->StartConveyorBelt();  //not pulsing
      bWasPressed = true;
    }
    else {
      xWasPressed = false;  //reset encoder count?
      bWasPressed = false;
    }

    // INTAKE
    double intakeJoy = _joy.GetLeftYAxis();

    if(intakeJoy > 0.2) {
      shootingController.IntakeOut();
    }
    else if (intakeJoy < -0.2) {
      shootingController.IntakeIn();
    }
    else {
      shootingController.IntakeStop();
    }

    // TURRET
    boolean buttonStart = _joy.GetButtonStart();
    boolean buttonBack = _joy.GetButtonBack();

    //SmartDashboard.putNumber("gyro heading", m_drive.GetHeading());

    if (buttonStart) {
      shootingController.TurretRight(TURRET_SPEED_MANUAL);
      // turretMotor -> Set(0.2); // run to right
    }
    else if (buttonBack) {
      shootingController.TurretLeft(TURRET_SPEED_MANUAL);
      // turretMotor -> Set(-0.2); // run to left
    }
    else if ((offsetFromVisionTarget == BAD_DATA) && (!_joy.GetButtonY()))
    {
      shootingController.StopTurret();
      // turretMotor -> Set(0); // within bounds
    }
    else if ((offsetFromVisionTarget < LOWER_BOUND) && (!_joy.GetButtonY())) {
      shootingController.TurretRight(TURRET_USING_VISION);
      // turretMotor -> Set(TURRET_USING_VISION); // run motor to right
    }
    else if ((offsetFromVisionTarget > UPPER_BOUND) && (!_joy.GetButtonY()))
    {
      shootingController.TurretLeft(TURRET_USING_VISION);
      // turretMotor -> Set(-TURRET_USING_VISION); // run motor to left
    }
    else {
      shootingController.StopTurret();
      // turretMotor -> Set(0); // within bounds
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
}
