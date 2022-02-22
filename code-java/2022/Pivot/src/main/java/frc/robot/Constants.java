package frc.robot;

//8 values are TBD
public class Constants {
    public static final int PULSES_PER_REVOLUTION = 2048;
    public static final int MOTOR_ID = 8; //TBD
    public static final int CONTROLLER_PORT_ID = 8; //TBD
    public static final int TOP_LIMIT_SWITCH_ID = 8; //TBD
    public static final int BOTTOM_LIMIT_SWITCH_ID = 8; //TBD
    public static final int GEAR_RATIO = 100;

    // PID 
    public static final int kTimeoutMs = 1;
    public static final int kPIDLoopIdx = 0;

    public static final double kP = 0.15;
    public static final double kI = 0.0;
    public static final double kD = 1.0;
    public static final double kF = 0.0;
    
    //TBD
    public static final double INTAKE_ANGLE = 105; 
    public static final double STARTING_ANGLE = 0;
    public static final double SHOOTER_ANGLE = 15; 
    public static final double CLIMB_ANGLE = 85; 

    public static final double INTAKE_ANGLE_PULSES = INTAKE_ANGLE / 360 * GEAR_RATIO * PULSES_PER_REVOLUTION; 
    public static final double STARTING_ANGLE_PULSES = STARTING_ANGLE / 360 * GEAR_RATIO * PULSES_PER_REVOLUTION;
    public static final double SHOOTER_ANGLE_PULSES = SHOOTER_ANGLE / 360 * GEAR_RATIO * PULSES_PER_REVOLUTION;
    public static final double CLIMB_ANGLE_PULSES = CLIMB_ANGLE / 360 * GEAR_RATIO * PULSES_PER_REVOLUTION;
}
