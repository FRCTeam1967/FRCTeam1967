package frc.robot;
import org.janksters.CommonClassesThisYear.*;
import org.janksters.jankyLib.JankyStateMachine;


import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.XboxController;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorController;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.motorcontrol.PWMTalonFX;
import edu.wpi.first.wpilibj.motorcontrol.PWMVictorSPX;
import edu.wpi.first.wpilibj.motorcontrol.Talon;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.math.controller.PIDController;
import edu.wpi.first.wpilibj2.command.Command;
import edu.wpi.first.wpilibj2.command.SequentialCommandGroup;


import com.ctre.phoenix.sensors.CANCoder;
import com.fasterxml.jackson.databind.annotation.JsonAppend.Attr;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.ErrorCode;
import com.ctre.phoenix.motorcontrol.ControlMode;
import com.ctre.phoenix.motorcontrol.can.TalonSRX;
import com.ctre.phoenix.motorcontrol.can.TalonFX;
import com.ctre.phoenix.motorcontrol.can.VictorSPX;
import edu.wpi.first.wpilibj.ADIS16470_IMU;


import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;

import edu.wpi.first.wpilibj.Timer;

public class AutoStateMachine extends JankyStateMachine {
    private WPI_TalonSRX m_leftLeader;//m2
    private WPI_TalonFX rlmotor;//m3
    private WPI_TalonFX frmotor;//m1
    private WPI_TalonSRX m_rightFollower;//m0

    private MotorControllerGroup m_left;
    private MotorControllerGroup m_right;
    DifferentialDrive m_myRobot;

    Timer delayTimer = new Timer();
    public static int delay;
    //DifferentialDrive m_myRobot = new DifferentialDrive(m_leftFollower,m_rightLeader);

    private final int firstDelay = 0;
    private final int firstMove = 1;
    private final int turn = 2;
    private final int secondMove = 3;
    private final int liftAndShoot = 4;
    private final int finishAuto = 5;

    public void resetDelayTimer(){
        delayTimer.reset();
    }


    public void setInitialDelayTime(SendableChooser<Command> autoDelayChooser){}

    public int getDelay() {
        return delay;
    }


    public double getDelayTimer() {
        return delayTimer.get();
    }

    public void startDelayTimer() {
        delayTimer.start();
    }

    public void endDelayTimer() {
        delayTimer.stop();
        delayTimer.reset();
    }

    public AutoStateMachine() {
        delay = 3;

        m_leftLeader = new WPI_TalonSRX(2);//m2
        rlmotor = new WPI_TalonFX(3);//m3
        frmotor = new WPI_TalonFX(1);//m1
        m_rightFollower = new WPI_TalonSRX(0);//m0

        m_left = new MotorControllerGroup(m_leftLeader, rlmotor);
        m_right = new MotorControllerGroup(frmotor, m_rightFollower);
        m_myRobot = new DifferentialDrive(m_left,m_right);
    

        SetMachineName("auto");
        SetName(firstDelay, "firstDelay");
        SetName(firstMove, "firstMove");
        SetName(turn, "turn");
        SetName(secondMove, "secondMove");
        SetName(liftAndShoot, "liftAndShoot");
        SetName(finishAuto, "finishAuto");
        start();

    }

    public void StateEngine(int curState, boolean onStateEntered) {
        switch (curState) {
            case firstDelay:
                if (onStateEntered) {
                    delayTimer.start();
                }
                if (delayTimer.get() <=delay) {
                    m_myRobot.tankDrive(0, 0);
                    System.out.println("I am in delay");
                } else {
                    NewState(firstMove, "Delay timer has ended");
                }
                break;
            case firstMove:
                m_myRobot.tankDrive(0.4, -0.4);
                System.out.println(inchesToEncoder(10));
                System.out.println(getAverageEncoderValues());
                if (inchesToEncoder(10) <= getAverageEncoderValues()) {
                    m_myRobot.tankDrive(0, 0);
                    NewState(turn, "reached average encoder for distance");
                }
                break;
            case turn:
                //m_myRobot.tankDrive(0.4, 0.4);
                System.out.println("Turn state entered");
                break;
            case secondMove:
                break;
            case liftAndShoot:
                break;
            case finishAuto:
                break;
        }
    }
    public void displayCurrentState() {
        SmartDashboard.putNumber("current state", GetCurrentState());
    }

    public double inchesToEncoder(int inches) {
        double newEncoderValue = (inches / 2.99) * 2048;
        return newEncoderValue;
    }

    public double getAverageEncoderValues() {
        double frmotorEncoder = frmotor.getSensorCollection().getIntegratedSensorPosition();
        double rlmotorEncoder = rlmotor.getSensorCollection().getIntegratedSensorPosition();
        double motorEncoderAverage = (frmotorEncoder + rlmotorEncoder) / 2;
        return motorEncoderAverage;
    }
}

