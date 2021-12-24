/*
    AutoSelector.cpp

    Modified on: February 8, 2020
    Authors: Maddie, Kara, Angela, Sofia
*/

// includes
#include <iostream>
#include "frc/smartdashboard/SmartDashboard.h"
#include "frc/smartdashboard/SendableChooser.h"
#include "AutoSelector2021.h"
#include "AutoSettings2021.h"

using namespace std;
using namespace frc;

// paths 
int simplePath = SIMPLE_PATH;
int bouncePath = BOUNCE_PATH;
int barrelPath = BARREL_PATH;
int slalomPath = SLALOM_PATH;
int doNothing = DO_NOTHING;
int testPath1 = TEST_PATH_1;
int testPath2 = TEST_PATH_2;
int testPath3 = TEST_PATH_3;

//robot used:
int usingJankybot = JANKYBOT;
int usingLuca = LUCA;

// create sendable chooser
SendableChooser<int*> autoPaths;
SendableChooser<int*> robotSelector;


AutoSelector::AutoSelector() {
	autoMode = DO_NOTHING; //set default mode to do nothing
	robotUsed = JANKYBOT;
}

AutoSelector::~AutoSelector() {

}

void AutoSelector::DisplayAutoOptions() {
	autoPaths.SetDefaultOption("Do Nothing", &doNothing);
	autoPaths.AddOption("Simple Path", &simplePath);
	autoPaths.AddOption("Bounce Path", &bouncePath);
	autoPaths.AddOption("Barrel Path", &barrelPath);
	autoPaths.AddOption("Slalom Path", &slalomPath);
	autoPaths.AddOption("Test Path 1", &testPath1);
	autoPaths.AddOption("Test Path 2", &testPath2);
	autoPaths.AddOption("Test Path 3", &testPath3);
	SmartDashboard::PutData("Auto Paths", &autoPaths);

	robotSelector.SetDefaultOption("Jankybot", &usingJankybot);
	robotSelector.AddOption("Luca", &usingLuca);
	SmartDashboard::PutData("Selected Robot", &robotSelector);

}

#ifdef USE_DELAY_TIME
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

void AutoSelector::PrintValues() {

	//paths
	printf("simmple path: %d \n", simplePath);
	printf("bounce path: %d \n", bouncePath);
	printf("barrel racing path: %d \n", barrelPath);
	printf("slalom path %d \n", slalomPath);
	printf("do nothing: %d \n", doNothing);
	printf("test path 1: %d \n", testPath1);
	printf("test path 2: %d \n", testPath2);
	printf("test path 3: %d \n", testPath3);

	printf("Jankybot: %d \n", usingJankybot);
	printf("Luca: %d \n", usingLuca);

#ifdef USE_DELAY_TIME
	// delay times
	printf("zero_sec: %d \n", zero_sec);
	printf("one_sec: %d \n", one_sec);
	printf("two_sec: %d \n", two_sec);
	printf("three_sec: %d \n", three_sec);
	printf("four_sec: %d \n", four_sec);
	printf("five_sec: %d \n", five_sec);
#endif

}

int AutoSelector::GetAutoMode() {
	// variables for selected positions
	int* selectedPath = autoPaths.GetSelected();
#ifdef USE_DELAY_TIME
	int* selectedDelay = delay.GetSelected();
#endif

	// print values
	printf("selected path: %d \n", *selectedPath);
#ifdef USE_DELAY_TIME
	printf("selected delay: %d \n", *selectedDelay);
#endif

	// decide what auto mode will be:
	if (selectedPath == &simplePath)
	{
		autoMode = SIMPLE_PATH;
	}
	else if (selectedPath == &bouncePath)
	{
		autoMode = BOUNCE_PATH;
	}
	else if (selectedPath == &barrelPath)
	{
		autoMode = BARREL_PATH;
	}
	else if (selectedPath == &slalomPath)
	{
		autoMode = SLALOM_PATH;	
	}
	else if (selectedPath == &testPath1)
	{
		autoMode = TEST_PATH_1;
	}
	else if (selectedPath == &testPath2)
	{
		autoMode = TEST_PATH_2;
	}
	else if (selectedPath == &testPath3)
	{
		autoMode = TEST_PATH_3;
	}
	else
	{
		autoMode = DO_NOTHING;
	}

	printf("selected auto mode: %d \n", autoMode);

    return autoMode;
}

int AutoSelector::GetSelectedRobot() {
	int* selectedRobot = robotSelector.GetSelected();

	printf ("Selected robot: %d \n", *selectedRobot);

	if (selectedRobot == &usingJankybot){
		robotUsed = JANKYBOT;
	}
	else if (selectedRobot == &usingLuca){
		robotUsed = LUCA;
	}
	else {
		printf("No robot was chosen \n");
	}

	return robotUsed;
}

