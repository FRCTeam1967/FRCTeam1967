package org.janksters.robotcode;

import edu.wpi.first.wpilibj.SpeedController;
import edu.wpi.first.wpilibj.SpeedControllerGroup;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;


import com.ctre.phoenix.motorcontrol.can.WPI_VictorSPX;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import com.ctre.phoenix.ErrorCode;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;
import com.ctre.phoenix.motorcontrol.IMotorController;
import com.ctre.phoenix.motorcontrol.can.BaseMotorController;

public class AutoSanDiego<ShooterControllerInfiniteRecharge> {
    final int FLYWHEEL_TIME = 1;
    final int CONVEYOR_TIME = 3;
    final int DRIVING_TIME = 2;
    final int DISTANCE = 50;
    final int FORWARD = -1;
    final int BACKWARD = 1;
    final String VISION_OFFSET = "Offset";
    final int NO_DATA_DEFAULT = -1000;
    final int BAD_DATA = -1000;
    final int LOWER_BOUND = -3;
    final int UPPER_BOUND = 3;

    ShooterControllerInfiniteRecharge shootercontroller;
    WPI_VictorSPX flmotor;
    static WPI_TalonSRX rlmotor;

    static WPI_TalonSRX frmotor;
    WPI_VictorSPX rrmotor;

    AutoSanDiego(ShooterControllerInfiniteRecharge shootercontroller) {
        ShooterControllerInfiniteRecharge shootercontroller = shootercontroller;
        flmotor = new WPI_VictorSPX(0);
        rlmotor = new WPI_TalonSRX(2);

        frmotor = new WPI_TalonSRX(1);
        rrmotor = new WPI_VictorSPX(3);

        Initialize();
    }

    Timer shootingTimer;
    Timer flywheelTimer;
    Timer drivingTimer;
    SpeedControllerGroup leftMotor;
    SpeedControllerGroup rightMotor;
    DifferentialDrive drive;
    int caseNum;
    double offsetFromVisionTarget;

    void Initialize() {
        SpeedControllerGroup leftMotor = new SpeedControllerGroup(flmotor, rlmotor);
        SpeedControllerGroup rightMotor = new SpeedControllerGroup(frmotor, rrmotor);
        DifferentialDrive drive = new DifferentialDrive(leftMotor, rightMotor);

        // CHASSIS ENCODERS
        rlmotor.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute, 0, 0);
        rlmotor.setSelectedSensorPosition(0, 0, 10);
        rlmotor.getSensorCollection().setQuadraturePosition(0, 10);
        frmotor.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute, 0, 0);
        frmotor.setSelectedSensorPosition(0, 0, 10);
        frmotor.getSensorCollection().setQuadraturePosition(0, 10);

        // VISION DATA
        SmartDashboard.putNumber(VISION_OFFSET, NO_DATA_DEFAULT);
        double offsetFromVisionTarget = (SmartDashboard.getNumber(VISION_OFFSET, NO_DATA_DEFAULT)); // positive is to
                                                                                                    // the right

        // TIMERS
        Timer shootingTimer = new Timer();
        shootingTimer.reset();
        Timer flywheelTimer = new Timer();
        flywheelTimer.reset();
        Timer drivingTimer = new Timer();
        drivingTimer.reset();

        // VARIABLES
        boolean isFinishedDriving = false;
        int caseNum = 0;
    }

    void ResetEncoders() {
        rlmotor.getSensorCollection().setQuadraturePosition(0, 10);
        frmotor.getSensorCollection().setQuadraturePosition(0, 10);
    }

    double GetRightEncoder() {
        return frmotor.getSensorCollection().getQuadraturePosition(); // was originally setQuadraturePosition(0, 10)
    }

    double GetLeftEncoder() {
        return rlmotor.getSensorCollection().getQuadraturePosition(); // was originally setQuadraturePosition(0, 10)
    }
    
    void StartShootingTimer() {
        shootingTimer.start();
    }
    
    void EndShootingTimer() {
        shootingTimer.stop();
        shootingTimer.reset();
    }
    
    void StartFlywheelTimer() {
        flywheelTimer.start();
    }
    
    void EndFlywheelTimer() {
        flywheelTimer.stop();
        flywheelTimer.reset();
    }
    
    boolean isFinishedDriving = true;

    void DriveStraight(int distancePulses, int forwardOrBack) {
        double avgEncoders = (GetRightEncoder() + GetLeftEncoder()) / 2;
        System.out.println("DISTANCE: " + avgEncoders);
        if (drivingTimer.get() < DRIVING_TIME) {
            double speed = 0.5 * forwardOrBack;
            drive.tankDrive(-speed, -speed);
        }
        else {
            drive.tankDrive(0.0, 0.0);
        }
        System.out.println(avgEncoders);
    }
    
    void CenterTurretAutomatically() {
        if (offsetFromVisionTarget == BAD_DATA)
        {
            ((ShooterControllerInfiniteRecharge) shootercontroller).StopTurret();
        }
        else if (offsetFromVisionTarget < LOWER_BOUND) 
        {
            ((Object) shootercontroller).TurretRight();
        }
        else if (offsetFromVisionTarget > UPPER_BOUND)
        {
          shootercontroller.TurretLeft();
        }
        else {
          shootercontroller.StopTurret();
        }
    }
    
    void RunAuto() {
        CenterTurretAutomatically();
        // AUTO SEQUENCE
        switch(caseNum) {
            case 0:
                System.out.println("CASE 0");
                CenterTurretAutomatically();
                StartFlywheelTimer();
                drive.tankDrive(0.0, 0.0);
                caseNum = 1;
                break;
            case 1: 
                System.out.println("CASE 1");                CenterTurretAutomatically();
                shootercontroller.SetRPMAuto();
                if(flywheelTimer.get() > FLYWHEEL_TIME)
                {
                    StartShootingTimer();
                    EndFlywheelTimer();
                    caseNum = 2;
                }
                break; 
                drive.tankDrive(0.0, 0.0);
            case 2: 
                System.out.println("CASE 2");                CenterTurretAutomatically();
                CenterTurretAutomatically();
                shootercontroller.StartConveyorBelt();
                shootercontroller.BridgeForward(); 
                if(shootingTimer.get() > CONVEYOR_TIME)
                {
                    shootercontroller.StopConveyorBelt();
                    shootercontroller.StopBridge(); 
                    shootercontroller.StopTarget();
                    drivingTimer.start();
                    EndShootingTimer();
                    caseNum = 3;
                }
                drive.tankDrive(0.0, 0.0);
                break;
            case 3:
                System.out.println("CASE 3");
                CenterTurretAutomatically();
                DriveStraight(DISTANCE, BACKWARD);
                if (isFinishedDriving == true) {
                    caseNum = 4;
                }
                break;
            case 4: 
                System.out.println("CASE 4");
                drive.tankDrive(0.0, 0.0);
                System.out.println("IS FINISHED AUTO");
                break;
        }
    }
}
