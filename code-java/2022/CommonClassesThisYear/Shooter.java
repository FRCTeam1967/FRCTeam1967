package org.janksters.CommonClassesThisYear;

import org.janksters.jankyLib.JankyStateMachine;
import org.janksters.jankyLib.jankyXboxJoystick;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableInstance;

import com.revrobotics.CANSparkMax;
import com.revrobotics.CANSparkMaxLowLevel.MotorType;
import com.revrobotics.SparkMaxPIDController;
import com.revrobotics.RelativeEncoder;

import edu.wpi.first.wpilibj.Timer;

//8 values are TBD 

public class Shooter extends JankyStateMachine {

    // Declaring states
    private final int Idle = 0;
    private final int Intake = 1;
    private final int SlowEject = 2;
    private final int RevUp = 3;
    private final int Fire = 4;

    private jankyXboxJoystick XboxController;

    private NetworkTable table;

    private CANSparkMax topRollerMotor;
    private SparkMaxPIDController topPIDController;
    public RelativeEncoder topMotorEncoder;

    private CANSparkMax bottomRollerMotor;
    private SparkMaxPIDController bottomPIDController;
    public RelativeEncoder bottomMotorEncoder;

    private static final int topRollerMotorID = 20; 
    private static final int bottomRollerMotorID = 21;
    
    public double kP, kI, kD, kIz, kFF, kMaxOutput, kMinOutput, maxRPM;

    private Timer fireTimer;
    private Timer revTimer;
    private Timer slowEjectTimer;

    // Remember, velocity values are between -1.0 and 1.0 for motor.set
    // PID controller set reference takes RPM values, max 5700
    private static final double INTAKE_ROLLER_SPEED = 1200;
    private static final double SLOW_EJECT_SPEED = 0.2;
    private static final double TOP_ROLLER_FIRE_SPEED = 2000; 
    private static final double BOTTOM_ROLLER_FIRE_SPEED = 2000;

    private boolean revUpFlag = false;
    private boolean fireCompleteFlag = false;

    public Shooter() {
        XboxController = new jankyXboxJoystick(2); 
        table = NetworkTableInstance.getDefault().getTable("limelight");

        topRollerMotor = new CANSparkMax(topRollerMotorID, MotorType.kBrushless);
        bottomRollerMotor = new CANSparkMax(bottomRollerMotorID, MotorType.kBrushless);
        topRollerMotor.restoreFactoryDefaults();
        bottomRollerMotor.restoreFactoryDefaults();

        topPIDController = topRollerMotor.getPIDController();
        bottomPIDController = bottomRollerMotor.getPIDController();
        topMotorEncoder = topRollerMotor.getEncoder();
        bottomMotorEncoder = bottomRollerMotor.getEncoder();

        fireTimer = new Timer();
        revTimer = new Timer();
        slowEjectTimer = new Timer();

        kP = 0.00015;
        kI = 0;
        kD = 0;
        kIz = 0;
        kFF = 0.000178;
        kMaxOutput = 1;
        kMinOutput = -1;
        maxRPM = 5700;

        topPIDController.setP(kP);
        topPIDController.setI(kI);
        topPIDController.setD(kD);
        topPIDController.setIZone(kIz);
        topPIDController.setFF(kFF);
        topPIDController.setOutputRange(kMinOutput, kMaxOutput);

        bottomPIDController.setP(kP);
        bottomPIDController.setI(kI);
        bottomPIDController.setD(kD);
        bottomPIDController.setIZone(kIz);
        bottomPIDController.setFF(kFF);
        bottomPIDController.setOutputRange(kMinOutput, kMaxOutput);

        SetMachineName("Shooter");
        SetName(Idle, "Idle");
        SetName(Intake, "Intake");
        SetName(SlowEject, "SlowEject");
        SetName(RevUp, "RevUp");
        SetName(Fire, "Fire");

        start();
    }

