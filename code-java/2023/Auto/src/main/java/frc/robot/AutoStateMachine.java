package frc.robot;

import org.janksters.jankyLib.JankyStateMachine;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.TimedRobot;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj2.command.Command;

import edu.wpi.first.wpilibj.ADIS16470_IMU;
import com.ctre.phoenix.motorcontrol.can.TalonFX;
import edu.wpi.first.wpilibj.ADIS16470_IMU;

import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;

import edu.wpi.first.wpilibj.Timer;

public class AutoStateMachine extends JankyStateMachine {
    private WPI_TalonFX rlmotor;//m3
    private WPI_TalonFX frmotor;//m1

    private MotorControllerGroup m_left;
    private MotorControllerGroup m_right;
    DifferentialDrive m_myRobot;
    public ADIS16470_IMU gyroClassLevel;

    Timer delayTimer = new Timer();
    public static int delay;
    public static int path;
    public int stateMachineSelected;

    private final int simpleDelay = 0;
    private final int simpleMove = 1;
    private final int simpleFinishAuto = 2;

    private final int ocShoot = 0;
    private final int ocDelay = 1;
    private final int ocMove = 2;
    private final int ocFinishAuto = 3;

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

    public AutoStateMachine(int _delay, int autoPathSelected, ADIS16470_IMU m_gyro) {
        delay = _delay;
        path = autoPathSelected;
        gyroClassLevel = m_gyro;
        rlmotor = new WPI_TalonFX(3);//m3
        frmotor = new WPI_TalonFX(1);//m1

        m_left = new MotorControllerGroup(rlmotor);
        m_right = new MotorControllerGroup(frmotor);
        m_myRobot = new DifferentialDrive(m_left,m_right);

        frmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
        rlmotor.getSensorCollection().setIntegratedSensorPosition(0,10);

        m_gyro.reset();
        if (path == AutoConstants.ONE_CUBE_AUTOPATH) {
            SetMachineName("oneCubeAuto");
            SetName(ocShoot, "ocShoot");
            SetName(ocDelay, "ocDelay");
            SetName(ocMove, "ocMove");
            SetName(ocFinishAuto, "ocFinishAuto");
            stateMachineSelected = AutoConstants.ONE_CUBE_AUTOPATH;
            start();
        } else { //leaving tarmac path
            SetMachineName ("simpleAuto");
            SetName (simpleDelay, "simpleDelay");
            SetName(simpleMove, "simpleMove");
            SetName (simpleFinishAuto, "simpleFinishAuto");
            stateMachineSelected = AutoConstants.SIMPLE_AUTOPATH;
            start();
        }
    }

    public void StateEngine(int curState, boolean onStateEntered) {
        if (stateMachineSelected == AutoConstants.ONE_CUBE_AUTOPATH) {
            switch (curState) {
                case ocShoot:
                    if (onStateEntered) {
                        System.out.println("shoot");
                    } else {
                        NewState(ocDelay, "shooting done!");
                    }
                    break;
                case ocDelay:
                    if (onStateEntered) {
                        delayTimer.start();
                        System.out.println("start timer");
                    }
                    if (waitDelay(delay)) {
                        System.out.println("delay timer end");
                        NewState(ocMove, "Delay timer has ended");
                    }
                    break;
                case ocMove:
                    m_myRobot.tankDrive(-0.4, 0.4); //go backwards
                    if (inchesToEncoder(30) <= getAverageEncoderValues()) {
                        m_myRobot.tankDrive(0, 0);
                        NewState(ocFinishAuto, "reached average encoder for distance");
                    }
                    break;
                case ocFinishAuto:
                    Terminate();
                    break;
            }
        } else if (stateMachineSelected == AutoConstants.SIMPLE_AUTOPATH) { //leaving tarmac path
            switch (curState) {
                case simpleDelay:
                    if (onStateEntered) {
                        delayTimer.start();
                    }
                    if (waitDelay(delay)) {
                        NewState(simpleMove, "Delay timer has ended");
                    }
                    break;
                case simpleMove:
                    m_myRobot.tankDrive(0.4, -0.4);
                    if (inchesToEncoder(30) <= getAverageEncoderValues()) { //expect 30, actual 84
                        m_myRobot.tankDrive(0, 0);
                        NewState(simpleFinishAuto, "reached average encoder for distance");
                    }
                    break;
                case simpleFinishAuto:
                    Terminate();
                    break;
            }
        }
    }

    public void displayCurrentState() {
        SmartDashboard.putNumber("auto current state", GetCurrentState()); //displays auto current state
    }

    public void endStateMachine() {
        System.out.println("auto state machine terminating");
        Terminate();
        System.out.println("auto state machine terminated");
    }

    public double inchesToEncoder(int inches) {
        //double newEncoderValue = ((inches / AutoConstants.CHASSIS_GEAR_RATIO) * Constants.FALCON_PULSES_PER_REVOLUTION) * AutoConstants.WHEEL_CIRCUMFERENCE;
        double newEncoderValue = inches / AutoConstants.WHEEL_CIRCUMFERENCE * AutoConstants.CHASSIS_GEAR_RATIO * AutoConstants.FALCON_PULSES_PER_REVOLUTION;
        //((distance / Constants.CLIMB_GEAR_RATIO) * Constants.FALCON_PULSES_PER_REVOLUTION) / Constants.WINCH_CIRCUMFERENCE;
        return newEncoderValue;
    }

    public double getAverageEncoderValues() {
        double frmotorEncoder = frmotor.getSensorCollection().getIntegratedSensorPosition();
        double rlmotorEncoder = rlmotor.getSensorCollection().getIntegratedSensorPosition();
        double motorEncoderAverage = (Math.abs(frmotorEncoder) + Math.abs(rlmotorEncoder)) / 2;
        return motorEncoderAverage;
    }

    public boolean waitDelay(int delayVal) {
        if (delayTimer.get() <= delayVal) {
            m_myRobot.tankDrive(0, 0);
        } else {
            return true;
        }
        return false;
    }
}