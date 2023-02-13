package frc.robot;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.SupplyCurrentLimitConfiguration;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import edu.wpi.first.math.filter.SlewRateLimiter;

public class KitBotChassis {
    private static WPI_TalonFX leftLeader, leftFollower, rightLeader, rightFollower;
    private static MotorControllerGroup leftGroup;
    private static MotorControllerGroup rightGroup;
    private static WPI_TalonFX left, right; 

    private Joystick leftJoystick, rightJoystick;
    private double averageSpeed;
    private DifferentialDrive kitBot;

    public KitBotChassis(){
    }

    public void driveStraight(double leftY, double rightY){
        averageSpeed = (leftY + rightY)/2;
        kitBot.tankDrive(-averageSpeed, averageSpeed);
    }
    
    public void loweringSensitivity(double leftY, double rightY) {
        double sixthJoystickLeft = Math.pow(leftY, 5);
        double sixthJoystickRight = Math.pow(rightY, 5);
        kitBot.tankDrive(-sixthJoystickLeft, sixthJoystickRight);
    }
    
    public void accelerate(double leftY, double rightY){
        double newJoystickLeft = leftY*1.5;
        double newJoystickRight = rightY*1.5;
        kitBot.tankDrive(newJoystickLeft, newJoystickRight);
    }

    public void setBrake(boolean isBrakeMode, WPI_TalonFX falcon) {
        if(isBrakeMode){
            falcon.setNeutralMode(NeutralMode.Brake);
        } else {
            rightLeader.setNeutralMode(NeutralMode.Coast);
        }
       
    }

    public void configCurrentLimits(){ 
        //configure the scaling factor for using drive methods with motor controllers
        kitBot.setMaxOutput(0.7); 
        
        //supply-side current limiting. This is typically used to prevent breakers from tripping
        SupplyCurrentLimitConfiguration supplyCurrentLimit = new SupplyCurrentLimitConfiguration(Constants.SUPPLY_CURRENT_ENABLED, Constants.SUPPLY_CURRENT_LIMIT, Constants.SUPPLY_CURRENT_THRESHOLD, Constants.SUPPLY_CURRENT_TIME);
        leftLeader.configSupplyCurrentLimit(supplyCurrentLimit);
    }

    public DifferentialDrive getRobot(){
        return kitBot;
    }

}