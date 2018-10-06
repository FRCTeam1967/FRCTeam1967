/*
 * CubeUp.cpp
 *
 *  Created on: Mar 3, 2018
 *      Author: AnishaKabir
 */

#include <CubeUp.h>
#define ARM_DOWN_TIME 0.4

Timer*uptimer;
CubeUp::CubeUp(InAndOut*inAndOut,UpAndDown*upAndDown, char height) {
	// TODO Auto-generated constructor stub
	IAO = inAndOut;
	UAD = upAndDown;
	_height = height;
	uptimer = new Timer();
	uptimer->Reset();
}

CubeUp::~CubeUp() {
	// TODO Auto-generated destructor stub
	delete uptimer;
}

void CubeUp::Start(){
	//UAD->StartUpInit();
	uptimer->Start();
	IAO->MotorClawIntoRobot(); //causes it to go in cuz arm encoders don't work
	UAD->Start();

}

bool CubeUp::JobDone(){

	/*if(uptimer->Get()>0.25){
		IAO->MotorClawStop();
	}*/
	if(_height=='l'){
		if(!UAD->GetIfMechIsRunning()){
			UAD->SwitchHeight();
		}
		else if(UAD->GetIfMechIsRunning()&&(uptimer->Get()>=ARM_DOWN_TIME)){
			IAO->MotorClawStop();
			return true;
		}
	}
	else if(_height=='h'){
		if(uptimer->Get()>ARM_DOWN_TIME){
			IAO->MotorClawStop();
		}
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
	IAO->MotorClawStop();
}
