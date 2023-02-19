package frc.robot;

public interface DriveSystem {

    public void configDashboard();

    public void updateDashboardValues();

    public void drive(double leftJoystick, double rightJoystick);

    public void driveStraight(double leftJoystick, double rightJoystick);

    public void slowMode(double leftJoystick, double rightJoystick);

    public void fastMode(double leftJoystick, double rightJoystick);
    
    public void autoAlign();
}
