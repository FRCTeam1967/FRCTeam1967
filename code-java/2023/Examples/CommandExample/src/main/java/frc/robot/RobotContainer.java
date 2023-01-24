// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import edu.wpi.first.wpilibj.DataLogManager;
import edu.wpi.first.wpilibj.DriverStation;
import edu.wpi.first.wpilibj.GenericHID;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.XboxController;
import edu.wpi.first.wpilibj.shuffleboard.EventImportance;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.CommandScheduler;
import edu.wpi.first.wpilibj2.command.ParallelCommandGroup;
import edu.wpi.first.wpilibj2.command.ParallelRaceGroup;
import edu.wpi.first.wpilibj2.command.RunCommand;
import edu.wpi.first.wpilibj2.command.SequentialCommandGroup;
import edu.wpi.first.wpilibj2.command.WaitCommand;
import edu.wpi.first.wpilibj2.command.button.JoystickButton;
import frc.robot.Constants.Sound;
import frc.robot.commands.FadeImageCommand;
import frc.robot.commands.PlayMusicCommand;
import frc.robot.commands.RepeatingCommand;
import frc.robot.commands.ShowColorCommand;
import frc.robot.commands.USAColorCommand;
import frc.robot.subsystems.FalconOrchestraSubsystem;
import frc.robot.subsystems.GerryRigLEDSubsystem;

/**
 * This class is where the bulk of the robot should be declared. Since Command-based is a
 * "declarative" paradigm, very little robot logic should actually be handled in the {@link Robot}
 * periodic methods (other than the scheduler calls). Instead, the structure of the robot (including
 * subsystems, commands, and button mappings) should be declared here.
 */
public class RobotContainer {
    // The robot's subsystems and commands are defined here...
    private final GerryRigLEDSubsystem m_ledSubsystem = new GerryRigLEDSubsystem();
    private final FalconOrchestraSubsystem m_orchestraSubsystem = new FalconOrchestraSubsystem();

    private final SendableChooser<Sound> m_chooser = new SendableChooser<>();

    /** The container for the robot. Contains subsystems, OI devices, and commands. */
    public RobotContainer() {
        // Init logging
        configureLogging();
        // Configure the button bindings
        configureButtonBindings();

        // Push stuff to the dashboard
        configureDashboard();

        // Configure default (background) commands
        configureDefaultCommandsForSubsystems();

        // Setup the autonomous chooser
        m_chooser.setDefaultOption("Beethoven's Fifth", Constants.Sound.kFifthSymphony);
        m_chooser.addOption("Beethoven's Moonlight", Constants.Sound.kMoonlight);
        m_chooser.addOption("YMCA", Constants.Sound.kYMCA);
        m_chooser.addOption("???", Constants.Sound.kRickRoll);
        SmartDashboard.putData("Autonomous Music Selection", m_chooser);
    }

    public void configureLogging() {
        DataLogManager.start();
        DriverStation.startDataLog(DataLogManager.getLog());

        CommandScheduler.getInstance()
        .onCommandInitialize(command -> Shuffleboard.addEventMarker("Command init", command.getName(), EventImportance.kNormal));
        
        CommandScheduler.getInstance()
        .onCommandInterrupt(command -> Shuffleboard.addEventMarker("Command interrupt", command.getName(), EventImportance.kNormal));

        CommandScheduler.getInstance()
        .onCommandFinish(command -> Shuffleboard.addEventMarker("Command finish", command.getName(), EventImportance.kNormal));

    }

    /**
     * Use this method to define your button->command mappings. Buttons can be created by
     * instantiating a {@link GenericHID} or one of its subclasses ({@link
     * edu.wpi.first.wpilibj.Joystick} or {@link XboxController}), and then passing it to a {@link
     * edu.wpi.first.wpilibj2.command.button.JoystickButton}.
     */
    private void configureButtonBindings() {
        Joystick joystick = new Joystick(Constants.kJoystickID);
        JoystickButton fireButton = new JoystickButton(joystick, 1);
        fireButton.onTrue(new ParallelCommandGroup(
            new ShowColorCommand(Color.kRed, m_ledSubsystem),
            new PlayMusicCommand(Constants.Sound.kStartMatch, m_orchestraSubsystem)
        ).withName("Fire!"));
    }

    private void configureDashboard() {
        ShuffleboardTab tab = Shuffleboard.getTab("Scheduler");
        tab.add("The Scheduler", CommandScheduler.getInstance());

        // Add some fabricated commands. We use alongWith/WaitCommand to make it last a specified duration.
        // We could, of course, create commands that displayed a color for a specific time, but this illustrates
        // how commands can be easily composed.
        tab = Shuffleboard.getTab("Commands");
        tab.add("Green", new ShowColorCommand(Color.kGreen, m_ledSubsystem).alongWith(new WaitCommand(5.0)).withName("Green/5"));
        tab.add("Purple", new ShowColorCommand(Color.kPurple, m_ledSubsystem).alongWith(new WaitCommand(5.0)).withName("Purple/5"));
        tab.add("USA", new USAColorCommand(m_ledSubsystem).withName("Red+White+Blue"));

        tab = Shuffleboard.getTab("Subsystems");
        m_ledSubsystem.addToShuffleboard(tab);
        m_orchestraSubsystem.addToShuffleboard(tab);
    }

    private void configureDefaultCommandsForSubsystems() {
        // If the LED subsystem isn't asked to do anything else, show a rainbow pattern
        m_ledSubsystem.setDefaultCommand(new RunCommand(m_ledSubsystem::drawRainbow, m_ledSubsystem).withName("Idle Rainbow"));
    }

    /**
     * Use this to pass the autonomous command to the main {@link Robot} class.
     *
     * @return the command to run in autonomous
     */
    public Command getAutonomousCommand() {
        // An ExampleCommand will run in autonomous
        return new ParallelRaceGroup(
            new PlayMusicCommand(m_chooser.getSelected(), m_orchestraSubsystem),
            new RepeatingCommand(
                new SequentialCommandGroup(
                    new FadeImageCommand(Constants.Images.kJanksterBow, 5000, true, m_ledSubsystem),
                    new FadeImageCommand(Constants.Images.kJanksterBow, 5000, false, m_ledSubsystem)
                )
            )
        );
        }
}
