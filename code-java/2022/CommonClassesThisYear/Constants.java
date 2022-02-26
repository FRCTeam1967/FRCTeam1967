package org.janksters.CommonClassesThisYear;

//8 values are TBD
public class Constants {
    public static final int PULSES_PER_REVOLUTION = 2048;
    public static final int MOTOR_ID = 8; //TBD
    public static final int CONTROLLER_PORT_ID = 2; //TBD 2022 real
    public static final int TOP_LIMIT_SWITCH_ID = 9; //TBD 2022 real
    public static final int BOTTOM_LIMIT_SWITCH_ID = 8; //TBD

    // PID 
    public static final int kTimeoutMs = 1;
    public static final int kPIDLoopIdx = 0;

    public static final double kP = 0.15;
    public static final double kI = 0.0;
    public static final double kD = 1.0;
    public static final double kF = 0.0;

    /* climb constants */
    public static final double WINCH_MOTOR_UP_FACTOR = 1;
    public static final double WINCH_MOTOR_DOWN_FACTOR = 0.5;
    public static final double WINCH_MOTOR_STOP = 0.0;
    public static final double ROBOT_UP_SPEED_SEQUENCE = 0.5;
    public static final double ROBOT_DOWN_SPEED_SEQUENCE = 0.5;
    public static final double ARM_UP_SPEED_SEQUENCE = 0.2;
    public static final double WINCH_MOTOR_GENTLE = 0.1;
     //to do: figure out distance per revolution
    public static final double LATCH_TIME = 1.0; //check value - in seconds
    public static final double TESTING_TIME = 3.0;
    public static final double WINCH_UP_DISTANCE = 28; //in inches
    public static final int WINCH_FINAL_DISTANCE = 2; //in inches
    public static final double WINCH_ENCODER_DIFFERENCE = 2000;
    public static final double WINCH_ENCODER_DOWN = -2000;
    public static final double HOOKED_ON_CURRENT = 1;
    public static final double TOO_FAR_DOWN = 26;
    public static final double POS_FACTOR = 10;
    public static final double GEAR_RATIO = 34/66;  //motor to winch ratio

    /* climb PID constants */
    public static int testingError = 0; //just for testing

    public static double winchDownKP = 0.15;
    public static double winchDownKI = 0.0;
    public static double winchDownKD = 0.1;
    public static double winchDownKF = 0.0;

    public static double manualWinchKP = 0.15;
    public static double manualWinchKI = 0.0;
    public static double manualWinchKD = 1.0;
    public static double manualWinchKF = 0.0;

    //climb constants
    public static final int WINCH_MOTOR_CHANNEL_L = 1;
    public static final int WINCH_MOTOR_CHANNEL_R = 2;
    public static final int PCM_CHANNEL = 0;
    public static final int MID_LATCH_CHANNEL_L = 2;
    public static final int MID_LATCH_CHANNEL_R = 3;

    //pivot constants
    public static final double INTAKE_ANGLE = 105; 
    public static final double STARTING_ANGLE = 0;
    public static final double SHOOTER_ANGLE = 15; 
    public static final double CLIMB_ANGLE = 85; 

    public static final double INTAKE_ANGLE_PULSES = INTAKE_ANGLE / 360 * GEAR_RATIO * PULSES_PER_REVOLUTION; 
    public static final double STARTING_ANGLE_PULSES = STARTING_ANGLE / 360 * GEAR_RATIO * PULSES_PER_REVOLUTION;
    public static final double SHOOTER_ANGLE_PULSES = SHOOTER_ANGLE / 360 * GEAR_RATIO * PULSES_PER_REVOLUTION;
    public static final double CLIMB_ANGLE_PULSES = CLIMB_ANGLE / 360 * GEAR_RATIO * PULSES_PER_REVOLUTION;
}
