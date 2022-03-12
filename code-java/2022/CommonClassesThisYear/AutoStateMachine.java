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

import edu.wpi.first.wpilibj.ADXRS450_Gyro;
import edu.wpi.first.wpilibj.SPI;
import edu.wpi.first.wpilibj.ADIS16470_IMU.IMUAxis;
import edu.wpi.first.wpilibj.ADIS16470_IMU;


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
    public ADIS16470_IMU gyroClassLevel;

    Timer delayTimer = new Timer();
    public static int delay;
    public int stateMachineSelected;
    private final int twoBFirstDelay = 0;
    private final int twoBFirstMove = 1;
    private final int twoBTurn = 2;
    private final int twoBSecondMove = 3;
    private final int twoBSecondTurn = 4;
    private final int twoBLiftAndShoot = 5;
    private final int twoBFinishAuto = 6;

    private final int threeBFirstDelay = 0;
    private final int threeBFirstShoot = 1;
    private final int threeBLoweredArm = 2;
    private final int threeBFirstMove = 3;
    private final int threeBFirstTurn = 4;
    private final int threeBSecondMove = 5;
    private final int threeBSecondTurn = 6;
    private final int threeBThirdMove = 7;
    private final int threeBThirdTurn = 8;
    private final int threeBFourthMove = 9;
    private final int threeBLiftAndShoot = 10;
    private final int threeBFinishAuto = 11;

    private final int simpleDelay = 0;
    private final int simpleMove = 1;
    private final int simpleFinishAuto = 2;

    boolean armLifted = false;
    boolean hasShoot = false;
    boolean armLowered = false;

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
        gyroClassLevel = m_gyro;
        m_leftLeader = new WPI_TalonSRX(2);//m2
        rlmotor = new WPI_TalonFX(3);//m3
        frmotor = new WPI_TalonFX(1);//m1
        m_rightFollower = new WPI_TalonSRX(0);//m0

        m_left = new MotorControllerGroup(m_leftLeader, rlmotor);
        m_right = new MotorControllerGroup(frmotor, m_rightFollower);
        m_myRobot = new DifferentialDrive(m_left,m_right);

        frmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
        rlmotor.getSensorCollection().setIntegratedSensorPosition(0,10);

        m_gyro.reset();
        
        if (autoPathSelected == 2) {
            SetMachineName("autoSupreme");
            SetName(threeBFirstDelay, "threeBFirstDelay");
            SetName(threeBFirstShoot, "threeBFirstShoot");
            SetName(threeBLoweredArm, "threeBLoweredArm");
            SetName(threeBFirstMove, "threeBFirstMove");
            SetName(threeBFirstTurn, "threeBFirstTurn");
            SetName(threeBSecondMove, "threeBSecondMove");
            SetName(threeBSecondTurn, "threeBSecondTurn");
            SetName(threeBThirdMove, "threeBThirdMove");
            SetName(threeBThirdTurn, "threeBThirdTurn");
            SetName(threeBFourthMove, "threeBFourthMove");
            SetName(threeBLiftAndShoot, "threeBLiftAndShoot");
            SetName(threeBFinishAuto, "threeBFinishAuto");
            stateMachineSelected = 2;
            start();
        } else if (autoPathSelected == 1) {
            SetMachineName("auto");
            SetName(twoBFirstDelay, "twoBFirstDelay");
            SetName(twoBFirstMove, "twoBFirstMove");
            SetName(twoBTurn, "twoBTurn");
            SetName(twoBSecondMove, "twoBSecondMove");
            SetName(twoBSecondTurn, "twoBSecondTurn");
            SetName(twoBLiftAndShoot, "twoBLiftAndShoot");
            SetName(twoBFinishAuto, "twoBFinishAuto");
            stateMachineSelected = 1;
            start();
        }  else {
            SetMachineName ("simpleTarmac");
            SetName (simpleDelay, "simpleDelay");
            SetName(simpleMove, "simpleMove");
            SetName (simpleFinishAuto, "simpleFinishAuto");
            stateMachineSelected = 0;
            start();

        }

    }

    public void StateEngine(int curState, boolean onStateEntered) {
        if (stateMachineSelected == 2) {  
            switch (curState) {
                case threeBFirstDelay:
                    if (onStateEntered) {
                        delayTimer.start();
                    }
                    if (delayTimer.get() <=delay) {
                        m_myRobot.tankDrive(0, 0);
                    } else {
                        NewState(threeBFirstShoot, "Delay timer has ended");
                    }
                    break;
                case threeBFirstShoot:
                    if (onStateEntered) {
                        System.out.println("Shoot");
                    }
                    hasShoot = true;

                    if (hasShoot) {
                        hasShoot = false;
                        NewState(threeBLoweredArm, "Finished shooting");
                    }
                    break;
                case threeBLoweredArm:
                    if (onStateEntered) {
                        System.out.println("Lowered Arm");
                    }
                    armLowered = true;
                    
                    if (armLowered) {
                        armLowered = false;
                        NewState(threeBFirstMove, "Arm is down");
                    }
                    break;
                case threeBFirstMove:
                    m_myRobot.tankDrive(-0.4, 0.4);
                    if (inchesToEncoder(70) <= getAverageEncoderValues()) {
                        m_myRobot.tankDrive(0, 0);
                        NewState(threeBFirstTurn, "reached average encoder for distance");
                    }
                    break;
                case threeBFirstTurn:
                    SmartDashboard.putNumber("gyro angle new", gyroClassLevel.getAngle());
                    m_myRobot.tankDrive(-0.4, -0.4);
                    int desiredAngle = 166; //supposed to be 180 but gyro is off by 14 degrees
                    System.out.println(gyroClassLevel.getAngle() + 14);
                    if(gyroClassLevel.getAngle() >= desiredAngle) {
                        NewState(threeBSecondMove, "reached desired gyro angle");
                    }
                    frmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                    rlmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                    break;
                case threeBSecondMove:
                    m_myRobot.tankDrive(0.4, -0.4);
                    if (inchesToEncoder(20) <= getAverageEncoderValues()) {
                        m_myRobot.tankDrive(0, 0);
                        NewState(threeBSecondTurn, "reached average encoder for distance 2");
                    }
                    gyroClassLevel.reset();
                    break;
                case threeBSecondTurn:
                    SmartDashboard.putNumber("gyro angle new", gyroClassLevel.getAngle());
                    m_myRobot.tankDrive(-0.4, -0.4);
                    desiredAngle = 46; //60 - 14
                    System.out.println(gyroClassLevel.getAngle() + 14);
                    if(gyroClassLevel.getAngle() >= desiredAngle) {
                        NewState(threeBThirdMove, "reached desired gyro angle");
                    }
                    frmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                    rlmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                    break;
                case threeBThirdMove:
                    m_myRobot.tankDrive(0.4, -0.4);
                    if (inchesToEncoder(117) <= getAverageEncoderValues()) {
                        m_myRobot.tankDrive(0, 0);
                        NewState(threeBThirdTurn, "reached average encoder for distance 2");
                    }
                    break;
                case threeBThirdTurn:
                    SmartDashboard.putNumber("gyro angle new", gyroClassLevel.getAngle());
                    m_myRobot.tankDrive(-0.4, -0.4);
                    desiredAngle = 50; //63 - 13
                    if(gyroClassLevel.getAngle() >= desiredAngle) {
                        NewState(threeBFourthMove, "reached desired gyro angle");
                    }
                    frmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                    rlmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                    break;
                case threeBFourthMove:
                    m_myRobot.tankDrive(0.4, -0.4);
                    if (inchesToEncoder(115) <= getAverageEncoderValues()) {
                        m_myRobot.tankDrive(0, 0);
                        NewState(threeBLiftAndShoot, "reached average encoder for distance 2");
                    }
                    break;
                case threeBLiftAndShoot:
                    if (onStateEntered) {
                        System.out.println("Lift Arm and shoot");
                    }
                    armLifted = true;
                    hasShoot = true;

                    if (armLifted && hasShoot) {
                        NewState(threeBFinishAuto, "Arm is up and finished shooting");
                    }
                    break;
                case threeBFinishAuto:
                    Terminate();
                    break;
            }
        } else if (stateMachineSelected == 1) {
            switch (curState) {
            case twoBFirstDelay:
                if (onStateEntered) {
                    delayTimer.start();
                }
                if (delayTimer.get() <=delay) {
                    m_myRobot.tankDrive(0, 0);
                } else {
                    NewState(twoBFirstMove, "Delay timer has ended");
                }
                break;
            case twoBFirstMove:
                m_myRobot.tankDrive(0.4, -0.4);
                if (inchesToEncoder(50) <= getAverageEncoderValues()) {
                    m_myRobot.tankDrive(0, 0);
                    NewState(twoBTurn, "reached average encoder for distance");
                }
                break;
            case twoBTurn:
                SmartDashboard.putNumber("gyro angle first turn", gyroClassLevel.getAngle());
                m_myRobot.tankDrive(-0.4, -0.4);
                int desiredAngle = 167;
                if(gyroClassLevel.getAngle() >= desiredAngle) {
                    NewState(twoBSecondMove, "reached desired gyro angle");
                }
                frmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                rlmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                break;
            case twoBSecondMove:
                m_myRobot.tankDrive(0.4, -0.4);
                if (inchesToEncoder(60) <= getAverageEncoderValues()) {
                    m_myRobot.tankDrive(0, 0);
                    NewState(twoBSecondTurn, "reached average encoder for distance 2");
                }
                gyroClassLevel.reset();
                break;
            case twoBSecondTurn:
                SmartDashboard.putNumber("gyro angle second turn", gyroClassLevel.getAngle());
                m_myRobot.tankDrive(-0.4, -0.4);
                desiredAngle = 30;
                if(gyroClassLevel.getAngle() >= desiredAngle) {
                    NewState(twoBLiftAndShoot, "reached desired gyro angle 2");
                }
                break;
            case twoBLiftAndShoot:
                if (onStateEntered) {
                    System.out.println("Lift Arm and shoot");
                }
                armLifted = true;
                hasShoot = true;

                if (armLifted && hasShoot) {
                    NewState(twoBFinishAuto, "Arm is up and finished shooting");
                }
                break;
            case twoBFinishAuto:
                Terminate();
                break;
        }
        } else if (stateMachineSelected == 0) {
            switch (curState) {
                case simpleDelay:
                    if (onStateEntered) {
                        delayTimer.start();
                    }
                    if (delayTimer.get() <=delay) {
                        m_myRobot.tankDrive(0, 0);
                    } else {
                        NewState(simpleMove, "Delay timer has ended");
                    }
                    break;
                case simpleMove:
                    m_myRobot.tankDrive(0.4, -0.4);
                    if (inchesToEncoder(50) <= getAverageEncoderValues()) {
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
        SmartDashboard.putNumber("current state", GetCurrentState());
    }

    public void endStateMachine() {
        Terminate();
        System.out.println("auto state machine terminate");
    }

    public double inchesToEncoder(int inches) {
        double newEncoderValue = (inches / 2.99) * 2048;
        return newEncoderValue;
    }

    public double getAverageEncoderValues() {
        double frmotorEncoder = frmotor.getSensorCollection().getIntegratedSensorPosition();
        double rlmotorEncoder = rlmotor.getSensorCollection().getIntegratedSensorPosition();
        double motorEncoderAverage = (Math.abs(frmotorEncoder) + Math.abs(rlmotorEncoder)) / 2;
        return motorEncoderAverage;
    }
}

