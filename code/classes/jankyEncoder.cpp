/*
 * jankyEncoder.cpp
 *
 *  Created on: Jan 31, 2014
 *      Author: Team1967
 */

#include "WPILib.h"
#include "jankyEncoder.h"
#define RESET_WAIT 0.05

/*
 * Construct instance of an encoder that runs in its own task.
 * 
*/
JankyEncoder::JankyEncoder(int encoderOneChannel, int encoderTwoChannel, int motorOneChannel)
{
	pMotor = new Talon(motorOneChannel);
	pEncoder = new Encoder(encoderOneChannel, encoderTwoChannel);
	this->Reset();
	motorSpeed = RESET_INITMOTORSPEED;
//	printf ("Constructor");
}

/*
 * Destructor
 */
JankyEncoder::~JankyEncoder()
{
	delete pMotor;
	delete pEncoder;
}

Talon * JankyEncoder::returnMotor()
{
	return pMotor;
}

void JankyEncoder::setRevolution(float desiredRevolution)
{
	targetcount = (int)(desiredRevolution * 360);
	SmartDashboard::PutNumber("Set Revolution", targetcount);
}

float JankyEncoder::getRevolution()
{
	return (float)(pEncoder->Get() / 360);
}

void JankyEncoder::stopMotor()
{
	pMotor->Set(0.0);
}

void JankyEncoder::startMotor()
{
	pMotor->Set(motorSpeed);
}

void JankyEncoder::setSpeed(float desiredSpeed)
{
	motorSpeed = desiredSpeed;
	SmartDashboard::PutNumber ("Set Speed", motorSpeed);
}

void JankyEncoder::Reset(void)
{
	stopMotor(); 
	Wait(RESET_WAIT);
	if (pEncoder)
	{
		pEncoder->Reset(); //Mentor Bob says there's more to do
		pEncoder->Start();
	} 
	bDone = false;
	bEncoding = false;
	printf ("RESETTTING\n");

}

bool JankyEncoder::Go(void)
{
	printf ("INSIDE GO\n");
	if (bEncoding == false)
	{
		printf ("bEncoding = false\n");
		startMotor();
		bEncoding = true;
		return true;
	}
	else
	{
		return false;
	}
}

void JankyEncoder::Run(void)
{	
	if (pEncoder)
	{
		//printf ("GoodEncoder\n");
		SmartDashboard::PutNumber ("Encodercount", pEncoder->Get());
		if (bEncoding == true)
		{
			if (pEncoder->Get() >= targetcount)
			{
				bDone = true;
				printf ("Task targetcount reached\n");
				stopMotor();
			}
			else
			{
				printf ("Targetcount NOT reached\n");
				startMotor();
			}
		}
	
	}
	else
	{
		printf ("WARNING: TASK: pEncoder is NULL\n");
		bEncoding = false;
	}
}
bool JankyEncoder::isDone(void)
{
	return bDone; 
}

