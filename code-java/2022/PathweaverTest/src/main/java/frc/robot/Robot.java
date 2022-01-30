// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.math.trajectory.constraint.DifferentialDriveVoltageConstraint;
import edu.wpi.first.math.trajectory.TrajectoryConfig;
import edu.wpi.first.math.trajectory.Trajectory;
import edu.wpi.first.wpilibj2.command.RamseteCommand;
import edu.wpi.first.wpilibj.Filesystem;
import java.nio.file.Path;
import edu.wpi.first.math.trajectory.TrajectoryUtil;
import java.io.IOException;
import edu.wpi.first.wpilibj.DriverStation;
import edu.wpi.first.math.controller.RamseteController;
import edu.wpi.first.math.controller.SimpleMotorFeedforward;
import edu.wpi.first.math.controller.PIDController;
import frc.robot.org.janksters.robotcode.*;
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
  
  AutoDriveSubsystem m_drive = new AutoDriveSubsystem(); 
  AutoSelector selector =  new AutoSelector();
  Trajectory trajectory;
  int j;
  int i;



  RamseteCommand ramseteCommand;

  /**
   * This function is run when the robot is first started up and should be used for any
   * initialization code.
   */
  @Override
  public void robotInit() {
    m_chooser.setDefaultOption("Default Auto", kDefaultAuto);
    m_chooser.addOption("My Auto", kCustomAuto);
    SmartDashboard.putData("Auto choices", m_chooser);
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
    int selectedPath = selector.getAutoMode();
    int selectedRobot = selector.getSelectedRobot();

    double ksSelected = AutoConstants.jankybotKS;
    double kvSelected = AutoConstants.jankybotKV;
    double kaSelected = AutoConstants.jankybotKA; 

    double kPDriveVel = AutoConstants.jankybotKPDriveVel;
    double kDDriveVel = AutoConstants.jankybotKDDriveVel;

    String pathName = "doNothing.wpilib.json";
    //    String trajectoryJSON = "paths/YourPath.wpilib.json";
    if (selectedPath == AutoConstants.SIMPLE_PATH){
      pathName = "simpleTest.wpilib.json";
    }
    else if (selectedPath == AutoConstants.BOUNCE_PATH){
      pathName = "bounceTest.wpilib.json";
    }
    else if (selectedPath == AutoConstants.BARREL_PATH){
      pathName = "barrelTest.wpilib.json";
    }
    else if (selectedPath == AutoConstants.SLALOM_PATH){
      pathName = "slalomTest.wpilib.json";
    }
    else if (selectedPath == AutoConstants.TEST_PATH_1){
      pathName = "TestPath1.wpilib.json";
    }
    else if (selectedPath ==  AutoConstants.TEST_PATH_2){
      pathName = "TestPath2.wpilib.json";
    }
    else if (selectedPath == AutoConstants.TEST_PATH_3){
      pathName = "TestPath3.wpilib.json";
    } 
    SmartDashboard.putString("Trajectory Path: ", pathName);
    
    if (selectedRobot == AutoConstants.JANKYBOT){
      ksSelected = AutoConstants.jankybotKS;
      kvSelected = AutoConstants.jankybotKV;
      kaSelected = AutoConstants.jankybotKA;

      kPDriveVel = AutoConstants.jankybotKPDriveVel;
      kDDriveVel = AutoConstants.jankybotKDDriveVel;
    }
    else if (selectedRobot == AutoConstants.LUCA){
      ksSelected = AutoConstants.lucaKS;
      kvSelected = AutoConstants.lucaKV;
      kaSelected = AutoConstants.lucaKA;

      kPDriveVel = AutoConstants.lucaKPDriveVel;
      kDDriveVel = AutoConstants.lucaKDDriveVel;
    }
    
    // Create a voltage constraint to ensure we don't accelerate too fast
     var autoVoltageConstraint =
         new DifferentialDriveVoltageConstraint(
             new SimpleMotorFeedforward(
                 ksSelected,
                 kvSelected,
                 kaSelected),
             AutoConstants.kDriveKinematics,
             10);
 
     // Create config for trajectory
     TrajectoryConfig config =
         new TrajectoryConfig(
                 AutoConstants.kMaxSpeed,
                 AutoConstants.kMaxAcceleration)
             // Add kinematics to ensure max speed is actually obeyed
             .setKinematics(AutoConstants.kDriveKinematics)
             // Apply the voltage constraint
            .addConstraint(autoVoltageConstraint);

    // An example trajectory to follow.  All units in meters.
    /*
    Trajectory exampleTrajectory =
        TrajectoryGenerator.generateTrajectory(
            // Start at the origin facing the +X direction
            new Pose2d(0, 0, new Rotation2d(0)),
            // Pass through these two interior waypoints, making an 's' curve path
            List.of(new Translation2d(1, 1), new Translation2d(2, -1)),
            // End 3 meters straight ahead of where we started, facing forward
            new Pose2d(3, 0, new Rotation2d(0)),
            // Pass config
            config);
            */


    trajectory = new Trajectory();
    try {
      Path trajectoryPath = Filesystem.getDeployDirectory().toPath().resolve(pathName);
      trajectory = TrajectoryUtil.fromPathweaverJson(trajectoryPath);
    } catch (IOException ex) {
      DriverStation.reportError("Unable to open trajectory: " + pathName, ex.getStackTrace());
      System.out.println ("failed to get trajectory");
    }
    //NOTE: Loading a PathWeaver JSON from file in Java can take more than one loop iteration so it is highly recommended that the robot load these paths on startup.
    
    ramseteCommand = new RamseteCommand(
      trajectory,
      m_drive::getPose,
      new RamseteController(AutoConstants.kRamseteB, AutoConstants.kRamseteZeta),
      new SimpleMotorFeedforward(
        ksSelected,
        kvSelected,
        kaSelected
        ),
      AutoConstants.kDriveKinematics,
      m_drive::getWheelSpeeds,
      new PIDController(kPDriveVel, 0, 0),
      new PIDController(kPDriveVel, 0, 0),
      // RamseteCommand passes volts to the callback
      m_drive::tankDriveVolts,
      m_drive);

    // Reset odometry to the starting pose of the trajectory.
    m_drive.resetOdometry(trajectory.getInitialPose());

    // Run path following command, then stop at the end.
  //  return ramseteCommand.andThen(().m_drive.tankDriveVolts(0, 0));
    i = 0;
    j = 0;
    ramseteCommand.initialize();
  }

  /** This function is called periodically during autonomous. */
  @Override
  public void autonomousPeriodic() {
    // Execute ramsete command
    System.out.println(" ");
    if(!ramseteCommand.isFinished() )
    {
       SmartDashboard.putNumber("Execute COUNT???", i);
       i++;
       ramseteCommand.execute();
       SmartDashboard.putNumber("Chassis Left Encoder: ", m_drive.getLeftEncoder());
       SmartDashboard.putNumber("Chassis Right Encoder: ", m_drive.getRightEncoder());
       SmartDashboard.putNumber("Avg Dist: ", m_drive.getAverageEncoderDistance());
         
       m_drive.periodic();

      double currentTime = SmartDashboard.getNumber("time", 0.0); //get time from AutoDriveSubsystem, cast to second_t

      String currentTrajectoryInfo; //should be exactly 145, originally char[160]
      //what it returns in order: running iteration count, time from SmartDashboard, seconds from state, velocity, acceleration, curvature
      //trajectory pose rotation in degrees, trajectory pose x translation, trajectory pose y translation, 
      //real robot heading degrees, robot heading radians, robot pose rotation , robot left wheel speed, robot right wheel speed, robot pose x translation, robot pose y translation
      currentTrajectoryInfo = String.format("I:%3d SDT:%4.0f S:%4.0f V:%6.3f A:%6.3f C:%5.0f TR:%5.1f TX,Y:(%5.2f, %5.2f), RHD:%5.1f,RHR: %5.1f, RR:%5.1f, RLWS: %7.1f, RRWS: %7.1f, RX,Y:(%5.2f, %5.2f)",
              i, currentTime, 
              trajectory.sample(currentTime).timeSeconds, 
              trajectory.sample(currentTime).velocityMetersPerSecond, 
              trajectory.sample(currentTime).accelerationMetersPerSecondSq,
              trajectory.sample(currentTime).curvatureRadPerMeter,
              trajectory.sample(currentTime).poseMeters.getRotation().getDegrees(), 
              trajectory.sample(currentTime).poseMeters.getTranslation().getX(), 
              trajectory.sample(currentTime).poseMeters.getTranslation().getY(), 
              m_drive.getHeading(), 
              m_drive.getHeading()*(Math.PI/180), 
              m_drive.getPose().getRotation().getDegrees(), 
              m_drive.getWheelSpeeds().leftMetersPerSecond, 
              m_drive.getWheelSpeeds().rightMetersPerSecond, 
              m_drive.getPose().getTranslation().getX(), 
              m_drive.getPose().getTranslation().getY());
      System.out.println(currentTrajectoryInfo);

       if (currentTime<=0.1){
         SmartDashboard.putNumber("Robot distance X at start: ", m_drive.getPose().getTranslation().getX());
         SmartDashboard.putNumber("Robot distance Y at start: ", m_drive.getPose().getTranslation().getY());
       }

       if (currentTime<=5){
         SmartDashboard.putNumber("Robot distance X at 5 sec: ", m_drive.getPose().getTranslation().getX());
         SmartDashboard.putNumber("Robot distance Y at 5 sec: ", m_drive.getPose().getTranslation().getY());
       }

       if (currentTime<=10){
         SmartDashboard.putNumber("Robot distance X at 10 sec: ", m_drive.getPose().getTranslation().getX());
         SmartDashboard.putNumber("Robot distance Y at 10 sec: ", m_drive.getPose().getTranslation().getY());
       }

      SmartDashboard.putNumber("Trajectory Velocity: ", trajectory.sample(currentTime).accelerationMetersPerSecondSq);
      SmartDashboard.putNumber("Left Wheel Spped: ",m_drive.getWheelSpeeds().leftMetersPerSecond);
      SmartDashboard.putNumber("Right Wheel Speed: ", m_drive.getWheelSpeeds().rightMetersPerSecond);

      SmartDashboard.putNumber("Auto Left Wheel Distance: ", m_drive.getLeftEncoder() * (AutoConstants.WHEEL_CIRCUMFERENCE / AutoConstants.PULSES_PER_REVOLUTION));
      SmartDashboard.putNumber("Auto Right Wheel Distance: ", m_drive.getRightEncoder() * (AutoConstants.WHEEL_CIRCUMFERENCE / AutoConstants.PULSES_PER_REVOLUTION));


     }
     else {
       SmartDashboard.putNumber("End COUNT???", j);
       j++;
       System.out.println("end");
       //rc.End(true);  
       m_drive.stopAuto();
     }

    SmartDashboard.putNumber("gyro heading", m_drive.getHeading());
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
