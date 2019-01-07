/*
 * AutoPIDDrive.h
 *
 *  Created on: Jan 27, 2018
 *      Author: AnishaKabir
 */
#include "frc/WPILib.h"
#ifndef SRC_AUTOPIDDRIVE_H_
#define SRC_AUTOPIDDRIVE_H_

class AutoPIDDrive : public frc::PIDOutput {
public:
	AutoPIDDrive(frc::RobotDrive*drive);
	virtual ~AutoPIDDrive();
	frc::RobotDrive*chassis;
	void PIDWrite(double output);
};

#endif /* SRC_AUTOPIDDRIVE_H_ */
