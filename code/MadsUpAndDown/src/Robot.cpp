#include <iostream>
#include <string>

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>

//We added these includes:
#include <jankyXboxJoystick.h>
#include "WPILib.h"
#include "jankyTask.h"
#include "ctre/Phoenix.h"
#include <UpAndDown.h>

//These are NOT ACCURATE for the competition robot
#define L_MOTOR_CHANNEL 2
#define R_MOTOR_CHANNEL 2
#define BOTTOM_LIM_SWITCH_CHANNEL 6
#define TOP_LIM_SWITCH_CHANNEL 9
#define GAME_MOTOR_ENCODER_CHANNEL_1 4
#define GAME_MOTOR_ENCODER_CHANNEL_2 5

#define GC_XBOX_CHANNEL 0

class Robot : public frc::IterativeRobot {
	jankyXboxJoystick*gameJoystick;
	UpAndDown*upDown;
public:

	Robot() {
		gameJoystick = NULL;
		upDown = NULL;
	}
	~Robot(){
		delete gameJoystick;
		delete upDown;
	}
	void RobotInit() {
		upDown = new UpAndDown(L_MOTOR_CHANNEL, R_MOTOR_CHANNEL, BOTTOM_LIM_SWITCH_CHANNEL, TOP_LIM_SWITCH_CHANNEL, GAME_MOTOR_ENCODER_CHANNEL_1, GAME_MOTOR_ENCODER_CHANNEL_2);
		gameJoystick = new jankyXboxJoystick(GC_XBOX_CHANNEL);
		upDown->ResetEncoder();
	}


	void AutonomousInit() override {
		upDown->Start();
	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {
	}

	void TeleopPeriodic() {
		//assign button variables to the corresponding buttons on the joystick
		bool buttonX = gameJoystick -> GetButtonX();
		bool buttonY = gameJoystick -> GetButtonY();
		bool buttonA = gameJoystick -> GetButtonA();
		bool buttonB = gameJoystick -> GetButtonB();
		bool buttonRT = gameJoystick -> GetRightThrottle();

		//Display SmartDashboard Comments on the driver station
		upDown -> SmartDashboardComments();

		//have mechanism go up to different heights based on what button is pressed
		if (buttonX) {
			upDown -> SwitchHeight();
		}
		else if (buttonY) {
			upDown -> ScaleLowHeight();
		}
		else if (buttonB) {
			upDown -> ScaleMedHeight();
		}
		else if (buttonA) {
			upDown -> ScaleHight();
		}
		else if (buttonRT) {
			upDown -> RegularHeight();
		}
	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
