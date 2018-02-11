/*
 * DriveSegment.h

 *
 *  Created on: Feb 8, 2018
 *      Author: Anisha Kabir
 */

#ifndef DRIVESEGMENT_H_
#define DRIVESEGMENT_H_
#include "JankyAutoEntry.h"
#include "ctre/Phoenix.h"

class DriveSegment : public JankyAutoEntry{
public:
	DriveSegment(RobotDrive*drive, SensorCollection*leftEncoder, SensorCollection*rightEncoder, int inchDistance, double speed);
	virtual ~DriveSegment();

protected:
	/**
	 * @brief Returns whether the turn is complete or not
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	bool JobDone();

	/**
	 * @brief is called repeatedly by jankyAutoEntry by IsComplete()
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	void RunAction();

	/**
	 * @brief One time action for preparing or starting
	 * sets the encoders
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	void Start();

	/**
	 * @brief disable the chassis
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	void End();

private:
	RobotDrive*chassis;
	SensorCollection*_leftEncoder;
	SensorCollection*_rightEncoder;
	int distance;
	double _speed;
};

#endif /* DRIVESEGMENT_H_ */
