/*
    AutoSelector.cpp

    Modified on: February 8, 2020
    Authors: Maddie, Kara, Angela, Sofia
*/

// includes
#include <iostream>
#include "frc/smartdashboard/SmartDashboard.h"
#include "frc/smartdashboard/SendableChooser.h"
#include "AutoSelector.h"
#include "AutoSettings.h"

using namespace std;
using namespace frc;

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
int shootPreloadedBallsFirst = SHOOT_PRELOADED_BALLS_FIRST;
int doNothing1 = DO_NOTHING;

// step 2
int shootPreloadedBalls = SHOOT_PRELOADED_BALLS;
int driveToRP = DRIVE_TO_RONDEVOUS_POINT;
int driveToTrench = DRIVE_TO_TRENCH;
int doNothing2 = DO_NOTHING;

// step 3
int pickUpBallsT = PICK_UP_BALLS_TRENCH;
int pickUpBallsRP = PICK_UP_BALLS_RONDEVOUS_POINT;
int driveToRP3 = DRIVE_TO_RONDEZVOUSPT_3;
int driveToT3 = DRIVE_TO_TRENCH_3;
int doNothing3 = DO_NOTHING;

// step 4
int shootPickedUpBalls = SHOOT_PICKED_UP_BALLS;
int doNothing4 = DO_NOTHING;

// create sendable chooser
SendableChooser<int*> startingPosition;
SendableChooser<int*> delay;
SendableChooser<int*> actionStep1;
SendableChooser<int*> actionStep2;
SendableChooser<int*> actionStep3;
SendableChooser<int*> actionStep4;

