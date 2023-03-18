package frc.robot;

import org.janksters.jankyLib.JankyStateMachine;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

import edu.wpi.first.wpilibj.ADIS16470_IMU;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.util.Color;

public class Auto extends JankyStateMachine {
    public Arm autoArm;
    public Intake autoIntake;
    public LED autoLED;
    
    //motors
    private WPI_TalonFX leftLeader, rightLeader, leftFollower, rightFollower;

    //shuffleboard
    public static int delay, path;
    public int stateMachineSelected;

    //gyro
    public ADIS16470_IMU gyroClassLevel;
    public double motorEncoderAverage, newEncoderValue, gyroAngle;

    Timer delayTimer = new Timer();

    //state IDs
    //simple path (SIMPLE)
    private final int SIMPLE_DELAY = 0, SIMPLE_MOVE = 1, SIMPLE_FINISH_AUTO = 2;

    //one cube (OC)
    private final int OC_ARM_MOVE = 0, OC_SHOOT = 1, OC_DELAY = 2, OC_MOVE = 3, OC_FINISH_AUTO = 4;

    //charge station (CS)
    private final int ARM_TO_SHOOT = 0, CS_SHOOT = 1, CROSS_RAMP = 4, CS_REVERSE_LOWER_ARM = 5, UP_RAMP = 6, IDLE = 8, GO_BACK = 9, GO_FRONT = 10;

    public Auto(int _delay, int _path, ADIS16470_IMU m_gyro, Arm arm, Intake intake, LED led) {
        delay = _delay;
        path = _path;
        gyroClassLevel = m_gyro;
        gyroAngle = m_gyro.getYComplementaryAngle();

        autoArm = arm;
        autoIntake = intake;
        autoLED = led;

        //defining motors
        leftLeader = new WPI_TalonFX(Constants.Chassis.LEFT_LEADER_ID);
        leftFollower = new WPI_TalonFX(Constants.Chassis.LEFT_FOLLOWER_ID);
        rightLeader = new WPI_TalonFX(Constants.Chassis.RIGHT_LEADER_ID);
        rightFollower = new WPI_TalonFX(Constants.Chassis.RIGHT_FOLLOWER_ID);

        //run motor config method
        setUpChassisMotors(leftLeader);
        setUpChassisMotors(rightLeader);
        setUpChassisMotors(leftFollower);
        setUpChassisMotors(rightFollower);

        //reset encoders + gyro
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

        } else if (path == Constants.Auto.SIMPLE_AUTOPATH) { 
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
    
   //TIMER HELPER METHODS
    public void resetDelayTimer(){
        delayTimer.reset();
    }

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

    public boolean isTimerReached(int delayVal) {
        if (delayTimer.get() <= delayVal) {
            leftLeader.set(TalonFXControlMode.PercentOutput, 0);
            rightLeader.set(TalonFXControlMode.PercentOutput, 0);
            leftFollower.set(TalonFXControlMode.Follower, 4);
            rightFollower.set(TalonFXControlMode.Follower, 5);
        } else {
            return true;
        }
        return false;
    }

    /**
     * Configure falcon motor settings, falcon encoder, and PID constants
     * @param falcon - motor to configure
     */
    public void setUpChassisMotors(WPI_TalonFX falcon) {
        falcon.configFactoryDefault();

        falcon.config_kP(0, Constants.Chassis.kP, 75);
        falcon.config_kI(0, Constants.Chassis.kI, 75);
        falcon.config_kD(0, Constants.Chassis.kD, 75);
        falcon.config_kF(0, Constants.Chassis.kF, 75);

        falcon.setNeutralMode(NeutralMode.Coast);
        falcon.getSensorCollection().setIntegratedSensorPosition(0,10);
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

    public void endStateMachine() {
        System.out.println("Auto SM terminating");
        Terminate();
        System.out.println("Auto SM terminated");
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
                            NewState(OC_MOVE, "shooting done!");
                        }
                    }
                    break;

                case OC_MOVE:
                    leftLeader.set(TalonFXControlMode.Velocity, -2300);
                    rightLeader.set(TalonFXControlMode.Velocity, 2300);
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);

