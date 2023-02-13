package frc.robot;

import org.janksters.jankyLib.JankyStateMachine;

import edu.wpi.first.wpilibj.ADIS16470_IMU;
import edu.wpi.first.wpilibj.Timer;

import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj2.command.Command;

import com.ctre.phoenix.motorcontrol.ControlMode;
import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;

import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;

public class AutoStateMachine extends JankyStateMachine {
    private WPI_TalonFX leftLeader;//m3
    private WPI_TalonFX rightFollower;
    private WPI_TalonFX rightLeader;//m1
    private WPI_TalonFX leftFollower;
    
    public double motorEncoderAverage;
    public double newEncoderValue;

    private MotorControllerGroup m_left;
    private MotorControllerGroup m_right;
    DifferentialDrive m_myRobot;

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
    public double gyroAngle;

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
    private final int downRamp = 4;

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
<<<<<<< Updated upstream

        //need to move into AutoConstants file
        gyroAngle = gyroClassLevel.getYComplementaryAngle()*-1;
        minAngle = -2.5;
        maxAngle = 2.5;
        bothkP = 0.48072;
        targetVelocity = 400;

        rlmotor = new WPI_TalonFX(3);//m3
        frmotor = new WPI_TalonFX(1);//m1

        rlmotor.configFactoryDefault();
        rlmotor.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 0, 75);
        rlmotor.configNominalOutputForward(0, 75);
        rlmotor.configNominalOutputReverse(0, 75);
        rlmotor.configPeakOutputForward(1, 75);
        rlmotor.configPeakOutputReverse(-1, 75);
        //falcon.config_kF(0, 0.1074, 75);
        rlmotor.config_kP(0, bothkP, 75);
        rlmotor.config_kI(0, 0.0, 75);
        rlmotor.config_kD(0, 0.0, 75);
        rlmotor.configClosedloopRamp(0.7); 

        frmotor.configFactoryDefault();
        frmotor.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, 0, 75);
        frmotor.configNominalOutputForward(0, 75);
        frmotor.configNominalOutputReverse(0, 75);
        frmotor.configPeakOutputForward(1, 75);
        frmotor.configPeakOutputReverse(-1, 75);
        //falcon.config_kF(0, 0.1074, 75);
        frmotor.config_kP(0, bothkP, 75);
        frmotor.config_kI(0, 0.0, 75);
        frmotor.config_kD(0, 0.0, 75);
        frmotor.configClosedloopRamp(0.7); 

        m_left = new MotorControllerGroup(rlmotor);
        m_right = new MotorControllerGroup(frmotor);
        m_myRobot = new DifferentialDrive(m_left,m_right);
=======
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
        leftLeader = new WPI_TalonFX(4);
        rightLeader = new WPI_TalonFX(5);
        rightFollower = new WPI_TalonFX(7);
        leftFollower = new WPI_TalonFX(6);

        leftLeader.config_kP(0, velocitykP, 75);
        leftLeader.config_kI(0, velocitykI, 75);
        leftLeader.config_kD(0, velocitykD, 75);
        //leftLeader.configClosedloopRamp(0.7);  -- need to readdduring tuning?
        leftLeader.setNeutralMode(NeutralMode.Brake);
        leftLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
    
        rightLeader.config_kP(0, velocitykP, 75);
        rightLeader.config_kI(0, velocitykI, 75);
        rightLeader.config_kD(0, velocitykD, 75);
        //rightLeader.configClosedloopRamp(0.7); -- need to readdduring tuning?
        rightLeader.setNeutralMode(NeutralMode.Brake);
        rightLeader.getSensorCollection().setIntegratedSensorPosition(0,10);

        rightFollower.config_kP(0, velocitykP, 75);
        rightFollower.config_kI(0, velocitykI, 75);
        rightFollower.config_kD(0, velocitykD, 75);
        rightFollower.setNeutralMode(NeutralMode.Brake);
