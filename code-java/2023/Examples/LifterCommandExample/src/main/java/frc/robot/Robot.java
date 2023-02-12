// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.shuffleboard.BuiltInLayouts;
import edu.wpi.first.wpilibj.shuffleboard.EventImportance;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardLayout;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;
import frc.robot.Commands.MoveArmCommand;
import frc.robot.Subsystems.ArmSubsystem;
import edu.wpi.first.wpilibj2.command.CommandScheduler;
import edu.wpi.first.wpilibj2.command.InstantCommand;
import edu.wpi.first.wpilibj2.command.PrintCommand;
import edu.wpi.first.wpilibj2.command.SequentialCommandGroup;
import edu.wpi.first.wpilibj2.command.WaitCommand;
import edu.wpi.first.wpilibj2.command.button.CommandJoystick;
import edu.wpi.first.wpilibj2.command.button.CommandXboxController;
import edu.wpi.first.wpilibj2.command.button.Trigger;

import java.util.Map;  


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

  //construct arm state machine object
  private Arm m_arm;
  private ArmSubsystem m_armSubsystem;
  // private CommandXboxController xboxCommandController = new CommandXboxController(0);
  private CommandJoystick joystick = new CommandJoystick(0);


  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);

    m_arm = new Arm(Constants.Arm.MOTOR_L_ID, Constants.Arm.MOTOR_R_ID);
    m_arm.configDashboard();
    m_armSubsystem = new ArmSubsystem(m_arm);
  
    
    //configure CANCoder when robot is turned on
    m_arm.initEncoder();

    //run arm homing method
    m_arm.armHoming();

    // Add commands to the dashboard for testing
    ShuffleboardTab tab = Shuffleboard.getTab("Arm");
    ShuffleboardLayout commandList = tab.getLayout("Commands", BuiltInLayouts.kList)
      .withProperties(Map.of("Label position", "TOP"));
    commandList.add("Front Top", new MoveArmCommand(Constants.Arm.fTOP_ANGLE, m_armSubsystem));
    commandList.add("Front Middle", new MoveArmCommand(Constants.Arm.fMIDDLE_ANGLE, m_armSubsystem));
    commandList.add("Front Intake", new MoveArmCommand(Constants.Arm.INTAKE_ANGLE, m_armSubsystem));
    commandList.add("Back Middle", new MoveArmCommand(Constants.Arm.bMIDDLE_ANGLE, m_armSubsystem));
    commandList.add("Back Top", new MoveArmCommand(Constants.Arm.bTOP_ANGLE, m_armSubsystem));
    commandList.add("Safe", new MoveArmCommand(Constants.Arm.SAFE_ANGLE, m_armSubsystem));
    commandList.add("Reset Home", new InstantCommand(m_armSubsystem::resetHome, m_armSubsystem));

    // Just for fun, add a multiple step thing. 
    commandList.add("Do All The Things!", new SequentialCommandGroup(
      new MoveArmCommand(Constants.Arm.fTOP_ANGLE, m_armSubsystem),
      new WaitCommand(2.0),
      new MoveArmCommand(Constants.Arm.fMIDDLE_ANGLE, m_armSubsystem),
      new WaitCommand(2.0),
      new MoveArmCommand(Constants.Arm.INTAKE_ANGLE, m_armSubsystem),
      new WaitCommand(2.0),
      new MoveArmCommand(Constants.Arm.bMIDDLE_ANGLE, m_armSubsystem),
      new WaitCommand(2.0),
      new MoveArmCommand(Constants.Arm.bTOP_ANGLE, m_armSubsystem)
    ));

    // Add the scheduler itself so we can see what it's doing
    tab.add("Scheduler", CommandScheduler.getInstance());

    // Set the scheduler to log Shuffleboard events for command initialize, interrupt, finish
   CommandScheduler.getInstance()
       .onCommandInitialize(
           command ->
               Shuffleboard.addEventMarker(
                   "Command initialized", command.getName(), EventImportance.kNormal));
   CommandScheduler.getInstance()
       .onCommandInterrupt(
           command ->
               Shuffleboard.addEventMarker(
                   "Command interrupted", command.getName(), EventImportance.kNormal));
   CommandScheduler.getInstance()
       .onCommandFinish(
           command ->
               Shuffleboard.addEventMarker(
                   "Command finished", command.getName(), EventImportance.kNormal));

    // Setup triggers. 
    // xboxCommandController.leftBumper().onTrue(new MoveArmCommand(Constants.Arm.fMIDDLE_ANGLE, m_armSubsystem));
    // xboxCommandController.rightBumper().onTrue(new MoveArmCommand(Constants.Arm.bMIDDLE_ANGLE, m_armSubsystem));
    // xboxCommandController.start().onTrue(new InstantCommand(m_armSubsystem::resetHome, m_armSubsystem));
    // xboxCommandController.leftTrigger(0.95).onTrue(new MoveArmCommand(Constants.Arm.fTOP_ANGLE, m_armSubsystem));
    // xboxCommandController.rightTrigger(0.95).onTrue(new MoveArmCommand(Constants.Arm.bTOP_ANGLE, m_armSubsystem));

    // // We need to make our own triggers for these
    // new Trigger(() -> xboxCommandController.getLeftY() > Constants.Arm.CONTROLLER_Y_AXIS_DEADBAND)
    //   .onTrue(new MoveArmCommand(Constants.Arm.INTAKE_ANGLE, m_armSubsystem));
    // new Trigger(() -> xboxCommandController.getRightY() > Constants.Arm.CONTROLLER_Y_AXIS_DEADBAND)
    //   .onTrue(new MoveArmCommand(Constants.Arm.SAFE_ANGLE, m_armSubsystem));
    
    joystick.button(1).onTrue(new SequentialCommandGroup(
      new MoveArmCommand(Constants.Arm.fTOP_ANGLE, m_armSubsystem),
      new WaitCommand(2.0),
      new MoveArmCommand(Constants.Arm.fMIDDLE_ANGLE, m_armSubsystem),
      new WaitCommand(2.0),
      new MoveArmCommand(Constants.Arm.INTAKE_ANGLE, m_armSubsystem),
      new WaitCommand(2.0),
      new MoveArmCommand(Constants.Arm.bMIDDLE_ANGLE, m_armSubsystem),
      new WaitCommand(2.0),
      new MoveArmCommand(Constants.Arm.bTOP_ANGLE, m_armSubsystem)
    ) );
    joystick.button(2).onTrue(new MoveArmCommand(Constants.Arm.fTOP_ANGLE, m_armSubsystem));
    joystick.button(3).whileTrue(new PrintCommand("Button 3 pressed!"));
  }

  /**
   * This function is called every 20 ms, no matter the mode. Use this for items like diagnostics
   * that you want ran during disabled, autonomous, teleoperated and test.
   *
   * <p>This runs after the mode specific periodic functions, but before LiveWindow and
   * SmartDashboard integrated updating.
   */
  @Override
  public void robotPeriodic() {
    CommandScheduler.getInstance().run();
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

     //run arm homing method
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
     //run arm homing method
     m_arm.armHoming();
  }

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {
    //why is the deadband different from left and right throttle?
    
    /* 
    if (xboxController.GetLeftThrottle()==1){
      m_arm.setDesiredPosition(Positions.FRONT_TOP);

    } else if(xboxController.GetButtonLB()){
      m_arm.setDesiredPosition(Positions.FRONT_MIDDLE);

    } else if(xboxController.GetLeftYAxis()>Constants.Arm.CONTROLLER_Y_AXIS_DEADBAND){
      m_arm.setDesiredPosition(Positions.FRONT_INTAKE);
      
    } else if(xboxController.GetRightThrottle()==1){
      m_arm.setDesiredPosition(Positions.BACK_TOP);
      
    } else if(xboxController.GetButtonRB()){
      m_arm.setDesiredPosition(Positions.BACK_MIDDLE);
      
    } else if(xboxController.GetRightYAxis()>Constants.Arm.CONTROLLER_Y_AXIS_DEADBAND){
      m_arm.setDesiredPosition(Positions.SAFE);
      
    } else if (xboxController.GetButtonStart()){
      //if arm slips during match, press the start button to re-home arm
      m_arm.armHoming();
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
  public void testInit() {
     //run arm homing method
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
