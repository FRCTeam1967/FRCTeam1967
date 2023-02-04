package frc.robot;

public class Constants {
    public static final class Encoder {
        public static final int canID = 0;
    };

    public static final class Arm {
        //arm angles, tbd
        public static final double SAFE_ANGLE = 1000;
        public static final double INTAKE_ANGLE = 1000;
        public static final double fTOP_ANGLE = 1000;
        public static final double fMIDDLE_ANGLE = 1000;
        public static final double bTOP_ANGLE = 1000; 
        public static final double bMIDDLE_ANGLE = 1000; 
        
        //conversion factors
        public static final int ARM_GEAR_RATIO = 110;
        public static final double FALCON_TICKS_PER_REVOLUTION = 2048;
        public static final double ABSOLUTE_TICKS_PER_REVOLUTION = 4096;
        
        //pid constants
        public static final int ARM_kPIDLoopIdx = 0;
        public static final int ARM_K_TIMEOUT_MS = 75; //tbd, based off of 2022
        public static final double ARM_kP = 0.5; //tbd
        public static final double ARM_kI = 0.0; //known dynamics, tbd
        public static final double ARM_kD = 0.0; //tbd
        public static final double ARM_kF = 0.0; //feedforward constant, tbd
        public static final double ARM_CRUISE_VELOCITY = 1000; //tbd
        public static final double ARM_ACCELERATION = 1000; //tbd

        //encoder sanity check error constants
        public static final double ARM_CHECK_ENCODER_ERROR = 0.0; //tbd

        //joystick deadbands
        public static final double CONTROLLER_Y_AXIS_DEADBAND = 0.8; //tbd
    }
}