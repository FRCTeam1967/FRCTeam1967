package frc.robot;

public class Constants {
    public static final class Chassis {

        //simple vs pid chassis switch
        public static final boolean USE_SIMPLE_CHASSIS = false;

        //jankybot chassis motor ids
        public static final int LEFT_LEADER_ID = 4; //tbd
        public static final int LEFT_FOLLOWER_ID = 6; //tbd
        public static final int RIGHT_LEADER_ID = 5; //tbd
        public static final int RIGHT_FOLLOWER_ID = 7; //tbd

        //pid
        public static final int kPIDLoopIdx = 0;
        public static final int K_TIMEOUT_MS = 75;

        //conversion factor
        public static final double MAX_RPM_NORMAL = 6380.0;
        public static final double MAX_RPM_SLOW = 1000.0;
        public static final double MAX_RPM_FAST = 7000.0;
        public static final double JOYSTICK_TO_UNITSPER100MS_FACTOR = 2048.0 / 600.0;
            //2048 is falcon pulses per rev, will be replaced with constant when we merge
            //600 is for conversion to per 100ms
    }


}
