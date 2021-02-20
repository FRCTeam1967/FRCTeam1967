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
#include "AutoSettings2021.h"

using namespace std;
using namespace frc;

// paths 
int simplePath = SIMPLE_PATH;
int bouncePath = BOUNCE_PATH;
int barrelPath = BARREL_PATH;
int slalomPath = SLALOM_PATH;
int doNothing = DO_NOTHING;

// create sendable chooser
SendableChooser<int*> autoPaths;


AutoSelector::AutoSelector() {
	autoMode = DO_NOTHING; //set default mode to do nothing
}

AutoSelector::~AutoSelector() {

}

void AutoSelector::DisplayAutoOptions() {
	autoPaths.SetDefaultOption("Do Nothing", &doNothing);
	autoPaths.AddOption("Simple Path", &simplePath);
	autoPaths.AddOption("Bounce Path", &bouncePath);
	autoPaths.AddOption("Barrel Path", &barrelPath);
	autoPaths.AddOption("Slalom Path", &slalomPath);
	SmartDashboard::PutData("Auto Paths", &autoPaths);


}

#if 0
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
#endif

#if 0
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
#endif

int AutoSelector::GetAutoMode() {
#if 0
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
#endif 

	return 0;
}

