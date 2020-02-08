/*
    AutoSelector.cpp

    Modified on: February 8, 2020
    Authors: Maddie, Kara
*/

// includes
#include <iostream>
#include "frc/WPILib.h"
#include "AutoSelector.h"

//delay times
#define ZERO_SEC 0
#define ONE_SEC 1
#define TWO_SEC 2
#define THREE_SEC 3
#define FOUR_SEC 4
#define FIVE_SEC 5

//starting places
#define START_LEFT_WALL 1
#define START_RIGHT_WALL 2
#define START_CENTER_FIELD 3
#define START_RIGHT_OF_CENTER_FIELD 4 // centered with target
#define START_LEFT_OF_CENTER_FIELD 5

// step 1
#define CROSS_INITITATION_LINE_FORWARD 1
#define CROSS_INITIATION_LINE_BACKWARD 2

// step 2
#define SHOOT_PRELOADED_BALLS 1
#define DRIVE_TO_RONDEVOUS_POINT 2
#define DRIVE_TO_TRENCH 3

// step 3
#define PICK_UP_BALLS_TRENCH 1
#define PICK_UP_BALLS_RONDEVOUS_POINT 2

// step 4
#define SHOOT_PICKED_UP_BALLS 1

#define DO_NOTHING 6

// delay times
int delayTime = 0;
int zero_sec = ZERO_SEC;
int one_sec = ONE_SEC;
int two_sec = TWO_SEC;
int three_sec = THREE_SEC;
int four_sec = FOUR_SEC;
int five_sec = FIVE_SEC;

// starting positions
int leftStart = START_LEFT_WALL;
int rightStart = START_RIGHT_WALL;
int centerStart = START_CENTER_FIELD;
int leftOfCenterStart = START_LEFT_OF_CENTER_FIELD;
int rightOfCenterStart = START_RIGHT_OF_CENTER_FIELD;

// step 1
int crossLineForward = CROSS_INITITATION_LINE_FORWARD;
int crossLineBackward = CROSS_INITIATION_LINE_BACKWARD;
int doNothing1 = DO_NOTHING;

// step 2
int shootPreloadedBalls = SHOOT_PRELOADED_BALLS;
int driveToRP = DRIVE_TO_RONDEVOUS_POINT;
int driveToTrench = DRIVE_TO_TRENCH;
int doNothing2 = DO_NOTHING;

// step 3
int pickUpBallsT = PICK_UP_BALLS_TRENCH;
int pickUpBallsRP = PICK_UP_BALLS_RONDEVOUS_POINT;
int doNothing3 = DO_NOTHING;

// step 4
int shootPickedUpBalls = SHOOT_PICKED_UP_BALLS;
int doNothing4 = DO_NOTHING;

// create sendable chooser
frc::SendableChooser<int*> startingPosition;
frc::SendableChooser<int*> delay;
frc::SendableChooser<int*> actionStep1;
frc::SendableChooser<int*> actionStep2;
frc::SendableChooser<int*> actionStep3;
frc::SendableChooser<int*> actionStep4;

AutoSelector::AutoSelector() {

}

AutoSelector::~AutoSelector() {

}

