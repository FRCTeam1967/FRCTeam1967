package frc.robot;

public interface DriveSystem {
        
    public void drive(double leftJoystick, double rightJoystick);

    public void driveStraight(double leftJoystick, double rightJoystick);

    public void autoAlign(double leftCommand, double rightCommand);

    public void autoDist(double leftCommand, double rightCommand);
}