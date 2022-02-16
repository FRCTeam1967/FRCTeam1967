package frc.robot;

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
//我爱你, <3 !! ♥️

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
    private RelativeEncoder topMotorEncoder;

    private CANSparkMax bottomRollerMotor;
    private SparkMaxPIDController bottomPIDController;
    private RelativeEncoder bottomMotorEncoder;
    public double kP, kI, kD, kIz, kFF, kMaxOutput, kMinOutput, maxRPM;

    private Timer fireTimer;

    private static final int topRollerMotorID = 14; // TBD
    // private static final int bottomRollerMotorID = 18; 

    private static final int INTAKE_ROLLER_SPEED = 700; // TBD
    private static final int SLOW_EJECT_SPEED = 100; // TBD
    private static final int TOP_ROLLER_FIRE_SPEED = 1000; // TBD

    public Shooter() {

        XboxController = new jankyXboxJoystick(0); // TBD
        table = NetworkTableInstance.getDefault().getTable("limelight");

        topRollerMotor = new CANSparkMax(topRollerMotorID, MotorType.kBrushless);
        // bottomRollerMotor = new CANSparkMax(bottomRollerMotorID, MotorType.kBrushless);
        topRollerMotor.restoreFactoryDefaults();
        // bottomRollerMotor.restoreFactoryDefaults();

        topPIDController = topRollerMotor.getPIDController();
        // bottomPIDController = bottomRollerMotor.getPIDController();
        topMotorEncoder = topRollerMotor.getEncoder();
        // bottomMotorEncoder = bottomRollerMotor.getEncoder();

        fireTimer = new Timer();

        // TBD
        kP = 6e-5;
        kI = 0;
        kD = 0;
        kIz = 0;
        kFF = 0.000015;
        kMaxOutput = 1;
        kMinOutput = 1;
        maxRPM = 5700;

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
                SmartDashboard.putString("State print", "Entered Idle");
                zeroMotors();
                if ((XboxController.GetLeftThrottle() == 1) && (XboxController.GetRightThrottle() == 1)) {
                    NewState(Intake, "pressed left and right throttle buttons");
                }
                if ((XboxController.GetButtonLB() == true) && (XboxController.GetButtonRB() == true)) {
                    NewState(Fire, "LB and RB pressed");
                }
                if (XboxController.GetButtonStart() == true) {
                    NewState(SlowEject, "RB pressed");
                }
                break;
            case Intake:
                runIntake();
                if ((XboxController.GetLeftThrottle() == 0) || (XboxController.GetRightThrottle() == 0)) {
                    NewState(Idle, "released left and right throttle buttons");
                }
                break;
            case SlowEject:
                runSlowEject();
                if (XboxController.GetButtonRB() == false) {
                    NewState(Idle, "RB released");
                }
                break;
            case RevUp:
                double bottomTargetVelocity = calcVelocity();
                setBottomVelocity(bottomTargetVelocity);
                if ((XboxController.GetButtonLB() == false) || (XboxController.GetButtonRB() == false)) {
                    NewState(Idle, "LB and RB released");
                }
                // TODO: unknown range; need to determine
                // if ((getBottomVelocity() > bottomTargetVelocity - 8)
                //        && (getBottomVelocity() < bottomTargetVelocity + 8)) {
                //    NewState(Fire, "bottom rollers reached target velocity");
                //}
                                                    
                break;
            case Fire:
                if(onStateEntered){
                    fireTimer.reset();
                    fireTimer.start();
                }
                setTopVelocity();
                if (fireTimer.get() >= 5){
                    fireTimer.stop();
                    NewState(Idle, "finished firing sequence");
                }
                break;
        }
    }

    // ACTION METHODS (ROLLERS, MOTORS, ETC)
    public void zeroMotors() {
        topRollerMotor.set(0.0);
        // bottomRollerMotor.set(0.0);
    }

    public void runIntake() {
        // TODO: check diagram, test directions
        // Top roller goes out, bottom goes in
        topRollerMotor.set(INTAKE_ROLLER_SPEED);
        // bottomRollerMotor.set(INTAKE_ROLLER_SPEED);
    }

    public void runSlowEject() {
        // TODO: test speed for slow eject
        // Top roller goes in, bottom goes out
        topRollerMotor.set(SLOW_EJECT_SPEED);
        // bottomRollerMotor.set(SLOW_EJECT_SPEED);
    }

    public double calcVelocity() {
        // Calculate distance from hub (Limelight)
        double ta = table.getEntry("ta").getDouble(0);
        // TBD, TODO: test and derive new equation
        double distanceFromHub = 156.15637194655 - (36.746837632824 * Math.log(ta));
        SmartDashboard.putNumber("Distance From Hub", distanceFromHub);

        // Calculate velocity using distance
        // TBD, taken from Luca's code, TODO: plot points, derive new equation
        double desiredRPM = (5.4545 * distanceFromHub) + 4000;
        double targetVelocity = desiredRPM * 2048 / 600; // Units per 100 m/s

        return targetVelocity;
    }

    public void getBottomVelocity() { // revert to double
        // return bottomMotorEncoder.getVelocity();
    }

    public void setBottomVelocity(double targetVelocity) {
        // "Revving" rollers with PID
        // bottomPIDController.setReference(targetVelocity, CANSparkMax.ControlType.kVelocity);
    }

    public void setTopVelocity() {
        topPIDController.setReference(TOP_ROLLER_FIRE_SPEED, CANSparkMax.ControlType.kVelocity);
    }

    public void displayCurrentState(){
        SmartDashboard.putNumber("current state", GetCurrentState());
    }

    
}