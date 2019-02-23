/*
 * TurnSegment.cpp
 *
 *  Created on: Feb 10, 2018
 *      Author: Maxine, Bob W
 */
#include "Settings.h"
#include <DifferentialTurnSegment.h>
#include "frc/WPILib.h"
#define TURN_TIME 4

DifferentialTurnSegment::DifferentialTurnSegment(frc::ADXRS450_Gyro*gyro, frc::DifferentialDrive*chassis, double degreeOfTurn, double speed, double p, double i, double d) {
	_gyro = gyro;
	targetAngle = degreeOfTurn;
	drive = chassis;
	_speed = speed;
	kP = p;
	kI = i;
	kD = d;
	pid = new frc::PIDController(kP,kI,kD,_gyro,this);
	turnTimer = new frc::Timer();
}

DifferentialTurnSegment::~DifferentialTurnSegment() {
	// TODO Auto-generated destructor stub
	pid->Disable();
	delete pid;
	delete turnTimer;
}

void DifferentialTurnSegment::Start()
{
	c = 0;//c for counter
	_gyro->Reset();
	pid->SetInputRange(-180.0, 180.0);
	pid->SetOutputRange(-1.0, 1.0);
	pid->SetSetpoint(targetAngle);
	pid->Enable();
	turnTimer->Start();
}

void DifferentialTurnSegment::PIDWrite(double output)
{
	drive->ArcadeDrive(_speed, output); //may need to change back
}

bool DifferentialTurnSegment::JobDone()
{
	float angle = _gyro->GetAngle();
	if(abs(targetAngle-angle)<=1)
	{
		c++;
	}
	if(turnTimer->Get()>=TURN_TIME){ //in case robot can't complete turn
		return true;
	}
	if(c>3)//possibly change to timer
	{
		return true;
	}
	return false;
}

void DifferentialTurnSegment::RunAction()
{

}

void DifferentialTurnSegment::End()
{
	pid->Disable();
	drive->TankDrive(0.0, 0.0);
	turnTimer->Stop();
	turnTimer->Reset();
}
