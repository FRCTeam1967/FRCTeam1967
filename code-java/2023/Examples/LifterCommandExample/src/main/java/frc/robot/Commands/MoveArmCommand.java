package frc.robot.Commands;

import edu.wpi.first.wpilibj2.command.CommandBase;

import frc.robot.Subsystems.ArmSubsystem;

public class MoveArmCommand extends CommandBase {
    ArmSubsystem m_armSubsystem;
    double m_desiredPosition;

    public MoveArmCommand(double angle, ArmSubsystem subsystem) {
        m_armSubsystem = subsystem;
        m_desiredPosition = angle;

        addRequirements(subsystem);
    }

    @Override
    public void initialize() {
        super.initialize();
        m_armSubsystem.setDesiredPosition(m_desiredPosition);
    }

    @Override
    public boolean isFinished() {
        return m_armSubsystem.isAtDesiredPosition();
    }

    public void end(boolean interrupted) {
        super.end(interrupted);
    }
}
