/*
 * TurnSegment.cpp

 *
 *  Created on: Feb 10, 2018
 *      Author: Maxine, Bob W
 */

#include <TurnSegment.h>
#include "WPILib.h"


TurnSegment::TurnSegment(frc::ADXRS450_Gyro*gyro, RobotDrive*chassis, double degreeOfTurn, double speed, double p, double i, double d) {
	_gyro = gyro;
	targetAngle = degreeOfTurn;
	drive = chassis;
	_speed = speed;
	kP = p;
	kI = i;
	kD = d;
	pid = new PIDController(kP,kI,kD,_gyro,this);
}


TurnSegment::~TurnSegment() {
	// TODO Auto-generated destructor stub
	delete pid;
}

void TurnSegment::Start()
{
	c = 0;//c for counter
	_gyro->Reset();
	pid->SetInputRange(-180.0, 180.0);
	pid->SetOutputRange(-1.0, 1.0);
	pid->SetSetpoint(targetAngle);
	pid->Enable();
}

void TurnSegment::PIDWrite(double output)
{
	drive->Drive(_speed, output);
}

bool TurnSegment::JobDone()
{
	float angle = _gyro->GetAngle();

	if(abs(targetAngle-angle)<=1)
	{
		c++;
	}
	if(c>3)//possibly change to timer
	{
		return true;
	}
	return false;
}

void TurnSegment::RunAction()
{

}
void TurnSegment::End()
{
	pid->Disable();
}
