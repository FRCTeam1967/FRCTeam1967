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
#include <InAndOut.h>

//Add channel numbers later (THESE ARE NOT ACCURATE)
#define MOTOR_CLAW_CHANNEL 2
//#define PISTON_PUSH_CHANNEL 2
#define PISTON_DOOR_LEFT_CHANNEL 2
#define PISTON_DOOR_RIGHT_CHANNEL 3
#define LIM_SWITCH_UP_CHANNEL 6
#define LIM_SWITCH_DOWN_CHANNEL 9
#define GC_XBOX_CHANNEL 0
#define MOTOR_ROLL_CHANNEL 2

class Robot : public frc::IterativeRobot {
	InAndOut*inOut;
	jankyXboxJoystick*gameJoystick;
	bool lbHasNotBeenPressed = true;
	bool rbHasNotBeenPressed = true;
	bool clawShouldGoUp;
	bool clawShouldGoDown;
public:
	Robot() {
		inOut = NULL;
		gameJoystick = NULL;
	}
	~Robot(){
		delete inOut;
		delete gameJoystick;
	}
	void RobotInit() {
		inOut = new InAndOut(MOTOR_CLAW_CHANNEL, PISTON_DOOR_RIGHT_CHANNEL, PISTON_DOOR_LEFT_CHANNEL, LIM_SWITCH_UP_CHANNEL, LIM_SWITCH_DOWN_CHANNEL, MOTOR_ROLL_CHANNEL);
		gameJoystick = new jankyXboxJoystick(GC_XBOX_CHANNEL);
	}

	void AutonomousInit() override {

	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {

		//		Define all of the buttons on the joystick
		bool buttonRB = gameJoystick -> GetButtonRB();
		bool buttonLB = gameJoystick -> GetButtonLB();

		//		Open/Close the claw's "doors" with pistons
		if (buttonLB && lbHasNotBeenPressed == true){
			inOut->PistonDoorOpen(); 	//if button B is pressed & B has not been pressed before, the piston will go out
			lbHasNotBeenPressed = false; //Makes it so that b has now been pressed
		}
		else if (buttonLB && lbHasNotBeenPressed == false) {
			inOut->PistonDoorClose(); 	//if button B is pressed & B has been pressed before, the piston will go back in
			lbHasNotBeenPressed = true; //Makes it so that b has not been pressed
		}

		//Put claw mechanism up/down based on what limit switches are pressed
		if (buttonRB && rbHasNotBeenPressed) {
			clawShouldGoDown = true;
			rbHasNotBeenPressed = false;
		}
		else if (buttonRB && rbHasNotBeenPressed == false) {
			clawShouldGoUp = true;
			rbHasNotBeenPressed = true;
		}

		if (clawShouldGoDown && (inOut -> GetLimSwitchDown() == false)) {
			inOut -> MotorClawForward();
			clawShouldGoDown = false;
		}
		else if (clawShouldGoUp && (inOut -> GetLimSwitchUp() == false)) {
			inOut -> MotorClawReverse();
			clawShouldGoUp = false;
		}

		//Make the rollers go forward and backward:
		float leftValue = gameJoystick -> GetLeftYAxis();

		if (leftValue > 0.2) {
			inOut -> MotorRollForward();
		}
		else if (leftValue < -0.2) {
			inOut -> MotorRollReverse();
		}
		else {
			inOut -> MotorRollStop();
		}

	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
