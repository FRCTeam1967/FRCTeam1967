// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.
package frc.robot;

import org.janksters.CommonClassesThisYear.*;
import org.janksters.jankyLib.jankyXboxJoystick;

import edu.wpi.first.wpilibj.ADIS16470_IMU;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.StatorCurrentLimitConfiguration;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

/*
 * The VM is configured to automatically run this class, and to call the functions corresponding to
 * each mode, as described in the TimedRobot documentation. If you change the name of this class or
 * the package after creating this project, you must also update the build.gradle file in the
 * project.
 */
public class Robot extends TimedRobot {  
  private Joystick leftJoystick;
  private Joystick rightJoystick;
  private jankyXboxJoystick XboxController;

  //2022 Real Bot (wifi is bob)
  private WPI_TalonFX leftLeader = new WPI_TalonFX(Constants.CHASSIS_L_LEADER_CHANNEL);//m2
  private WPI_TalonFX leftFollower = new WPI_TalonFX(Constants.CHASSIS_L_FOLLOWER_CHANNEL);//m3
  private WPI_TalonFX rightLeader = new WPI_TalonFX(Constants.CHASSIS_R_LEADER_CHANNEL);//m1
  private WPI_TalonFX rightFollower = new WPI_TalonFX(Constants.CHASSIS_R_FOLLOWER_CHANNEL);//m0

  private MotorControllerGroup left = new MotorControllerGroup(leftLeader, leftFollower);
  private MotorControllerGroup right = new MotorControllerGroup(rightLeader, rightFollower);
  private DifferentialDrive myRobot = new DifferentialDrive(left,right);

  private Shooter shooter; //may remove for pid
  private PivotMagic pivotmagic = null;
  private AutoStateMachine autoSM = null;
  private ClimbMBR climbMech = null;

  private ADIS16470_IMU m_gyro = new ADIS16470_IMU();

  //move forward
  final int simplePath = 0;
  //move forward, get ball, turn, move, shoot
  final int standardPath = 1;
  //turn 180 degrees
  final int turnPath = 2;

  public void instantiateClimb(){
    climbMech = new ClimbMBR(Constants.CLIMB_WINCH_MOTOR_CHANNEL_L, Constants.CLIMB_WINCH_MOTOR_CHANNEL_R, pivotmagic);
  }

  public void instantiatePivotMagic(){
    pivotmagic = new PivotMagic();
  }

  public void instantiateShooter(){
    shooter = new Shooter();
  }

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    leftLeader.setNeutralMode(NeutralMode.Coast);
    leftFollower.setNeutralMode(NeutralMode.Coast);
    rightLeader.setNeutralMode(NeutralMode.Coast);
    rightFollower.setNeutralMode(NeutralMode.Coast);

    leftJoystick = new Joystick(0);
    rightJoystick = new Joystick(1);
    XboxController = new jankyXboxJoystick(2);

    int autoPathSelected = AutoConstants.twoBall;
    int autoDelaySelected = AutoConstants.ZeroDelay;

    StatorCurrentLimitConfiguration chassisCurrentLimit = new StatorCurrentLimitConfiguration(true, Constants.CHASSIS_CURRENT_LIMIT_MAX, Constants.CHASSIS_TRIGGER_CURRENT, Constants.CHASSIS_THRESHOLD_TIME);
    
    leftLeader.configFactoryDefault();
    leftLeader.configStatorCurrentLimit(chassisCurrentLimit); 
    leftFollower.configFactoryDefault();
    leftFollower.configStatorCurrentLimit(chassisCurrentLimit);

    rightLeader.configFactoryDefault();
    rightLeader.configStatorCurrentLimit(chassisCurrentLimit);
    rightFollower.configFactoryDefault();
    rightFollower.configStatorCurrentLimit(chassisCurrentLimit);

    //instatiations - may remove
    if (pivotmagic == null){
      instantiatePivotMagic();
    }
    if (climbMech == null){
      instantiateClimb();
    }
    if (shooter == null){
      instantiateShooter();
    }
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
    if (pivotmagic == null){
      instantiatePivotMagic();
   }

   autoSM = new AutoStateMachine(0,1, m_gyro, shooter, pivotmagic);
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {
    autoSM.displayCurrentState();
  }

  /** This function is called once when teleop is enabled. */
  @Override
  public void teleopInit() {
    if (autoSM != null) {
      autoSM.endStateMachine();
    }

    if (pivotmagic == null){
      instantiatePivotMagic();
    }
    if (climbMech == null){
      instantiateClimb();
    }
    if(shooter == null){
      instantiateShooter();
    }
  }

  /** This function is called periodically during operator control. */
  @Override
  public void teleopPeriodic() {
    double YAxisValue = XboxController.GetLeftYAxis();
    if(YAxisValue >= Constants.SIMPLE_SHOOT_INTAKE_DEADBAND) {
      shooter.simpleShoot(Constants.SIMPLE_SHOOT_SPEED);
    } else if (YAxisValue <= -Constants.SIMPLE_SHOOT_INTAKE_DEADBAND) {
      shooter.simpleIntake(Constants.SIMPLE_INTAKE_SPEED);
    }

    shooter.displayCurrentState();

    double averageSpeed = (leftJoystick.getY() + rightJoystick.getY()) / 2;
    if (leftJoystick.getRawButton(1) || rightJoystick.getRawButton(1)) {
      myRobot.tankDrive(-averageSpeed, averageSpeed);
    }
    myRobot.tankDrive(-leftJoystick.getY(), rightJoystick.getY());
  }

  /** This function is called once when the robot is disabled. */
  @Override
  public void disabledInit() {
    if((pivotmagic != null) && (!pivotmagic.isInIdle())){
      pivotmagic.killStateMachine();
      pivotmagic = null;
      //instantiatePivotMagic();
      System.out.println("instantiated pivot in disabledInit()");
    }
    if((climbMech != null) && (!climbMech.isInIdle())){
      climbMech.killStateMachine();
      //instantiateClimb();
      System.out.println("instantiated climb in disabledInit()");
    }
    //add one for shooter?
  }

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
