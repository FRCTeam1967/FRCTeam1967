package frc.robot;
public class Constants {
    public static final int LEFT_FOLLOWER = 2;
    public static final int LEFT_LEADER = 1;
    public static final int RIGHT_FOLLOWER = 3;
    public static final int RIGHT_LEADER = 0;

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

}