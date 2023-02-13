package frc.robot;

import org.janksters.jankyLib.JankyStateMachine;
import org.janksters.jankyLib.jankyXboxJoystick;

import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.Timer;

import edu.wpi.first.wpilibj.motorcontrol.MotorController;
import com.revrobotics.CANSparkMax;
import com.revrobotics.CANSparkMaxLowLevel.MotorType;


public class Intake extends JankyStateMachine {
    private static final int Idle = 0;
    private static final int Intake = 1;
    private static final int Shoot = 2; 

    private jankyXboxJoystick XboxController; 

    private CANSparkMax topRollerMotor;
    private CANSparkMax bottomRollerMotor;

    private static final int topRollerMotorID = 777; //tbd
    private static final int bottomRollerMotorID = 777; //tbd

    private Timer idleTimer;

    private static double INTAKE_BOTTOM_ROLLER_SPEED = 777; //tbd
    private static double INTAKE_TOP_ROLLER_SPEED = 777; //tbd
    private static double SHOOT_BOTTOM_ROLLER_SPEED = 777; //tbd
    private static double SHOOT_TOP_ROLLER_SPEED = 777; //tbd

    private static final double IDLE_TIMER = 2; //seconds

    private boolean shootCompleteFlag = false; 

    public Intake(){
        XboxController = new jankyXboxJoystick(777); //tbd
        topRollerMotor = new CANSparkMax(topRollerMotorID, MotorType.kBrushless);
        bottomRollerMotor = new CANSparkMax(bottomRollerMotorID, MotorType.kBrushless);
        topRollerMotor.restoreFactoryDefaults();
        bottomRollerMotor.restoreFactoryDefaults();

        idleTimer = new Timer();

        SetMachineName("Intake");
        SetName(Idle, "Idle");
        SetName(Intake, "Intake");
        SetName(Shoot, "Shoot");

        start();
    }
    
    public void StateEngine(int curState, boolean onStateEntered){
        switch(curState){
            case Idle:
                zeroMotors();
                //optimize the conditions
                if ((XboxController.GetLeftThrottle() == 1) && (XboxController.GetRightThrottle() == 0)){
                    NewState(Intake, "Going to intake because only left throttle pressed");
                }
                else if (XboxController.GetRightThrottle() == 1){
                    NewState(Shoot, "Going to shoot because only right throttle pressed");
                }
                break;

            case Intake:
                runIntake();
                if ((XboxController.GetLeftThrottle() == 0) && (XboxController.GetRightThrottle() == 0)){
                    NewState(Idle, "Going to idle because throttles released");
                }
                break;
            case Shoot:
                runShooter();
                if (onStateEntered){
                    idleTimer.reset();
                    idleTimer.start();
                    shootCompleteFlag = false;
                }
                if (idleTimer.get() >= IDLE_TIMER){
                    idleTimer.stop();
                    shootCompleteFlag = true;
                    NewState(Idle, "Timer exceeded "+IDLE_TIMER+" seconds");
                }
                break;
        }

    }

    public void zeroMotors(){
        topRollerMotor.set(0.0);
        bottomRollerMotor.set(0.0);
    }

    public void runIntake(){
        topRollerMotor.set(INTAKE_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(INTAKE_BOTTOM_ROLLER_SPEED);
    }

    public void runShooter(){
        topRollerMotor.set(SHOOT_TOP_ROLLER_SPEED*-1);
        bottomRollerMotor.set(SHOOT_BOTTOM_ROLLER_SPEED*-1);
    }

    public boolean getShootCompleteFlag(){
        return shootCompleteFlag;
    }
}