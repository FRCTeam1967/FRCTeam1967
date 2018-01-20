#include <iostream>
#include <string>

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
//We added these includes:
#include <jankyXboxJoystick.h>
#include "WPILib.h"
#include "ctre/Phoenix.h"
#include <UpAndDown.h>

//These are NOT ACCURATE for the competition robot
#define L_MOTOR_CHANNEL 1
#define R_MOTOR_CHANNEL 2
#define LIM_SWITCH_ONE_CHANNEL 3
#define LIM_SWITCH_TWO_CHANNEL 4
#define GAME_MOTOR_ENCODER_CHANNEL_1 5
#define GAME_MOTOR_ENCODER_CHANNEL_2 6

class Robot : public frc::IterativeRobot {
	jankyXboxJoystick*gameJoystick;
	UpAndDown*upDown;
//	double newDistance;
//	double amountToMove;
public:


	Robot() {
		upDown = new UpAndDown(L_MOTOR_CHANNEL, R_MOTOR_CHANNEL, LIM_SWITCH_ONE_CHANNEL, LIM_SWITCH_TWO_CHANNEL, GAME_MOTOR_ENCODER_CHANNEL_1, GAME_MOTOR_ENCODER_CHANNEL_2);
		gameJoystick = NULL;
		upDown = NULL;
	}
	~Robot(){
		delete gameJoystick;
		delete upDown;
	}
	void RobotInit() {

	}


	void AutonomousInit() override {

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
		bool buttonBack = gameJoystick -> GetButtonBack();

		//have mechanism go up to different heights based on what button is pressed
		if (buttonX) {
			upDown -> SwitchHeight();
		}
		else if (buttonY) {
			upDown -> ScaleLowHeight();
		}
		else if (buttonA) {
			upDown -> ScaleMedHeight();
		}
		else if (buttonB) {
			upDown -> ScaleHight();
		}
		else if (buttonBack) {
			upDown -> RegularHeight();
		}

		upDown->MoveToNewHeight();
		upDown->EmergencyStopMechanism();

	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
