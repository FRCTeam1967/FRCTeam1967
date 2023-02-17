package frc.robot;

public class Constants {
    public static final class Encoder {
        public static final int ARM_ABS_ENCODER_ID = 22;
        public static final double ARM_ABS_ENCODER_OFFSET = 85; //degrees, tbd
    };

    public static final class Arm {
        //IDs
        public static final int MOTOR_L_ID = 20;
        public static final int MOTOR_R_ID = 21;

        //arm angles, tbd
        public static final double INTAKE_ANGLE = 0; //left y axis
        public static final double fMIDDLE_ANGLE = 80; //left button
        public static final double fTOP_ANGLE = 100; //left throttle
        public static final double SAFE_ANGLE = 120; //right y axis
        public static final double bTOP_ANGLE = 140; //right throttle
        public static final double bMIDDLE_ANGLE = 160; //right button
        
        //conversion factors
        public static final double GEAR_RATIO = 50.0/1.0; //120 on arm //42/26 (might be wrong)
        public static final double FALCON_TICKS_PER_REVOLUTION = 2048;
        public static final double ABSOLUTE_TICKS_PER_REVOLUTION = 4096;
        
        //pid constants
        public static final int kPIDLoopIdx = 0;
        public static final int K_TIMEOUT_MS = 75; //tbd, based off of 2022
        public static final double kP = 0.5; //tbd
        public static final double kI = 0.0; //known dynamics, tbd
        public static final double kD = 0.8; //tbd
        public static final double kF = 0.0; //feedforward constant, tbd
        public static final double CRUISE_VELOCITY = 1000; //tbd
        public static final double ACCELERATION = 1000; //tbd
        
        //encoder sanity check error constants
        public static final double CHECK_ENCODER_ERROR = 10.0; //tbd, degrees

        //joystick deadbands
        public static final double CONTROLLER_Y_AXIS_DEADBAND = -0.8; //tbd
    }
}