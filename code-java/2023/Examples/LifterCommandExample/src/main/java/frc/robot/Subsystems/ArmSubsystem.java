package frc.robot.Subsystems;

import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.SubsystemBase;
import frc.robot.Arm;

public class ArmSubsystem extends SubsystemBase {
    Arm armStateMachine = null;

    public ArmSubsystem(Arm arm) {
        armStateMachine = arm;
    }

    @Override
    public void periodic() {
        // Nothing to do since the state machine uses its own thread. We could use this
        // to push the current state to the dashboard if we wanted.
    }

    public void setDesiredPosition(double angle) {
        armStateMachine.setDesiredPosition(angle);
    }

    public boolean isAtDesiredPosition() {
        return armStateMachine.isIdle();
    }

    public void resetHome() {
        armStateMachine.armHoming();
    }
}