                    if (inchesToEncoder(170) <= getAverageEncoderValues()) { 
                        leftLeader.set(TalonFXControlMode.Velocity, 0);
                        rightLeader.set(TalonFXControlMode.Velocity, 0);
                        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                        NewState(OC_FINISH_AUTO, "crossed community");
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
                    NewState(SIMPLE_MOVE, "delay over");
                    break;
                
                case SIMPLE_MOVE:
                    leftLeader.set(TalonFXControlMode.Velocity, 2300);
                    rightLeader.set(TalonFXControlMode.Velocity, -2300);
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);

                    if (inchesToEncoder(170) <= getAverageEncoderValues()) { 
                        leftLeader.set(TalonFXControlMode.Velocity, 0);
                        rightLeader.set(TalonFXControlMode.Velocity, 0);
                        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                        NewState(CS_REVERSE_LOWER_ARM, "crossed community");
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
                    leftLeader.set(TalonFXControlMode.Velocity, -2300);
                    rightLeader.set(TalonFXControlMode.Velocity, 2300);
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);

                    if (inchesToEncoder(230) <= getAverageEncoderValues()) { 
                        leftLeader.set(TalonFXControlMode.Velocity, 0);
                        rightLeader.set(TalonFXControlMode.Velocity, 0);
                        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
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
                    
                    leftLeader.set(TalonFXControlMode.Velocity, 2000);
                    rightLeader.set(TalonFXControlMode.Velocity, -2000);
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                    
                    if (inchesToEncoder(160) >= getAverageEncoderValues()) { //130
                        leftLeader.set(TalonFXControlMode.Velocity, 0);
                        rightLeader.set(TalonFXControlMode.Velocity, 0);
                        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                        NewState(IDLE, "crossed community");
                    }
                    break;

                case IDLE: //don't move
                    leftLeader.setNeutralMode(NeutralMode.Brake);
                    rightLeader.setNeutralMode(NeutralMode.Brake);
                
                    leftLeader.set(TalonFXControlMode.Velocity, 0);
                    rightLeader.set(TalonFXControlMode.Velocity, 0); 
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);

                    autoLED.setChasingColors(Color.kGreen, Color.kBlack, 10, 0.005);
                    
                    if (gyroClassLevel.getYComplementaryAngle() < Constants.Auto.MIN_ANGLE) {
                        NewState(GO_FRONT, "need to move forward");
                    } else if (gyroClassLevel.getYComplementaryAngle() > Constants.Auto.MAX_ANGLE) {
                        NewState(GO_BACK, "need to move back"); 
                    }
                    break;

                case GO_FRONT: //forward
                    leftLeader.set(TalonFXControlMode.Velocity, -400); 
                    rightLeader.set(TalonFXControlMode.Velocity, 400); //500
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                    
                    if (gyroClassLevel.getYComplementaryAngle() > Constants.Auto.MAX_ANGLE) {
                        NewState(GO_BACK, "need to move back");
                    } else if (gyroClassLevel.getYComplementaryAngle() < Constants.Auto.MAX_ANGLE && gyroClassLevel.getYComplementaryAngle() > Constants.Auto.MIN_ANGLE){
                        NewState(IDLE, "fine!");
                    }
                    break;
                    
                case GO_BACK: //backwards
                    leftLeader.set(TalonFXControlMode.Velocity, 400); 
                    rightLeader.set(TalonFXControlMode.Velocity, -400); 
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                    
                    if (gyroClassLevel.getYComplementaryAngle() < Constants.Auto.MIN_ANGLE) {
                        NewState(GO_FRONT, "need to move forward");
                    } else if (gyroClassLevel.getYComplementaryAngle() < Constants.Auto.MAX_ANGLE && gyroClassLevel.getYComplementaryAngle() > Constants.Auto.MIN_ANGLE){
                        NewState(IDLE, "fine!");
                    }
                    break;
            }
        }
    }
}   
    