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
	PIDDrive(SpeedController * motor1, SpeedController * motor2,SpeedController * motor3,SpeedController * motor4);
	virtual ~PIDDrive(void);
	RobotDrive*driveRobot;

	void PIDWrite (double val);
};




#endif /* PIDDRIVE_H_ */
