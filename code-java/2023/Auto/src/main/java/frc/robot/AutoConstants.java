package frc.robot;

import edu.wpi.first.math.kinematics.DifferentialDriveKinematics;

public class AutoConstants {
    //defines
    public static final int CHASSIS_LEFT_ENCODER_CHANNEL= 3;
    public static final int FALCON_PULSES_PER_REVOLUTION = 4096; //2048
    public static final int CHASSIS_RIGHT_ENCODER_CHANNEL = 1;

    public static final double CHASSIS_WIDTH = 0.686; //_m
    public static final double WHEEL_DIAMETER_INCHES = 0.1016; //4in in meters
    
    //selector
    public static int ZeroDelay = 0;
    public static int OneDelay = 1;
    public static int  TwoDelay = 2;
    public static int ThreeDelay = 3;
    public static int  FourDelay = 4;
    public static int FiveDelay = 5;

    public static final int SIMPLE_AUTOPATH = 0;
    public static final int ONE_CUBE_AUTOPATH = 1;
    public static final int CHARGE_STATION = 2;

    public static final double kTrackwidth = CHASSIS_WIDTH; //232.9723996;
    public static DifferentialDriveKinematics kDriveKinematics;

    public static final double kEncoderCPR = 4096;
    public static final double kWheelDiameterInches = WHEEL_DIAMETER_INCHES;
    public static final double kEncoderDistancePerPulse =
        // Assumes the encoders are directly mounted on the wheel shafts
        (kWheelDiameterInches * Math.PI) /(kEncoderCPR);

    // These are example values only - DO NOT USE THESE FOR YOUR OWN ROBOT!
    // These characterization values MUST be determined either experimentally or
    // theoretically for *your* robot's drive. The Robot Characterization
    // Toolsuite provides a convenient tool for obtaining these values for your
    // robot.

    //Jankybot (or really chassis in a day bot):
    public static final double jankybotKS = 0.01338; //auto jankybotKS = 1.23_V; //1.52_V;
    public static final double jankybotKV = 9.2755; //auto jankybotKV = 2.29 * 1_V * 1_s / 1_m;  //0.00935 * 1_V * 1_s / 1_m; 
    public static final double jankybotKA = 17.964; //auto jankybotKA = 0.218 * 1_V * 1_s * 1_s / 1_m; //0.000222 * 1_V * 1_s * 1_s / 1_m; 
    //Luca
    public static final double lucaKS = 1.35; //auto lucaKS = 1.35_V; //1.52_V;
    public static final double lucaKV = 2.35; //auto lucaKV = 2.35 * 1_V * 1_s / 1_m;  //0.00935 * 1_V * 1_s / 1_m; 
    public static final double lucaKA = 0.478; //auto lucaKA = 0.478 * 1_V * 1_s * 1_s / 1_m; //0.000222 * 1_V * 1_s * 1_s / 1_m; 

    // Example value only - as above, this must be tuned for your drive!
    //Jankybot
    public static final double jankybotKPDriveVel = 1.1014;//0.0000559;
    public static final double jankybotKDDriveVel =0.0; // 0.000027;
    //Luca
    public static final double lucaKPDriveVel = 0.0000559;
    public static final double lucaKDDriveVel = 0.000027;

    //was originally in a namespace
    public static final double kMaxSpeed = 0.5; //value may not be accurate, type auto mps
    public static final double kMaxAcceleration = 0.25; //value may not be accurate, type auto mps-sqr

    // Reasonable baseline values for a RAMSETE follower in units of meters and
    // seconds
    public static final double kRamseteB = 2;
    public static final double kRamseteZeta = 0.7 ;

    public static final double kDriverControllerPort = 1;


    // constants for distance calculation
    public static final double PULSES_PER_REVOLUTION = 4096;
    public static final double WHEEL_DIAMETER = 6; //4 inches, was 0.1524 m or 6 inches
    public static final double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER * Math.PI;
    public static final double CHASSIS_GEAR_RATIO = ((50.0/14) * (54/20)); //50/14, 40/20 vision bot
 

}