/*
 * ReleaseCube.cpp

 *
 *  Created on: Feb 11, 2018
 *      Author: Maxine
 */

#include <ReleaseCube.h>

#include "frc/WPILib.h"

//timings not accurate to be tested
#define ROLL_OUT_TIME 2
#define RELEASE_TIME 2

frc::Timer*intimer;
frc::Timer*releasetimer;
double timeToGoUp;

ReleaseCube::ReleaseCube(frc::RobotDrive*drive, InAndOut*inAndOut,UpAndDown*upAndDown, char height) {
	// TODO Auto-generated constructor stub
	chassis=drive;
	IAO = inAndOut;
	UAD = upAndDown;
	_height=height;
	intimer=new frc::Timer();
	releasetimer=new frc::Timer();
}

ReleaseCube::~ReleaseCube() {
	// TODO Auto-generated destructor stub
	delete intimer;
	delete releasetimer;
}

bool ReleaseCube::JobDone()
{
	if(UAD->GetGameMotorEncoderDistance()>=50){
		if(!UAD->GetIfMechIsRunning() && !timerHasStarted && (releasetimer->Get()>RELEASE_TIME)){ //to be tested
			intimer->Start();
			//IAO->MotorRollForward();
			IAO->PistonDoorClose(); //flipflopped
			timerHasStarted = true;
		}
		if((intimer->Get()>ROLL_OUT_TIME)){
			return true;
		}
	}
	else{
		if(!UAD->GetIfMechIsRunning() && !timerHasStarted){ //to be tested
			intimer->Start();
			IAO->MotorRollForward();
			IAO->PistonDoorClose(); //flipflopped
			timerHasStarted = true;
		}
		if((intimer->Get()>ROLL_OUT_TIME)){
			return true;
		}
	}
	return false;
}

void ReleaseCube::RunAction()
{

}

void ReleaseCube::Start()
{
	timerHasStarted = false;
	releasetimer->Start();
	//intimer->Start();
	//IAO->MotorRollForward();
}

void ReleaseCube::End()
{
	IAO->MotorRollStop();
	intimer->Stop();
	intimer->Reset();
	releasetimer->Stop();
	releasetimer->Reset();
	timerHasStarted = false;
}
