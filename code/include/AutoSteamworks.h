/*
 * AutoSteamworks.h
 *
 *  Created on: Mar 29, 2017
 *      Author: Sana
 */

#ifndef INCLUDE_AUTOSTEAMWORKS_H_
#define INCLUDE_AUTOSTEAMWORKS_H_
#include "WPILib.h"
#include "CANTalon.h"


#include "jankyTask.h"

class Autonomous:public JankyTask {
public:
	Autonomous(RobotDrive*drive);
	virtual ~Autonomous();

	bool AutoIsInInit();
	void LineUpTapeMiddlePosition();
	void LineUpTapeLeftPosition();
	void LineUpTapeRightPosition();
	void DriveTowardsTape();
	void PushGearOut();
	void BackUpTape();
	void DrivePastBaseLine();
	void StopAutoDriving();
	void SwitchAutoStates();
	void Run();

private:
	int autoStates;
	bool reachedTape;
	bool gearPushedOut;
	bool linedUpMiddle;
	bool linedUpLeft;
	bool linedUpRight;
	bool stateMachineIsInAutoInit;
	RobotDrive * autoDrive;
};


#endif /* AUTOSTEAMWORKS_H_ */




#endif /* INCLUDE_AUTOSTEAMWORKS_H_ */
