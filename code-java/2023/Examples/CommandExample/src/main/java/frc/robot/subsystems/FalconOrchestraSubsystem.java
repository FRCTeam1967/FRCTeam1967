package frc.robot.subsystems;

import frc.robot.Constants;

import java.util.ArrayList;


import com.ctre.phoenix.ErrorCode;
import com.ctre.phoenix.motorcontrol.can.TalonFX;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.music.Orchestra;

import edu.wpi.first.util.sendable.SendableBuilder;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardContainer;

public class FalconOrchestraSubsystem extends LoggableSubsystem {
    private static ArrayList<TalonFX> m_instruments;
    private static Orchestra m_orchestra;
    private static Constants.Sound m_currentSound;
    
    public FalconOrchestraSubsystem() {
        m_instruments = new ArrayList<TalonFX>();
        m_instruments.add(new WPI_TalonFX(Constants.kFirstFalconOrchestraID, "rio"));
        m_instruments.add(new WPI_TalonFX(Constants.kSecondFalconOrchestraID, "rio"));

        m_orchestra = new Orchestra(m_instruments);
    }

    public boolean loadMusic(Constants.Sound sound) {
        m_currentSound = sound;
        ErrorCode error =  m_orchestra.loadMusic(sound.filename);
        return error == ErrorCode.OK;
    }

    public void play() {
        m_orchestra.play();
    }

    public boolean isPlaying() {
        return m_orchestra.isPlaying() && m_orchestra.getCurrentTime() < m_currentSound.lengthMS;
    }

    public void stop() {
        m_orchestra.stop();
    }

    @Override
    public void initSendable(SendableBuilder builder) {
        super.initSendable(builder);
        builder.addBooleanProperty(".isPlaying", this::isPlaying, null);
        // builder.setSmartDashboardType("OrchestraSubsystem");
    }

    @Override
    public void addToShuffleboard(ShuffleboardContainer container) {
        super.addToShuffleboard(container);
        container.add("Motor 0", (WPI_TalonFX)m_instruments.get(0));
        container.add("Motor 1", (WPI_TalonFX)m_instruments.get(1));
        container.addBoolean("OrchestraPlaying", this::isPlaying);
        container.addNumber("TimeIndex", () -> (double)m_orchestra.getCurrentTime());
    }

}
