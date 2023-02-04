package frc.robot;

import org.janksters.jankyLib.JankyStateMachine;

import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.can.TalonFXConfiguration;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.sensors.CANCoderConfiguration;
import com.ctre.phoenix.sensors.CANCoderStatusFrame;
import com.ctre.phoenix.sensors.SensorTimeBase;
import com.ctre.phoenix.sensors.WPI_CANCoder;

/**
 * Arm Xbox Controller Button Mapping
 * GetLeftThrottle- front top
 * GetButtonLB- front middle
 * GetLeftYAxis- intake
 * GetRightThrottle- back top
 * GetButtonRB - back middle
 * GetRightYAxis- safe
 * Arm homing- start
*/

public class Arm extends JankyStateMachine {
    private WPI_TalonFX armMotorL, armMotorR;
    private WPI_CANCoder m_encoder;

    //desiredAngle is changed through the setDesiredAngle method, which is triggered through buttons in Robot.java
    private double desiredAngle;

    //state IDs
    private final int IDLE = 0, IN_MOTION = 1, POS_REACHED = 2;
    //private final int fINTAKE = 1, fMIDDLE = 2, fTOP = 3, SAFE = 4, bTOP = 5, bMIDDLE = 6;

    //mapping positions to arm angles for the setDesiredPosition method
    enum Positions {
        FRONT_INTAKE(Constants.Arm.INTAKE_ANGLE),
        FRONT_MIDDLE(Constants.Arm.fMIDDLE_ANGLE),
        FRONT_TOP(Constants.Arm.fTOP_ANGLE),
        SAFE(Constants.Arm.SAFE_ANGLE),
        BACK_TOP(Constants.Arm.bTOP_ANGLE),
        BACK_MIDDLE(Constants.Arm.bMIDDLE_ANGLE);

        public final double angle;

        private Positions(double angle){
            this.angle = angle;
        }
    }
    
    /**
    * Brief- Constructor for Arm class
    * Defining falcons, encoders, and setting state names
    * @param armMotorLID - motor ID for left arm motor
    * @param armMotorRID - motor ID for right arm motor
    * @return Arm object with defined and configured falcons, encoder, state machine, and states
    */
    public Arm(int armMotorLID, int armMotorRID){
        armMotorL = new WPI_TalonFX(armMotorLID);
        armMotorR = new WPI_TalonFX(armMotorRID);

        configMotors(armMotorL);
        configMotors(armMotorR);

        m_encoder = new WPI_CANCoder(Constants.Encoder.canID);

        SetMachineName("JankyStateMachineArm");

        SetName(IDLE, "Idle");
        SetName(IN_MOTION, "In Motion");
        SetName(POS_REACHED, "Position Reached");
       
        /*
            SetName(fINTAKE, "Front Intake");
            SetName(fMIDDLE, "Front Middle");
            SetName(fTOP, "Front Top");
            SetName(SAFE, "Safe");
            SetName(bTOP, "Back Top");
            SetName(bMIDDLE, "Back Middle");
        */
        
        start();
    }
    
    /**
     * Brief- Called in robotInit to configure CANCoder at beginning of match
     */
    public void initEncoder(){
        CANCoderConfiguration config = new CANCoderConfiguration();
        config.unitString = "deg";
        config.sensorTimeBase = SensorTimeBase.PerSecond; //rpm?
        m_encoder.configAllSettings(config);
        m_encoder.setStatusFramePeriod(CANCoderStatusFrame.SensorData, 10); // changes the period of the sensor data frame to 10ms 
    }
    
    /**
     * Brief- Called in all inits to offset falcon encoder to absolute position
     * "Get the absolute encoder and the falcon encoder on the same page"
     * Absolute encoder ticks --> arm angle --> falcon ticks --> offset falcon
     * Can be triggered by Start button in teleopPeriodic if arm slips during match
     */
    public void armHoming(){
        //read the absolute encoder ticks
        double absTicks = m_encoder.getAbsolutePosition(); //degrees of revolution (absolute)
        
        //convert absolute encoder ticks to arm angle
        double angle = absoluteTicksToAngle(absTicks);

        //convert arm angle to falcon encoder ticks
        
        double falconTicks = angleToFalconTicks(angle);

        //offset falcon encoder
        armMotorL.setSelectedSensorPosition(falconTicks);
        armMotorR.setSelectedSensorPosition(falconTicks);
    }
    
