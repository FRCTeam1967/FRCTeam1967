/*
 * jankyEncoder.cpp
 *
 *  Created on: Jan 31, 2014
 *      Author: Team1967
 */

#include "WPILib.h"
#include "CANTalon.h"
#include "jankyEncoder.h"
#include "jankyTask.h"

#define RESET_WAIT 0.05

/*
 * Construct instance of an encoder that runs in its own task.
 * 
*/
JankyEncoder::JankyEncoder(int encoderOneChannel, int encoderTwoChannel, int motorOneChannel, int motorTwoChannel)
{
	pMotor = new CANTalon(motorOneChannel);
	pMotor2 = new CANTalon(motorTwoChannel);
	pEncoder = new Encoder(encoderOneChannel, encoderTwoChannel);
	maxTimer = new Timer();
	maxTimer->Reset();
	this->Reset();
	motorSpeed = RESET_INITMOTORSPEED;
	desiredMaxTime = DEFAULT_MAX_TIME;
	targetcount = 1;
	initialVal = 0.0;
	bEncoding = false;
	bDone = false;
	motorStop = false;
	itStopped = false;
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

CANTalon * JankyEncoder::returnMotor()
{
	return pMotor;
}

CANTalon * JankyEncoder::returnMotor2()
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
	motorStop = true;
	itStopped=true;
	//printf("Stopping Motors\n");
}


void JankyEncoder::motorGo() //sets motorStop to false; because motorStop is false, the motor will not stop going
{
	motorStop = false;
	printf("Setting motorStop to false\n");
}


void JankyEncoder::Stop() //sets motorStop to true, so that when the emergency stop button is pressed, the condition in the run function will be met and the motor will stop
{
	printf("Stopping Motors\n");
	motorStop = true;
}

void JankyEncoder::startMotor()
{
	pMotor->Set(motorSpeed);
	pMotor2->Set(motorSpeed);
	itStopped=false;
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
	initialVal = pEncoder->Get();
	printf ("INSIDE GO\n");
	printf("Encoder Start Value Go %d\n", abs(initialVal));
	startMotor();
	maxTimer->Start();
	bEncoding = true;
	printf("bEncoding = true\n");
	return true;
}

bool JankyEncoder::ReverseGo(void)
{
	initialVal = pEncoder->Get();
	printf ("INSIDE REVERSEGO\n");
	printf("Encoder Start Value RevGo %d\n", abs(initialVal));
	startMotor();
	maxTimer->Start();
	bEncoding = true;
	printf("bEncoding = true\n");
	return true;
}

void JankyEncoder::Run(void)
{	
	if (pEncoder)
	{
		SmartDashboard::PutNumber ("Encodercount", abs(pEncoder->Get())/360);

		if (bEncoding == true)
		{
			if ( abs(pEncoder->Get() - initialVal) >= targetcount || maxTimer->Get() >= desiredMaxTime )
			{
				bDone = true;
				stopMotor();
				printf("Encoder Stop Value %d \n", abs(pEncoder->Get()));
				bEncoding = false;
				maxTimer->Stop();
			}
			else if (motorStop == true)
			{
				stopMotor();
			}
			else
			{
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

