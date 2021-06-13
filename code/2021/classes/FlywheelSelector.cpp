// includes
#include <iostream>
#include "frc/smartdashboard/SmartDashboard.h"
#include "frc/smartdashboard/SendableChooser.h"
#include "FlywheelSelector.h"
#include "Settings.h"
using namespace std;
using namespace frc;
int shootingZone1 = SHOOTING_ZONE_1;
int shootingZone2 = SHOOTING_ZONE_2;
int shootingZone3 = SHOOTING_ZONE_3;
int shootingZone4 = SHOOTING_ZONE_4;
int shootingDistance50 = SHOOTING_DISTANCE_50;
int shootingDistance75 = SHOOTING_DISTANCE_75;
int shootingDistance100 = SHOOTING_DISTANCE_100;
int shootingDistance125 = SHOOTING_DISTANCE_125;
int shootingDistance150 = SHOOTING_DISTANCE_150;
int shootingDistance175 = SHOOTING_DISTANCE_175;
int shootingDistance200 = SHOOTING_DISTANCE_200;
int shootingDistance225 = SHOOTING_DISTANCE_225;
int shootingDistance250 = SHOOTING_DISTANCE_250;
int shootingDistance275 = SHOOTING_DISTANCE_275;
int shootingDistance300 = SHOOTING_DISTANCE_300;

// create sendable chooser
SendableChooser<int*> shootingZoneSelector;
FlywheelSelector::FlywheelSelector() {
    shootingZone = SHOOTING_ZONE_1;
}
FlywheelSelector::~FlywheelSelector() {
}
void FlywheelSelector::DisplayShootingOptions() {
    shootingZoneSelector.SetDefaultOption("Zone 1", &shootingZone1);
    shootingZoneSelector.AddOption("Zone 2", &shootingZone2);
    shootingZoneSelector.AddOption("Zone 3", &shootingZone3);
    shootingZoneSelector.AddOption("Zone 4", &shootingZone4);
	shootingZoneSelector.AddOption("Distance 50", &shootingDistance50);
	shootingZoneSelector.AddOption("Distance 75", &shootingDistance75);
	shootingZoneSelector.AddOption("Distance 100", &shootingDistance100);
	shootingZoneSelector.AddOption("Distance 125", &shootingDistance125);
	shootingZoneSelector.AddOption("Distance 150", &shootingDistance150);
	shootingZoneSelector.AddOption("Distance 175", &shootingDistance175);
	shootingZoneSelector.AddOption("Distance 200", &shootingDistance200);
	shootingZoneSelector.AddOption("Distance 225", &shootingDistance225);
	shootingZoneSelector.AddOption("Distance 250", &shootingDistance250);
	shootingZoneSelector.AddOption("Distance 275", &shootingDistance275);
	shootingZoneSelector.AddOption("Distance 300", &shootingDistance300);
    SmartDashboard::PutData("Shooting Zones", &shootingZoneSelector);
}
void FlywheelSelector::PrintValues() {
	//paths
	printf("Zone 1: %d \n", shootingZone1);
    printf("Zone 2: %d \n", shootingZone2);
    printf("Zone 3: %d \n", shootingZone3);
    printf("Zone 4: %d \n", shootingZone4);
	printf("Distance 50: %d \n", shootingDistance50);
	printf("Distance 75: %d \n", shootingDistance75);
	printf("Distance 100: %d \n", shootingDistance100);
	printf("Distance 125: %d \n", shootingDistance125);
	printf("Distance 150: %d \n", shootingDistance150);
	printf("Distance 175: %d \n", shootingDistance175);
	printf("Distance 200: %d \n", shootingDistance200);
	printf("Distance 225: %d \n", shootingDistance225);
	printf("Distance 250: %d \n", shootingDistance250);
	printf("Distance 275: %d \n", shootingDistance275);
	printf("Distance 300: %d \n", shootingDistance300);
}
int FlywheelSelector::GetShootingZone() {
	// variables for selected positions
	int* selectedZone = shootingZoneSelector.GetSelected();
	// print values
	printf("selected path: %d \n", *selectedZone);
	// decide what auto mode will be:
	if (selectedZone == &shootingZone1)
	{
		shootingZone = SHOOTING_ZONE_1;
	}
    else if (selectedZone == &shootingZone2)
	{
		shootingZone = SHOOTING_ZONE_2;
	}
	else if (selectedZone == &shootingZone3)
	{
		shootingZone = SHOOTING_ZONE_3;
	}
	else if (selectedZone == &shootingZone4)
	{
		shootingZone = SHOOTING_ZONE_4;
	}
	else if (selectedZone == &shootingDistance50)
	{
		shootingZone = SHOOTING_DISTANCE_50;
	}
	else if (selectedZone == &shootingDistance75)
	{
		shootingZone = SHOOTING_DISTANCE_75;
	}
	else if (selectedZone == &shootingDistance100)
	{
		shootingZone = SHOOTING_DISTANCE_100;
	}
	else if (selectedZone == &shootingDistance125)
	{
		shootingZone = SHOOTING_DISTANCE_125;
	}
	else if (selectedZone == &shootingDistance150)
	{
		shootingZone = SHOOTING_DISTANCE_150;
	}
	else if (selectedZone == &shootingDistance175)
	{
		shootingZone = SHOOTING_DISTANCE_175;
	}
	else if (selectedZone == &shootingDistance200)
	{
		shootingZone = SHOOTING_DISTANCE_200;
	}
	else if (selectedZone == &shootingDistance225)
	{
		shootingZone = SHOOTING_DISTANCE_225;
	}
	else if (selectedZone == &shootingDistance250)
	{
		shootingZone = SHOOTING_DISTANCE_250;
	}
	else if (selectedZone == &shootingDistance275)
	{
		shootingZone = SHOOTING_DISTANCE_275;
	}
	else if (selectedZone == &shootingDistance300)
	{
		shootingZone = SHOOTING_DISTANCE_300;
	}

	printf("selected auto mode: %d \n", shootingZone);
    return shootingZone;
}