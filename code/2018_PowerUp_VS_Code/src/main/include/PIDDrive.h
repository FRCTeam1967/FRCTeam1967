/*
 * SanaPIDDrive.h
 *
 *  Created on: Feb 12, 2017
 *      Author: Sana
 */

#ifndef PIDDRIVE_H_
#define PIDDRIVE_H_
#include "frc/WPILib.h"

class PIDDrive : public frc::PIDOutput {
public:
	PIDDrive(frc::RobotDrive*drive);
	virtual ~PIDDrive(void);
	frc::RobotDrive*driveRobot;

	void PIDWrite (double val);
};




#endif /* PIDDRIVE_H_ */
