/*
 * AutoPIDDrive.cpp
 *
 *  Created on: Jan 27, 2018
 *      Author: AnishaKabir
 */
#include "AutoPIDDrive.h"
//#include <AutoPIDDrive.h>
#include "frc/WPIlib.h"
AutoPIDDrive::AutoPIDDrive(frc::RobotDrive*drive) {
	chassis=drive;
}

AutoPIDDrive::~AutoPIDDrive() {

}

void AutoPIDDrive::PIDWrite(double output){
	chassis->Drive(0.4, output);
	frc::SmartDashboard::PutNumber("PID Output", output);
}
