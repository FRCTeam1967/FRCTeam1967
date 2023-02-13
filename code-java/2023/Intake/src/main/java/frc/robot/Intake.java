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
public class Intake {
    //case numbers to assigned states

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
        bottomRollerMotor.set(-Constants.Intake.INTAKE_BOTTOM_ROLLER_SPEED);
    }

    /**
     * Brief- Method that sets the speeds of the rollers for top row
     */
    public void runTopEject(){ 
        topRollerMotor.set(-Constants.Intake.SHOOT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(Constants.Intake.SHOOT_BOTTOM_ROLLER_SPEED);
        
    }

    /**
     * Brief- Method that sets the speeds of the rollers for middle row
     */
    public void runMiddleEject(){ 
        topRollerMotor.set(-Constants.Intake.MEDIUM_EJECT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(Constants.Intake.MEDIUM_EJECT_BOTTOM_ROLLER_SPEED);
        
    }

    /**
     * Brief- Method that sets the speeds of the rollers for bottom row
     */
    public void runBottomEject(){
        topRollerMotor.set(-Constants.Intake.SLOW_EJECT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(Constants.Intake.SLOW_EJECT_BOTTOM_ROLLER_SPEED);
        
    }

    /**
     * Brief- Method that runs the motors for shooting in auto
     */
    public void runAutoShooter(){
        timer.start();
        runTopEject();
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
        }else{
            return false;
        }
    }

}