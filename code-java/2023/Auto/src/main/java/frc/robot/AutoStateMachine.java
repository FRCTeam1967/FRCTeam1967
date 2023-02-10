package frc.robot;

import org.janksters.jankyLib.JankyStateMachine;

import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import edu.wpi.first.wpilibj.ADIS16470_IMU;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj2.command.Command;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;

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
            } else if (stateMachineSelected == AutoConstants.CHARGE_STATION) {
                switch(curState) {
                    case upRamp:
                       // m_myRobot.tankDrive(0.8, -0.8);
                        //if (inchesToEncoder(10) <= getAverageEncoderValues()) { //VROOM VROOM go up ramp speedy
                           // m_myRobot.tankDrive(0, 0);
                            NewState(idle, "on the station");
                        //}
                        break;
                    case idle: //don't move
                        rlmotor.set(TalonFXControlMode.Velocity, 0);
                        frmotor.set(TalonFXControlMode.Velocity, 0); 
                        System.out.println("not moving");
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
                            NewState(idle, "fine!");
                        }
                        break;
                    case goBack: //backwards
                        // rlmotor.set(TalonFXControlMode.Velocity, -500); //left pos, right neg
                        // frmotor.set(TalonFXControlMode.Velocity, 500); //what is 500 and why
                        m_myRobot.tankDrive(-0.35, 0.35);
                        System.out.println("moving backward");
                        if (gyroAngle < minAngle) {
                            NewState(goFront, "need to move forward");
                        } else if (gyroAngle < maxAngle && gyroAngle > minAngle){
                            NewState(idle, "fine!");
                        }
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
        double newEncoderValue = inches / AutoConstants.WHEEL_CIRCUMFERENCE * AutoConstants.CHASSIS_GEAR_RATIO * AutoConstants.FALCON_PULSES_PER_REVOLUTION;
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