package frc.robot;
import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.SupplyCurrentLimitConfiguration;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonFX;
import edu.wpi.first.math.filter.SlewRateLimiter;

public class KitBotChassis {
    private static WPI_TalonFX leftLeader, leftFollower, rightLeader, rightFollower;

    private static MotorControllerGroup leftGroup;
    private static MotorControllerGroup rightGroup;
    private static DifferentialDrive myRobot;

    // private Joystick leftJoystick, rightJoystick;
    private double averageSpeed;

    public KitBotChassis(){
        // leftJoystick = new Joystick(0);
        // rightJoystick = new Joystick(1);

        leftLeader = new WPI_TalonFX(Constants.LEFT_LEADER);
        leftFollower = new WPI_TalonFX(Constants.LEFT_FOLLOWER);
        rightLeader = new WPI_TalonFX(Constants.RIGHT_LEADER);
        rightFollower = new WPI_TalonFX(Constants.RIGHT_FOLLOWER);

        leftGroup = new MotorControllerGroup(leftFollower, leftLeader);
        rightGroup = new MotorControllerGroup(rightFollower, rightLeader);

        myRobot = new DifferentialDrive(leftGroup, rightGroup);
    }

    public double driveStraight(Joystick leftJoystick, Joystick rightJoystick){
        averageSpeed = (leftJoystick.getY() + rightJoystick.getY())/2;
        return averageSpeed;
    }

   
    // public void chargeStationDrive () {
    //     //myRobot.setDeadband(0.02);
    //     double cubeJoystickLeft = Math.pow(leftJoystick.getY(), 3);
    //     double cubeJoystickRight = Math.pow(leftJoystick.getY(), 3);
    //     myRobot.tankDrive(-cubeJoystickLeft, cubeJoystickRight);
    // }

    // public void setChassisModes(boolean isBrakeMode) {
    //     if(isBrakeMode){
    //         leftLeader.setNeutralMode(NeutralMode.Brake);
    //         leftFollower.setNeutralMode(NeutralMode.Brake);
    //         leftLeader.setNeutralMode(NeutralMode.Brake);
    //         rightFollower.setNeutralMode(NeutralMode.Brake);
    //     } else {
    //         leftLeader.setNeutralMode(NeutralMode.Coast);
    //         leftFollower.setNeutralMode(NeutralMode.Coast);
    //         leftLeader.setNeutralMode(NeutralMode.Coast);
    //         rightFollower.setNeutralMode(NeutralMode.Coast);
    //     }
       
    // }

    // public void configCurrentLimits(){ 
    //     //configure the scaling factor for using drive methods with motor controllers
    //     myRobot.setMaxOutput(0.7); 
        
    //     //supply-side current limiting. This is typically used to prevent breakers from tripping
    //     SupplyCurrentLimitConfiguration supplyCurrentLimit = new SupplyCurrentLimitConfiguration(Constants.SUPPLY_CURRENT_ENABLED, Constants.SUPPLY_CURRENT_LIMIT, Constants.SUPPLY_CURRENT_THRESHOLD, Constants.SUPPLY_CURRENT_TIME);
    //     leftLeader.configSupplyCurrentLimit(supplyCurrentLimit);
    // }
}