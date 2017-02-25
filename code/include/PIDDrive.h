/*
 * SanaPIDDrive.h
 *
 *  Created on: Feb 12, 2017
 *      Author: Sana
 */

#ifndef PIDDRIVE_H_
#define PIDDRIVE_H_
#include "WPIlib.h"

class PIDDrive : public frc::PIDOutput {
public:
	PIDDrive(RobotDrive*drive);
	virtual ~PIDDrive(void);
	RobotDrive*driveRobot;

	void PIDWrite (double val);
};




#endif /* PIDDRIVE_H_ */
