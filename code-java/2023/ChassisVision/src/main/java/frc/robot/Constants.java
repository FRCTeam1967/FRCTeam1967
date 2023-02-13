package frc.robot;
public class Constants {
    //motor controller constants 
    public static final int LEFT_FOLLOWER = 3;
    public static final int LEFT_LEADER = 0;
    public static final int RIGHT_FOLLOWER = 5;
    public static final int RIGHT_LEADER = 1;

    //supply configuration limit constants 
    public static final boolean SUPPLY_CURRENT_ENABLED =  false; 
    public static final int SUPPLY_CURRENT_LIMIT = 10;
    public static final int SUPPLY_CURRENT_THRESHOLD = 15;
    public static final double SUPPLY_CURRENT_TIME = 0.5;

    //deadband values
    public static final double KITBOT_DEADBAND = 0.1;
    public static final double KITBOT_POST_DEADBAND = 0.01;

    //acceleration/speed values
    public static final double SLEW_RATE_CHANGE = 0.5;
  
    //velocity control
    public static final double CHASSIS_RPM = 6380.0;
    public static final int FALCON_PULSES_PER_REVOLUTION = 2048;

    public static final int KPID_LOOP_IDX = 0;
    public static final int KTIMEOUT_MS= 30;

    public static final double VELOCITY_KF_LEFT = 0.036; //Example values: kF: 1023 -> 100% output value , 20660 -> Velocity units at 100% 
    public static final double VELOCITY_KP_LEFT = 3.596;
    public static final double VELOCITY_KI_LEFT = 0.0;
    public static final double VELOCITY_KD_LEFT = 0.0;

    public static final double VELOCITY_KF_RIGHT = 0.037; 
    public static final double VELOCITY_KP_RIGHT = 3.596;
    public static final double VELOCITY_KI_RIGHT = 0.0;
    public static final double VELOCITY_KD_RIGHT = 0.0;
}