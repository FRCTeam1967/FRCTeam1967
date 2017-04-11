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
#include "PIDVision.h"
#include "GearsFuel.h"

#include "jankyTask.h"

class Autonomous:public JankyTask {
public:
	Autonomous(RobotDrive*drive, PIDVision*pv, GearsFuel*gefu);
	virtual ~Autonomous();

	bool AutoIsInInit();
	void StartVisionDriving();
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
	void AutoStop();


private:
	int autoStates;
	bool reachedTape;
	bool gearPushedOut;
	bool linedUpMiddle;
	bool linedUpLeft;
	bool linedUpRight;
	bool stateMachineIsInAutoInit;
	RobotDrive * autoDrive;
	PIDVision * pV;
	GearsFuel * geFu;
	Timer * autoTimer;
};


#endif /* AUTOSTEAMWORKS_H_ */




