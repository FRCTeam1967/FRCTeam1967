/*
 * AutoSteamworks.cpp
 *
 *  Created on: Mar 29, 2017
 *      Author: Sana
 */
#include "WPILib.h"
#include "AutoSteamworks.h"
#include "CANTalon.h"

#define INIT 0
#define LEFT 1
#define MIDDLE 2
#define RIGHT 3
#define PUSH_GEAR 4
#define BACK_UP 5
#define CROSS_BASE_LINE 6
#define STOP 7


Autonomous::Autonomous(RobotDrive*drive) {
	autoStates = STOP;
	reachedTape = false;
	gearPushedOut = false;
	linedUpMiddle = false;
	linedUpLeft = false;
	linedUpRight = false;
	stateMachineIsInAutoInit = true;
	autoDrive = drive;
}


Autonomous::~Autonomous() {

}


bool Autonomous::AutoIsInInit() {
	autoStates = STOP;
	return stateMachineIsInAutoInit;
}


void Autonomous::LineUpTapeMiddlePosition() {
	stateMachineIsInAutoInit = false;
	//Put in code to line up with the tape in the middle position
	autoStates = MIDDLE;
	linedUpMiddle = true;
}


void Autonomous::LineUpTapeLeftPosition() {
	stateMachineIsInAutoInit = false;
	//Put in code to line up with the tape in the left position
	autoStates = LEFT;
	linedUpLeft = true;
}


void Autonomous::LineUpTapeRightPosition() {
	stateMachineIsInAutoInit = false;
	//Put in code to line up with the tape in the right position
	autoStates = RIGHT;
	linedUpRight = true;
}
/*
void Autonomous::DriveTowardsTape() {
	if (linedUpMiddle == true) {
		//Put in code to drive towards the tape
	}
	if (linedUpLeft == true) {
		//Put in code to drive towards the tape
	}
	if (linedUpRight == true) {
		//Put in code to drive towards the tape
	}
	reachedTape = true;
}
void Autonomous::PushGearOut() {
	if (reachedTape == true) {
		//Put in code to push gear out
		gearPushedOut = true;
	}
	else {
		//Put in code to push pistons back in
		gearPushedOut = false;
	}
}


void Autonomous::BackUpTape() {
	if (gearPushedOut == true)  {
		//Put in code to back up
	}
}

*/
void Autonomous::DrivePastBaseLine() {
	stateMachineIsInAutoInit = false;
	if (linedUpMiddle) { //name of position is subject to change
		//put in code to drive past base line
	}
	else if (linedUpLeft) { //name of position is subject to change
		//put in code to drive past base line
	}
	else if (linedUpRight) { //name of position is subject to change
		//put in code to drive past base line
	}
}


void Autonomous::StopAutoDriving() {
	autoDrive -> Drive(0.0, 0.0);
}


void Autonomous::SwitchAutoStates() {
	switch (autoStates) {
	case LEFT:
		LineUpTapeLeftPosition();
		autoStates = PUSH_GEAR;
		break;
	case MIDDLE:
		LineUpTapeMiddlePosition();
		autoStates = PUSH_GEAR;
		break;
	case RIGHT:
		LineUpTapeRightPosition();
		autoStates = PUSH_GEAR;
		break;
	case PUSH_GEAR:
		PushGearOut();
		autoStates = BACK_UP;
		break;
	case BACK_UP:
		BackUpTape();
		autoStates = CROSS_BASE_LINE;
		break;
	case CROSS_BASE_LINE:
		DrivePastBaseLine();
		autoStates = STOP;
		break;
	case STOP:
		StopAutoDriving();
	}
}


void Autonomous::Run()
{
	SwitchAutoStates();
}



