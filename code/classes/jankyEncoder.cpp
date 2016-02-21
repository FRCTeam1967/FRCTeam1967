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
JankyEncoder::JankyEncoder(int encoderOneChannel, int encoderTwoChannel, int motorOneChannel, int motorTwoChannel)
{
	pMotor = new Talon(motorOneChannel);
	pMotor2 = new Talon(motorTwoChannel);
	pEncoder = new Encoder(encoderOneChannel, encoderTwoChannel);
	maxTimer = new Timer();
	maxTimer->Reset();
	this->Reset();
	motorSpeed = RESET_INITMOTORSPEED;
	desiredMaxTime = DEFAULT_MAX_TIME;
	targetcount = 1;
	bEncoding = false;
	bDone = false;
	motorStop = false;
}

/*
 * Destructor
 */
JankyEncoder::~JankyEncoder()
{
	delete pMotor;
	delete pMotor2;
	delete pEncoder;
	delete maxTimer;
}

Talon * JankyEncoder::returnMotor()
{
	return pMotor;
}

Talon * JankyEncoder::returnMotor2()
{
	return pMotor2;
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
	pMotor2->Set(0.0);
	printf("Stopping Motors\n");
}


void JankyEncoder::motorGo()
{
	motorStop = false;
	printf("Setting motorStop to false\n");
}

void JankyEncoder::Stop()
{
	motorStop = true;
	printf("Stopping Motors\n");
}

void JankyEncoder::startMotor()
{
	pMotor->Set(motorSpeed);
	pMotor2->Set(motorSpeed);
}

void JankyEncoder::setSpeed(float desiredSpeed)
{
	motorSpeed = desiredSpeed;
	SmartDashboard::PutNumber ("Set Speed", motorSpeed);
}

void JankyEncoder::SetMaxTime(float maxTime)
{
	desiredMaxTime = maxTime;
}

void JankyEncoder::Reset(void)
{
	stopMotor(); 
	maxTimer->Reset();
	Wait(RESET_WAIT);
	if (pEncoder)
	{
		pEncoder->Reset(); //Mentor Bob says there's more to do
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
		maxTimer->Start();
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
		SmartDashboard::PutNumber ("Encodercount", abs(pEncoder->Get()));
		printf("abs(pEncoder->Get()) %d", abs(pEncoder->Get()));
		if (bEncoding == true)
		{
			if (abs(pEncoder->Get()) >= targetcount || maxTimer->Get() >= desiredMaxTime || motorStop == true)
			{
				bDone = true;
				//printf ("Task targetcount reached\n");
				stopMotor();
				maxTimer->Stop();
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

