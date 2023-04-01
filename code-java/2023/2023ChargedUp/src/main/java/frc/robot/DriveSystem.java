package frc.robot;

import edu.wpi.first.wpilibj.shuffleboard.ShuffleboardTab;

public interface DriveSystem {
        
    public void configDashboard(ShuffleboardTab tab);
    
    public void drive(double leftJoystick, double rightJoystick);

    public void driveStraight(double leftJoystick, double rightJoystick);

    public void slowMode(double leftJoystick, double rightJoystick);

    public void setBrakeMode(boolean isBrake);

    public void autoAlign(double leftCommand, double rightCommand);
}
