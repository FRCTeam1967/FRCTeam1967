package frc.robot;

import com.ctre.phoenix.motorcontrol.ControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import org.janksters.jankyLib.JankyStateMachine;
import org.janksters.jankyLib.jankyXboxJoystick;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

// 8 values are usually TBD
//我爱你, <3 !! ♥️

public class Lifter extends JankyStateMachine {
    // Declaring states
    private final int Idle = 0;
    private final int IntakeConfig = 1;
    private final int ShooterConfig = 2;

    private jankyXboxJoystick XboxController;

    private WPI_TalonFX m_motor;
    private static final int M_MOTOR_ID = 8; // TBD

    public Lifter() {
        XboxController = new jankyXboxJoystick(0); // TBD

        m_motor = new WPI_TalonFX(M_MOTOR_ID);

        SetMachineName("Lifter");
        SetName(Idle, "Idle");
        SetName(IntakeConfig, "IntakeConfig");
        SetName(ShooterConfig, "ShooterConfig");

        start();
    }

    public void StateEngine(int curState, boolean onStateEntered) {
        switch (curState) {
            case Idle:
                displayCurrentState();
                if (XboxController.GetButtonBack()) {
                    NewState(IntakeConfig, "back button pressed");
                }
                if (XboxController.GetButtonStart()) {
                    NewState(ShooterConfig, "start button pressed");
                }
                break;
            case IntakeConfig:
                displayCurrentState();
                if (XboxController.GetButtonBack() == false) {
                    NewState(Idle, "back button released");
                }
                break;
            case ShooterConfig:
                displayCurrentState();
                if (XboxController.GetButtonStart() == false) {
                    NewState(Idle, "start button released");
                }
                break;
        }
    }

    public void displayCurrentState() {
        SmartDashboard.putNumber("Current state", GetCurrentState());
    }
}