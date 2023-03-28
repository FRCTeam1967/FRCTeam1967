//Auto to do list
// 1. add back delays for one cube and simple
// 2. making tolerance + ramp speeds constants easier to troubleshoot


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
    public double motorEncoderAverage, newEncoderValue, gyroAnglePitch, gyroAngleYaw;

    Timer delayTimer = new Timer();

    //state IDs
    //simple path (SIMPLE)
    private final int SIMPLE_DELAY = 0, SIMPLE_MOVE = 1, SIMPLE_FINISH_AUTO = 2;

    //one cube (OC)
    private final int OC_ARM_MOVE = 0, OC_SHOOT = 1, OC_DELAY = 2, OC_MOVE = 3, OC_FINISH_AUTO = 4;

    //charge station (CS)
    private final int CS_ARM_TO_SHOOT = 0, CS_SHOOT = 1, CS_CROSS_RAMP = 4, CS_REVERSE_LOWER_ARM = 5, CS_UP_RAMP = 6, CS_IDLE = 8, CS_GO_BACK = 9, CS_GO_FRONT = 10;

    //deluxe charge station (with intake 1 cube) (DCS)
    private final int DCS_ARM_TO_SHOOT = 0, DCS_SHOOT = 1, DCS_CROSS_RAMP = 4, DCS_TURN_TO_CUBE = 5, DCS_INTAKE_DOWN = 6, DCS_INTAKE_CUBE = 7, DCS_GO_TO_CS = 8, DCS_RAISE_INTAKE = 9, DCS_TURN_TO_CS = 10, DCS_REVERSE_LOWER_ARM = 11, DCS_UP_RAMP = 12, DCS_IDLE = 13, DCS_GO_BACK = 14, DCS_GO_FRONT = 15;
    
    public Auto(int _delay, int _path, ADIS16470_IMU m_gyro, Arm arm, Intake intake, LED led) {
        delay = _delay;
        path = _path;
        gyroClassLevel = m_gyro;
        gyroAnglePitch = m_gyro.getYComplementaryAngle();
        gyroAngleYaw = m_gyro.getAngle();

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
            SetName (CS_ARM_TO_SHOOT, "cs armToShoot");
            SetName(CS_SHOOT, "cs csShoot");
            SetName (CS_CROSS_RAMP, "cs crossRamp");
            SetName (CS_REVERSE_LOWER_ARM, "cs csReverseLowerArm");
            SetName (CS_UP_RAMP, "cs upRamp");
            SetName(CS_IDLE, "cs noMove");
            SetName (CS_GO_BACK, "cs goBack");
            SetName (CS_GO_FRONT, "cs goFront");
            stateMachineSelected = Constants.Auto.CHARGE_STATION;
            start();
            
        } else if (path == Constants.Auto.DELUXE_CHARGE_STATION) {
            SetMachineName ("dcs");
            SetName (DCS_ARM_TO_SHOOT, "dcs armToShoot");
            SetName(DCS_SHOOT, "dcs Shoot");
            SetName (DCS_CROSS_RAMP, "dcs crossRamp");
            SetName (DCS_TURN_TO_CUBE, "dcs turnToCube");
            SetName (DCS_INTAKE_DOWN, "dcs intakeDown");
            SetName (DCS_INTAKE_CUBE, "dcs turnedToCube");
            SetName (DCS_GO_TO_CS, "dcs goToCS");
            SetName (DCS_RAISE_INTAKE, "dcs raiseIntake");
            SetName (DCS_TURN_TO_CS, "dcs turnToCS");
            SetName (DCS_REVERSE_LOWER_ARM, "dcs csReverseLowerArm");
            SetName (DCS_UP_RAMP, "dcs upRamp");
            SetName(DCS_IDLE, "dcs noMove");
            SetName (DCS_GO_BACK, "dcs goBack");
            SetName (DCS_GO_FRONT, "dcs goFront");
            stateMachineSelected = Constants.Auto.DELUXE_CHARGE_STATION;
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
                        NewState(SIMPLE_FINISH_AUTO, "crossed community");
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
                case CS_ARM_TO_SHOOT:
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
                            NewState(CS_CROSS_RAMP, "shooting done!");
                        }
                    }
                    break;

                case CS_CROSS_RAMP:
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
                            NewState(CS_UP_RAMP, "arm in position");
                        }
                    }
                    break;

                case CS_UP_RAMP:
                    if (onStateEntered) {
                        leftLeader.setNeutralMode(NeutralMode.Coast);
                        rightLeader.setNeutralMode(NeutralMode.Coast);
                    }
                    
                    leftLeader.set(TalonFXControlMode.Velocity, 2000);
                    rightLeader.set(TalonFXControlMode.Velocity, -2000);
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                    
                    if (inchesToEncoder(160) >= getAverageEncoderValues()) { 
                        leftLeader.set(TalonFXControlMode.Velocity, 0);
                        rightLeader.set(TalonFXControlMode.Velocity, 0);
                        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                        NewState(CS_IDLE, "crossed community");
                    }
                    break;

                case CS_IDLE: //don't move
                    leftLeader.setNeutralMode(NeutralMode.Brake);
                    rightLeader.setNeutralMode(NeutralMode.Brake);
                
                    leftLeader.set(TalonFXControlMode.Velocity, 0);
                    rightLeader.set(TalonFXControlMode.Velocity, 0); 
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);

                    autoLED.setChasingColors(Color.kGreen, Color.kBlack, 10, 0.005);
                    
                    if (gyroAnglePitch < Constants.Auto.MIN_ANGLE) {
                        NewState(CS_GO_FRONT, "need to move forward");
                    } else if (gyroClassLevel.getYComplementaryAngle() > Constants.Auto.MAX_ANGLE) {
                        NewState(CS_GO_BACK, "need to move back"); 
                    }
                    break;

                case CS_GO_FRONT: //forward
                    leftLeader.set(TalonFXControlMode.Velocity, -400); 
                    rightLeader.set(TalonFXControlMode.Velocity, 400); 
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                    
                    if (gyroAnglePitch > Constants.Auto.MAX_ANGLE) {
                        NewState(CS_GO_BACK, "need to move back");
                    } else if (gyroAnglePitch < Constants.Auto.MAX_ANGLE && gyroAnglePitch > Constants.Auto.MIN_ANGLE){
                        NewState(CS_IDLE, "fine!");
                    }
                    break;
                    
                case CS_GO_BACK: //backwards
                    leftLeader.set(TalonFXControlMode.Velocity, 400); 
                    rightLeader.set(TalonFXControlMode.Velocity, -400); 
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                    
                    if (gyroAnglePitch < Constants.Auto.MIN_ANGLE) {
                        NewState(CS_GO_FRONT, "need to move forward");
                    } else if (gyroAnglePitch < Constants.Auto.MAX_ANGLE && gyroAnglePitch > Constants.Auto.MIN_ANGLE){
                        NewState(CS_IDLE, "fine!");
                    }
                    break;
            }
        } else if (stateMachineSelected == Constants.Auto.DELUXE_CHARGE_STATION) {
            switch(curState) {
                case DCS_ARM_TO_SHOOT:
                    if (onStateEntered) {
                        autoArm.setDesiredPosition(Constants.Arm.fTOP_ANGLE); 
                    } else {
                        if (autoArm.GetCurrentState() == 2) {
                            NewState(DCS_SHOOT, "arm in position");
                        }
                    }
                    break;

                case DCS_SHOOT:
                    if (onStateEntered) {
                        autoIntake.runAutoShooter();
                    } else {
                        if (autoIntake.isShooterComplete()) {
                            NewState(DCS_CROSS_RAMP, "shooting done!");
                        }
                    }
                    break;

                case DCS_CROSS_RAMP:
                    leftLeader.set(TalonFXControlMode.Velocity, -2300);
                    rightLeader.set(TalonFXControlMode.Velocity, 2300);
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);

                    if (inchesToEncoder(250) <= getAverageEncoderValues()) { //increased from 230 to 250 for extra turning room
                        leftLeader.set(TalonFXControlMode.Velocity, 0);
                        rightLeader.set(TalonFXControlMode.Velocity, 0);
                        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                        NewState(DCS_TURN_TO_CUBE, "crossed community");
                    }   
                    break;
                
                case DCS_TURN_TO_CUBE:
                    leftLeader.set(TalonFXControlMode.Velocity, -2000);
                    rightLeader.set(TalonFXControlMode.Velocity, -2000);
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);

                    if (gyroAngleYaw >= 130){
                        leftLeader.set(TalonFXControlMode.Velocity, 0);
                        rightLeader.set(TalonFXControlMode.Velocity, 0);
                        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                        NewState(DCS_INTAKE_DOWN, "facing cube");
                    }
                    break;
                    
                case DCS_INTAKE_DOWN:
                    if(onStateEntered){
                        leftLeader.setNeutralMode(NeutralMode.Brake);
                        rightLeader.setNeutralMode(NeutralMode.Brake);
                        autoArm.setDesiredPosition(Constants.Arm.INTAKE_ANGLE);
                    } else {
                        if (autoArm.GetCurrentState() == 2) {
                            NewState(DCS_INTAKE_CUBE, "intake lowered");
                        }

                    }
                    break;

                case DCS_INTAKE_CUBE:
                    if(onStateEntered){
                        rightLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
                        leftLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
                    }
                    autoIntake.runIntake();
                    leftLeader.set(TalonFXControlMode.Velocity, 2300);
                    rightLeader.set(TalonFXControlMode.Velocity, -2300);
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                   
                    if (inchesToEncoder(120) <= getAverageEncoderValues()) {
                        leftLeader.set(TalonFXControlMode.Velocity, 0);
                        rightLeader.set(TalonFXControlMode.Velocity, 0);
                        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                        NewState(DCS_GO_TO_CS, "cube intaked");
                    }   
                    break;
                
                case DCS_GO_TO_CS:
                    if(onStateEntered){
                        rightLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
                        leftLeader.getSensorCollection().setIntegratedSensorPosition(0,10);
                    }
                    leftLeader.set(TalonFXControlMode.Velocity, -2300);
                    rightLeader.set(TalonFXControlMode.Velocity, 2300);
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                   
                    if (inchesToEncoder(120) <= getAverageEncoderValues()) {
                        leftLeader.set(TalonFXControlMode.Velocity, 0);
                        rightLeader.set(TalonFXControlMode.Velocity, 0);
                        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                        NewState (DCS_RAISE_INTAKE, "moved back to charge station");
                    }   
                    break;
                
                case DCS_RAISE_INTAKE:
                    if(onStateEntered){
                        leftLeader.setNeutralMode(NeutralMode.Brake);
                        rightLeader.setNeutralMode(NeutralMode.Brake);
                        autoArm.setDesiredPosition(Constants.Arm.fTOP_ANGLE);
                    }
                    if (autoArm.GetCurrentState() == 2) {
                            NewState(DCS_TURN_TO_CS, "raised intake");
                        }
                    break;
                
                case DCS_TURN_TO_CS:
                    if(onStateEntered){
                        leftLeader.set(TalonFXControlMode.Velocity, 2000);
                        rightLeader.set(TalonFXControlMode.Velocity, 2000);
                        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                    }
                    if (gyroAngleYaw >= 130){
                        leftLeader.set(TalonFXControlMode.Velocity, 0);
                        rightLeader.set(TalonFXControlMode.Velocity, 0);
                        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                        NewState(DCS_REVERSE_LOWER_ARM, "turned to cs");
                        }
                    break;
                    
                case DCS_REVERSE_LOWER_ARM:
                    if (onStateEntered) {
                        leftLeader.setNeutralMode(NeutralMode.Brake);
                        rightLeader.setNeutralMode(NeutralMode.Brake);
                        autoArm.setDesiredPosition(Constants.Arm.INTAKE_ANGLE); 
                    } else {
                        if (autoArm.GetCurrentState() == 2) {
                            NewState(DCS_UP_RAMP, "arm in position");
                        }
                    }
                    break;

                case DCS_UP_RAMP:
                    if (onStateEntered) {
                        leftLeader.setNeutralMode(NeutralMode.Coast);
                        rightLeader.setNeutralMode(NeutralMode.Coast);
                    }
                    
                    leftLeader.set(TalonFXControlMode.Velocity, 2000);
                    rightLeader.set(TalonFXControlMode.Velocity, -2000);
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                    
                    if (inchesToEncoder(160) >= getAverageEncoderValues()) { 
                        leftLeader.set(TalonFXControlMode.Velocity, 0);
                        rightLeader.set(TalonFXControlMode.Velocity, 0);
                        leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                        rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                        NewState(DCS_IDLE, "crossed community");
                    }
                    break;

                case DCS_IDLE: //don't move
                    leftLeader.setNeutralMode(NeutralMode.Brake);
                    rightLeader.setNeutralMode(NeutralMode.Brake);
                
                    leftLeader.set(TalonFXControlMode.Velocity, 0);
                    rightLeader.set(TalonFXControlMode.Velocity, 0); 
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);

                    autoLED.setChasingColors(Color.kGreen, Color.kBlack, 10, 0.005);
                    
                    if (gyroClassLevel.getYComplementaryAngle() < Constants.Auto.MIN_ANGLE) {
                        NewState(DCS_GO_FRONT, "need to move forward");
                    } else if (gyroClassLevel.getYComplementaryAngle() > Constants.Auto.MAX_ANGLE) {
                        NewState(DCS_GO_BACK, "need to move back"); 
                    }
                    break;

                case DCS_GO_FRONT: //forward
                    leftLeader.set(TalonFXControlMode.Velocity, -400); 
                    rightLeader.set(TalonFXControlMode.Velocity, 400); 
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                    
                    if (gyroClassLevel.getYComplementaryAngle() > Constants.Auto.MAX_ANGLE) {
                        NewState(DCS_GO_BACK, "need to move back");
                    } else if (gyroClassLevel.getYComplementaryAngle() < Constants.Auto.MAX_ANGLE && gyroClassLevel.getYComplementaryAngle() > Constants.Auto.MIN_ANGLE){
                        NewState(CS_IDLE, "fine!");
                    }
                    break;
                    
                case DCS_GO_BACK: //backwards
                    leftLeader.set(TalonFXControlMode.Velocity, 400); 
                    rightLeader.set(TalonFXControlMode.Velocity, -400); 
                    leftFollower.set(TalonFXControlMode.Follower, Constants.Chassis.LEFT_LEADER_ID);
                    rightFollower.set(TalonFXControlMode.Follower, Constants.Chassis.RIGHT_LEADER_ID);
                    
                    if (gyroClassLevel.getYComplementaryAngle() < Constants.Auto.MIN_ANGLE) {
                        NewState(DCS_GO_FRONT, "need to move forward");
                    } else if (gyroClassLevel.getYComplementaryAngle() < Constants.Auto.MAX_ANGLE && gyroClassLevel.getYComplementaryAngle() > Constants.Auto.MIN_ANGLE){
                        NewState(DCS_IDLE, "fine!");
                    }
                    break;
            }
        }
    }
}   
    