package frc.robot.commands;

import edu.wpi.first.wpilibj.util.Color;
import edu.wpi.first.wpilibj2.command.SequentialCommandGroup;
import edu.wpi.first.wpilibj2.command.WaitCommand;
import frc.robot.subsystems.GerryRigLEDSubsystem;


public class USAColorCommand extends SequentialCommandGroup {
    public USAColorCommand(GerryRigLEDSubsystem ledSubsystem) {
        addCommands(
            parallel(
                new ShowColorCommand(Color.kRed, ledSubsystem),
                new WaitCommand(5.0)
            ),
            parallel(
                new ShowColorCommand(Color.kWhite, ledSubsystem),
                new WaitCommand(5.0)
            ),
            parallel(
                new ShowColorCommand(Color.kBlue, ledSubsystem),
                new WaitCommand(5.0)
            )
        );
    }
}
