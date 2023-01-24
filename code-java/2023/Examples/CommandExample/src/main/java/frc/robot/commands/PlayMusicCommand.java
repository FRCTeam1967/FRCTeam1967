package frc.robot.commands;

import edu.wpi.first.util.sendable.SendableBuilder;
import edu.wpi.first.wpilibj2.command.CommandBase;
import frc.robot.Constants;
import frc.robot.subsystems.FalconOrchestraSubsystem;

public class PlayMusicCommand extends CommandBase {
    private Constants.Sound m_sound;
    private FalconOrchestraSubsystem m_ochestraSubsystem;

    public PlayMusicCommand(Constants.Sound soundDescriptor, FalconOrchestraSubsystem subsystem) {
        m_ochestraSubsystem = subsystem;
        m_sound = soundDescriptor;

        addRequirements(subsystem);
    }

    @Override
    public void initialize() {
        super.initialize();
        if (m_ochestraSubsystem.loadMusic(m_sound)) {
            m_ochestraSubsystem.play();
        }
    }

    @Override
    public boolean isFinished() {
        return !m_ochestraSubsystem.isPlaying();
    }

    public void end(boolean interrupted) {
        super.end(interrupted);
        m_ochestraSubsystem.stop();
    }

    public String musicFile() {
        return m_sound.filename;
    }
    
    @Override
    public void initSendable(SendableBuilder builder) {
        super.initSendable(builder);

        builder.addStringProperty("Music File", this::musicFile, null);
        builder.addBooleanProperty("Is Playing", this::isFinished, null);
    }
}
