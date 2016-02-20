#include "WPILib.h"
#include "JankyScaling.h"
#include "JankyTask.h"
#include "jankyDrivestick.h"
#include "jankyXboxJoystick.h"
#include "jankyEncoder.h"

#define DRIVE_JOYSTICK_PORT 1
#define GC_JOYSTICK_PORT 0
#define SCALING_ENCODER_CHANNELA 1
#define SCALING_ENCODER_CHANNELB 0
#define SCALING_MOTOR_CHANNEL 1
#define SCALING_PISTON_CHANNEL 1

class Robot:public IterativeRobot{

	jankyDrivestick*drivestick;			//creating pointer for jankyDrivestick to be called drivestick
	jankyXboxJoystick*joystick;			//creating pointer for jankyXboxJoystick to be called joystick
	jankyScaling*scaling;				//creating pointer for jankyScaling to be called scaling

public:

	Robot(){
			drivestick = NULL;
			joystick = NULL;
			scaling = NULL;
		}
	~Robot(){
			delete drivestick;
			delete joystick;
			delete scaling;

			drivestick = NULL;
			joystick = NULL;
			scaling = NULL;
		}

private:
	void RobotInit(){
		printf("Made it to RobotInit\n");
		drivestick = new jankyDrivestick(DRIVE_JOYSTICK_PORT);
		joystick = new jankyXboxJoystick(GC_JOYSTICK_PORT);
		scaling = new jankyScaling(SCALING_ENCODER_CHANNELA, SCALING_ENCODER_CHANNELB, SCALING_MOTOR_CHANNEL, SCALING_PISTON_CHANNEL);
	}

	void TeleopInit(){
		printf("Made it to TeleopInit\n");
		scaling->ScalingStart();
	}

	void TeleopPeriodic(){
		if (joystick->GetButtonA() == true){			//scaling Release; when button A on the Xbox controller is pressed, Release will be enabled
			printf("Going to Release now\n");
			scaling->Release();
		}
		if (joystick->GetButtonB() == true){			//scaling WindUp; when button B on the Xbox controller is pressed, WindUp will be enabled
			printf("Going to Wind Up now\n");
			scaling->WindUp();
		}
		if (joystick->GetButtonY() == true){
			printf("Stopping WindUp\n");
			scaling->StopWU();
		}
	}
};

START_ROBOT_CLASS(Robot)