    public void StateEngine(int curState, boolean onStateEntered) {
        switch (curState) {
            case Idle:
                zeroMotors();
                if ((XboxController.GetLeftThrottle() == 1) && (XboxController.GetRightThrottle() == 1)) {
                    NewState(Intake, "pressed left and right throttle buttons");
                }
                if ((XboxController.GetButtonLB() == true) && (XboxController.GetButtonRB() == true)) {
                    NewState(RevUp, "LB and RB pressed");
                }
                if(revUpFlag){
                    NewState(RevUp, "revUpFlag is true");
                    revUpFlag = false;
                }
                //if (XboxController.GetLeftStickButton() == true) {
                if (XboxController.GetLeftYAxis() >= 0.2) {
                    NewState(SlowEject, "left joystick up");
                }
                break;
            case Intake:
                runIntake();
                if ((XboxController.GetLeftThrottle() == 0) || (XboxController.GetRightThrottle() == 0)) {
                    NewState(Idle, "released left and right throttle buttons");
                }
                break;
            case SlowEject:
                if (onStateEntered){
                    slowEjectTimer.reset();
                    slowEjectTimer.start();
                }
                runSlowEject();
                if (slowEjectTimer.get() >= 0.1){
                    slowEjectTimer.stop();
                    NewState(Idle, "slow eject timer reached");
                }
                break;
            case RevUp:
                setBottomVelocity();
                if (bottomMotorEncoder.getVelocity() >= 1800){
                    NewState(Fire, "finished bottom roller running");
                }
                break;
            case Fire:
                if (onStateEntered) {
                    fireTimer.reset();
                    fireTimer.start();
                    fireCompleteFlag = false; //set to false somewhere else?
                }
                setTopVelocity();
                if (fireTimer.get() >= .5) {
                    fireTimer.stop();
                    fireCompleteFlag = true;
                    NewState(Idle, "finished firing sequence");
                }
                break;
        }
    }

    // ACTION METHODS (ROLLERS, MOTORS, ETC)
    public void zeroMotors() {
        topRollerMotor.set(0.0);
        bottomRollerMotor.set(0.0);
    }

    public void runIntake() {
        // TODO: check diagram, test directions
        // Top roller goes out, bottom goes in
        // topRollerMotor.set(INTAKE_ROLLER_SPEED);
        // bottomRollerMotor.set(-INTAKE_ROLLER_SPEED);
        topPIDController.setReference(INTAKE_ROLLER_SPEED, CANSparkMax.ControlType.kVelocity);
        bottomPIDController.setReference(-INTAKE_ROLLER_SPEED, CANSparkMax.ControlType.kVelocity);
    }

    public void runSlowEject() {
        // TODO: test speed for slow eject
        // Top roller goes in, bottom goes out
        topRollerMotor.set(-SLOW_EJECT_SPEED);
        bottomRollerMotor.set(SLOW_EJECT_SPEED);
    }

    public double calcVelocity() {
        // Calculate distance from hub (Limelight)
        double ta = table.getEntry("ta").getDouble(0);

        //TODO: test and derive new equation
        double distanceFromHub = 156.15637194655 - (36.746837632824 * Math.log(ta));
        SmartDashboard.putNumber("Distance From Hub", distanceFromHub);

        // Calculate velocity using distance
        // TBD, taken from Luca's code, TODO: plot points, derive new equation
        double desiredRPM = (5.4545 * distanceFromHub) + 4000;
        double targetVelocity = desiredRPM * 2048 / 600; // Units per 100 m/s

        return targetVelocity;
    }

    public void setBottomVelocity() {
        // "Revving" rollers with PID
        bottomPIDController.setReference(BOTTOM_ROLLER_FIRE_SPEED, CANSparkMax.ControlType.kVelocity);
    }

    public void setTopVelocity() {
        // Second set of rollers
        topPIDController.setReference(-TOP_ROLLER_FIRE_SPEED, CANSparkMax.ControlType.kVelocity);
    }

    public void displayCurrentState() {
        SmartDashboard.putNumber("current state", GetCurrentState());
    }

    //auto calls this to take shooter to rev up state
    public void shooterRevUp(){
        revUpFlag = true;
    }

    public boolean fireComplete(){
        return fireCompleteFlag;
    }

}