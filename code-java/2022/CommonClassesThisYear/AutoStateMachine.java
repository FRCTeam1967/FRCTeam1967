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

    Timer delayTimer = new Timer();
    public static int delay;

    // private final int firstDelay = 0;
    // private final int firstMove = 1;
    // private final int turn = 2;
    // private final int secondMove = 3;
    // private final int liftAndShoot = 4;
    // private final int finishAuto = 5;

    private final int firstDelay = 0;
    private final int firstShoot = 1;
    private final int loweredArm = 2;
    private final int firstMove = 3;
    private final int firstTurn = 4;
    private final int secondMove = 5;
    private final int secondTurn = 6;
    private final int thirdMove = 7;
    private final int thirdTurn = 8;
    private final int fourthMove = 9;
    private final int liftAndShoot = 10;
    private final int finishAuto = 11;

    boolean armLifted = false;
    boolean hasShoot = false;
    boolean armLowered = false;

    public ADIS16470_IMU m_gyro = new ADIS16470_IMU();

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

        frmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
        rlmotor.getSensorCollection().setIntegratedSensorPosition(0,10);

        m_gyro.reset();

        SetMachineName("autoSupreme");
        SetName(firstDelay, "firstDelay");
        SetName(firstShoot, "firstShoot");
        SetName(loweredArm, "loweredArm");
        SetName(firstMove, "firstMove");
        SetName(firstTurn, "firstTurn");
        SetName(secondMove, "secondMove");
        SetName(secondTurn, "secondTurn");
        SetName(thirdMove, "thirdMove");
        SetName(thirdTurn, "thirdTurn");
        SetName(fourthMove, "fourthMove");
        SetName(liftAndShoot, "liftAndShoot");
        SetName(finishAuto, "finishAuto");
    

        // SetMachineName("auto");
        // SetName(firstDelay, "firstDelay");
        // SetName(firstMove, "firstMove");
        // SetName(turn, "turn");
        // SetName(secondMove, "secondMove");
        // SetName(liftAndShoot, "liftAndShoot");
        // SetName(finishAuto, "finishAuto");

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
                    //For testing purposes - test later on carpet
                    //System.out.println("I am in delay");
                } else {
                    NewState(firstShoot, "Delay timer has ended");
                }
                break;
            case firstShoot:
                if (onStateEntered) {
                    System.out.println("Shoot");
                }
                hasShoot = true;

                if (hasShoot) {
                    hasShoot = false;
                    NewState(loweredArm, "Finished shooting");
                }
            case loweredArm:
                if (onStateEntered) {
                    System.out.println("Lowered Arm");
                }
                armLowered = true;
                
                if (armLowered) {
                    armLowered = false;
                    NewState(firstMove, "Arm is down");
                }
            case firstMove:
                m_myRobot.tankDrive(-0.4, 0.4);
                //For testing purposes - test later on carpet
                //System.out.println(inchesToEncoder(50));
                //System.out.println(getAverageEncoderValues());
                if (inchesToEncoder(70) <= getAverageEncoderValues()) {
                    m_myRobot.tankDrive(0, 0);
                    NewState(firstTurn, "reached average encoder for distance");
                }
                break;
            case firstTurn:
                SmartDashboard.putNumber("gyro angle new", m_gyro.getAngle());
                m_myRobot.tankDrive(-0.4, -0.4);
                int desiredAngle = 166; //supposed to be 180 but gyro is off by 14 degrees
                System.out.println(m_gyro.getAngle() + 14);
                if(m_gyro.getAngle() >= desiredAngle) {
                    NewState(secondMove, "reached desired gyro angle");
                }
                frmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                rlmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                break;
            case secondMove:
                m_myRobot.tankDrive(0.4, -0.4);
                //For testing purposes - test later on carpet
                //System.out.println(inchesToEncoder(50));
                //System.out.println(getAverageEncoderValues());
                if (inchesToEncoder(20) <= getAverageEncoderValues()) {
                    m_myRobot.tankDrive(0, 0);
                    NewState(secondTurn, "reached average encoder for distance 2");
                }
                m_gyro.reset();
                break;
            case secondTurn:
                SmartDashboard.putNumber("gyro angle new", m_gyro.getAngle());
                m_myRobot.tankDrive(-0.4, -0.4);
                desiredAngle = 46; //60 - 14
                System.out.println(m_gyro.getAngle() + 14);
                if(m_gyro.getAngle() >= desiredAngle) {
                    NewState(finishAuto, "reached desired gyro angle");
                }
                frmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                rlmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                break;
            case thirdMove:
                m_myRobot.tankDrive(0.4, -0.4);
                //For testing purposes - test later on carpet
                //System.out.println(inchesToEncoder(50));
                //System.out.println(getAverageEncoderValues());
                if (inchesToEncoder(117) <= getAverageEncoderValues()) {
                    m_myRobot.tankDrive(0, 0);
                    NewState(thirdTurn, "reached average encoder for distance 2");
                }
                break;
            case thirdTurn:
                SmartDashboard.putNumber("gyro angle new", m_gyro.getAngle());
                m_myRobot.tankDrive(-0.4, -0.4);
                desiredAngle = 50; //63 - 13
                if(m_gyro.getAngle() >= desiredAngle) {
                    NewState(fourthMove, "reached desired gyro angle");
                }
                frmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                rlmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
                break;
            case fourthMove:
                m_myRobot.tankDrive(0.4, -0.4);
                //For testing purposes - test later on carpet
                //System.out.println(inchesToEncoder(50));
                //System.out.println(getAverageEncoderValues());
                if (inchesToEncoder(115) <= getAverageEncoderValues()) {
                    m_myRobot.tankDrive(0, 0);
                    NewState(liftAndShoot, "reached average encoder for distance 2");
                }
                break;
            case liftAndShoot:
                if (onStateEntered) {
                    System.out.println("Lift Arm and shoot");
                }
                armLifted = true;
                hasShoot = true;

                if (armLifted && hasShoot) {
                    NewState(finishAuto, "Arm is up and finished shooting");
                }
                break;
            case finishAuto:
                Terminate();
                break;
        }
    }

    // public void StateEngine(int curState, boolean onStateEntered) {
    //     switch (curState) {
    //         case firstDelay:
    //             if (onStateEntered) {
    //                 delayTimer.start();
    //             }
    //             if (delayTimer.get() <=delay) {
    //                 m_myRobot.tankDrive(0, 0);
    //                 //For testing purposes - test later on carpet
    //                 //System.out.println("I am in delay");
    //             } else {
    //                 NewState(firstMove, "Delay timer has ended");
    //             }
    //             break;
    //         case firstMove:
    //             m_myRobot.tankDrive(0.4, -0.4);
    //             //For testing purposes - test later on carpet
    //             //System.out.println(inchesToEncoder(50));
    //             //System.out.println(getAverageEncoderValues());
    //             if (inchesToEncoder(50) <= getAverageEncoderValues()) {
    //                 m_myRobot.tankDrive(0, 0);
    //                 NewState(turn, "reached average encoder for distance");
    //             }
    //             break;
    //         case turn:
    //             SmartDashboard.putNumber("gyro angle new", m_gyro.getAngle());
    //             m_myRobot.tankDrive(-0.4, -0.4);
    //             int desiredAngle = 167;
    //             if(m_gyro.getAngle() >= desiredAngle) {
    //                 NewState(secondMove, "reached desired gyro angle");
    //             }
    //             frmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
    //             rlmotor.getSensorCollection().setIntegratedSensorPosition(0,10);
    //             break;
    //         case secondMove:
    //             m_myRobot.tankDrive(0.4, -0.4);
    //             //For testing purposes - test later on carpet
    //             //System.out.println(inchesToEncoder(50));
    //             //System.out.println(getAverageEncoderValues());
    //             if (inchesToEncoder(70) <= getAverageEncoderValues()) {
    //                 m_myRobot.tankDrive(0, 0);
    //                 NewState(liftAndShoot, "reached average encoder for distance 2");
    //             }
    //             break;
    //         case liftAndShoot:
    //             if (onStateEntered) {
    //                 System.out.println("Lift Arm and shoot");
    //             }
    //             armLifted = true;
    //             hasShoot = true;

    //             if (armLifted && hasShoot) {
    //                 NewState(finishAuto, "Arm is up and finished shooting");
    //             }
    //             break;
    //         case finishAuto:
    //             Terminate();
    //             break;
    //     }
    // }
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
        double motorEncoderAverage = (Math.abs(frmotorEncoder) + Math.abs(rlmotorEncoder)) / 2;
        return motorEncoderAverage;
    }
}

