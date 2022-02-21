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
import com.ctre.phoenix.motorcontrol.can.TalonFX;
import com.ctre.phoenix.motorcontrol.can.VictorSPX;
import edu.wpi.first.wpilibj.ADIS16470_IMU;


import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;

import edu.wpi.first.wpilibj.Timer;

public class Auto extends SequentialCommandGroup{

    //int caseNum = 0;
    //boolean isFinishedDriving = false;
    TalonFX frmotor;
    TalonFX rlmotor;
    DifferentialDrive m_myRobot;
    Timer delayTimer = new Timer();
    Timer drivingTimer = new Timer();
    boolean hasShot = false;
    boolean armUp = false; 
    public static int delay;
    public static int autoPathFinal;
    boolean autoForwardIsFinished = false;

    int drivingTimeSec = 3;
    // going straight and turning in standard
    final int standardFirstDrivingTime = 4;
    // coming back to tarmac to shoot in standard
    final int standardSecondDrivingTime = 9;

    //driving forward initially - maybe make private?
    public void standardDrive(double speed) {
        m_myRobot.tankDrive(speed,-speed);
    }
    //turning 180
    public void standardTurn(double speed) {
        m_myRobot.tankDrive(-speed,-speed);
    }


    public Auto(TalonFX _frmotor, TalonFX _rlmotor, DifferentialDrive _m_myRobot, int _delay) {
        this.frmotor = _frmotor;
        this.rlmotor = _rlmotor;
        this.m_myRobot = _m_myRobot;
        this.delayTimer.reset();
        Auto.delay = _delay;
    }

    public Auto(TalonFX _frmotor, TalonFX _rlmotor, DifferentialDrive _m_myRobot){
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



    public void AutoSequenceStandard(DifferentialDrive m_myrobot, int delay, ADIS16470_IMU m_gyro, int caseNum) {


        
        SmartDashboard.putNumber("Delay Timer", delayTimer.get());
        SmartDashboard.putNumber("Delay Desired", delay);
        SmartDashboard.putNumber("Case Number", caseNum);
        SmartDashboard.putBoolean("Did Auto Go Forward? Probably not.", autoForwardIsFinished);


                startDelayTimer();
                if (delayTimer.get() <=delay) {
                    standardDrive(0);
                    System.out.println("I am in delay");
                    //test without delayTimer.get() >= delay and see if order works
                } else if (delayTimer.get() <= delay+ standardFirstDrivingTime) {
                    standardDrive(0.4);
                    System.out.println("Intake is running");
                    //test without delayTimer.get() >=  delay+ standardFirstDrivingTime and see if order works --> switch to else
                
                } else {
                    int desiredAngle = 150;
                    if (desiredAngle - m_gyro.getAngle() >= 0) {
                        standardTurn(0.6);
                        System.out.println("Stop intake");
                    //check if rotation is finished
                     } else {
                        //moving to tarmac
                        if (delayTimer.get() <= delay + standardSecondDrivingTime) {
                            standardDrive(0.4);
                            System.out.println("Moving back to tarmac");
                        } else {
                            standardDrive(0);
                            System.out.println("Move arm up");
                            //arm up --> checking the state
                            if (armUp == true) {
                                if (hasShot == false) {
                                    System.out.println("Shoot1");
                                    hasShot = true;
                                }
                            }
                        }
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
                    standardDrive(0.4);
                } else if (delayTimer.get() <= delay) {
                    standardDrive(0);
                } 

        SmartDashboard.putString("is simple running", "yes");
            
    }

    public void autoTurn180(DifferentialDrive m_myRobot, ADIS16470_IMU m_gyro){
        System.out.println("I am turning");
        int desiredAngle = 170;
        if (desiredAngle - m_gyro.getAngle() >= 0) {
        m_myRobot.tankDrive(-0.5, -0.5);
        } else {
        m_myRobot.tankDrive(0,0);
        }
    }
}
