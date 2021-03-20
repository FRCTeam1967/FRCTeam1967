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
    SmartDashboard::PutData("Shooting Zones", &shootingZoneSelector);
}
void FlywheelSelector::PrintValues() {
	//paths
	printf("Zone 1: %d \n", shootingZone1);
    printf("Zone 2: %d \n", shootingZone2);
    printf("Zone 3: %d \n", shootingZone3);
    printf("Zone 4: %d \n", shootingZone4);
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
	printf("selected auto mode: %d \n", shootingZone);
    return shootingZone;
}