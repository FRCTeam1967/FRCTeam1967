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
	pMotor = new CANTalon(motorOneChannel);
	pMotor2 = new CANTalon(motorTwoChannel);
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
	directionCheck = false;
	hasStopBeenPressed = false;
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
	printf("Stopping Motors\n");
}


void JankyEncoder::motorGo() //sets motorStop to false; because motorStop is false, the motor will not stop going
{
	motorStop = false;
	printf("Setting motorStop to false\n");
}

void JankyEncoder::stopCheck() //sets hasBeenPressed to true; shows that the emergency stop button has been pressed
{
	hasStopBeenPressed = true;
}

void JankyEncoder::reverseWindCheck() //reverses the value of windCheck which is a label for what type of wind was used (windUP vs. windDN)
{
	if (windCheck == true)
	{
		windCheck = false;
	}
	else if (windCheck == false)
	{
		windCheck = true;
	}
}

bool JankyEncoder::isStopButtonPressed() //allows hasStopBeenPressed to be a variable used in jankyScaling
{
	return hasStopBeenPressed;
}

bool JankyEncoder::typeWind() //allows windCheck to be a variable used in jankyScaling
{
	return windCheck;
}

void JankyEncoder::setWind(bool wind) //allows jankyScaling to set the value of the windCheck
{
	windCheck = wind;
}

void JankyEncoder::Stop() //sets motorStop to true, so that when the emergency stop button is pressed, the condition in the run function will be met and the motor will stop
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
	if (abs(pEncoder->Get()) <= targetcount) //used to be...if (bEncoding == false && abs(pEncoder->Get()) <= targetcount)
	{
		printf ("bEncoding = false\n");
		directionCheck = true; //going "forward"
		if (hasStopBeenPressed == false)
		{
			pEncoderStartVal = pEncoder->Get();
		}
		hasStopBeenPressed = false;
		printf("Encoder Start Value Go %f\n", pEncoderStartVal);
		startMotor();
		maxTimer->Start();
		bEncoding = true;
		printf("bEncoding = true\n");
		return true;
	}
	else
	{
		return false;
	}
}

bool JankyEncoder::ReverseGo(void)
{
	printf ("INSIDE REVERSEGO\n");
	if (pEncoder->Get() > 0)	//used to be...if (bEncoding == false && pEncoder->Get() > 0)
	{
		printf ("bEncoding = false\n");
		directionCheck = false; //going "backwards"
		//printf("Switching Revolution to Negative %f \n", getRevolution()*-1);
		//setRevolution(getRevolution() * -1);
		if (hasStopBeenPressed == false)
		{
			pEncoderStartVal = pEncoder->Get();
		}
		hasStopBeenPressed = false;
		printf("Encoder Start Value RevGo %f\n", pEncoderStartVal);
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

		if (bEncoding == true)
		{
			printf("abs(pEncoder->Get()) %d \n", abs(pEncoder->Get()));
			printf("motor stop value %d\n", motorStop);

			if ( abs(pEncoder->Get() - pEncoderStartVal) >= targetcount || maxTimer->Get() >= desiredMaxTime )
			{
				bDone = true;
				//printf ("Task targetcount reached\n");
				reverseWindCheck();
				stopMotor();
				printf("Encoder Stop Value %d\n", pEncoder->Get());
				bEncoding = false;
				maxTimer->Stop();
			}
			else if (motorStop == true)
			{
				stopMotor();
			}
			else
			{
				//printf ("Targetcount NOT reached\n");
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

