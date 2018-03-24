/*
 * ReleaseCube.cpp

 *
 *  Created on: Feb 11, 2018
 *      Author: Maxine
 */

#include <ReleaseCube.h>

#include "WPILib.h"

//timings not accurate to be tested
#define ROLL_OUT_TIME 2

Timer*intimer;
double timeToGoUp;

ReleaseCube::ReleaseCube(RobotDrive*drive, InAndOut*inAndOut,UpAndDown*upAndDown, char height) {
	// TODO Auto-generated constructor stub
	chassis=drive;
	IAO = inAndOut;
	UAD = upAndDown;
	_height=height;
	intimer=new Timer();
}

ReleaseCube::~ReleaseCube() {
	// TODO Auto-generated destructor stub
	delete intimer;
}

bool ReleaseCube::JobDone()
{
	if(!UAD->GetIfMechIsRunning() && !timerHasStarted){ //to be tested
		intimer->Start();
		IAO->MotorRollForward();
		IAO->PistonDoorClose(); //flipflopped
		timerHasStarted = true;
	}
	if((intimer->Get()>ROLL_OUT_TIME)){
		return true;
	}
	return false;
}

void ReleaseCube::RunAction()
{

}

void ReleaseCube::Start()
{
	timerHasStarted = false;
	//intimer->Start();
	//IAO->MotorRollForward();
}

void ReleaseCube::End()
{
	IAO->MotorRollStop();
	intimer->Stop();
	intimer->Reset();
	timerHasStarted = false;
}