>>>>>>> Stashed changes

        leftFollower.config_kP(0, velocitykP, 75);
        leftFollower.config_kI(0, velocitykI, 75);
        leftFollower.config_kD(0, velocitykD, 75);
        leftFollower.setNeutralMode(NeutralMode.Brake);

        m_left = new MotorControllerGroup(leftLeader,leftFollower);
        m_right = new MotorControllerGroup(rightLeader,rightFollower);
        m_myRobot = new DifferentialDrive(m_left,m_right);

        rightLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
        leftLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
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
        } else {
            SetMachineName ("chargeStation");
            SetName (upRamp, "upRamp");
            SetName(idle, "noMove");
            SetName (goBack, "goBack");
            SetName (goFront, "goFront");
            SetName (downRamp, "downRamp");
            stateMachineSelected = AutoConstants.CHARGE_STATION;
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
                    if (inchesToEncoder(30) <= getAverageEncoderValues()) {
                        m_myRobot.tankDrive(0, 0);
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
                        if (inchesToEncoder(90) >= getAverageEncoderValues()) { 
                            leftLeader.set(TalonFXControlMode.PercentOutput, 0.2);
                            rightLeader.set(TalonFXControlMode.PercentOutput, -0.2); 
                            leftFollower.set(ControlMode.Follower, 4);
                            rightFollower.set(ControlMode.Follower, 5);
                        } else {
                            NewState(idle, "moving up the ramp");
                        }
                        break;
                    case idle: //don't move
                        leftLeader.set(TalonFXControlMode.Velocity, 0);
                        rightLeader.set(TalonFXControlMode.Velocity, 0); 
                        leftFollower.set(ControlMode.Follower, 4);
                        rightFollower.set(ControlMode.Follower, 5);
                        System.out.println("not moving");
<<<<<<< Updated upstream
                        if (gyroAngle < minAngle) {
                            NewState(goFront, "need to move forward");
                        } else if (gyroAngle > maxAngle) {
                            NewState(goBack, "need to move back");
                        }
                        break;
                    case goFront: //forward
                        //m_myRobot.tankDrive(0.35, -0.35);  //commented out for testing
                        rlmotor.set(TalonFXControlMode.Velocity, targetVelocity); //left pos, right neg
                        frmotor.set(TalonFXControlMode.Velocity, -targetVelocity); //what is 500 and why
                        System.out.println("moving forward");
                        if (gyroAngle > maxAngle) {
                            NewState(goBack, "need to move back");
                        } else if (gyroAngle < maxAngle && gyroAngle > minAngle){
=======
                        if (gyroClassLevel.getYComplementaryAngle() < engageMinAngle) {
                            NewState(goFront, "need to move forward");
                        } else if (gyroClassLevel.getYComplementaryAngle() > engageMaxAngle) {
                            NewState(goBack, "need to move back"); 
                        }
                        break;
                    case goFront: //forward
                        leftLeader.set(TalonFXControlMode.Velocity, 1000); 
                        rightLeader.set(TalonFXControlMode.Velocity, -1000); 
                        leftFollower.set(ControlMode.Follower, 4);
                        rightFollower.set(ControlMode.Follower, 5);
                        System.out.println("moving forward");
                        if (gyroClassLevel.getYComplementaryAngle() > engageMaxAngle) {
                            NewState(goBack, "need to move back");
                        } else if (gyroClassLevel.getYComplementaryAngle() < engageMaxAngle && gyroClassLevel.getYComplementaryAngle() > engageMinAngle){
>>>>>>> Stashed changes
                            NewState(idle, "fine!");
                        }
                        break;
                    case goBack: //backwards
                        leftLeader.set(TalonFXControlMode.Velocity, -1000); 
                        rightLeader.set(TalonFXControlMode.Velocity, 1000); 
                        leftFollower.set(ControlMode.Follower, 4);
                        rightFollower.set(ControlMode.Follower, 5);
                        System.out.println("moving backward");
<<<<<<< Updated upstream
                        if (gyroAngle < minAngle) {
                            NewState(goFront, "need to move forward");
                        } else if (gyroAngle < maxAngle && gyroAngle > minAngle){
=======
                        if (gyroClassLevel.getYComplementaryAngle() < engageMinAngle) {
                            NewState(goFront, "need to move forward");
                        } else if (gyroClassLevel.getYComplementaryAngle() < engageMaxAngle && gyroClassLevel.getYComplementaryAngle() > engageMinAngle){
>>>>>>> Stashed changes
                            NewState(idle, "fine!");
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

    public double inchesToEncoder(int inches) {
        newEncoderValue = inches / AutoConstants.WHEEL_CIRCUMFERENCE * AutoConstants.CHASSIS_GEAR_RATIO * AutoConstants.FALCON_PULSES_PER_REVOLUTION;
        return newEncoderValue;
    }

    public double getAverageEncoderValues() {
        double rightLeaderEncoder = rightLeader.getSensorCollection().getIntegratedSensorPosition();
        double leftLeaderEncoder = leftLeader.getSensorCollection().getIntegratedSensorPosition();
        motorEncoderAverage = (Math.abs(rightLeaderEncoder) + Math.abs(leftLeaderEncoder)) / 2;
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