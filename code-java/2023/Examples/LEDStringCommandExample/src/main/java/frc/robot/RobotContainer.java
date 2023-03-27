// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot;

import java.util.ArrayList;
import java.util.function.DoubleSupplier;

import org.janksters.ExampleCommonClasses.Commands.LEDStringChaseCommand;
import org.janksters.ExampleCommonClasses.Commands.LEDStringFadeInCommand;
import org.janksters.ExampleCommonClasses.Commands.LEDStringFlashCommand;
import org.janksters.ExampleCommonClasses.Commands.LEDStringRainbowCommand;
import org.janksters.ExampleCommonClasses.Subsystems.LEDStringSubsystem;
import org.janksters.ExampleCommonClasses.Subsystems.LEDSubstringSubsystem;

import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.CommandScheduler;
import edu.wpi.first.wpilibj2.command.Commands;
import edu.wpi.first.wpilibj2.command.InstantCommand;
import edu.wpi.first.wpilibj2.command.ParallelCommandGroup;
import edu.wpi.first.wpilibj2.command.ParallelRaceGroup;
import edu.wpi.first.wpilibj2.command.SequentialCommandGroup;
import edu.wpi.first.wpilibj2.command.WaitCommand;

public class RobotContainer {
  // The real LED subsystem
  private LEDStringSubsystem m_ledSubsystem;
  private ArrayList<LEDSubstringSubsystem> m_gerryRigLEDStrings;

  // A timer we use in the sychronized command version
  private Timer m_sharedTimeBase = new Timer();
  private double m_currentLoopTime = 0;

  public RobotContainer() {
    configureBindings();

    m_ledSubsystem = new LEDStringSubsystem(16 * 16 * 3, 0);
    m_ledSubsystem.brightness = 0.10;
    m_gerryRigLEDStrings = createGerryRigSubstrings();

    synchronizedScheduleDefaultCommands();
  }

  // This method is called in period from Robot.java. You wouldn't normally want to do that, but 
  // we want to latch the current time for all of our LED commands to use so that they all operate 
  // on the same time. Otherwise, even though we're having them all use the same timer, it's possible
  // that the first command reads that timer at t = 1.99999999, but the last command sees time t = 2.0
  // when it executes in the same scheduler loop. That could cause the first and last commands to react
  // differently -- i.e., the first thinks we're flashing red, while the last things we've transitioned 
  // to flashing blue.
  public void updateSynchronizedTimebase() {
    m_currentLoopTime = m_sharedTimeBase.get();
  }

  public ArrayList<LEDSubstringSubsystem> createGerryRigSubstrings() {
    // We create a separate substring for each column in the 3 panel GerryRig panel
    ArrayList<LEDSubstringSubsystem> substrings = new ArrayList<LEDSubstringSubsystem>();
    for (int i = 0; i < 16 * 3; i++) {
      // Even (starting at 0) strings are forward (down), odd strings are backward (up). Each substring is
      // 16 pixels.
      LEDSubstringSubsystem substring = new LEDSubstringSubsystem(m_ledSubsystem, i * 16, 16, (i % 2) == 1);
      substrings.add(substring);
    }

    return substrings;
  }

  public void simpleScheduleDefaultCommands() {
    // We could schedule each substring with an independent default command, but they can become
    // desynchronized with so many independent subsystems. It'll work better if we keep one wait 
    // command for the entire group, rather than having each subsystem contain its own wait command.
    // But even these can become desychronized since there's a delay in setting up each command's timer.
    ArrayList<Color> chaseColors = new ArrayList<Color>();
    chaseColors.add(Color.kRed);
    chaseColors.add(Color.kWhite);
    chaseColors.add(Color.kBlue);

    SequentialCommandGroup commandGroup = new SequentialCommandGroup();
    Command decoratedCommand = commandGroup.ignoringDisable(true).repeatedly();

    ParallelRaceGroup chaseGroup = new ParallelRaceGroup(new WaitCommand(10.0));
    for (LEDSubstringSubsystem substring : m_gerryRigLEDStrings) {
      chaseGroup.addCommands(new LEDStringChaseCommand(chaseColors, 3, 0.2, substring));
    }
    commandGroup.addCommands(chaseGroup);

    ParallelRaceGroup reverseChaseGroup = new ParallelRaceGroup(new WaitCommand(10.0));
    for (LEDSubstringSubsystem substring : m_gerryRigLEDStrings) {
      LEDStringChaseCommand chaseCommand = new LEDStringChaseCommand(chaseColors, 3, 0.2, substring);
      chaseCommand.isReversed = true;
      reverseChaseGroup.addCommands(chaseCommand);
    }
    commandGroup.addCommands(reverseChaseGroup);

    ParallelRaceGroup flashGroup = new ParallelRaceGroup(new WaitCommand(10.0));
    for (LEDSubstringSubsystem substring : m_gerryRigLEDStrings) {
      flashGroup.addCommands(new LEDStringFlashCommand(chaseColors, 0.25, substring));
    }
    commandGroup.addCommands(flashGroup);

    ParallelRaceGroup rainbowGroup = new ParallelRaceGroup(new WaitCommand(10.0));
    for (LEDSubstringSubsystem substring : m_gerryRigLEDStrings) {
      rainbowGroup.addCommands(new LEDStringRainbowCommand(0.1, substring));
    }
    commandGroup.addCommands(rainbowGroup);

    CommandScheduler.getInstance().schedule(decoratedCommand);
  }

