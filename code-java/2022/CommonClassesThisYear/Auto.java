package frc.robot;
import org.janksters.CommonClassesThisYear.*;


import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;

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
import com.ctre.phoenix.motorcontrol.can.VictorSPX;
import edu.wpi.first.wpilibj.ADIS16470_IMU;


import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;

import edu.wpi.first.wpilibj.Timer;

public class Auto extends SequentialCommandGroup{

    //int caseNum = 0;
    //boolean isFinishedDriving = false;
    WPI_TalonSRX frmotor;
    WPI_TalonSRX rlmotor;
    DifferentialDrive m_myRobot;
    Timer delayTimer = new Timer();
    Timer drivingTimer = new Timer();

    public static int delay;
    public static int autoPathFinal;
    boolean autoForwardIsFinished = false;

    int drivingTime = 3;


    public Auto(WPI_TalonSRX _frmotor, WPI_TalonSRX _rlmotor, DifferentialDrive _m_myRobot, int _delay) {
        this.frmotor = _frmotor;
        this.rlmotor = _rlmotor;
        this.m_myRobot = _m_myRobot;
        this.delayTimer.reset();
        Auto.delay = _delay;
    }

    public Auto(WPI_TalonSRX _frmotor, WPI_TalonSRX _rlmotor, DifferentialDrive _m_myRobot){
        this.frmotor = _frmotor;
        this.rlmotor = _rlmotor;
        this.m_myRobot = _m_myRobot;
    }

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

    public void  resetEncoders() {
        rlmotor.getSensorCollection().setQuadraturePosition(0,10);
        frmotor.getSensorCollection().setQuadraturePosition(0,10);
    } 

    public int getRightEncoder(WPI_TalonSRX frmotor) {
        return frmotor.getSensorCollection().getQuadraturePosition();
    }
    
    public int getLeftEncoder(WPI_TalonSRX rlmotor) {
        return rlmotor.getSensorCollection().getQuadraturePosition();
    }




    public void AutoSequenceStandard(DifferentialDrive m_myrobot, int delay, ADIS16470_IMU m_gyro, int caseNum) {


        
        SmartDashboard.putNumber("Delay Timer", delayTimer.get());
        SmartDashboard.putNumber("Delay Desired", delay);
        SmartDashboard.putNumber("Case Number", caseNum);
        SmartDashboard.putBoolean("Did Auto Go Forward? Probably not.", autoForwardIsFinished);
        // int endTime;
        
        // if (delay <= 3){
        //     endTime = 12;
        // }
        // // else if (delay == 2){
        // //     endTime = 10;
        // // }
        // else if (delay == 5){
        //     endTime = 15;
        // }
        // else {
        //     endTime = 10;
        // }

        //SmartDashboard.putNumber("Driving Time", endTime);
                startDelayTimer();
                if (delayTimer.get() >= delay && delayTimer.get() <= delay+4) {
                    m_myRobot.tankDrive(0.6,-0.6);
                } else if (delayTimer.get() <= delay) {
                    m_myRobot.tankDrive(0, 0);
                } else if (delayTimer.get() >=  delay+4) {
                    int desiredAngle = 145;
                    if (desiredAngle - m_gyro.getAngle() >= 0) {
                        m_myRobot.tankDrive(-0.7,-0.7);
                    } else if (m_gyro.getAngle() > 170) {
                        if (delayTimer.get() <= delay + 8) {
                            m_myRobot.tankDrive(0.7,-0.7);
                        } else {
                            m_myRobot.tankDrive(0,0);
                        }
                    } else {
                    m_myRobot.tankDrive(0,0);
                    }
                }
            
    }

    public void AutoSequenceSimple(DifferentialDrive m_myrobot, int delay, ADIS16470_IMU m_gyro, int caseNum) {


        SmartDashboard.putNumber("Delay Timer", delayTimer.get());
        SmartDashboard.putNumber("Delay Desired", delay);
        SmartDashboard.putNumber("Case Number", caseNum);
        SmartDashboard.putBoolean("Did Auto Go Forward? Probably not.", autoForwardIsFinished);

                startDelayTimer();
                if (delayTimer.get() >= delay && delayTimer.get() <= delay+4) {
                    m_myRobot.tankDrive(0.6,-0.6);
                } else if (delayTimer.get() <= delay) {
                    m_myRobot.tankDrive(0, 0);
                } 

        SmartDashboard.putString("is simple running", "yes");
            
    }

    public void autoTurn180(DifferentialDrive m_myRobot, ADIS16470_IMU m_gyro){
        System.out.println("I am turning");
        int desiredAngle = 130;
    if (desiredAngle - m_gyro.getAngle() >= 0) {
      m_myRobot.tankDrive(-0.7, -0.7);
    } else {
      m_myRobot.tankDrive(0,0);
    }



    }
}
