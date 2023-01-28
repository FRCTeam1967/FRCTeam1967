package frc.robot;

import org.janksters.jankyLib.*;
//import com.revrobotics.CANSparkMax;
//import com.revrobotics.CANSparkMaxLowLevel.MotorType; //kBrushed or kBrushless
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

public class Lifter extends JankyStateMachine {
    private final int IDLE = 0, SAFE = 1, INTAKE = 2, TOP = 3, MIDDLE = 4, BOTTOM = 5;
    private WPI_TalonFX armMotorL, armMotorR;
    private jankyXboxJoystick xbox;
    private String flag = "";

    //constants - will be moved
    private final int SAFE_ANGLE=1000, INTAKE_ANGLE=1000, TOP_ANGLE=1000, MIDDLE_ANGLE=1000, BOTTOM_ANGLE=1000;
    private final int ARM_GEAR_RATIO=1000, FALCON_PULSES_PER_REVOLUTION=2048;

    public Lifter(int pivotMotorLID, int pivotMotorRID, int xboxPort){
        armMotorL = new WPI_TalonFX(pivotMotorLID);
        armMotorR = new WPI_TalonFX(pivotMotorRID);
        xbox = new jankyXboxJoystick(xboxPort); //usually 2

        SetMachineName("JankyStateMachineLifter");

        SetName(IDLE, "Idle");
        SetName(SAFE, "Safe");
        SetName(INTAKE, "Intake");
        SetName(TOP, "Top");
        SetName(MIDDLE, "Middle");
        SetName(BOTTOM, "Bottom");

        start();
    }

    //METHODS
    
    //setup PID
    public void setArmPos(double angle) {
        double pulses = (angle / 360) * ARM_GEAR_RATIO * FALCON_PULSES_PER_REVOLUTION;
        //armMotorL.set(TalonFXControlMode.MotionMagic, pulses);
        //armMotorR.set(TalonFXControlMode.MotionMagic, pulses);
    }
   

    public void StateEngine(int curState, boolean onStateEntered){
        /**
         * start button - go to safe
         * X button - go to intake
         * Y button - go to top
         * B button - go to middle
         * A button - go to bottom
        */
        
        switch (curState){
            case IDLE:
                if (onStateEntered) {
                    
                }

                //if condition
                    NewState(SAFE, "moving from idle to safe because limit switch pressed");
                break;
            case SAFE:
                if (onStateEntered) {
                    
                }
                
                setArmPos(SAFE_ANGLE);

                if(xbox.GetButtonX()){
                    NewState(INTAKE, "moving from safe to intake because button X pressed");
                }

                if(xbox.GetButtonY()){
                    NewState(TOP, "moving from safe to top because button Y pressed");
                }

                if(xbox.GetButtonB()){
                    NewState(MIDDLE, "moving from safe to middle because button B pressed");
                }

                if(xbox.GetButtonA()){
                    NewState(BOTTOM, "moving from safe to bottom because button A pressed");
                }
                break;
        }
    }
}
