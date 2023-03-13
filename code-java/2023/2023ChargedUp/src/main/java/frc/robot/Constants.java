package frc.robot;

public class Constants {
    public static final double FALCON_TICKS_PER_REVOLUTION = 2048;
    
    public static final class Controllers {
        public static final int LEFT_JOYSTICK_PORT = 0;
        public static final int RIGHT_JOYSTICK_PORT = 1;
        public static final int XBOX_PORT = 2;
    }

    public static final class Intake {
        //neo ids
        public static final int TOP_ROLLER_MOTOR_ID = 31;
        public static final int BOTTOM_ROLLER_MOTOR_ID = 30;

        //intake roller speeds
        public static final double INTAKE_TOP_ROLLER_SPEED = 0.25;
        public static final double INTAKE_BOTTOM_ROLLER_SPEED = 0.25;

        //high row shooting speeds 
        public static final double HIGH_EJECT_TOP_ROLLER_SPEED = 0.35; 
        public static final double HIGH_EJECT_BOTTOM_ROLLER_SPEED = 0.35; 

        //middle row shooting speeds 
        public static final double MIDDLE_EJECT_TOP_ROLLER_SPEED = 0.2; 
        public static final double MIDDLE_EJECT_BOTTOM_ROLLER_SPEED = 0.2; 

        //low row shooting speeds
        public static final double LOW_EJECT_TOP_ROLLER_SPEED = 0.15;
        public static final double LOW_EJECT_BOTTOM_ROLLER_SPEED = 0.15;

        //shooting timer for autonomous routine
        public static final double AUTO_SHOOTER_TIMER_SECONDS = 0.25; 
    }

    public static final class Arm {
        //cancoder
        public static final int ARM_ABS_ENCODER_ID = 22;
        public static final double ARM_ABS_ENCODER_OFFSET = 127; //compbot, degrees

        //encoder sanity check error constants
        public static final double CHECK_ENCODER_ERROR = 5.0; 

        //motor ids
        public static final int MOTOR_L_ID = 20;
        public static final int MOTOR_R_ID = 21;

        //arm angles, tbd
        public static final double INTAKE_ANGLE = 77; 
        public static final double fMIDDLE_ANGLE = 120; //[redacted]
        public static final double fTOP_ANGLE = 150;
        public static final double SAFE_ANGLE = 180;
        public static final double bTOP_ANGLE = 220; 
        public static final double bMIDDLE_ANGLE = 240;  //[redacted]
        
        //conversion factors
        public static final double GEAR_RATIO = 150.0/1.0;
        public static final double ABSOLUTE_TICKS_PER_REVOLUTION = 4096;
        
        //pid constants
        public static final int kPIDLoopIdx = 0;
        public static final int K_TIMEOUT_MS = 75;
        public static final double kP = 0.5; 
        public static final double kI = 0.0; 
        public static final double kD = 0.8; 
        public static final double kF = 0.0; 
        public static final double CRUISE_VELOCITY = 10000;
        public static final double ACCELERATION = 8000;
    }

    public static final class Chassis {
        //joystick sensitivity
        public static final int JOYSTICK_EXP = 1;
        public static final double JOYSTICK_DEADBAND = 0.1;

        //simple vs pid chassis switch
        public static final boolean USE_SIMPLE_CHASSIS = false;

        //chassis motor ids
        public static final int LEFT_LEADER_ID = 4; 
        public static final int LEFT_FOLLOWER_ID = 6; 
        public static final int RIGHT_LEADER_ID = 5; 
        public static final int RIGHT_FOLLOWER_ID = 7; 

        //pid
        public static final int kPIDLoopIdx = 0;
        public static final int K_TIMEOUT_MS = 75;
        public static final double kP = 0.0; //jankybot- 0.061574, compbot- 0.031812
        public static final double kI = 0.0; 
        public static final double kD = 0.0; 
        public static final double kF = 0.15; 

        //speed factor
        public static final double SLOW_MODE_FACTOR = 0.5;

        //conversion factor
        public static final double MAX_RPM_NORMAL = 2000.0;
        public static final double MAX_RPM_SLOW = 1000.0;
        public static final double JOYSTICK_TO_UNITSPER100MS_FACTOR = FALCON_TICKS_PER_REVOLUTION / 600.0;
            //600 is for conversion to per 100ms
    }
    
    public static final class Vision {

    }

    public static final class Auto {
        //selectors
        public static final int ZERO_DELAY = 0;
        public static final int ONE_DELAY = 1;
        public static final int TWO_DELAY = 2;
        public static final int THREE_DELAY = 3;
        public static final int FOUR_DELAY = 4;
        public static final int FIVE_DELAY = 5;

        public static final int SIMPLE_AUTOPATH = 0;
        public static final int ONE_CUBE_AUTOPATH = 1;
        public static final int CHARGE_STATION = 2;

        //Auto PID constants
        public static final double VELOCITY_KP = 0.0; 
        public static final double VELOCITY_KI = 0.0;
        public static final double VELOCITY_KD = 0.0;
        public static final double VELOCITY_KF = 0.15;

        //constants for distance calculation
        public static final double PULSES_PER_REVOLUTION = 4096;
        public static final double WHEEL_DIAMETER = 6; 
        public static final double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * Math.PI;
        public static final double CHASSIS_GEAR_RATIO = ((50.0/14) * (54/20)); //50/14, 40/20 vision bot
    }

    public static final class LED {
        public static final int WIDTH = 1;
        public static final int LENGTH = 300; //tbd
        public static final int PWM_PORT = 0;
    }
}
