/*
 * CubeUp.cpp
 *
 *  Created on: Mar 3, 2018
 *      Author: AnishaKabir
 */

#include <CubeUp.h>
#define ARM_DOWN_TIME 1

Timer*uptimer;
CubeUp::CubeUp(InAndOut*inAndOut,UpAndDown*upAndDown, char height) {
	// TODO Auto-generated constructor stub
	IAO = inAndOut;
	UAD = upAndDown;
	_height = height;
	uptimer = new Timer();
}

CubeUp::~CubeUp() {
	// TODO Auto-generated destructor stub
	delete uptimer;
}

void CubeUp::Start(){
	uptimer->Start();
	IAO->MotorClawIntoRobot(); //causes it to go in cuz arm encoders don't work
	UAD->Start();
}

bool CubeUp::JobDone(){
	/*if(uptimer->Get()>ARM_DOWN_TIME){
		IAO->MotorClawStop();
	}*/
	if(_height=='l'){
		if(UAD->GetIfMechIsRunning()){
			return true;
		}
	}
	else if(_height=='h'){
		if(!UAD->GetIfMechIsRunning()){
			return true;
		}
	}
	return false;
}

void CubeUp::RunAction(){
	if(_height=='l')
	{
		UAD->SwitchHeight();
	}
	else if(_height=='m')
	{
		UAD->ScaleMedHeight();
	}
	else if(_height=='h')
	{
		UAD->ScaleHight();
	}
}

void CubeUp::End(){
	//IAO->MotorClawStop();
	uptimer->Stop();
	uptimer->Reset();
}