  public void synchronizedScheduleDefaultCommands() {
    // To make sure all of the commands stay sychronized, we setup a timer instance that all of the commands
    // can share, and reset it at the proper times. This is probably more complicated than you'd normally care
    // about, but it demonstrates how we can share a timing resource between commands.
    ArrayList<Color> chaseColors = new ArrayList<Color>();
    chaseColors.add(Color.kRed);
    chaseColors.add(Color.kWhite);
    chaseColors.add(Color.kBlue);

    // Create the time base lambda up front
    DoubleSupplier timeSupplier = () -> m_currentLoopTime;

    // Create the top-level sequention command group that will run everything. Make it repeat and 
    // ignore disable so we don't have to enable the robot to see it work.
    SequentialCommandGroup commandGroup = new SequentialCommandGroup();
    Command decoratedCommand = commandGroup.ignoringDisable(true).repeatedly();

    // Start/reset the timer. This shows how you can make a simple command without having to create an
    // entire class for it. In this case, there's no subsystem involved with this timer.
    commandGroup.addCommands(new InstantCommand(() -> {
      m_sharedTimeBase.reset();
      m_sharedTimeBase.start();
    }));

    // Create a parallel group. This will wait until all strings have finished animating.
    var fadeInGroup = new ParallelCommandGroup();
    for (LEDSubstringSubsystem substring : m_gerryRigLEDStrings) {
      fadeInGroup.addCommands(new LEDStringFadeInCommand(Color.kBlue, 0.1, timeSupplier, substring));
    }
    commandGroup.addCommands(fadeInGroup);

    // Reset the timer.
    commandGroup.addCommands(new InstantCommand(() -> {
      m_sharedTimeBase.reset();
    }));
    
    // Create a parallel group. This will wait until all strings have finished animating.
    var reverseFadeInGroup = new ParallelCommandGroup();
    for (LEDSubstringSubsystem substring : m_gerryRigLEDStrings) {
      var command = new LEDStringFadeInCommand(Color.kGreen, 0.1, timeSupplier, substring);
      command.isReversed = true;
      reverseFadeInGroup.addCommands(command);
    }
    commandGroup.addCommands(reverseFadeInGroup);

    // Reset the timer.
    commandGroup.addCommands(new InstantCommand(() -> {
      m_sharedTimeBase.reset();
    }));
  
    // Create a parallel race group. The chase commands will run forever, but the race group will end
    // when the first command terminates, which in this case will be our WaitCommand.
    var chaseGroup = new ParallelRaceGroup(new WaitCommand(10.0));
    for (LEDSubstringSubsystem substring : m_gerryRigLEDStrings) {
      chaseGroup.addCommands(new LEDStringChaseCommand(chaseColors, 3, 0.2, timeSupplier, substring));
    }
    commandGroup.addCommands(chaseGroup);

    // Reset the timer.
    commandGroup.addCommands(new InstantCommand(() -> {
      m_sharedTimeBase.reset();
    }));

    // Now let's do a reverse chase.
    var reverseChaseGroup = new ParallelRaceGroup(new WaitCommand(10.0));
    for (LEDSubstringSubsystem substring : m_gerryRigLEDStrings) {
      LEDStringChaseCommand chaseCommand = new LEDStringChaseCommand(chaseColors, 3, 0.2, timeSupplier, substring);
      chaseCommand.isReversed = true;
      reverseChaseGroup.addCommands(chaseCommand);
    }
    commandGroup.addCommands(reverseChaseGroup);

    // Reset the timer.
    commandGroup.addCommands(new InstantCommand(() -> {
      m_sharedTimeBase.reset();
    }));

    // Now flash the strings. Again, the flash commands will run forever, so we pair them with a WaitCommand
    // so we stop after 10 seconds.
    var flashGroup = new ParallelRaceGroup(new WaitCommand(10.0));
    for (LEDSubstringSubsystem substring : m_gerryRigLEDStrings) {
      flashGroup.addCommands(new LEDStringFlashCommand(chaseColors, 0.25, timeSupplier, substring));
    }
    commandGroup.addCommands(flashGroup);

    // Reset the timer.
    commandGroup.addCommands(new InstantCommand(() -> {
      m_sharedTimeBase.reset();
    }));

    // Finally animate a rainbow, which also runs forever.
    var rainbowGroup = new ParallelRaceGroup(new WaitCommand(10.0));
    for (LEDSubstringSubsystem substring : m_gerryRigLEDStrings) {
      rainbowGroup.addCommands(new LEDStringRainbowCommand(0.1, timeSupplier, substring));
    }
    commandGroup.addCommands(rainbowGroup);

    // Tell the scheduler to run our sequence right now. Because of how we set it up, it'll keep 
    // running until some other command needs a subsystem that we're using.
    CommandScheduler.getInstance().schedule(decoratedCommand);
  }


  private void configureBindings() {}

  public Command getAutonomousCommand() {
    return Commands.print("No autonomous command configured");
  }
}