    /**
     * Brief- Configure falcon motor settings, falcon encoder, and PID constants
     */
    private void configMotors(WPI_TalonFX motor){
        TalonFXConfiguration config = new TalonFXConfiguration();
        motor.configAllSettings(config);
        
        /* fall back on this if configAllSettings() doesn't work
            motor.configFactoryDefault();
            motor.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.Arm.ARM_K_PID_LOOP_IDX, Constants.Arm.ARM_K_TIMEOUT_MS);
            motor.configNominalOutputForward(0, Constants.Arm.ARM_K_TIMEOUT_MS);
            motor.configNominalOutputReverse(0, Constants.Arm.ARM_K_TIMEOUT_MS);
            motor.configPeakOutputForward(1, Constants.Arm.ARM_K_TIMEOUT_MS);
            motor.configPeakOutputReverse(-1, Constants.Arm.ARM_K_TIMEOUT_MS);
            motor.setNeutralMode(NeutralMode.Brake);
        */

        //pid constants config
        motor.config_kP(Constants.Arm.ARM_kPIDLoopIdx, Constants.Arm.ARM_kP, Constants.Arm.ARM_K_TIMEOUT_MS);
        motor.config_kI(Constants.Arm.ARM_kPIDLoopIdx, Constants.Arm.ARM_kI, Constants.Arm.ARM_K_TIMEOUT_MS);
        motor.config_kD(Constants.Arm.ARM_kPIDLoopIdx, Constants.Arm.ARM_kD, Constants.Arm.ARM_K_TIMEOUT_MS);
        motor.config_kF(Constants.Arm.ARM_kPIDLoopIdx, Constants.Arm.ARM_kF, Constants.Arm.ARM_K_TIMEOUT_MS);
        
        motor.configMotionCruiseVelocity(Constants.Arm.ARM_CRUISE_VELOCITY, Constants.Arm.ARM_K_TIMEOUT_MS);
        motor.configMotionAcceleration(Constants.Arm.ARM_ACCELERATION, Constants.Arm.ARM_K_TIMEOUT_MS);
    }

    /**
     * Brief- Changes value of desiredAngle using Position parameter, shoves state machine into IN_MOTION
     * Called when button is pressed in Robot.java
     * Method is accessible to other mechanisms to trigger change in arm position
     * @param position - Desired position for the arm to reach
     */
    public void setDesiredPosition(Positions position){
        desiredAngle = position.angle;
        NewState(IN_MOTION, "Button pressed, transition to IN_MOTION");
    }
    
    /**
     * Brief- Calculate falcon ticks from angle parameter and feed to Motion Magic
     * @param angle - Desired angle in degrees to move the arm to
     */
    public void moveArm(double angle) {
        int falconTicks = angleToFalconTicks(angle);
        armMotorL.set(TalonFXControlMode.MotionMagic, falconTicks);
        armMotorR.set(TalonFXControlMode.MotionMagic, falconTicks);
    }

    /**
     * Brief- Converts falcon encoder ticks to angle of the arm in degrees relative to self-defined 0
     * Used in falcon encoder sanity check 
     * @param ticks - Falcon encoder ticks
     * @return angle - Angle of arm, double
    */
    public double falconTicksToAngle(int ticks){
        return (double) (ticks * 360)/(Constants.Arm.ARM_GEAR_RATIO * Constants.Arm.FALCON_TICKS_PER_REVOLUTION);
    }

    /**
     * Brief- Converts inputted angle of the arm relative to self-defined 0 to falcon encoder ticks
     * @param angle - Angle of arm
     * @return ticks - Falcon encoder ticks
     */
    public int angleToFalconTicks(double angle){
        return (int) ((angle / 360) * Constants.Arm.ARM_GEAR_RATIO * Constants.Arm.FALCON_TICKS_PER_REVOLUTION);
    }
    
    /**
     * Brief- Takes absolute encoder ticks and converts to angle of the arm relative to self-defined 0
     * @param ticks - Absolute encoder ticks
     * @return angle - Angle of arm, double
     */
    public double absoluteTicksToAngle(double ticks){
        return (double) (ticks * 360)/ Constants.Arm.ABSOLUTE_TICKS_PER_REVOLUTION;
    }

    /**
     * Brief- Checks if falcon encoder position is within inputted range
     * @param min - Minimum falcon encoder ticks for the range
     * @param max - Maximum falcon encoder ticks value for the range
     * @return boolean - Whether encoder motor position is within the range
     */
    public boolean isArmInRangeFalcon(double min, double max){
        return (armMotorL.getSelectedSensorPosition() > min && armMotorL.getSelectedSensorPosition() < max) 
                && (armMotorR.getSelectedSensorPosition() > min && armMotorR.getSelectedSensorPosition() < max);
    }
    
    /**
     * Brief- Checks if absolute encoder position is within inputted range
     * @param min - Minimum absolute encoder ticks value for the range
     * @param max - Maximum absolute encoder ticks value for the range
     * @return boolean - Whether absolute encoder position is within the range
     */
    public boolean isArmInRangeAbsolute(double min, double max){
        return (m_encoder.getAbsolutePosition() > min && m_encoder.getAbsolutePosition() < max);
    }
    
    public void StateEngine(int curState, boolean onStateEntered){
        switch (curState){
            case IDLE:
                if (onStateEntered){}
                break;
            
            case IN_MOTION:
                if (onStateEntered){
                    //use motion magic to move and hold arm at desired position
                    moveArm(desiredAngle); //does this go in onStateEntered?
                }
                
                //check if falcon encoder ticks are in range to transition out of IN_MOTION state
                if(isArmInRangeFalcon(Constants.Arm.ARM_FALCON_CHECK_MIN, Constants.Arm.ARM_FALCON_CHECK_MAX)){
                    NewState(POS_REACHED, "Angle within range, transition to POS_REACHED");
                }
                break;

            case POS_REACHED:
                if (onStateEntered){
                    //check if absolute encoder ticks are in range, send error message if not
                    if(!isArmInRangeAbsolute(Constants.Arm.ARM_ABS_CHECK_MIN, Constants.Arm.ARM_ABS_CHECK_MAX)){
                        System.out.println("Absolute encoder: arm is not in range");
                    }
                }
                break;
        }
    }
}
