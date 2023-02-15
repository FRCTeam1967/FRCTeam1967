// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

package frc.robot.commands;

import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj2.command.CommandBase;
import frc.robot.subsystems.GerryRigLEDSubsystem;


/** An example command that uses an example subsystem. */
public class ShowColorCommand extends CommandBase {
  private final GerryRigLEDSubsystem m_ledSubsystem;
  private Color m_color;

  /**
   * Creates a new ExampleCommand.
   *
   * @param color The color to show
   * @param subsystem The LED subsystem used by this command.
   */
  public ShowColorCommand(Color color, GerryRigLEDSubsystem subsystem) {
    m_ledSubsystem = subsystem;
    m_color = color;

    // Use addRequirements() here to declare subsystem dependencies.
    addRequirements(subsystem);
  }

  // Called when the command is initially scheduled.
  @Override
  public void initialize() {
    super.initialize();
    //Since we own the subsystem if we're running, we don't need to do this on every iteration
    m_ledSubsystem.setColor(m_color);
  }

  // Returns true when the command should end.
  @Override
  public boolean isFinished() {
    return true;
  }

}
