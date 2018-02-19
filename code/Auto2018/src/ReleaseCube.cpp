/*
 * ReleaseCube.cpp

 *
 *  Created on: Feb 11, 2018
 *      Author: Maxine
 */

#include <ReleaseCube.h>
#include"InAndOut.h"
#include "UpAndDown.h"

ReleaseCube::ReleaseCube(InAndOut*inAndOut,UpAndDown*upAndDown, char height) {
	// TODO Auto-generated constructor stub
	IAO = inAndOut;
	UAD = upAndDown;
	_height=height;
}

ReleaseCube::~ReleaseCube() {
	// TODO Auto-generated destructor stub

}

void ReleaseCube::Start()
{
	upDone = false;
	downDone = false;
	IAO->MotorClawOutOfRobot();
	UAD->Start();
	if(_height=='l')
	{
		UAD->ScaleLowHeight();
	}
	if(_height=='m')
	{
		UAD->ScaleMedHeight();
	}
	if(_height=='h')
	{
		UAD->ScaleHight();
	}
	//ask maddie to write return method

}
void ReleaseCube::End()
{

}

bool ReleaseCube::JobDone()
{
	if(!UAD->GetIfMechIsRunning())
	{
		upDone = true;
	}
	if(upDone)
	{
		UAD->RegularHeight();
		if(!UAD->GetIfMechIsRunning())
			{
				downDone = true;
			}
		return true;
	}
	return false;
}

void ReleaseCube::RunAction()
{

}

