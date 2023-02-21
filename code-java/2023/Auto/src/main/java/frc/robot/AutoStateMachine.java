package frc.robot;

import org.janksters.jankyLib.JankyStateMachine;

import edu.wpi.first.wpilibj.ADIS16470_IMU;
import edu.wpi.first.wpilibj.Timer;

//import edu.wpi.first.wpilibj.drive.DifferentialDrive;
//import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.motorcontrol.can.TalonSRX;
import com.ctre.phoenix.motorcontrol.TalonSRXFeedbackDevice;
//import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj2.command.Command;

import com.ctre.phoenix.motorcontrol.ControlMode;
import com.ctre.phoenix.motorcontrol.NeutralMode;
//import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
//import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import com.ctre.phoenix.motorcontrol.TalonSRXControlMode;
import com.ctre.phoenix.motorcontrol.SensorCollection;

import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;

public class AutoStateMachine extends JankyStateMachine {
    //private TalonSRX leftLeader;//m3
    private TalonSRX leftLeader = new TalonSRX(5); //7 is leftLeader for KitBot
    private TalonSRX rightLeader = new TalonSRX(6);
    //private WPI_TalonFX rightFollower;
    //private TalonSRX rightLeader;//m1
    //private WPI_TalonFX leftFollower;
    //private SensorCollection leftEncoderPosition;
    //private SensorCollection rightEncoderPosition;

    private SensorCollection leftEncoderPosition = new SensorCollection(leftLeader);
    private SensorCollection rightEncoderPosition = new SensorCollection(rightLeader);

    public double motorEncoderAverage;
    public double newEncoderValue;

    //private MotorControllerGroup m_left;
    //private MotorControllerGroup m_right;
    //DifferentialDrive m_myRobot;

    public double gyroAngle;
    public ADIS16470_IMU gyroClassLevel;

    public ShuffleboardTab AutoPIDTestingTab;

    Timer delayTimer = new Timer();
    public static int delay;
    public static int path;
    public int stateMachineSelected;
    public String currentState;

    public double engageMinAngle;
    public double engageMaxAngle;

    public double minAngle;
    public double maxAngle;
    public double targetVelocity;
    public double bothkP;

    private final int simpleDelay = 0;
    private final int simpleMove = 1;
    private final int simpleFinishAuto = 2;

    private final int ocShoot = 0;
    private final int ocDelay = 1;
    private final int ocMove = 2;
    private final int ocFinishAuto = 3;

    private final int upRamp = 0;
    private final int idle = 1;
    private final int goBack = 2;
    private final int goFront = 3;
    private final int backUpRamp = 4;

    private final int squareMove = 0;
    private final int squareTurn = 1;

    static final double velocitykP = 0.2;
    static final double velocitykI = 0.0;
    static final double velocitykD = 0.3;

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
        gyroAngle = m_gyro.getYComplementaryAngle();

        //move into autoConstants
        engageMinAngle = -5;
        engageMaxAngle = 5;

        AutoPIDTestingTab = Shuffleboard.getTab("Auto PID Testing");
        
        AutoPIDTestingTab.addDouble("kP Velocity", () -> velocitykP);
        AutoPIDTestingTab.addDouble("kI Velocity", () -> velocitykI);
        AutoPIDTestingTab.addDouble("kD Velocity", () -> velocitykD);

        AutoPIDTestingTab.addDouble("Avg Encoder", () -> motorEncoderAverage);
        AutoPIDTestingTab.addDouble("Inches to Encoder", () -> newEncoderValue);
        
        //need to move this into robot.java
    
        
        //rightFollower = new WPI_TalonFX(7);
        //leftFollower = new WPI_TalonFX(6);

        leftLeader.config_kP(0, velocitykP, 75);
        leftLeader.config_kI(0, velocitykI, 75);
        leftLeader.config_kD(0, velocitykD, 75);
        //leftLeader.configClosedloopRamp(0.7);  -- need to readdduring tuning?
        leftLeader.setNeutralMode(NeutralMode.Brake);
        //leftLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
        leftLeader.configSelectedFeedbackSensor(TalonSRXFeedbackDevice.CTRE_MagEncoder_Relative, 0, 10);

    
        rightLeader.config_kP(0, velocitykP, 75);
        rightLeader.config_kI(0, velocitykI, 75);
        rightLeader.config_kD(0, velocitykD, 75);
        //rightLeader.configClosedloopRamp(0.7); -- need to readdduring tuning?
        rightLeader.setNeutralMode(NeutralMode.Brake);
        //rightLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
        rightLeader.configSelectedFeedbackSensor(TalonSRXFeedbackDevice.CTRE_MagEncoder_Relative, 0, 10);

