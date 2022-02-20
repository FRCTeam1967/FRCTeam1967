package org.janksters.CommonClassesThisYear;

public class Constants {
    /* climb constants */
    public static final double WINCH_MOTOR_SPEED = 0.3;
    public static final double WINCH_MOTOR_STOP = 0.0;
    public static final double WINCH_MOTOR_GENTLE = 0.1;
    public static final double PULSES_PER_REVOLUTION = 2048; //2048 for falcon integrated, 4096 for MAG
    public static final double LATCH_TIME = 1.0; //check value - in seconds
    public static final double TESTING_TIME = 3.0;
    public static final double WINCH_UP_DISTANCE = 28; //in inches
    public static final int WINCH_FINAL_DISTANCE = 2; //in inches
    public static final double WINCH_ENCODER_DIFFERENCE = 2000;
    public static final double WINCH_ENCODER_DOWN = 1000;
    public static final double HOOKED_ON_CURRENT = 1;
    public static final double TOO_FAR_DOWN = 26;
    public static final double POS_FACTOR = 10;
    public static final double GEAR_RATIO = 34/66;  //motor to winch ratio

    /* climb PID constants */
    public static int kTimeoutMs = 1;
    public static int kPIDLoopIdx = 0;
    public static int testingError = 0; //just for testing

    public static double winchDownKP = 0.15;
    public static double winchDownKI = 0.0;
    public static double winchDownKD = 0.1;
    public static double winchDownKF = 0.0;

    public static double manualWinchKP = 0.15;
    public static double manualWinchKI = 0.0;
    public static double manualWinchKD = 1.0;
    public static double manualWinchKF = 0.0;
}