void AutoSelector::DisplayAutoOptions() {
    // starting position
    startingPosition.SetDefaultOption("Left Wall", &leftStart);
	startingPosition.AddOption("Right Wall", &rightStart);
	startingPosition.AddOption("Center Field", &centerStart);
    startingPosition.AddOption("Left of Center Field", &leftOfCenterStart);
    startingPosition.AddOption("Right of Center Field (lined up with target)", &rightOfCenterStart);
	frc::SmartDashboard::PutData("Starting Position", &startingPosition);

    // delay times
    delay.SetDefaultOption("0 seconds", &zero_sec);
	delay.AddOption("1 second", &one_sec);
	delay.AddOption("2 seconds", &two_sec);
	delay.AddOption("3 seconds", &three_sec);
	delay.AddOption("4 seconds", &four_sec);
    delay.AddOption("5 seconds", &five_sec);
	frc::SmartDashboard::PutData("Delay", &delay);

    // step 1
    actionStep1.SetDefaultOption("Cross Line Forward", &crossLineForward);
	actionStep1.AddOption("Cross Line Backward", &crossLineBackward);
	actionStep1.AddOption("Do Nothing (1)", &doNothing1);
	frc::SmartDashboard::PutData("Action: Step 1", &actionStep1);

    // step 2
    actionStep2.SetDefaultOption("Shoot Preloaded Balls", &shootPreloadedBalls);
	actionStep2.AddOption("Drive to Rondezvous Point", &driveToRP);
	actionStep2.AddOption("Do Nothing (2)", &doNothing2);
    actionStep2.AddOption("Drive to Trench", &driveToTrench);
	frc::SmartDashboard::PutData("Action: Step 2", &actionStep2);

    // step 3
    actionStep3.SetDefaultOption("Pick Up Balls From the Trench", &pickUpBallsT);
	actionStep3.AddOption("Pick Up Balls From the Rondezvous Point", &pickUpBallsRP);
	actionStep3.AddOption("Do Nothing (3)", &doNothing3);
	frc::SmartDashboard::PutData("Action: Step 3", &actionStep3);

    // step 4
    actionStep4.SetDefaultOption("Shoot Picked Up Balls", &shootPickedUpBalls);
	actionStep4.AddOption("Do Nothing (4)", &doNothing4);
	frc::SmartDashboard::PutData("Action: Step 4", &actionStep4);
}

int AutoSelector::GetDelayTime() {
    if(delay.GetSelected()==&zero_sec){
		return 0;
	}
	else if(delay.GetSelected()==&one_sec){
		return 1;
	}
	else if(delay.GetSelected()==&two_sec){
		return 2;
	}
	else if(delay.GetSelected()==&three_sec){
		return 3;
	}
	else if(delay.GetSelected()==&four_sec){
		return 4;
	}
	else{
		return 0;
	}
}

void AutoSelector::PrintValues() {
	// delay times
	printf("zero_sec: %d \n", zero_sec);
	printf("one_sec: %d \n", one_sec);
	printf("two_sec: %d \n", two_sec);
	printf("three_sec: %d \n", three_sec);
	printf("four_sec: %d \n", four_sec);
	printf("five_sec: %d \n", five_sec);

	// starting positions
	printf("leftStart: %d \n", leftStart);
	printf("rightStart: %d \n", rightStart);
	printf("centerStart: %d \n", centerStart);
	printf("leftOfCenterStart: %d \n", leftOfCenterStart);
	printf("rightOfCenterStart: %d \n", rightOfCenterStart);
	
	// step 1
	printf("crossLineForward: %d \n", crossLineForward);
	printf("crossLineBackward: %d \n", crossLineBackward);
	printf("doNothing1: %d \n", doNothing1);

	// step 2
	printf("shootPreloadedBalls: %d \n", shootPreloadedBalls);
	printf("driveToRP: %d \n", driveToRP);
	printf("driveToTrench: %d \n", driveToTrench);
	printf("doNothing2: %d \n", doNothing2);

	// step 3
	printf("pickUpBallsT: %d \n", pickUpBallsT);
	printf("pickUpBallsRP: %d \n", pickUpBallsRP);
	printf("doNothing3: %d \n", doNothing3);

	// step 4
	printf("shootPickedUpBalls: %d \n", shootPickedUpBalls);
	printf("doNothing4: %d \n", doNothing4);
}

int AutoSelector::GetAutoMode() {
	// variables for selected positions
	int* selectedPosition = startingPosition.GetSelected();
	int* selectedDelay = delay.GetSelected();
	int* selectedAction1 = actionStep1.GetSelected();
	int* selectedAction2 = actionStep2.GetSelected();
	int* selectedAction3 = actionStep3.GetSelected();
	int* selectedAction4 = actionStep4.GetSelected();

	// print values
	printf("selected position: %d \n", *selectedPosition);
	printf("selected delay: %d \n", *selectedDelay);
	printf("selected action 1: %d \n", *selectedAction1);
	printf("selected action 2: %d \n", *selectedAction2);
	printf("selected action 3: %d \n", *selectedAction3);
	printf("selected action 4: %d \n", *selectedAction4);

	// decide what auto mode will be

    return 0; // change later
}