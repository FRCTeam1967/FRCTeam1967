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

    private static final int topRollerMotorID = 1; 
    private static final int bottomRollerMotorID = 12; 

    private Timer idleTimer;

    private static double INTAKE_BOTTOM_ROLLER_SPEED = 0.25; //tbd
    private static double INTAKE_TOP_ROLLER_SPEED = 0.25; //tbd
    private static double SHOOT_BOTTOM_ROLLER_SPEED = 0.0; //tbd
    private static double SHOOT_TOP_ROLLER_SPEED = 0.07; //tbd

    private static final double IDLE_TIMER = 3; //seconds

    public Intake(){
        XboxController = new jankyXboxJoystick(1); //tbd
        topRollerMotor = new CANSparkMax(topRollerMotorID, MotorType.kBrushless);
        bottomRollerMotor = new CANSparkMax(bottomRollerMotorID, MotorType.kBrushless);
        topRollerMotor.restoreFactoryDefaults();
        bottomRollerMotor.restoreFactoryDefaults();

        idleTimer = new Timer();

        SetMachineName("IntakeStateMachine");
        SetName(Idle, "Idle");
        SetName(Intake, "Intake");
        SetName(Shoot, "Shoot");


        start();
    }
    
    public void StateEngine(int curState, boolean onStateEntered){
        switch(curState){
            case Idle: 
                setIdle();
                
                if (Math.abs(XboxController.GetLeftThrottle()) > 0.95){
                    NewState(Intake, "Left throttle pressed, enter Intake state");
                }
                else if (Math.abs(XboxController.GetRightThrottle()) > 0.95){
                    NewState(Shoot, "Right throttle pressed, enter Shoot state");
                }

                break;

            case Intake:
                if (onStateEntered){
                    runIntake();
                }
                
                if (Math.abs(XboxController.GetLeftThrottle()) < 0.05){
                    NewState(Idle, "Left throttle released, enter Idle state");
                }

                break;

            case Shoot:
                if (onStateEntered){
                    runShooter();
                }
                
                if (Math.abs(XboxController.GetRightThrottle()) < 0.05){
                    NewState(Idle, "Right throttle released, enter Idle state");
                }
                break;

        }

    }

    public void setIdle(){ //rename- setIdle
        topRollerMotor.set(0.0);
        bottomRollerMotor.set(0.0);
    }

    public void runIntake(){
        topRollerMotor.set(INTAKE_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(-INTAKE_BOTTOM_ROLLER_SPEED);
    }

    public void runShooter(){
        topRollerMotor.set(-SHOOT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(SHOOT_BOTTOM_ROLLER_SPEED);
    }
}