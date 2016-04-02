#include "WPILib.h"
#include "JankyScaling.h"
#include "JankyTask.h"
//#include "jankyDrivestick.h"
#include "jankyXboxJoystick.h"
#include "jankyEncoder.h"

//#define DRIVE_JOYSTICK_PORT 0
#define GC_JOYSTICK_PORT 1
#define SCALING_ENCODER_CHANNELA 2		//will be 2 in final bot
#define SCALING_ENCODER_CHANNELB 3		//will be 3 in final bot
#define SCALING_MOTOR_CHANNELA 7		//will be 7 in final bot
#define SCALING_MOTOR_CHANNELB 8		//will be 8 in final bot
#define SCALING_PISTON_CHANNEL 1

class Robot:public IterativeRobot{

	//jankyDrivestick*drivestick;			//creating pointer for jankyDrivestick to be called drivestick
	jankyXboxJoystick*joystick;			//creating pointer for jankyXboxJoystick to be called joystick
	jankyScaling*scaling;				//creating pointer for jankyScaling to be called scaling
	bool toggle = true;

public:

	Robot(){
			//drivestick = NULL;
			joystick = NULL;
			scaling = NULL;
		}
	~Robot(){
			//delete drivestick;
			delete joystick;
			delete scaling;

			//drivestick = NULL;
			joystick = NULL;
			scaling = NULL;
		}

private:
	void RobotInit(){
		printf("Made it to RobotInit\n");
		//drivestick = new jankyDrivestick(DRIVE_JOYSTICK_PORT);
		joystick = new jankyXboxJoystick(GC_JOYSTICK_PORT);
		scaling = new jankyScaling(SCALING_ENCODER_CHANNELA, SCALING_ENCODER_CHANNELB, SCALING_MOTOR_CHANNELA, SCALING_MOTOR_CHANNELB, SCALING_PISTON_CHANNEL);
	}

	void TeleopInit(){
		printf("Made it to TeleopInit\n");
		scaling->ScalingStart();
	}

	void TeleopPeriodic(){
		if (toggle && joystick->GetButtonX() == true){			//scaling Release; when button A on the Xbox controller is pressed, Release will be enabled
			toggle = false;
			printf("Going to Release now\n");
			scaling->Release();
		}
		else if (toggle && joystick->GetButtonY() == true){			//scaling WindUp; when button B on the Xbox controller is pressed, WindUp will be enabled
			toggle = false;
			printf("Going to Wind Up now\n");
			scaling->LiftUp();
		}
		else if (toggle && joystick->GetButtonA() == true){
			toggle = false;
			printf("Going to Wind Down now\n");
			scaling->DropDn();
		}
		else if (toggle && joystick->GetButtonB() == true){
			toggle = false;
			printf("Stopping WindUp\n");
			scaling->StopWU();
		}
		else if (!joystick->GetButtonX() && !joystick->GetButtonY() && !joystick->GetButtonA() && !joystick->GetButtonB()) {
			toggle = true;
		}
	}
};

START_ROBOT_CLASS(Robot)
