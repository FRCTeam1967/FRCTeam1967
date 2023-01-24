package frc.robot.subsystems;

import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardContainer;
import edu.wpi.first.wpilibj2.command.SubsystemBase;

public class LoggableSubsystem extends SubsystemBase {
    public void addToShuffleboard(ShuffleboardContainer container) {
        container.add(this);
    }
}
