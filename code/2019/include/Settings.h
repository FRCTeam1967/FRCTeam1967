#include "frc/WPILib.h"

//#define JANKY_BOT_2019 //COMMENT THIS OUT IF NOT TESTING ON JANKYBOT
#define ROBOT_2019 //COMMENT THIS OUT IF TESTING ON LOLA
#define DRIVE_TEAM_CAM //COMMENT THIS OUT IF CAMERA IS NOT ON BOT

//Motors
#ifdef ROBOT_2019
#define FRONT_LEFT_MOTOR_CHANNEL 1
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 4
#define REAR_RIGHT_MOTOR_CHANNEL 3
#define MOTOR_ROLL_CHANNEL 6 
#define MOTOR_PIVOT_CHANNEL 7 // might be switched w/ motor_roll_channel
#define L_ELEVATOR_MOTOR_CHANNEL 0
#define R_ELEVATOR_MOTOR_CHANNEL 5 
#define ELEVATOR_LIM_SWITCH_BOTTOM_CHANNEL 0 
#define ELEVATOR_LIM_SWITCH_TOP_CHANNEL 0 
#define TOP_HATCH_PISTON 0
#define BOTTOM_CARGO_PISTON 1
#define PISTON_FRONT_CHANNEL 2 //change for real robot
#define PISTON_BACK_CHANNEL 3 //change for real robot
//elevator definitions
#define GEAR_RATIO 7
#define UD_CIRCUMFERENCE 4 
//1.8125 * M_PI
#define THIRD_STAGE_PRESENT 1.88 
//define SPROCKET_INCHES_PER_TOOTH 0.25
//#define SPROCKET_TEETH_PER_REVOLUTION 16

#else //for Lola
#define FRONT_LEFT_MOTOR_CHANNEL 3
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 5
#define REAR_RIGHT_MOTOR_CHANNEL 4
#define MOTOR_ROLL_CHANNEL 8 //doesn't work because Lola has a talon here instead of a victor
#define MOTOR_PIVOT_CHANNEL 7
#define L_ELEVATOR_MOTOR_CHANNEL 6
#define R_ELEVATOR_MOTOR_CHANNEL 1
#define ELEVATOR_LIM_SWITCH_BOTTOM_CHANNEL 2 //doesn't exist on Lola
#define ELEVATOR_LIM_SWITCH_TOP_CHANNEL 3 //doesn't exist on Lola
#define TOP_HATCH_PISTON 5 // claw open and close
#define BOTTOM_CARGO_PISTON 4 //doesn't exist on Lola
#define PISTON_FRONT_CHANNEL 6 //doesn't exist on Lola
#define PISTON_BACK_CHANNEL 7 //doesn't exist on Lola
//elevator definitions
#define GEAR_RATIO 12
#define UD_CIRCUMFERENCE 5.5 //22 teeth & size 25 chain - recalculate for 2019
//1.8125 * M_PI
#define THIRD_STAGE_PRESENT 2
#endif
