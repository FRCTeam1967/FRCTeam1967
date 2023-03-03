package frc.robot;

import org.janksters.jankyLib.JankyStateMachine;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import edu.wpi.first.wpilibj.ADIS16470_IMU;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj2.command.Command;

public class Auto extends JankyStateMachine {
    public Arm autoArm;
    public Intake autoIntake;
    
    //motors, motor controller groups
    private WPI_TalonFX leftLeader, rightLeader, leftFollower, rightFollower;
    private MotorControllerGroup m_left, m_right;
    DifferentialDrive m_myRobot;

    //shuffleboard
    public static int delay, path;
    public int stateMachineSelected;

    //gyro
    public ADIS16470_IMU gyroClassLevel;
    public double motorEncoderAverage, newEncoderValue, gyroAngle;
    public double engageMinAngle, engageMaxAngle;

    Timer delayTimer = new Timer();

    //state IDs
    //simple path (SIMPLE)
    private final int SIMPLE_DELAY = 0, SIMPLE_MOVE = 1, SIMPLE_FINISH_AUTO = 2;

    //one cube (OC)
    private final int OC_ARM_MOVE = 0, OC_SHOOT = 1, OC_DELAY = 2, OC_MOVE = 3, OC_FINISH_AUTO = 4;

    //charge station (CS)
    private final int ARM_TO_SHOOT = 0, CS_SHOOT = 1, CROSS_RAMP = 4, CS_REVERSE_LOWER_ARM = 5, UP_RAMP = 6, IDLE = 8, GO_BACK = 9, GO_FRONT = 10;

