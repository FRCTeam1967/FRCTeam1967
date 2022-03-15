package org.janksters.CommonClassesThisYear;

public class Constants {
    public static final int FALCON_PULSES_PER_REVOLUTION = 2048;
    
    /* all shooter/lifter/pivot constants */
        public static final int PIVOT_MOTOR_ID = 4;
        public static final int PIVOT_CONTROLLER_PORT_ID = 2; //TBD 2022 real
        public static final int PIVOT_TOP_LIMIT_SWITCH_ID = 9; //TBD 2022 real
        public static final int PIVOT_BOTTOM_LIMIT_SWITCH_ID = 10; //TBD
        public static final double PIVOT_GEAR_RATIO = 100; 

        /* shooter/pivot PID constants */
        public static final int PIVOT_kTimeoutMs = 1;
        public static final int PIVOT_kPIDLoopIdx = 0;
        public static final double PIVOT_kP = 0.5; //0.015
        public static final double PIVOT_kI = 0.0;
        public static final double PIVOT_kD = 3.2;
        public static final double PIVOT_kF = 0.0;

        /* pivot constants */
        public static final double PIVOT_INTAKE_ANGLE = 83; //68
        public static final double PIVOT_STARTING_ANGLE = -3; //0
        public static final double PIVOT_SHOOTER_ANGLE = 0; //-15
        public static final double PIVOT_SHOOTER_MIDANGLE = 10;
        public static final double PIVOT_SHOOTER_LOWANGLE = 20;
        public static final double PIVOT_SHOOTER_LOWESTANGLE = 30;
        public static final double PIVOT_CLIMB_ANGLE = 70; //70

        public static final double PIVOT_INTAKE_ANGLE_PULSES = (PIVOT_INTAKE_ANGLE / 360) * PIVOT_GEAR_RATIO * FALCON_PULSES_PER_REVOLUTION; 
        public static final double PIVOT_STARTING_ANGLE_PULSES = (PIVOT_STARTING_ANGLE / 360) * PIVOT_GEAR_RATIO * FALCON_PULSES_PER_REVOLUTION;
        public static final double PIVOT_SHOOTER_ANGLE_PULSES = (PIVOT_SHOOTER_ANGLE / 360) * PIVOT_GEAR_RATIO * FALCON_PULSES_PER_REVOLUTION;
        public static final double PIVOT_CLIMB_ANGLE_PULSES = (PIVOT_CLIMB_ANGLE / 360) * PIVOT_GEAR_RATIO * FALCON_PULSES_PER_REVOLUTION;

    /* all climb constants */
        public static final double CLIMB_WINCH_MOTOR_UP_FACTOR = -1.0;
        public static final double CLIMB_WINCH_MOTOR_DOWN_FACTOR = 0.5;
        public static final double CLIMB_WINCH_MOTOR_STOP = 0.0;
        public static final double CLIMB_ROBOT_UP_SPEED_SEQUENCE = -0.5;
        public static final double CLIMB_ROBOT_DOWN_SPEED_SEQUENCE = -0.5;
        public static final double CLIMB_ARM_UP_SPEED_SEQUENCE = 0.2;
        public static final double CLIMB_WINCH_MOTOR_GENTLE = 0.1;
        //to do: figure out distance per revolution
        public static final double CLIMB_LATCH_TIME = 1.0; //check value - in seconds TBD
        public static final double CLIMB_TESTING_TIME = 3.0;
        public static final double CLIMB_WINCH_UP_DISTANCE = 28; //in inches TBD
        public static final double CLIMB_WINCH_FINAL_DISTANCE = 2.0; //in inches TBD
        public static final double CLIMB_WINCH_ENCODER_DIFFERENCE = 2000;
        public static final double CLIMB_WINCH_ENCODER_DOWN = -2000;
        public static final double CLIMB_HOOKED_ON_CURRENT = 1;
        public static final double CLIMB_TOO_FAR_DOWN = 26;
        public static final double CLIMB_POS_FACTOR = 10;
        public static final double CLIMB_GEAR_RATIO = 34/66; //motor to winch ratio

        /* climb PID constants */
        //not final because we will use input from smartdashboard to edit them during the testing process, will eventually be final
        public static int CLIMB_TESTING_ERROR = 0; //just for testing
        public static int CLIMB_ALLOWABLE_ERROR = 0;

        public static double CLIMB_WINCH_DOWN_KP = 0.15;
        public static double CLIMB_WINCH_DOWN_KI = 0.0;
        public static double CLIMB_WINCH_DOWN_KD = 0.1;
        public static double CLIMB_WINCH_DOWN_KF = 0.0;

        public static double CLIMB_MANUAL_WINCH_KP = 0.15;
        public static double CLIMB_MANUAL_WINCH_KI = 0.0;
        public static double CLIMB_MANUAL_WINCH_KD = 1.0;
        public static double CLIMB_MANUAL_WINCH_KF = 0.0;

        public static final int CLIMB_K_TIMEOUT_MS = 1;
        public static final int CLIMB_K_PID_LOOP_IDX = 0;

        /* climb channels */
        public static final int CLIMB_PCM_CHANNEL = 0;
        public static final int CLIMB_MID_LATCH_CHANNEL_L = 2;
        public static final int CLIMB_MID_LATCH_CHANNEL_R = 3;
        public static final int CLIMB_WINCH_MOTOR_CHANNEL_R = 5;
        public static final int CLIMB_WINCH_MOTOR_CHANNEL_L = 6;

    /* all chassis constant */
        public static final int CHASSIS_K_PID_LOOP_IDX = 0;
        public static final int CHASSIS_K_TIMEOUT_MS = 30;
        public static final double CHASSIS_kP = 0.1;
        public static final double CHASSIS_kI = 0.0;
        public static final double CHASSIS_kD = 0;
        public static final double CHASSIS_kF = 0.0;//1023.0/20660.0;
        public static final double CHASSIS_Iz = 300;
        public static final double CHASSIS_PeakOut = 1.00;
        public static final double CHASSIS_RPM = 5000; //was 2000






}