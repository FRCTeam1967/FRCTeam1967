/*
 * AutoPIDDrive.h
 *
 *  Created on: Jan 27, 2018
 *      Author: AnishaKabir
 */
#include "WPIlib.h"
#ifndef SRC_AUTOPIDDRIVE_H_
#define SRC_AUTOPIDDRIVE_H_

class AutoPIDDrive : public frc::PIDOutput {
public:
	AutoPIDDrive(RobotDrive*drive);
	virtual ~AutoPIDDrive();
	RobotDrive*chassis;
	void PIDWrite(double val);
};

#endif /* SRC_AUTOPIDDRIVE_H_ */