    public Auto(int _delay, int _path, ADIS16470_IMU m_gyro, Arm arm, Intake intake) {
        delay = _delay;
        path = _path;
        gyroClassLevel = m_gyro;
        gyroAngle = m_gyro.getYComplementaryAngle();

        autoArm = arm;
        autoIntake = intake;

        engageMinAngle = -9;
        engageMaxAngle = 9;

        //defining motors
        leftLeader = new WPI_TalonFX(4);
        rightLeader = new WPI_TalonFX(5);
        rightFollower = new WPI_TalonFX(7);
        leftFollower = new WPI_TalonFX(6);

        //run motor config method
        setUpChassisMotors(leftLeader);
        setUpChassisMotors(rightLeader);
        setUpChassisMotors(leftFollower);
        setUpChassisMotors(rightFollower);

        //defining motor controller groups + differential drive
        m_left = new MotorControllerGroup(leftLeader,leftFollower);
        m_right = new MotorControllerGroup(rightLeader,rightFollower);
        m_myRobot = new DifferentialDrive(m_left,m_right);

        //reset encoders/gyro
        rightLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
        leftLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
        m_gyro.reset();

        //state machine set up
        if (path == Constants.Auto.ONE_CUBE_AUTOPATH) {
            SetMachineName("oneCubeAuto");
            SetName(OC_ARM_MOVE, "ocArmMove");
            SetName(OC_SHOOT, "ocShoot");
            SetName(OC_DELAY, "ocDelay");
            SetName(OC_MOVE, "ocMove");
            SetName(OC_FINISH_AUTO, "ocFinishAuto");
            stateMachineSelected = Constants.Auto.ONE_CUBE_AUTOPATH;
            start();

        } else if (path == Constants.Auto.SIMPLE_AUTOPATH) { //leaving tarmac path
            SetMachineName ("simpleAuto");
            SetName (SIMPLE_DELAY, "simpleDelay");
            SetName(SIMPLE_MOVE, "simpleMove");
            SetName (SIMPLE_FINISH_AUTO, "simpleFinishAuto");
            stateMachineSelected = Constants.Auto.SIMPLE_AUTOPATH;
            start();

        } else if (path == Constants.Auto.CHARGE_STATION) {
            SetMachineName ("chargeStation");
            SetName (ARM_TO_SHOOT, "armToShoot");
            SetName(CS_SHOOT, "csShoot");
            SetName (CROSS_RAMP, "crossRamp");
            SetName (CS_REVERSE_LOWER_ARM, "csReverseLowerArm");
            SetName (UP_RAMP, "upRamp");
            SetName(IDLE, "noMove");
            SetName (GO_BACK, "goBack");
            SetName (GO_FRONT, "goFront");
            stateMachineSelected = Constants.Auto.CHARGE_STATION;
            start();
        }
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

    /**
     * Configure falcon motor settings, falcon encoder, and PID constants
     * @param falcon - motor to configure
     */
    public void setUpChassisMotors(WPI_TalonFX falcon) {
        falcon.configFactoryDefault();

        falcon.config_kP(0, Constants.Auto.VELOCITY_KP, 75);
        falcon.config_kI(0, Constants.Auto.VELOCITY_KI, 75);
        falcon.config_kD(0, Constants.Auto.VELOCITY_KD, 75);
        falcon.config_kF(0, Constants.Auto.VELOCITY_KF, 75);

        falcon.setNeutralMode(NeutralMode.Coast);
        falcon.getSensorCollection().setIntegratedSensorPosition(0,10);
    }
    
    /*
     * Puts current auto state on SmartDashboard
     */
    public void displayCurrentState() {

        SmartDashboard.putNumber("Auto current state", GetCurrentState()); //displays auto current state
    }

    public void endStateMachine() {
        System.out.println("Auto SM terminating");
        Terminate();
        System.out.println("Auto SM terminated");
    }

    /**
     * Converts inches to encoder value using wheel circumference and chassis gear ratio
     * @param inches - inches to convert
     * @return encoder value
    */
    public double inchesToEncoder(int inches) {
        newEncoderValue = inches / Constants.Auto.WHEEL_CIRCUMFERENCE * Constants.Auto.CHASSIS_GEAR_RATIO * Constants.FALCON_TICKS_PER_REVOLUTION;
        return newEncoderValue;
    }

    public double getAverageEncoderValues() {
        double rightLeaderEncoder = rightLeader.getSensorCollection().getIntegratedSensorPosition();
        double leftLeaderEncoder = leftLeader.getSensorCollection().getIntegratedSensorPosition();
        motorEncoderAverage = (Math.abs(rightLeaderEncoder) + Math.abs(leftLeaderEncoder)) / 2;
        return motorEncoderAverage;
    }

    public boolean isTimerReached(int delayVal) {
        if (delayTimer.get() <= delayVal) {
            m_myRobot.tankDrive(0, 0);
        } else {
            return true;
        }
        return false;
    }
    
    public void StateEngine(int curState, boolean onStateEntered) {
        /**
         * ONE CUBE PATH
         * move arm to front top, run shooter, move out of community zone
         */
        if (stateMachineSelected == Constants.Auto.ONE_CUBE_AUTOPATH) {
            switch (curState) {
                case OC_ARM_MOVE:
                    if (onStateEntered) {
                        autoArm.setDesiredPosition(Constants.Arm.fTOP_ANGLE); 
                    } else {
                        if (autoArm.GetCurrentState() == 2) {
                            NewState(OC_SHOOT, "arm in position");
                        }
                    }
                    break;

                case OC_SHOOT:
                    if (onStateEntered) {
                        autoIntake.runAutoShooter();
                    } else {
                        if (autoIntake.isShooterComplete()) {
                            NewState(OC_DELAY, "shooting done!");
                        }
                    }
                    break;

                case OC_DELAY:
                    if (onStateEntered) {
                        delayTimer.start();
                        System.out.println("start timer");
                    }
                    if (isTimerReached(delay)) {
                        System.out.println("delay timer end");
                        NewState(OC_MOVE, "delay timer has ended");
                    }
                    break;

                case OC_MOVE:
                    m_myRobot.tankDrive(0.4, -0.4); 
                    if (inchesToEncoder(30) <= getAverageEncoderValues()) {
                        m_myRobot.tankDrive(0, 0);
                        NewState(OC_FINISH_AUTO, "reached average encoder for distance");
                    }
                    break;

                case OC_FINISH_AUTO:
                    Terminate();
                    break;
            }

        /**
         * SIMPLE PATH
         * delay for selected amount of time, move out of community zone
         */
        } else if (stateMachineSelected == Constants.Auto.SIMPLE_AUTOPATH) { //leaving tarmac path
            switch (curState) {
                case SIMPLE_DELAY:
                    if (onStateEntered) {
                        delayTimer.start();
                    }
                    if (isTimerReached(delay)) {
                        NewState(SIMPLE_MOVE, "delay timer has ended");
                    }
                    break;

                case SIMPLE_MOVE:
                    m_myRobot.tankDrive(-0.4, 0.4); //going front
                    if (inchesToEncoder(30) <= getAverageEncoderValues()) {
                        m_myRobot.tankDrive(0, 0);
                        NewState(SIMPLE_FINISH_AUTO, "reached average encoder for distance");
                    }
                    break;

                case SIMPLE_FINISH_AUTO:
                    Terminate();
                    break;
            }

        /**
         * CHARGE STATION PATH
         * move arm to front top, shoot, cross the ramp, lower arm, go up the ramp, engage
         */
        } else if (stateMachineSelected == Constants.Auto.CHARGE_STATION) {
            switch(curState) {
                case ARM_TO_SHOOT:
                    if (onStateEntered) {
                        autoArm.setDesiredPosition(Constants.Arm.fTOP_ANGLE); 
                    } else {
                        if (autoArm.GetCurrentState() == 2) {
                            NewState(CS_SHOOT, "arm in position");
                        }
                    }
                    break;

                case CS_SHOOT:
                    if (onStateEntered) {
                        autoIntake.runAutoShooter();
                    } else {
                        if (autoIntake.isShooterComplete()) {
                            NewState(CROSS_RAMP, "shooting done!");
                        }
                    }
                    break;

                case CROSS_RAMP:
                    leftLeader.set(TalonFXControlMode.Velocity, 6000);
                    rightLeader.set(TalonFXControlMode.Velocity, -6000);
                    leftFollower.set(TalonFXControlMode.Follower, 4);
                    rightFollower.set(TalonFXControlMode.Follower, 5);

                    if (inchesToEncoder(220) <= getAverageEncoderValues()) { 
                        m_myRobot.tankDrive(0, 0);
                        NewState(CS_REVERSE_LOWER_ARM, "crossed community");
                    }   
                    break;

                case CS_REVERSE_LOWER_ARM:
                    if (onStateEntered) {
                        leftLeader.setNeutralMode(NeutralMode.Brake);
                        rightLeader.setNeutralMode(NeutralMode.Brake);
                        autoArm.setDesiredPosition(Constants.Arm.INTAKE_ANGLE); 
                    } else {
                        if (autoArm.GetCurrentState() == 2) {
                            NewState(UP_RAMP, "arm in position");
                        }
                    }
                    break;

                case UP_RAMP:
                    if (onStateEntered) {
                        leftLeader.setNeutralMode(NeutralMode.Coast);
                        rightLeader.setNeutralMode(NeutralMode.Coast);
                    }
                    
                    leftLeader.set(TalonFXControlMode.Velocity, -5000);
                    rightLeader.set(TalonFXControlMode.Velocity, 5000);

                    leftFollower.set(TalonFXControlMode.Follower, 4);
                    rightFollower.set(TalonFXControlMode.Follower, 5);
                    
                    if (inchesToEncoder(140) >= getAverageEncoderValues()) { //170
                        m_myRobot.tankDrive(0, 0);
                        NewState(IDLE, "crossed community");
                    }
                    break;

                case IDLE: //don't move
                    leftLeader.setNeutralMode(NeutralMode.Brake);
                    rightLeader.setNeutralMode(NeutralMode.Brake);
                
                    leftLeader.set(TalonFXControlMode.Velocity, 0);
                    rightLeader.set(TalonFXControlMode.Velocity, 0); 
                    leftFollower.set(TalonFXControlMode.Follower, 4);
                    rightFollower.set(TalonFXControlMode.Follower, 5);
                    System.out.println("not moving");
                    
                    if (gyroClassLevel.getYComplementaryAngle() < engageMinAngle) {
                        NewState(GO_FRONT, "need to move forward");
                    } else if (gyroClassLevel.getYComplementaryAngle() > engageMaxAngle) {
                        NewState(GO_BACK, "need to move back"); 
                    }
                    break;

                case GO_FRONT: //forward
                    leftLeader.set(TalonFXControlMode.Velocity, 1000); 
                    rightLeader.set(TalonFXControlMode.Velocity, -1000); //used to be 1000
                    leftFollower.set(TalonFXControlMode.Follower, 4);
                    rightFollower.set(TalonFXControlMode.Follower, 5);
                    System.out.println("moving forward");
                    
                    if (gyroClassLevel.getYComplementaryAngle() > engageMaxAngle) {
                        NewState(GO_BACK, "need to move back");
                    } else if (gyroClassLevel.getYComplementaryAngle() < engageMaxAngle && gyroClassLevel.getYComplementaryAngle() > engageMinAngle){
                        NewState(IDLE, "fine!");
                    }
                    break;
                    
                case GO_BACK: //backwards
                    leftLeader.set(TalonFXControlMode.Velocity, -1000); 
                    rightLeader.set(TalonFXControlMode.Velocity, 1000); 
                    leftFollower.set(TalonFXControlMode.Follower, 4);
                    rightFollower.set(TalonFXControlMode.Follower, 5);
                    System.out.println("moving backward");
                    
                    if (gyroClassLevel.getYComplementaryAngle() < engageMinAngle) {
                        NewState(GO_FRONT, "need to move forward");
                    } else if (gyroClassLevel.getYComplementaryAngle() < engageMaxAngle && gyroClassLevel.getYComplementaryAngle() > engageMinAngle){
                        NewState(IDLE, "fine!");
                    }
                    break;
            }
        }
    }
}   
    