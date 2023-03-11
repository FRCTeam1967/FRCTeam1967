package frc.robot;

import com.revrobotics.CANSparkMax;
import com.revrobotics.CANSparkMaxLowLevel.MotorType;

import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;
import edu.wpi.first.wpilibj.shuffleboard.BuiltInWidgets;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;

import edu.wpi.first.wpilibj.Timer;

/**
 * Intake Arm Xbox Controller Button Mapping
 * LT or RT- intake
 * POV top- low eject
 * POV right- middle eject
 * POV bottom- high eject
 */

public class Intake {
    private CANSparkMax topRollerMotor, bottomRollerMotor; 
    private Timer timer;
    SendableChooser <Integer> autoShootRowEntry;

    /**
     * Constructor for Intake class
     * <p> Defining motors and timer
     */
    public Intake(){
        topRollerMotor = new CANSparkMax(Constants.Intake.TOP_ROLLER_MOTOR_ID, MotorType.kBrushless);
        bottomRollerMotor = new CANSparkMax(Constants.Intake.BOTTOM_ROLLER_MOTOR_ID, MotorType.kBrushless);

        topRollerMotor.restoreFactoryDefaults();
        bottomRollerMotor.restoreFactoryDefaults();

        timer = new Timer();
    }

    /**
     * Adds auto speed options to Shuffleboard
     * @param tab - Shuffleboard tab to add options to
     */
    public void configDashboard(ShuffleboardTab tab){
        autoShootRowEntry = new SendableChooser<Integer>();
        autoShootRowEntry.addOption("Low Eject", 0);
        autoShootRowEntry.setDefaultOption("Middle Eject", 1);
        autoShootRowEntry.addOption("High Eject", 2);

        tab.add("Auto Shoot Select", autoShootRowEntry).withWidget(BuiltInWidgets.kSplitButtonChooser);
    }
    
    /**
     * Sets speeds of topRollerMotor and bottomRollerMotor to 0.0
     * <p> Expected behavoir is top and bottom rollers stop spinning
     */
    public void setMotorsToZero(){ 
        topRollerMotor.set(0.0);
        bottomRollerMotor.set(0.0);
    }

    /**
     * Sets speeds of topRollerMotor and bottomRollerMotor to intake speeds
     * <p> Expected behavoir is top and bottom rollers bring in game piece
     */
    public void runIntake(){
        topRollerMotor.set(Constants.Intake.INTAKE_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(Constants.Intake.INTAKE_BOTTOM_ROLLER_SPEED);
    }

    /**
     * Sets speeds of topRollerMotor and bottomRollerMotor to high eject speeds
     */
    public void runHighEject(){ 
        topRollerMotor.set(-Constants.Intake.HIGH_EJECT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(-Constants.Intake.HIGH_EJECT_BOTTOM_ROLLER_SPEED); 
    }

    /**
     * Sets speeds of topRollerMotor and bottomRollerMotor to middle eject speeds
     */
    public void runMiddleEject(){ 
        topRollerMotor.set(-Constants.Intake.MIDDLE_EJECT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(-Constants.Intake.MIDDLE_EJECT_BOTTOM_ROLLER_SPEED);
    }

    /**
     * Sets speeds of topRollerMotor and bottomRollerMotor to low eject speeds
     */
    public void runLowEject(){
        topRollerMotor.set(-Constants.Intake.LOW_EJECT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(-Constants.Intake.LOW_EJECT_BOTTOM_ROLLER_SPEED);   
    }

    /**
     * Method to trigger shooting in autonomous routine
     * <p> Starts timer and runs shooting method
     */
    public void runAutoShooter(){
        timer.start();
        switch (autoShootRowEntry.getSelected()){
            case 0:
                runLowEject();
                break;
            case 1:
                runMiddleEject();
                break;
            case 2:
                runHighEject();
                break;
        }
    }
    
    /**
     * Determine if shooting timer is over and stop motors
     * <p> Expected behavoir is if shooting timer reached, stop spinning rollers
     * @return boolean - Whether shooting timer has been reached
     */
    public boolean isShooterComplete(){
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