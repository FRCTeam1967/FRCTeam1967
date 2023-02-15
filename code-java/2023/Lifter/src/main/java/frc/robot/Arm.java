package frc.robot;

import org.janksters.jankyLib.JankyStateMachine;

import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXInvertType;
import com.ctre.phoenix.motorcontrol.can.TalonFXConfiguration;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.sensors.CANCoderConfiguration;
import com.ctre.phoenix.sensors.CANCoderStatusFrame;
import com.ctre.phoenix.sensors.SensorTimeBase;
import com.ctre.phoenix.sensors.WPI_CANCoder;

import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;

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

    private double desiredAngle;
    
    //shuffleboard tab
    private ShuffleboardTab m_armTab;
    
    //state IDs
    private final int IDLE = 0, IN_MOTION = 1, POS_REACHED = 2;
    
    /**
    * Constructor for Arm class
    * <p> Defining falcons, encoders, and setting state names
    * @param armMotorLID - motor ID for left arm motor
    * @param armMotorRID - motor ID for right arm motor
    * @return Arm object with defined and configured falcons, encoder, state machine, and states
    */
    public Arm(int armMotorLID, int armMotorRID){
        armMotorL = new WPI_TalonFX(armMotorLID);
        armMotorR = new WPI_TalonFX(armMotorRID);

        configMotors(armMotorL);
        configMotors(armMotorR);
        
        m_encoder = new WPI_CANCoder(Constants.Encoder.ARM_ABS_ENCODER_ID);

        SetMachineName("JankyStateMachineArm");

        SetName(IDLE, "Idle");
        SetName(IN_MOTION, "In Motion");
        SetName(POS_REACHED, "Position Reached");
        
        start();
    }
    
    /**
     * Adds encoders tab and values (both absolute and falcon) to Shuffleboard
     */
    public void configDashboard(){
        m_armTab = Shuffleboard.getTab("Arm");
        
        //updates encoder values on shuffleboard continuously
        m_armTab.addDouble("Abs Angle", () -> m_encoder.getAbsolutePosition()); //degrees
        m_armTab.addDouble("Abs Velocity", () ->  m_encoder.getVelocity());

        m_armTab.addDouble("Falcon Ticks R", () -> armMotorR.getSelectedSensorPosition());
        m_armTab.addDouble("Falcon Angle (Method Calc) R", () -> falconTicksToAngle(armMotorR.getSelectedSensorPosition()));

        m_armTab.addDouble("Falcon Ticks L", () -> armMotorL.getSelectedSensorPosition());
        m_armTab.addDouble("Falcon Angle (Method Calc) L", () -> falconTicksToAngle(armMotorL.getSelectedSensorPosition()));
        
        m_armTab.addBoolean("isArmInRange", () -> isArmInRangeFalcon(desiredAngle, Constants.Arm.CHECK_ENCODER_ERROR));
        m_armTab.addDouble("Desired Angle", () -> desiredAngle);
    }

    /**
     * Called in robotInit to configure CANCoder at beginning of match
     */
    public void initEncoder(){
        CANCoderConfiguration config = new CANCoderConfiguration();
        config.sensorDirection = true;
        config.magnetOffsetDegrees = Constants.Encoder.ARM_ABS_ENCODER_OFFSET;
        config.unitString = "deg";
        config.sensorTimeBase = SensorTimeBase.PerSecond; //rpm?
        m_encoder.configAllSettings(config);
        m_encoder.setStatusFramePeriod(CANCoderStatusFrame.SensorData, 10);
    }
    
    /**
     * Called in all inits to offset falcon encoder to absolute position
     * <p> Get the absolute encoder and the falcon encoder on the same page
     * <p> Absolute encoder ticks --> arm angle --> falcon ticks --> offset falcon
     * <p> Can be triggered by Start button in teleopPeriodic if arm slips during match
     */
    public void armHoming(){
        //read the absolute encoder ticks
        double absAngle = m_encoder.getAbsolutePosition(); //degrees of revolution (absolute)
        
        //convert arm angle to falcon encoder ticks
        double falconTicks = angleToFalconTicks(absAngle);

        //offset falcon encoder
        armMotorL.setSelectedSensorPosition(falconTicks);
        armMotorR.setSelectedSensorPosition(falconTicks);

        //reset "goalpost" desired angle
        setDesiredPosition(absAngle);
    }
    
    /**
     * Configure falcon motor settings, falcon encoder, and PID constants
     */
    private void configMotors(WPI_TalonFX motor){
        TalonFXConfiguration config = new TalonFXConfiguration();
        motor.configAllSettings(config);
        
        //pid constants config
        motor.config_kP(Constants.Arm.kPIDLoopIdx, Constants.Arm.kP, Constants.Arm.K_TIMEOUT_MS);
        motor.config_kI(Constants.Arm.kPIDLoopIdx, Constants.Arm.kI, Constants.Arm.K_TIMEOUT_MS);
        motor.config_kD(Constants.Arm.kPIDLoopIdx, Constants.Arm.kD, Constants.Arm.K_TIMEOUT_MS);
        motor.config_kF(Constants.Arm.kPIDLoopIdx, Constants.Arm.kF, Constants.Arm.K_TIMEOUT_MS);
        
        motor.configNeutralDeadband(0.001); //increases resistance to force applied (less wiggle room)

        motor.configMotionCruiseVelocity(Constants.Arm.CRUISE_VELOCITY, Constants.Arm.K_TIMEOUT_MS);
        motor.configMotionAcceleration(Constants.Arm.ACCELERATION, Constants.Arm.K_TIMEOUT_MS);

        motor.setInverted(TalonFXInvertType.Clockwise);
    }

    /**
     * If input angle is within range, updates value of desiredAngle and shoves state machine into IN_MOTION
     * <p> Called when button is pressed in Robot.java
     * <p> Method is accessible to other mechanisms to trigger change in arm position
     * @param position - Desired position for the arm to reach
     */
    public void setDesiredPosition(double angle){
        //input checks
        if(angle >= 0 && angle <= 180) {
            desiredAngle = angle;
            NewState(IN_MOTION, "Button pressed, transition to IN_MOTION");
        } else {
            System.out.println("setDesiredPosition, input angle out of range");
        }
    }
    
    /**
     * Calculate falcon ticks from angle parameter and feed to Motion Magic
     * @param angle - Desired angle in degrees to move the arm to
     */
    public void moveArm(double angle) {
        int falconTicks = angleToFalconTicks(angle);
        armMotorR.set(TalonFXControlMode.MotionMagic, falconTicks);
        armMotorL.set(TalonFXControlMode.MotionMagic, falconTicks);
    }
    
    /**
     * Converts falcon encoder ticks to angle of the arm in degrees relative to self-defined 0
     * <p> Used in falcon encoder sanity check 
     * @param ticks - Falcon encoder ticks
     * @return angle - Angle of arm, double
    */
    public double falconTicksToAngle(double ticks){
        return (double) (ticks * 360)/(Constants.Arm.GEAR_RATIO * Constants.Arm.FALCON_TICKS_PER_REVOLUTION);
    }

    /**
     * Converts inputted angle of the arm relative to self-defined 0 to falcon encoder ticks
     * @param angle - Angle of arm
     * @return ticks - Falcon encoder ticks
     */
    public int angleToFalconTicks(double angle){
        return (int) ((angle / 360) * Constants.Arm.GEAR_RATIO * Constants.Arm.FALCON_TICKS_PER_REVOLUTION);
    }
    
    /**
     * Read falcon encoder ticks and converts to angle, then checks if current angle is in allowed error range
     * @param angle - Desired angle, degrees
     * @param error - Constant which is added or subtracted from desired angle to create range, degrees 
     * @return boolean - Whether encoder motor position is within the range
     */
    public boolean isArmInRangeFalcon(double angle, double error){
        double lowerBound = angle - error, upperBound = angle + error;
        double currentAngleL = falconTicksToAngle(armMotorL.getSelectedSensorPosition());
        //if left is in range, but right is not, that's a what the heck is happening edgecase (hopefully)
        return (currentAngleL > lowerBound && currentAngleL < upperBound);
    }
    
    /**
     * Converts absolute encoder ticks and falcon encoder ticks to angle, then cross checks
     * @param error - Constant to create range for allowable difference between encoders, degrees
     * @return boolean - Whether difference between encoders is less than allowable difference
     */
    public boolean encoderCrossCheck(double error){
        double currentAngleAbs = m_encoder.getAbsolutePosition();
        double currentAngleFalconL = falconTicksToAngle(armMotorL.getSelectedSensorPosition());
        double calcError = Math.abs(currentAngleAbs - currentAngleFalconL); //difference in angle between 2 encoders
        return (calcError <= error);
    }
    
    public void StateEngine(int curState, boolean onStateEntered){
        switch (curState){
            case IDLE:
                if (onStateEntered){}
                break;
            case IN_MOTION:
                if (onStateEntered){
                    //use motion magic to move and hold arm at desired position
                    moveArm(desiredAngle);
                }
                
                //check if falcon encoder ticks are in range to transition out of IN_MOTION state
                if(isArmInRangeFalcon(desiredAngle, Constants.Arm.CHECK_ENCODER_ERROR)){
                    NewState(POS_REACHED, "Angle within range, transition to POS_REACHED");
                }
                break;
            case POS_REACHED:
                if (onStateEntered){}
                
                //sanity check- check if both encoders are synced, send error message if not
                /*
                if(!encoderCrossCheck(Constants.Arm.CHECK_ENCODER_ERROR)){
                    System.out.println("ENCODER INCONSISTENCY- Absolute and falcon encoders not synced");
                }
                */
                break;
        }
    }
}