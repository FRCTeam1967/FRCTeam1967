package frc.robot;

import com.revrobotics.CANSparkMax;
import com.revrobotics.CANSparkMaxLowLevel.MotorType;

import edu.wpi.first.wpilibj.Timer;


/*
 * @class methods for running the intake
 */
public class Intake {
    private CANSparkMax topRollerMotor; 
    private CANSparkMax bottomRollerMotor; 

    private Timer timer;
    
    public Intake(){
        topRollerMotor = new CANSparkMax(Constants.Intake.TOP_ROLLER_MOTOR_ID, MotorType.kBrushless);
        bottomRollerMotor = new CANSparkMax(Constants.Intake.BOTTOM_ROLLER_MOTOR_ID, MotorType.kBrushless);
        topRollerMotor.restoreFactoryDefaults();
        bottomRollerMotor.restoreFactoryDefaults();
        timer = new Timer();
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
        bottomRollerMotor.set(Constants.Intake.INTAKE_BOTTOM_ROLLER_SPEED);
    }

    /**
     * Brief- Method that sets the speeds of the motors for top row
     */
    public void runHighEject(){ 
        topRollerMotor.set(-Constants.Intake.HIGH_EJECT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(-Constants.Intake.HIGH_EJECT_BOTTOM_ROLLER_SPEED);
        
    }
    
    /**
     * Brief- Method that sets the speeds of the motors for middle row
     */
    public void runMiddleEject(){ 
        topRollerMotor.set(-Constants.Intake.MIDDLE_EJECT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(-Constants.Intake.MIDDLE_EJECT_BOTTOM_ROLLER_SPEED);
        
    }

    /**
     * Brief- Method that sets the speeds of the motors for bottom row
     */
    public void runLowEject(){
        topRollerMotor.set(-Constants.Intake.LOW_EJECT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(-Constants.Intake.LOW_EJECT_BOTTOM_ROLLER_SPEED);
        
    }
    /**
     * Brief- Method that runs the motors for shooting in auto
     */
    public void runAutoShooter(){
        timer.start();
        runHighEject();
    }

    /**
     * Brief- returns the boolean value of if the shooting action is complete
     */
    public boolean getShooterComplete(){
        if (timer.get() >= Constants.Intake.AUTO_SHOOTER_TIMER_SECONDS){
            setMotorsToZero();
            timer.stop();
            timer.reset();
            return true;
        } else {
            return false;
        }
    }

}