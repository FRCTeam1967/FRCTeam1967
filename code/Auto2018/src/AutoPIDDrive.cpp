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

}

void AutoPIDDrive::PIDWrite(double output){
	chassis->Drive(0.4, output);
	SmartDashboard::PutNumber("PID Output", output);
}
