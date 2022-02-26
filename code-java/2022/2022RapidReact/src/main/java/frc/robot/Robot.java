// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;
import org.janksters.CommonClassesThisYear.*;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
// import edu.wpi.first.wpilibj.motorcontrol.MotorController;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
// import edu.wpi.first.wpilibj.motorcontrol.PWMVictorSPX;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import com.ctre.phoenix.motorcontrol.can.WPI_VictorSPX;
import edu.wpi.first.wpilibj.motorcontrol.PWMMotorController;
import edu.wpi.first.wpilibj.motorcontrol.PWMVictorSPX;

import javax.swing.text.AbstractDocument.LeafElement;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import edu.wpi.first.math.controller.PIDController;


public class Robot extends TimedRobot {
  private static final String kTankDrive = "Tank Drive";
  private static final String kArcadeDrive = "Arcade Drive";
  private static final String kCurvatureDrive = "Curvature Drive";
  private static final String kSteeringWheel = "Steering Wheel";

  private String DriveSelected;
  private final SendableChooser<String> driveChooser = new SendableChooser<>();

  Joystick leftJoystick;
  Joystick rightJoystick;
  double averageSpeed;

  //Chassis in a Day - Lazlo
    /*private WPI_TalonSRX leftLeader = new WPI_TalonSRX(2);
    private PWMVictorSPX leftFollower = new PWMVictorSPX(3);
    private WPI_TalonFX rightLeader = new WPI_TalonFX(1);
    private PWMVictorSPX rightFollower = new PWMVictorSPX(0);*/

  //2022 Real Bot (wifi is bob)
    private WPI_TalonFX leftLeader = new WPI_TalonFX(2);//m2
    private WPI_TalonFX leftFollower = new WPI_TalonFX(3);//m3
    private WPI_TalonFX rightLeader = new WPI_TalonFX(1);//m1
    private WPI_TalonFX rightFollower = new WPI_TalonFX(0);//m0

  //2022 Janky (wifi is C1Day)
    /*private WPI_TalonSRX leftLeader = new WPI_TalonSRX(2);//m2
    private WPI_TalonFX leftFollower = new WPI_TalonFX(3);//m3
    private WPI_TalonFX rightLeader = new WPI_TalonFX(1);//m1
    private WPI_TalonSRX rightFollower = new WPI_TalonSRX(0);//m0*/

  private MotorControllerGroup left = new MotorControllerGroup(leftLeader, leftFollower);
  private MotorControllerGroup right = new MotorControllerGroup(rightLeader, rightFollower);
  DifferentialDrive myRobot = new DifferentialDrive(left,right);

  PIDController pidDistance = new PIDController(0.1, 0, 0);
  PIDController pidAngle = new PIDController(0.1, 0, 0);

  VisionSubsystem limeLight = new VisionSubsystem();

  Shooter shooter = new Shooter();
  Pivot pivot = new Pivot();
  private Climb climbMech;

  //AUTO
  AutoStateMachine autoSM;
  AutoSelector pathSelector = new AutoSelector();
  int autoCaseNum;

  //move forward
  final int simplePath = 0;
  //move forward, get ball, turn, move, shoot
  final int standardPath = 1;
  //turn 180 degrees
  final int turnPath = 2;

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    //AUTO
    pathSelector.DisplayActualAutoOptions();
    autoCaseNum = 0;

    //CHASSIS
    driveChooser.setDefaultOption("Tank Drive", kTankDrive);
    driveChooser.addOption("Arcade Drive", kArcadeDrive);
    driveChooser.addOption("Curvature Drive", kCurvatureDrive);
    driveChooser.addOption("Steering Wheel", kSteeringWheel);
    SmartDashboard.putData("Drive choices", driveChooser);

    leftJoystick = new Joystick(0);  //whatever is in port 0
    rightJoystick = new Joystick(1); //whatever is in port  0

    if (climbMech == null){
      climbMech = new Climb(Constants.WINCH_MOTOR_CHANNEL_L, Constants.WINCH_MOTOR_CHANNEL_R,
      Constants.PCM_CHANNEL, Constants.MID_LATCH_CHANNEL_L, Constants.MID_LATCH_CHANNEL_R, pivot);
    }

    myRobot.setMaxOutput(0.5); //default is 1

    System.out.println("Passed through initialization");
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
    //making the path selector
    Auto.autoPathFinal = pathSelector.getActualAutoMode();
   
    //idk where we use this but we are keeping it just in case
    autoCaseNum = 0;

    SmartDashboard.putNumber("initial auto path selected", Auto.autoPathFinal);

    autoSM = new AutoStateMachine(pivot, shooter);
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {
    autoSM.displayCurrentState();
  }

  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {
    DriveSelected = driveChooser.getSelected();
    System.out.println("Drive selected: " + DriveSelected);
  }

  /** This function is called periodically during operator control. :) */
  @Override
  public void teleopPeriodic() {


    //SHOOTER
    shooter.displayCurrentState();

    //CHASSIS
    if (leftJoystick.getRawButton(4)&& limeLight.targetValid()){ 
      myRobot.tankDrive(-limeLight.getDistance()-limeLight.getAngle(),limeLight.getDistance() + limeLight.getAngle());//Call limeLight.getAngle() subtract from one side, add to another 
    } else {
        switch (DriveSelected) {
          case kArcadeDrive:
            double rotateSpeed = leftJoystick.getRawAxis(1);
            double moveSpeed = leftJoystick.getRawAxis(0);
            myRobot.arcadeDrive(moveSpeed, -rotateSpeed); // + move , - rot
            break;
          case kCurvatureDrive:
            // Curvature drive with a given forward and turn rate, as well as a button for turning in-place.
            myRobot.curvatureDrive(leftJoystick.getX(), -leftJoystick.getY(), leftJoystick.getTrigger());//maybe take out negative?
            break;  
          case kSteeringWheel:
            //put if statement for printing instructions on how to use 
            double rSpeed = leftJoystick.getRawAxis(0);//10% works DON'T CHANGE
            double mSpeed = rightJoystick.getRawAxis(1);
            myRobot.arcadeDrive(rSpeed, -mSpeed); 
            break;
          default:
            averageSpeed = (leftJoystick.getY() + rightJoystick.getY()) / 2;
            if (leftJoystick.getRawButton(3) || rightJoystick.getRawButton(3)) {
              myRobot.tankDrive(-averageSpeed, averageSpeed);
            } else {
            myRobot.tankDrive(-leftJoystick.getY(), rightJoystick.getY());
            }
            break;
        } 
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