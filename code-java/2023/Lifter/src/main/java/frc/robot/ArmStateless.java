package frc.robot;

import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.can.TalonFXConfiguration;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import com.ctre.phoenix.sensors.CANCoderConfiguration;
import com.ctre.phoenix.sensors.CANCoderStatusFrame;
import com.ctre.phoenix.sensors.SensorTimeBase;
import com.ctre.phoenix.sensors.WPI_CANCoder;

import edu.wpi.first.wpilibj.shuffleboard.Shuffleboard;
import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;

public class ArmStateless {
    private WPI_TalonFX armMotorL, armMotorR;
    private WPI_CANCoder m_encoder;
    private double desiredAngle;
    private ShuffleboardTab m_encoderTab;

    /**
    * Constructor for Arm class
    * <p>
    * Defining falcons, encoders, and setting state names
    * @param armMotorLID - motor ID for left arm motor
    * @param armMotorRID - motor ID for right arm motor
    * @return Arm object with defined and configured falcons, encoder, state machine, and states
    */
    public ArmStateless(int armMotorLID, int armMotorRID){
        armMotorL = new WPI_TalonFX(armMotorLID);
        armMotorR = new WPI_TalonFX(armMotorRID);

        configMotors(armMotorL);
        configMotors(armMotorR);
        
        m_encoder = new WPI_CANCoder(Constants.Encoder.ARM_ABS_ENCODER_ID);
    }

    /**
     * Adds encoders tab and values (both absolute and falcon) to Shuffleboard
     */
    public void configDashboard(){
        m_encoderTab = Shuffleboard.getTab("Encoder");
        
        m_encoderTab.addDouble("Abs Angle", () -> m_encoder.getAbsolutePosition()); //degrees
        m_encoderTab.addDouble("Abs Velocity", () ->  m_encoder.getVelocity());

        m_encoderTab.addDouble("Falcon Ticks R", () -> armMotorR.getSelectedSensorPosition());
        m_encoderTab.addDouble("Falcon Angle (Method Calc) R", () -> falconTicksToAngle(armMotorR.getSelectedSensorPosition()));

        m_encoderTab.addDouble("Falcon Ticks L", () -> armMotorL.getSelectedSensorPosition());
        m_encoderTab.addDouble("Falcon Angle (Method Calc) L", () -> falconTicksToAngle(armMotorL.getSelectedSensorPosition()));
        
        m_encoderTab.addDouble("Closed Loop Error", () -> armMotorR.getClosedLoopError(0));
        
        m_encoderTab.addBoolean("isArmInRange", () -> isArmInRangeFalcon(desiredAngle, Constants.Arm.CHECK_ENCODER_ERROR));
        m_encoderTab.addDouble("Desired Angle", () -> desiredAngle);

        m_encoderTab.add(m_encoder);
    }

    /**
     * Called in robotInit to configure CANCoder at beginning of match
     */
    public void initEncoder(){
        CANCoderConfiguration config = new CANCoderConfiguration();
        config.unitString = "deg";
        config.sensorTimeBase = SensorTimeBase.PerSecond; //rpm?
        m_encoder.configAllSettings(config);
        m_encoder.setStatusFramePeriod(CANCoderStatusFrame.SensorData, 10); 
    }
    
    /**
     * Called in all inits to offset falcon encoder to absolute position
     * <p>
     * "Get the absolute encoder and the falcon encoder on the same page";
     * Absolute encoder ticks --> arm angle --> falcon ticks --> offset falcon;
     * Can be triggered by Start button in teleopPeriodic if arm slips during match
     */
    public void armHoming(){
        double absAngle = m_encoder.getAbsolutePosition(); 
        double falconTicks = angleToFalconTicks(absAngle);

        armMotorL.setSelectedSensorPosition(falconTicks);
        armMotorR.setSelectedSensorPosition(falconTicks);

        moveArm(absAngle);
    }

    /**
     * Configure falcon motor settings, falcon encoder, and PID constants
     * @param motor - Falcon motor to be configured
     */
    private void configMotors(WPI_TalonFX motor){
        TalonFXConfiguration config = new TalonFXConfiguration();
        motor.configAllSettings(config);

        motor.config_kP(Constants.Arm.kPIDLoopIdx, Constants.Arm.kP, Constants.Arm.K_TIMEOUT_MS);
        motor.config_kI(Constants.Arm.kPIDLoopIdx, Constants.Arm.kI, Constants.Arm.K_TIMEOUT_MS);
        motor.config_kD(Constants.Arm.kPIDLoopIdx, Constants.Arm.kD, Constants.Arm.K_TIMEOUT_MS);
        motor.config_kF(Constants.Arm.kPIDLoopIdx, Constants.Arm.kF, Constants.Arm.K_TIMEOUT_MS);
        motor.configNeutralDeadband(0.001); //increases resistance to force applied (less wiggle room)
        
        motor.configMotionCruiseVelocity(Constants.Arm.CRUISE_VELOCITY, Constants.Arm.K_TIMEOUT_MS);
        motor.configMotionAcceleration(Constants.Arm.ACCELERATION, Constants.Arm.K_TIMEOUT_MS);
    }

    /**
     * If input angle is within range, updates value of desiredAngle, converts angle to falcon ticks, and sets to motors
     * <p>
     * Called when button is pressed in Robot.java
     * Method is accessible to other mechanisms to trigger change in arm position
     * 
     * @param angle - Desired angle in degrees to move the arm to
     */
    public void moveArm(double angle) {
        //INPUT CHECK HERE
        if(angle >= 0 && angle <= 180) {
            desiredAngle = angle;
            int falconTicks = angleToFalconTicks(desiredAngle);
            armMotorR.set(TalonFXControlMode.MotionMagic, falconTicks);
            armMotorL.set(TalonFXControlMode.MotionMagic, falconTicks);
        } else {
            System.out.println("Input angle out of range");
        }
    }
    
    /**
     * Converts falcon encoder ticks to angle of the arm in degrees relative to self-defined 0
     * <p>
     * Used in falcon encoder sanity check 
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
        double lowerBound = angle - error;
        double upperBound = angle + error;
        double currentAngleR = falconTicksToAngle(armMotorR.getSelectedSensorPosition());

        return (currentAngleR > lowerBound && currentAngleR < upperBound);
    }
    
    /**
     * Converts absolute encoder ticks and falcon encoder ticks to angle, then cross checks
     * @param error - Constant to create range for allowable difference between encoders, degrees
     * @return boolean - Whether difference between encoders is less than allowable difference
     */
    public boolean encoderCrossCheck(double error){
        double currentAngleAbs = m_encoder.getAbsolutePosition();
        double currentAngleFalconR = falconTicksToAngle(armMotorR.getSelectedSensorPosition());
        double calcError = Math.abs(currentAngleAbs - currentAngleFalconR); 
        
        return (calcError <= error);
    }

    /**
     * Get value of desiredAngle
     * @return desiredAngle, double
     */
    public double getDesiredAngle(){
        return desiredAngle;
    }
}