AutoSelector::AutoSelector() {
	autoMode = STAY_STILL; //set default mode to do nothing
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
	SmartDashboard::PutData("Starting Position", &startingPosition);

    // delay times
    delay.SetDefaultOption("0 seconds", &zero_sec);
	delay.AddOption("1 second", &one_sec);
	delay.AddOption("2 seconds", &two_sec);
	delay.AddOption("3 seconds", &three_sec);
	delay.AddOption("4 seconds", &four_sec);
    delay.AddOption("5 seconds", &five_sec);
	SmartDashboard::PutData("Delay", &delay);

    // step 1
    actionStep1.SetDefaultOption("Cross Line Forward", &crossLineForward);
	actionStep1.AddOption("Cross Line Backward", &crossLineBackward);
	actionStep1.AddOption("Shoot Preloaded Balls", &shootPreloadedBallsFirst);
	actionStep1.AddOption("Do Nothing", &doNothing1);
	SmartDashboard::PutData("Action: Step 1", &actionStep1);

    // step 2
    actionStep2.SetDefaultOption("Shoot Preloaded Balls", &shootPreloadedBalls);
	actionStep2.AddOption("Drive to Rondezvous Point", &driveToRP);
	actionStep2.AddOption("Do Nothing", &doNothing2);
    actionStep2.AddOption("Drive to Trench", &driveToTrench);
	SmartDashboard::PutData("Action: Step 2", &actionStep2);

    // step 3
    actionStep3.SetDefaultOption("Pick Up Balls From the Trench", &pickUpBallsT);
	actionStep3.AddOption("Pick Up Balls From the Rondezvous Point", &pickUpBallsRP);
	actionStep3.AddOption("Do Nothing", &doNothing3);
	actionStep3.AddOption("Drive to Trench", &driveToT3);
	actionStep3.AddOption("Drive to Rondezvous Point", &driveToRP3);
	SmartDashboard::PutData("Action: Step 3", &actionStep3);

    // step 4
    actionStep4.SetDefaultOption("Shoot Picked Up Balls", &shootPickedUpBalls);
	actionStep4.AddOption("Do Nothing", &doNothing4);
	SmartDashboard::PutData("Action: Step 4", &actionStep4);
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
	printf("shootPreloadedBallsFirst: %d \n", shootPreloadedBallsFirst);
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
	printf("driveToT3: %d \n", driveToT3);
	printf("driveToRP3: %d \n", driveToRP3);

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

	if((selectedAction1 == &doNothing1) && (selectedAction2 == &doNothing2) &&  (selectedAction3 == &doNothing3) && (selectedAction4 == &doNothing4))
	{
		autoMode = STAY_STILL;
	}
	else if((selectedAction1 == &crossLineForward) && (selectedAction2 == &doNothing2) &&  (selectedAction3 == &doNothing3) && (selectedAction4 == &doNothing4)) 
	{
		autoMode = FORWARD_STILL;
	}
	else if((selectedAction1 == &crossLineBackward) && (selectedAction2 == &doNothing2) &&  (selectedAction3 == &doNothing3) && (selectedAction4 == &doNothing4))
	{
		autoMode = BACKWARD_STILL;
	}
	else if((selectedPosition == &centerStart) && (selectedAction1 == &shootPreloadedBalls) && (selectedAction2 == &doNothing2) && (selectedAction3 == &doNothing3) && (selectedAction4 == &doNothing4))
	{
		autoMode = CENTER_SHOOT;
	}
	else if((selectedPosition == &centerStart) && (selectedAction1 == &crossLineForward) && (selectedAction2 == &shootPreloadedBalls) && (selectedAction3 == &doNothing3) && (selectedAction4 == &doNothing4))
	{
		autoMode = CENTER_FORWARD_SHOOT;
	}
	else if((selectedPosition == &centerStart) && (selectedAction1 == &crossLineBackward) && (selectedAction2 == &shootPreloadedBalls) && (selectedAction3 == &doNothing3) && (selectedAction4 == &doNothing4))
	{
		autoMode = CENTER_BACKWARD_SHOOT;
	}
	else if((selectedPosition == &centerStart) && (selectedAction1 == &shootPreloadedBallsFirst) && (selectedAction2 == &driveToRP) && (selectedAction3 == &pickUpBallsRP) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = CENTER_SHOOT_RONDEZVOUS;
	}
	else if((selectedPosition == &centerStart) && (selectedAction1 == &shootPreloadedBallsFirst) && (selectedAction2 == &driveToTrench) && (selectedAction3 == &pickUpBallsT) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = CENTER_SHOOT_TRENCH;
	}
	else if((selectedPosition == &centerStart) && (selectedAction1 == &crossLineForward) && (selectedAction2 == &driveToRP) && (selectedAction3 == &pickUpBallsRP) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = CENTER_FORWARD_RONDEZVOUS;
	}
	else if((selectedPosition == &leftStart) && (selectedAction1 == &crossLineForward) && (selectedAction2 == &driveToRP) && (selectedAction3 == &pickUpBallsRP) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = LEFT_FORWARD_RONDEZVOUS;
	}
	else if((selectedPosition == &rightStart) && (selectedAction1 == &crossLineForward) && (selectedAction2 == &driveToRP) && (selectedAction3 == &pickUpBallsRP) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = RIGHT_FORWARD_RONDEZVOUS;
	}
	else if((selectedPosition == &leftOfCenterStart) && (selectedAction1 == &crossLineForward) && (selectedAction2 == &driveToRP) && (selectedAction3 == &pickUpBallsRP) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = LC_FORWARD_RONDEZVOUS;
	}
	else if((selectedPosition == &rightOfCenterStart) && (selectedAction1 == &crossLineForward) && (selectedAction2 == &driveToRP) && (selectedAction3 == &pickUpBallsRP) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = RC_FORWARD_RONDEZVOUS;
	}
	else if((selectedPosition == &centerStart) && (selectedAction1 == &crossLineBackward) && (selectedAction2 == &driveToRP) && (selectedAction3 == &pickUpBallsRP) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = CENTER_BACKWARD_RONDEZVOUS;
	}
	else if((selectedPosition == &leftStart) && (selectedAction1 == &crossLineBackward) && (selectedAction2 == &driveToRP) && (selectedAction3 == &pickUpBallsRP) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = LEFT_BACKWARD_RONDEZVOUS;
	}
	else if((selectedPosition == &rightStart) && (selectedAction1 == &crossLineBackward) && (selectedAction2 == &driveToRP) && (selectedAction3 == &pickUpBallsRP) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = RIGHT_BACKWARD_RONDEZVOUS;
	}
	else if((selectedPosition == &leftOfCenterStart) && (selectedAction1 == &crossLineBackward) && (selectedAction2 == &driveToRP) && (selectedAction3 == &pickUpBallsRP) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = LC_BACKWARD_RONDEZVOUS;
	}
	else if((selectedPosition == &rightOfCenterStart) && (selectedAction1 == &crossLineBackward) && (selectedAction2 == &driveToRP) && (selectedAction3 == &pickUpBallsRP) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = RC_BACKWARD_RONDEZVOUS;
	}
	else if((selectedPosition == &centerStart) && (selectedAction1 == &crossLineForward) && (selectedAction2 == &driveToTrench) && (selectedAction3 == &pickUpBallsT) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = CENTER_FORWARD_TRENCH;
	}
	else if((selectedPosition == &leftStart) && (selectedAction1 == &crossLineForward) && (selectedAction2 == &driveToTrench) && (selectedAction3 == &pickUpBallsT) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = LEFT_FORWARD_TRENCH;
	}
	else if((selectedPosition == &rightStart) && (selectedAction1 == &crossLineForward) && (selectedAction2 == &driveToTrench) && (selectedAction3 == &pickUpBallsT) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = RIGHT_FORWARD_TRENCH;
	}
	else if((selectedPosition == &leftOfCenterStart) && (selectedAction1 == &crossLineForward) && (selectedAction2 == &driveToTrench) && (selectedAction3 == &pickUpBallsT) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = LC_FORWARD_TRENCH;
	}
	else if((selectedPosition == &rightOfCenterStart) && (selectedAction1 == &crossLineForward) && (selectedAction2 == &driveToTrench) && (selectedAction3 == &pickUpBallsT) && (selectedAction4 == &shootPickedUpBalls))
	{
		autoMode = RC_FORWARD_TRENCH;
	}
	else if((selectedAction1 == &centerStart) && (selectedAction2 == &driveToTrench) && (selectedAction3 == &pickUpBallsT) && (selectedAction4 == &pickUpBallsRP))
	{
		autoMode = CENTER_BACKWARD_TRENCH;
	}
	else if((selectedAction1 == &leftStart) && (selectedAction2 == &driveToTrench) && (selectedAction3 == &pickUpBallsT) && (selectedAction4 == &pickUpBallsRP))
	{
		autoMode = LEFT_BACKWARD_TRENCH;
	}
	else if((selectedAction1 == &rightStart) && (selectedAction2 == &driveToTrench) && (selectedAction3 == &pickUpBallsT) && (selectedAction4 == &pickUpBallsRP))
	{
		autoMode = RIGHT_BACKWARD_TRENCH;
	}
	else if((selectedAction1 == &leftOfCenterStart) && (selectedAction2 == &driveToTrench) && (selectedAction3 == &pickUpBallsT) && (selectedAction4 == &pickUpBallsRP))
	{
		autoMode = LC_BACKWARD_TRENCH;
	}
	else if((selectedAction1 == &rightOfCenterStart) && (selectedAction2 == &driveToTrench) && (selectedAction3 == &pickUpBallsT) && (selectedAction4 == &pickUpBallsRP))
	{
		autoMode = RC_BACKWARD_TRENCH;
	}
	else if((selectedAction1 == &centerStart) && (selectedAction2 == &shootPreloadedBalls) && (selectedAction3 == &driveToRP3) && (selectedAction4 == &pickUpBallsRP))
	{
		autoMode = CENTER_BACKWARD_SHOOT_RONDEZVOUS;
	}
	else if((selectedAction1 == &rightStart) && (selectedAction2 == &shootPreloadedBalls) && (selectedAction3 == &driveToRP3) && (selectedAction4 == &pickUpBallsRP))
	{
		autoMode = RIGHT_BACKWARD_SHOOT_RONDEZVOUS;
	}
	else if((selectedAction1 == &rightOfCenterStart) && (selectedAction2 == &shootPreloadedBalls) && (selectedAction3 == &driveToRP3) && (selectedAction4 == &pickUpBallsRP))
	{
		autoMode = RC_BACKWARD_SHOOT_RONDEZVOUS;
	}
	else if((selectedAction1 == &centerStart) && (selectedAction2 == &shootPreloadedBalls) && (selectedAction3 == &driveToT3) && (selectedAction4 == &pickUpBallsT))
	{
		autoMode = CENTER_BACKWARD_SHOOT_TRENCH;
	}
	else if((selectedAction1 == &rightStart) && (selectedAction2 == &shootPreloadedBalls) && (selectedAction3 == &driveToT3) && (selectedAction4 == &pickUpBallsT))
	{
		autoMode = RIGHT_BACKWARD_SHOOT_TRENCH;
	}
	else if((selectedAction1 == &rightOfCenterStart) && (selectedAction2 == &shootPreloadedBalls) && (selectedAction3 == &driveToT3) && (selectedAction4 == &pickUpBallsT))
	{
		autoMode = RC_BACKWARD_SHOOT_TRENCH;
	}

	printf("selected auto mode: %d \n", autoMode);

    return autoMode;
}