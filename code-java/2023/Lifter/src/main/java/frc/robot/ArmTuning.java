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

import edu.wpi.first.networktables.NetworkTable;
import edu.wpi.first.networktables.NetworkTableEntry;
import edu.wpi.first.networktables.NetworkTableInstance;
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

public class ArmTuning extends JankyStateMachine {
    private WPI_TalonFX armMotorL, armMotorR;
    private WPI_CANCoder m_encoder;

    //desiredAngle is changed through the setDesiredAngle method, which is triggered through buttons in Robot.java
    private double desiredAngle;
    
    //shuffleboard fields
    private ShuffleboardTab m_armTab;
    
    //state IDs
    private final int IDLE = 0, IN_MOTION = 1, POS_REACHED = 2;

    //PID fields (for the purpose of PID tuning)
    private double kP, kI, kD, kF;
    private NetworkTable armTable;


    public ArmTuning(int armMotorLID, int armMotorRID){
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

    public void initEncoder(){
        CANCoderConfiguration config = new CANCoderConfiguration();
        config.sensorDirection = true;
        config.magnetOffsetDegrees = Constants.Encoder.ARM_ABS_ENCODER_OFFSET;
        config.unitString = "deg";
        config.sensorTimeBase = SensorTimeBase.PerSecond; //rpm?
        m_encoder.configAllSettings(config);
        m_encoder.setStatusFramePeriod(CANCoderStatusFrame.SensorData, 10); // changes the period of the sensor data frame to 10ms 
    }

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

        m_armTab.add(m_encoder);

        armTable = NetworkTableInstance.getDefault().getTable("arm"); //pid tuning
    }

    private void configMotors(WPI_TalonFX motor){
        TalonFXConfiguration config = new TalonFXConfiguration();
        motor.configAllSettings(config);
        
        //with values from shuffleboard for testing
        motor.config_kP(Constants.Arm.kPIDLoopIdx, kP, Constants.Arm.K_TIMEOUT_MS);
        motor.config_kI(Constants.Arm.kPIDLoopIdx, kI, Constants.Arm.K_TIMEOUT_MS);
        motor.config_kD(Constants.Arm.kPIDLoopIdx, kD, Constants.Arm.K_TIMEOUT_MS);
        motor.config_kF(Constants.Arm.kPIDLoopIdx, kF, Constants.Arm.K_TIMEOUT_MS);
        
        motor.configNeutralDeadband(0.001); //increases resistance to force applied (less wiggle room)

        motor.configMotionCruiseVelocity(Constants.Arm.CRUISE_VELOCITY, Constants.Arm.K_TIMEOUT_MS);
        motor.configMotionAcceleration(Constants.Arm.ACCELERATION, Constants.Arm.K_TIMEOUT_MS);

        motor.setInverted(TalonFXInvertType.Clockwise);
    }

    public void updatePID(){
        NetworkTableEntry arm_kP = armTable.getEntry("kP");
        kP = arm_kP.getDouble(0.0);

        NetworkTableEntry arm_kI = armTable.getEntry("kI");
        kI = arm_kI.getDouble(0.0);

        NetworkTableEntry arm_kD = armTable.getEntry("kD");
        kD = arm_kD.getDouble(0.0); 

        NetworkTableEntry arm_kF = armTable.getEntry("kF");
        kF = arm_kF.getDouble(0.0); 

        armMotorL.config_kP(Constants.Arm.kPIDLoopIdx, kP, Constants.Arm.K_TIMEOUT_MS);
        armMotorL.config_kI(Constants.Arm.kPIDLoopIdx, kI, Constants.Arm.K_TIMEOUT_MS);
        armMotorL.config_kD(Constants.Arm.kPIDLoopIdx, kD, Constants.Arm.K_TIMEOUT_MS);
        armMotorL.config_kF(Constants.Arm.kPIDLoopIdx, kF, Constants.Arm.K_TIMEOUT_MS);

        armMotorR.config_kP(Constants.Arm.kPIDLoopIdx, kP, Constants.Arm.K_TIMEOUT_MS);
        armMotorR.config_kI(Constants.Arm.kPIDLoopIdx, kI, Constants.Arm.K_TIMEOUT_MS);
        armMotorR.config_kD(Constants.Arm.kPIDLoopIdx, kD, Constants.Arm.K_TIMEOUT_MS);
        armMotorR.config_kF(Constants.Arm.kPIDLoopIdx, kF, Constants.Arm.K_TIMEOUT_MS);
    }

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

    public void setDesiredPosition(double angle){
        //input checks
        if(angle >= 0 && angle <= 180) {
            desiredAngle = angle;
            NewState(IN_MOTION, "Button pressed, transition to IN_MOTION");
        } else {
            System.out.println("setDesiredPosition, input angle out of range");
        }
    }

    public void moveArm(double angle) {
        int falconTicks = angleToFalconTicks(angle);
        armMotorR.set(TalonFXControlMode.MotionMagic, falconTicks);
        armMotorL.set(TalonFXControlMode.MotionMagic, falconTicks);
    }

    public double falconTicksToAngle(double ticks){
        return (double) (ticks * 360)/(Constants.Arm.GEAR_RATIO * Constants.Arm.FALCON_TICKS_PER_REVOLUTION);
    }

    public int angleToFalconTicks(double angle){
        return (int) ((angle / 360) * Constants.Arm.GEAR_RATIO * Constants.Arm.FALCON_TICKS_PER_REVOLUTION);
    }

    public boolean isArmInRangeFalcon(double angle, double error){
        double lowerBound = angle - error, upperBound = angle + error;
        double currentAngleL = falconTicksToAngle(armMotorL.getSelectedSensorPosition());
        //if left is in range, but right is not, that's a what the heck is happening edgecase (hopefully)
        return (currentAngleL > lowerBound && currentAngleL < upperBound);
    }
        
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