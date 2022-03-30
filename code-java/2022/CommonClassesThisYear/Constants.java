//All constants for 2022 season
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
        public static final double PIVOT_kP = 0.95; //0.015
        public static final double PIVOT_kI = 0.0;
        public static final double PIVOT_kD = 3.2;
        public static final double PIVOT_kF = 0.0;

        /* pivot constants */
        public static final double PIVOT_INTAKE_ANGLE = 88; //68, was 86
        public static final double PIVOT_STARTING_ANGLE = -1; //0
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
    // MBR
        public static final double CLIMB_MBR_WINCH_ARM_FACTOR = -0.8; //used to be 0.8
        public static final double CLIMB_MBR_WINCH_ROBOT_FACTOR = 0.45;
        public static final double CLIMB_WINCH_MOTOR_STOP = 0.0;
        public static final double CLIMB_DEADBAND = 0.07;
        public static final int CLIMB_K_TIMEOUT_MS = 1;
        public static final int CLIMB_K_PID_LOOP_IDX = 0;

    //SVR for future
    /*  public static final double CLIMB_WINCH_MOTOR_UP_FACTOR = -1.0;
        public static final double CLIMB_WINCH_MOTOR_DOWN_FACTOR = 0.5;
        public static final double CLIMB_WINCH_MOTOR_STOP = 0.0;
        public static final double CLIMB_WINCH_MOTOR_LOWER_SPEED = -0.2;
        public static final double CLIMB_ROBOT_UP_SPEED_SEQUENCE = -0.3; //to test
        public static final double CLIMB_ROBOT_DOWN_SPEED_SEQUENCE = -0.3;
        public static final double CLIMB_ARM_UP_SPEED_SEQUENCE = 0.2;
        public static final double CLIMB_WINCH_MOTOR_GENTLE = 0.1;
        public static final double WINCH_DIAMETER = 1.5;
        public static final double WINCH_CIRCUMFERENCE = WINCH_DIAMETER * Math.PI;
        public static final double CLIMB_DEADBAND = 0.07;
        //to do: figure out distance per revolution
        public static final double CLIMB_LATCH_TIME = 1.0; //check value - in seconds TBD
        public static final double CLIMB_TESTING_TIME = 3.0;
        public static final double CLIMB_WINCH_UP_DISTANCE = 3; //in inches TBD, was 28, should be neg if encoders reset (8 since it started at 12?)
        public static final double CLIMB_WINCH_FINAL_DISTANCE = 2.0; //in inches TBD
        public static final double CLIMB_WINCH_ENCODER_DIFFERENCE = 1000; //TBD
        public static final double CLIMB_HIGH_BAR_DISTANCE = CLIMB_WINCH_UP_DISTANCE - 2.0; //TBD
        public static final double CLIMB_WINCH_ENCODER_DOWN = -2000;
        public static final double CLIMB_HOOKED_ON_CURRENT = 0; //tbd
        public static final double CLIMB_TOO_FAR_DOWN = 12;
        public static final double CLIMB_POS_FACTOR = 1; //tbd
        public static final double CLIMB_GEAR_RATIO = 0.01717; //1/30 * 34/66, inverse is 52.94
        public static final double CLIMB_ENCODER_RESET_TIMER = 1;
        public static final double CLIMB_ENCODER_RESET_ALLOWABLE_ERROR = 1000;

        // climb PID constants
        //not final because we will use input from smartdashboard to edit them during the testing process, will eventually be final
        public static int CLIMB_TESTING_ERROR = 0; //just for testing
        public static int CLIMB_ALLOWABLE_ERROR = 0;

        public static double CLIMB_WINCH_DOWN_KP = 0.15;
        public static double CLIMB_WINCH_DOWN_KI = 0.0;
        public static double CLIMB_WINCH_DOWN_KD = 0.1;
        public static double CLIMB_WINCH_DOWN_KF = 0.0;

        public static double CLIMB_ROBOT_WEIGHT_KP = 0.5; //was 0.15
        public static double CLIMB_ROBOT_WEIGHT_KI = 0.0;
        public static double CLIMB_ROBOT_WEIGHT_KD = 1.0;
        public static double CLIMB_ROBOT_WEIGHT_KF = 0.0;

        public static final int CLIMB_K_TIMEOUT_MS = 1;
        public static final int CLIMB_K_PID_LOOP_IDX = 0;

        public static final double CLIMB_ROBOT_WEIGHT_PEAK = 0.3;

        // climb channels
        public static final int CLIMB_PCM_CHANNEL = 12;
        public static final int CLIMB_MID_LATCH_CHANNEL = 2; //probably not right
        */
        public static final int CLIMB_WINCH_MOTOR_CHANNEL_L = 6;
        public static final int CLIMB_WINCH_MOTOR_CHANNEL_R = 5;

    /* all chassis constants */
        /* PID */
        public static final int CHASSIS_K_PID_LOOP_IDX = 0;
        public static final int CHASSIS_K_TIMEOUT_MS = 30;
        public static final double CHASSIS_kP = 0.1;
        public static final double CHASSIS_kI = 0.0;
        public static final double CHASSIS_kD = 0;
        public static final double CHASSIS_kF = 0.0; //1023.0/20660.0;
        public static final double CHASSIS_Iz = 300;
        public static final double CHASSIS_PeakOut = 1.00;

        public static final double CHASSIS_RPM = 4250; //was 3750, was 2500, was 5000

        public static final double MAX_TOTAL_CHASSIS_CURRENT = 180;
        public static final double SET_CHASSIS_CURRENT_IF_OVER = (MAX_TOTAL_CHASSIS_CURRENT - 20) / 4;

        /*current limiting*/
        public static final boolean ENABLE_CHASSIS_CURRENT_LIMIT = true;
        public static final double CHASSIS_CURRENT_LIMIT_MAX = 40;
        public static final double CHASSIS_TRIGGER_CURRENT = 35;
        public static final double CHASSIS_THRESHOLD_TIME = 0;

        /* channel numbers */
        public static final int CHASSIS_L_LEADER_CHANNEL = 2;
        public static final int CHASSIS_L_FOLLOWER_CHANNEL = 3;
        public static final int CHASSIS_R_LEADER_CHANNEL = 1;
        public static final int CHASSIS_R_FOLLOWER_CHANNEL = 0;
}
