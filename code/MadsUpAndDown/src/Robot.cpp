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

		//SmartDashboard comments
		SmartDashboard::PutNumber("Game Component Encoder: ", upDown->GetEncoderDistance());
		SmartDashboard::PutNumber("Desired Height", upDown -> desiredHeight);
		SmartDashboard::PutNumber("Amount To Move", upDown -> amountToMove);
		SmartDashboard::PutNumber("Distance Per Pulse", upDown -> GetEncoderDistancePerPulse());
		SmartDashboard::PutBoolean("Limit switch top value", upDown -> GetTopLimSwitch());
		SmartDashboard::PutBoolean("Limit switch bottom value", upDown -> GetBottomLimSwitch());

		//Stop the mechanism from going too high/low
		upDown->EmergencyStopMechanism();

		//have mechanism go up to different heights based on what button is pressed
		if (buttonX) {
			upDown -> SwitchHeight();
			upDown -> isMechanismRunning = true;
		}
		else if (buttonY) {
			upDown -> ScaleLowHeight();
			upDown -> isMechanismRunning = true;
		}
		else if (buttonB) {
			upDown -> ScaleMedHeight();
			upDown -> isMechanismRunning = true;
		}
		else if (buttonA) {
			upDown -> ScaleHight();
			upDown -> isMechanismRunning = true;
		}
		else if (buttonRT) {
			upDown -> RegularHeight();
			upDown -> isMechanismRunning = true;
		}

		//Move the Up&Down mechanism to the height it needs to be at
		if (upDown -> isMechanismRunning) {
			upDown->MoveToNewHeight();
		}
	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
