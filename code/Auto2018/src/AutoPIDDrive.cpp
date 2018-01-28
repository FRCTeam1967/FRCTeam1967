/*
 * AutoPIDDrive.cpp
 *
 *  Created on: Jan 27, 2018
 *      Author: AnishaKabir
 */
#include "AutoPIDDrive.h"
//#include <AutoPIDDrive.h>
#include "WPIlib.h"
AutoPIDDrive::AutoPIDDrive(RobotDrive*drive) {
	chassis=drive;
}

AutoPIDDrive::~AutoPIDDrive() {
	delete chassis;
}

void AutoPIDDrive::PIDWrite(double val){
	chassis->Drive(0.4, val);
	SmartDashboard::PutNumber("val", val);
	printf("val: %f \n", val);
}
