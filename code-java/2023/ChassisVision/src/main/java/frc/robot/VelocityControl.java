package frc.robot;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import com.ctre.phoenix.motorcontrol.TalonFXControlMode;
import com.ctre.phoenix.motorcontrol.TalonFXFeedbackDevice; 
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;

public class VelocityControl {
    //objects for chassis target velocity equation
    private double chassisTargetVelocityLeft, chassisTargetVelocityRight;

    public void configTargetVelocityLeft(WPI_TalonFX falcon){
        // motor controller configurations

        //falcon.configFactoryDefault(); // Factory Default all hardware to prevent unexpected behaviour 
        //falcon.configNeutralDeadband(0.02); //Config neutral deadband to be the smallest possible

        falcon.configNominalOutputForward(0, Constants.KTIMEOUT_MS); 
        falcon.configNominalOutputReverse(0, Constants.KTIMEOUT_MS);
        falcon.configPeakOutputForward(1, Constants.KTIMEOUT_MS);
        falcon.configPeakOutputReverse(-1, Constants.KTIMEOUT_MS);
        
        falcon.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.KPID_LOOP_IDX, Constants.KTIMEOUT_MS);
        falcon.config_kF(Constants.KPID_LOOP_IDX, Constants.VELOCITY_KF_LEFT, Constants.KTIMEOUT_MS);
        falcon.config_kP(Constants.KPID_LOOP_IDX, Constants.VELOCITY_KP_LEFT, Constants.KTIMEOUT_MS);
        falcon.config_kI(Constants.KPID_LOOP_IDX, Constants.VELOCITY_KI_LEFT, Constants.KTIMEOUT_MS);
        falcon.config_kD(Constants.KPID_LOOP_IDX, Constants.VELOCITY_KD_LEFT, Constants.KTIMEOUT_MS);
    }

    public void configTargetVelocityRight(WPI_TalonFX falcon) {
        // motor controller configurations

        //falcon.configFactoryDefault(); // Factory Default all hardware to prevent unexpected behaviour 
        //falcon.configNeutralDeadband(0.02); //Config neutral deadband to be the smallest possible

        falcon.configNominalOutputForward(0, Constants.KTIMEOUT_MS); 
        falcon.configNominalOutputReverse(0, Constants.KTIMEOUT_MS);
        falcon.configPeakOutputForward(1, Constants.KTIMEOUT_MS);
        falcon.configPeakOutputReverse(-1, Constants.KTIMEOUT_MS);

        falcon.configSelectedFeedbackSensor(TalonFXFeedbackDevice.IntegratedSensor, Constants.KPID_LOOP_IDX, Constants.KTIMEOUT_MS);
        falcon.config_kF(Constants.KPID_LOOP_IDX, Constants.VELOCITY_KF_RIGHT, Constants.KTIMEOUT_MS);
        falcon.config_kP(Constants.KPID_LOOP_IDX, Constants.VELOCITY_KP_RIGHT, Constants.KTIMEOUT_MS);
        falcon.config_kI(Constants.KPID_LOOP_IDX, Constants.VELOCITY_KI_RIGHT, Constants.KTIMEOUT_MS);
        falcon.config_kD(Constants.KPID_LOOP_IDX, Constants.VELOCITY_KD_RIGHT, Constants.KTIMEOUT_MS);
    }

    public void velocityEquationConfig(double leftY, double rightY){
        double cubeJoystickLeft = Math.pow(leftY, 3);
        double cubeJoystickRight = Math.pow(rightY, 3); 
        
        chassisTargetVelocityLeft = -1.0 * cubeJoystickLeft * Constants.CHASSIS_RPM * Constants.FALCON_PULSES_PER_REVOLUTION / 600.0; 
        chassisTargetVelocityRight = cubeJoystickRight * Constants.CHASSIS_RPM * Constants.FALCON_PULSES_PER_REVOLUTION / 600.0;
    }
    
    public void setVelocityLeft(WPI_TalonFX falcon){
        falcon.set(TalonFXControlMode.Velocity, chassisTargetVelocityLeft);
    }

    public void setVelocityRight(WPI_TalonFX falcon){
        falcon.set(TalonFXControlMode.Velocity, chassisTargetVelocityRight);
    }
    
}