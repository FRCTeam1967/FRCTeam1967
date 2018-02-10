/*
 * DriveSegment.h
 *
 *  Created on: Feb 8, 2018
 *      Author: AnishaKabir
 */

#ifndef DRIVESEGMENT_H_
#define DRIVESEGMENT_H_
#include "JankyAutoEntry.h"
#include "ctre/Phoenix.h"

class DriveSegment : public JankyAutoEntry{
public:
	DriveSegment(RobotDrive*drive, SensorCollection*leftEncoder, SensorCollection*rightEncoder, int inchDistance);
	virtual ~DriveSegment();
	RobotDrive*chassis;
	SensorCollection*_leftEncoder;
	SensorCollection*_rightEncoder;
	bool JobDone();
	void RunAction();
	void Start();
	void End();
};

#endif /* DRIVESEGMENT_H_ */