        /* 

        rightFollower.config_kP(0, velocitykP, 75);
        rightFollower.config_kI(0, velocitykI, 75);
        rightFollower.config_kD(0, velocitykD, 75);
        rightFollower.setNeutralMode(NeutralMode.Brake);

        leftFollower.config_kP(0, velocitykP, 75);
        leftFollower.config_kI(0, velocitykI, 75);
        leftFollower.config_kD(0, velocitykD, 75);
        leftFollower.setNeutralMode(NeutralMode.Brake);
        */

        //m_left = new MotorControllerGroup(leftLeader,leftFollower);
        //m_right = new MotorControllerGroup(rightLeader,rightFollower);
        //m_myRobot = new DifferentialDrive(m_left,m_right);

        //rightLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
        //leftLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
        m_gyro.reset();

        if (path == AutoConstants.ONE_CUBE_AUTOPATH) {
            SetMachineName("oneCubeAuto");
            SetName(ocShoot, "ocShoot");
            SetName(ocDelay, "ocDelay");
            SetName(ocMove, "ocMove");
            SetName(ocFinishAuto, "ocFinishAuto");
            stateMachineSelected = AutoConstants.ONE_CUBE_AUTOPATH;
            start();
        } else if (path == AutoConstants.SIMPLE_AUTOPATH) { //leaving tarmac path
            SetMachineName ("simpleAuto");
            SetName (simpleDelay, "simpleDelay");
            SetName(simpleMove, "simpleMove");
            SetName (simpleFinishAuto, "simpleFinishAuto");
            stateMachineSelected = AutoConstants.SIMPLE_AUTOPATH;
            start();
        } else if(path == AutoConstants.CHARGE_STATION) {
            SetMachineName ("chargeStation");
            SetName (upRamp, "upRamp");
            SetName(idle, "noMove");
            SetName (goBack, "goBack");
            SetName (goFront, "goFront");
            SetName (backUpRamp, "backUpRamp");
            stateMachineSelected = AutoConstants.CHARGE_STATION;
            start();
        } else if(path == AutoConstants.SQUARE) { //for showcase
            SetMachineName("square");
            SetName(squareMove, "square move");
            SetName(squareTurn, "square turn");
            stateMachineSelected = AutoConstants.SQUARE;
            start();
        }
    }

    //still need to move all of the distances + speeds into a constants file (working on tuning it right now)
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
                    //m_myRobot.tankDrive(-0.4, 0.4); //go backwards
                    leftLeader.set(TalonSRXControlMode.PercentOutput, -0.4);
                    rightLeader.set(TalonSRXControlMode.PercentOutput, 0.4); 
                    if (inchesToEncoder(30) <= getAverageEncoderValues()) {
                        //m_myRobot.tankDrive(0, 0);
                        leftLeader.set(TalonSRXControlMode.PercentOutput, 0.0);
                        rightLeader.set(TalonSRXControlMode.PercentOutput, 0.0);
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
                    //m_myRobot.tankDrive(0.4, -0.4);
                    leftLeader.set(TalonSRXControlMode.PercentOutput, 0.4);
                    rightLeader.set(TalonSRXControlMode.PercentOutput, -0.4);
                    if (inchesToEncoder(30) <= getAverageEncoderValues()) {
                        //m_myRobot.tankDrive(0, 0);
                        leftLeader.set(TalonSRXControlMode.PercentOutput, 0.0);
                        rightLeader.set(TalonSRXControlMode.PercentOutput, 0.0);
                        NewState(simpleFinishAuto, "reached average encoder for distance");
                    }
                    break;
                case simpleFinishAuto:
                    Terminate();
                    break;
            }
            } else if (stateMachineSelected == AutoConstants.CHARGE_STATION) {
                switch(curState) {
                    case upRamp:
                        currentState = "up ramp";
                        if (inchesToEncoder(5) >= getAverageEncoderValues()) { 
                            leftLeader.set(TalonSRXControlMode.PercentOutput, 0.2);
                            rightLeader.set(TalonSRXControlMode.PercentOutput, -0.2); 
                            //leftFollower.set(ControlMode.Follower, 4);
                            //rightFollower.set(ControlMode.Follower, 5);
                        } else {
                            rightLeader.setSelectedSensorPosition(0);
                            leftLeader.setSelectedSensorPosition(0);
                            NewState(backUpRamp, "back up ramp");
                        }
                        break;
                     case backUpRamp:
                        currentState = "back up ramp";
                        if (inchesToEncoder(9)>= getAverageEncoderValues()) { 
                            leftLeader.set(TalonSRXControlMode.PercentOutput, -0.2);
                            rightLeader.set(TalonSRXControlMode.PercentOutput, 0.2); 
                            //leftFollower.set(ControlMode.Follower, 4);
                            //rightFollower.set(ControlMode.Follower, 5);
                        } else {
                            NewState(idle, "moving up the ramp");
                        }
                        break;
                    case idle: //don't move
                        leftLeader.set(TalonSRXControlMode.Velocity, 0);
                        rightLeader.set(TalonSRXControlMode.Velocity, 0); 
                        //leftFollower.set(ControlMode.Follower, 4);
                        //rightFollower.set(ControlMode.Follower, 5);
                        System.out.println("not moving");
                        if (gyroClassLevel.getYComplementaryAngle() < engageMinAngle) {
                            NewState(goFront, "need to move forward");
                        } else if (gyroClassLevel.getYComplementaryAngle() > engageMaxAngle) {
                            NewState(goBack, "need to move back"); 
                        }
                        break;
                    case goFront: //forward
                        leftLeader.set(TalonSRXControlMode.Velocity, 1000); 
                        rightLeader.set(TalonSRXControlMode.Velocity, -1000); 
                        //leftFollower.set(ControlMode.Follower, 4);
                        //rightFollower.set(ControlMode.Follower, 5);
                        System.out.println("moving forward");
                        if (gyroClassLevel.getYComplementaryAngle() > engageMaxAngle) {
                            NewState(goBack, "need to move back");
                        } else if (gyroClassLevel.getYComplementaryAngle() < engageMaxAngle && gyroClassLevel.getYComplementaryAngle() > engageMinAngle){
                            NewState(idle, "fine!");
                        }
                        break;
                    case goBack: //backwards
                        leftLeader.set(TalonSRXControlMode.Velocity, -1000); 
                        rightLeader.set(TalonSRXControlMode.Velocity, 1000); 
                        //leftFollower.set(ControlMode.Follower, 4);
                        //rightFollower.set(ControlMode.Follower, 5);
                        System.out.println("moving backward");
                        if (gyroClassLevel.getYComplementaryAngle() < engageMinAngle) {
                            NewState(goFront, "need to move forward");
                        } else if (gyroClassLevel.getYComplementaryAngle() < engageMaxAngle && gyroClassLevel.getYComplementaryAngle() > engageMinAngle){
                            NewState(idle, "fine!");
                        }
                        break;
                }
            } else if (stateMachineSelected == AutoConstants.SQUARE) {
                switch(curState) {
                    case squareMove:
                        if (inchesToEncoder(10) >= getAverageEncoderValues()) { 
                            leftLeader.set(TalonSRXControlMode.PercentOutput, 0.4);
                            rightLeader.set(TalonSRXControlMode.PercentOutput, -0.4); 
                        } else {
                            rightLeader.setSelectedSensorPosition(0);
                            leftLeader.setSelectedSensorPosition(0);
                            NewState(squareTurn, "square turn");
                        }
                        break;
                    case squareTurn:
                        if (inchesToEncoder(2.2) >= getAverageEncoderValues()) { 
                            leftLeader.set(TalonSRXControlMode.PercentOutput, 0.3);
                            rightLeader.set(TalonSRXControlMode.PercentOutput, 0.3); 
                        } else {
                            rightLeader.setSelectedSensorPosition(0);
                            leftLeader.setSelectedSensorPosition(0);
                            NewState(squareMove, "square move");
                        }
                        break; 
                }
            }
        }

    public void displayCurrentState() {
        SmartDashboard.putNumber("Auto current state", GetCurrentState()); //displays auto current state
    }

    public void endStateMachine() {
        System.out.println("Auto SM terminating");
        Terminate();
        System.out.println("Auto SM terminated");
    }

    public double inchesToEncoder(double inches) { //changed to double
        newEncoderValue = inches / AutoConstants.WHEEL_CIRCUMFERENCE * AutoConstants.CHASSIS_GEAR_RATIO * AutoConstants.FALCON_PULSES_PER_REVOLUTION;
        return newEncoderValue;
    }


    public double getAverageEncoderValues() {
        //double rightLeaderEncoder = rightLeader.getSensorCollection().getIntegratedSensorPosition();
        //double leftLeaderEncoder = leftLeader.getSensorCollection().getIntegratedSensorPosition();
        //double rightLeaderEncoder = rightEncoderPosition.getPulseWidthPosition(); 
        //double leftLeaderEncoder = leftEncoderPosition.getPulseWidthPosition();

        double rightLeaderEncoder = rightLeader.getSelectedSensorPosition();
        double leftLeaderEncoder = leftLeader.getSelectedSensorPosition();

        motorEncoderAverage = (Math.abs(rightLeaderEncoder) + Math.abs(leftLeaderEncoder)) / 2; //orginally leftLeaderEncoder
        return motorEncoderAverage;
    }

    public boolean waitDelay(int delayVal) {
        if (delayTimer.get() <= delayVal) {
            //m_myRobot.tankDrive(0, 0);
            leftLeader.set(TalonSRXControlMode.PercentOutput, 0.0);
            rightLeader.set(TalonSRXControlMode.PercentOutput, 0.0);
        } else {
            return true;
        }
        return false;
    }
}