package frc.robot;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;

import com.ctre.phoenix.motorcontrol.NeutralMode;
import com.ctre.phoenix.motorcontrol.SupplyCurrentLimitConfiguration;
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import edu.wpi.first.math.filter.SlewRateLimiter;

public class KitBotChassis {
    private static WPI_TalonSRX leftLeader, leftFollower, rightLeader, rightFollower;

    private static MotorControllerGroup leftGroup, rightGroup;
    private static DifferentialDrive myRobot;

    private double averageSpeed;

    public KitBotChassis(){

        leftLeader = new WPI_TalonSRX(Constants.LEFT_LEADER);
        leftFollower = new WPI_TalonSRX(Constants.LEFT_FOLLOWER);
        rightLeader = new WPI_TalonSRX(Constants.RIGHT_LEADER);
        rightFollower = new WPI_TalonSRX(Constants.RIGHT_FOLLOWER);

        leftGroup = new MotorControllerGroup(leftFollower, leftLeader);
        rightGroup = new MotorControllerGroup(rightFollower, rightLeader);

        myRobot = new DifferentialDrive(leftGroup, rightGroup);
    }

    public void driveStraight(double leftSpeed, double rightSpeed){
        averageSpeed = (leftSpeed + rightSpeed)/2;
        myRobot.tankDrive(averageSpeed, -averageSpeed);
    }
    
    public void driveTank(double leftSpeed, double rightSpeed) {
        myRobot.tankDrive(leftSpeed, -rightSpeed);
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