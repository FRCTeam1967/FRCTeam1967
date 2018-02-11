#include <iostream>
#include <string>

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>

#include <jankyXboxJoystick.h>
#include "WPILib.h"
#include "ctre/Phoenix.h"
#include <InAndOut.h>

//Add channel numbers later (THESE ARE NOT ACCURATE)
#define MOTOR_CLAW_CHANNEL 6
#define PISTON_DOOR_LEFT_CHANNEL 0
#define PISTON_DOOR_RIGHT_CHANNEL 1
#define LIM_SWITCH_INSIDE_CHANNEL 1
#define LIM_SWITCH_OUTSIDE_CHANNEL 2
#define GC_XBOX_CHANNEL 0
#define MOTOR_ROLL_CHANNEL 6
#define CLAW_ENCODER_CHANNEL_1 4
#define CLAW_ENCODER_CHANNEL_2 5
//#define PISTON_IN_OUT_1_CHANNEL 2
//#define PISTON_IN_OUT_2_CHANNEL 3

class Robot : public frc::IterativeRobot {
	InAndOut*inOut;
	jankyXboxJoystick*gameJoystick;
	bool lbHasNotBeenPressed = true;
	bool rbHasNotBeenPressed = true;
	bool toggleDoor = true;
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
		inOut = new InAndOut(PISTON_DOOR_LEFT_CHANNEL, PISTON_DOOR_RIGHT_CHANNEL, MOTOR_ROLL_CHANNEL, MOTOR_CLAW_CHANNEL);
		gameJoystick = new jankyXboxJoystick(GC_XBOX_CHANNEL);
	}

	void AutonomousInit() override {

	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {

		// Define all of the buttons/throttles on the joystick
		bool buttonRB = gameJoystick -> GetButtonRB();
		bool buttonLB = gameJoystick -> GetButtonLB();
		float leftValue = gameJoystick -> GetLeftYAxis();
		float rightValue = gameJoystick -> GetRightYAxis();
		//bool buttonBack = gameJoystick ->GetButtonBack();
		//bool buttonStart = gameJoystick -> GetButtonStart();

		//Put claw mechanism up/down based on what limit switches are pressed
		if (buttonRB) {
			if (inOut -> GetLimSwitchInside()) {
				inOut -> MotorClawOutOfRobot();
			}
			else if (inOut -> GetLimSwitchInside() == false){
				inOut -> MotorClawIntoRobot();
			}
		}
		inOut -> MotorClawStopWithLimSwitches();

		// Open/Close the claw's "doors" with pistons
		if (buttonLB && lbHasNotBeenPressed == true){
			if (toggleDoor) {
				inOut->PistonDoorOpen();
			}
			else {
				inOut->PistonDoorClose();
			}
			toggleDoor = !toggleDoor;
			lbHasNotBeenPressed = false;
		}
		else if (!buttonLB && !lbHasNotBeenPressed) {
			lbHasNotBeenPressed = true;
		}

		//Make the rollers go forward and backward:
		if (leftValue > 0.2) {
			inOut -> MotorRollForward();
		}
		else if (leftValue < -0.2) {
			inOut -> MotorRollReverse();
		}
		else {
			inOut -> MotorRollStop();
		}

		//Make the claw mechanism go forward or backward manually --> use this for small movements
		if (rightValue > 0.2) {
			inOut -> MotorClawIntoRobot();
		}
		else if (rightValue <-0.2) {
			inOut -> MotorClawOutOfRobot();
		}
		else {
			inOut -> MotorClawStop();
		}

		// Unused
		/*
		//Move claw mechanism with encoders
		if (buttonBack) {
			inOut -> OutsideDistance();
		}
		else if (buttonStart) {
			inOut -> InsideDistance();
		}

		inOut -> MoveClawMechanism();
		 */

	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
