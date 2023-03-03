package frc.robot;

public interface DriveSystem {
        
    public void drive(double leftJoystick, double rightJoystick);

    public void driveStraight(double leftJoystick, double rightJoystick);

    public void slowMode(double leftJoystick, double rightJoystick);

    public void setBrakeMode(boolean isBrake);

    public void autoAlign();
}
