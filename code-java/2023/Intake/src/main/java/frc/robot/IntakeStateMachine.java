package frc.robot;

import org.janksters.jankyLib.JankyStateMachine;
import org.janksters.jankyLib.jankyXboxJoystick;

import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.XboxController;
import edu.wpi.first.wpilibj.motorcontrol.MotorController;
import com.revrobotics.CANSparkMax;
import com.revrobotics.CANSparkMaxLowLevel.MotorType;

/*
 * @class StateMachine for running the intake
 */
public class IntakeStateMachine extends JankyStateMachine {
    //case numbers to assigned states
    private static final int Idle = 0;
    private static final int Intake = 1;
    private static final int Shoot = 2;
    private static final int MediumEject = 3;
    private static final int SlowEject = 4;


    private jankyXboxJoystick xboxController; 

    private CANSparkMax topRollerMotor;
    private CANSparkMax bottomRollerMotor;
    
    //Motor IDs
    private static final int topRollerMotorID = Constants.Intake.TOP_ROLLER_MOTOR_ID; //tbd
    private static final int bottomRollerMotorID = Constants.Intake.BOTTOM_ROLLER_MOTOR_ID; //tbd

    private boolean shooterCompleteFlag = false;
    
    public IntakeStateMachine(){
        xboxController = new jankyXboxJoystick(1);
        topRollerMotor = new CANSparkMax(topRollerMotorID, MotorType.kBrushless);
        bottomRollerMotor = new CANSparkMax(bottomRollerMotorID, MotorType.kBrushless);
        topRollerMotor.restoreFactoryDefaults();
        bottomRollerMotor.restoreFactoryDefaults();


        SetMachineName("IntakeStateMachine");
        SetName(Idle, "Idle");
        SetName(Intake, "Intake");
        SetName(Shoot, "Shoot");
        SetName (MediumEject, "MediumEject");
        SetName(SlowEject, "SlowEject");


        start();
    }
    
    public void StateEngine(int curState, boolean onStateEntered){
        switch(curState){
            case Idle: 
                setMotorsToZero();
                if (xboxController.GetButtonX()== true && xboxController.GetButtonA()== false && xboxController.GetButtonB()== false && xboxController.GetButtonY()== false) {
                    NewState(Intake, "Only X Button pressed, enter Intake state");
                }
                else if (xboxController.GetButtonA()==false && xboxController.GetButtonX()== false && xboxController.GetButtonB()== false && xboxController.GetButtonY()== true){
                    NewState(Shoot, "Only A Button pressed, enter Shoot state");
                }
                else if (xboxController.GetButtonB()==true && xboxController.GetButtonA()== false && xboxController.GetButtonX()== false && xboxController.GetButtonY()== false){
                    NewState(MediumEject, "Only B Button pressed, enter MediumEject state");
                }else if(xboxController.GetButtonY()== false && xboxController.GetButtonA()== true && xboxController.GetButtonB()== false && xboxController.GetButtonX()== false){
                    NewState(SlowEject, "Only Y Button pressed, enter SlowEject state");
                }

                break;

            case Intake:
                if (onStateEntered){
                    runIntake();
                }
                
                if (xboxController.GetButtonX()==false){
                    NewState(Idle, "X Button released, enter Idle state");
                }

                break;

            case Shoot:
                if (onStateEntered){
                    runMotors(Constants.Intake.SHOOT_TOP_ROLLER_SPEED, Constants.Intake.SHOOT_BOTTOM_ROLLER_SPEED);
                    shooterCompleteFlag = true;
                }
                
                if (xboxController.GetButtonY()== false){
                    NewState(Idle, "Y Button released, enter Idle state");
                }
                break;

            case MediumEject:
                if (onStateEntered) {
                    runMotors(Constants.Intake.MEDIUM_EJECT_TOP_ROLLER_SPEED,Constants.Intake.MEDIUM_EJECT_BOTTOM_ROLLER_SPEED );
                }
                if (xboxController.GetButtonB()==false){
                    NewState(Idle, "B Button released, enter Idle state");
                }
                break;
            case SlowEject:
                if (onStateEntered) {
                    runMotors(Constants.Intake.SLOW_EJECT_TOP_ROLLER_SPEED, Constants.Intake.SLOW_EJECT_BOTTOM_ROLLER_SPEED);
                }
                if (xboxController.GetButtonA()==false){
                    NewState(Idle, "A Button released, enter Idle state");
                }
                break;

        }

    }

    /**
     * Brief - Method sets speeds of topRollerMotor and bottomRollerMotor to 0.0
     */
    public void setMotorsToZero(){ 
        topRollerMotor.set(0.0);
        bottomRollerMotor.set(0.0);
    }

    /**
     * Brief- Method that runs the motors for intake
     */
    public void runIntake(){
        topRollerMotor.set(Constants.Intake.INTAKE_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(-Constants.Intake.INTAKE_BOTTOM_ROLLER_SPEED);
    }


    /**
     * 
     * @param topRoller
     * @param bottomRoller
     * Brief- Method that sets the speeds of the rollers based on the parameters
     */
    public void runMotors(double topRoller, double bottomRoller){
        topRollerMotor.set(-topRoller);
        bottomRollerMotor.set(bottomRoller);
    }

    /**
     * Brief- returns the boolean value of the shooterCompleteFlag
     */
    public boolean shooterComplete(){
        return shooterCompleteFlag;
    }
}