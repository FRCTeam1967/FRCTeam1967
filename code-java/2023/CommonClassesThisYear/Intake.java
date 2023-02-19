package org.janksters.CommonClassesThisYear;

import com.revrobotics.CANSparkMax;
import com.revrobotics.CANSparkMaxLowLevel.MotorType;

import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;
import edu.wpi.first.wpilibj.shuffleboard.BuiltInWidgets;
import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;


import edu.wpi.first.networktables.GenericEntry;

import edu.wpi.first.wpilibj.Timer;

/**
 * Methods- setMotorsToZero(), runIntake(), runHighEject(), runMiddleEject(), runLowEject(), runAutoShooter(), isShooterComplete()
 * Intake Arm Xbox Controller Button Mapping
 * GetButtonX- intake
 * GetButtonA- low eject
 * GetButtonB- middle eject
 * GetButtonY- high eject
 */

public class Intake {
    private CANSparkMax topRollerMotor; 
    private CANSparkMax bottomRollerMotor; 

    private Timer timer;

    SendableChooser <Integer> autoShootRowEntry;

    /**
     * Constructor for Intake class
     * Defining motors and timer
     */
    public Intake(){
        topRollerMotor = new CANSparkMax(Constants.Intake.TOP_ROLLER_MOTOR_ID, MotorType.kBrushless);
        bottomRollerMotor = new CANSparkMax(Constants.Intake.BOTTOM_ROLLER_MOTOR_ID, MotorType.kBrushless);

        topRollerMotor.restoreFactoryDefaults();
        bottomRollerMotor.restoreFactoryDefaults();

        timer = new Timer();
    }

    public void configDashboard(ShuffleboardTab tab){
        //
        autoShootRowEntry = new SendableChooser<Integer>();
        autoShootRowEntry.addOption("Low Eject", 0);
        autoShootRowEntry.addOption("Middle Eject", 1);
        autoShootRowEntry.setDefaultOption​("High Eject", 2);

        tab.add("Auto Shoot Select", autoShootRowEntry).withWidget(BuiltInWidgets.kSplitButtonChooser);
    }
    
    /**
     * Brief- sets speeds of topRollerMotor and bottomRollerMotor to 0.0
     * Expected behavoir is top and bottom rollers stop spinning
     */
    public void setMotorsToZero(){ 
        topRollerMotor.set(0.0);
        bottomRollerMotor.set(0.0);
    }

    /**
     * Brief- sets speeds of topRollerMotor and bottomRollerMotor to intake speeds
     * Expected behavoir is top and bottom rollers bring in game piece
     */
    public void runIntake(){
        topRollerMotor.set(Constants.Intake.INTAKE_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(Constants.Intake.INTAKE_BOTTOM_ROLLER_SPEED);
    }

    /**
     * Brief- sets speeds of topRollerMotor and bottomRollerMotor to high eject speeds
     */
    public void runHighEject(){ 
        topRollerMotor.set(-Constants.Intake.HIGH_EJECT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(-Constants.Intake.HIGH_EJECT_BOTTOM_ROLLER_SPEED); 
    }

    /**
     * Brief- sets speeds of topRollerMotor and bottomRollerMotor to middle eject speeds
     */
    public void runMiddleEject(){ 
        topRollerMotor.set(-Constants.Intake.MIDDLE_EJECT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(-Constants.Intake.MIDDLE_EJECT_BOTTOM_ROLLER_SPEED);
    }

    /**
     * Brief- sets speeds of topRollerMotor and bottomRollerMotor to low eject speeds
     */
    public void runLowEject(){
        topRollerMotor.set(-Constants.Intake.LOW_EJECT_TOP_ROLLER_SPEED);
        bottomRollerMotor.set(-Constants.Intake.LOW_EJECT_BOTTOM_ROLLER_SPEED);   
    }

    
    /**
     * Brief- method to trigger shooting in autonomous routine
     * Starts timer and runs shooting method
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
     * Brief- returns true if shooting timer has been reached, false otherwise
     * Expected behavoir is if shooting timer reached, stop spinning rollers
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