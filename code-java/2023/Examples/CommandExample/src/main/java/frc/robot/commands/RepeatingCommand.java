package frc.robot.commands;

import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.CommandBase;
import edu.wpi.first.wpilibj2.command.Subsystem;

public class RepeatingCommand extends CommandBase {
    private Command m_commandToRepeat;

    public RepeatingCommand(Command commandToRepeat) {
        m_commandToRepeat = commandToRepeat;
        for (Subsystem req : commandToRepeat.getRequirements()) {
            addRequirements(req);
        }
    }

    @Override
    public void initialize() {
        super.initialize();
        m_commandToRepeat.initialize();
    }

    @Override
    public void execute() {
        super.execute();
        m_commandToRepeat.execute();
    }

    @Override
    public boolean isFinished() {
        if (m_commandToRepeat.isFinished()) {
            m_commandToRepeat.end(false);
            m_commandToRepeat.initialize();
        }

        return false;
    }

    @Override
    public void end(boolean interrupted) {
        super.end(interrupted);
        m_commandToRepeat.end(interrupted);
    }
}
