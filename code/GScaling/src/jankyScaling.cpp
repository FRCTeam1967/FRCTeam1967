/*
 * jankyScaling.cpp
 *
 *  Created on: Feb 6, 2016
 *      Author: Gillian
 */
#include "WPILib.h"
#include "jankyScaling.h"
#include <math.h>
#include "jankyTask.h"
#include "jankyEncoder.h"
#include <cmath>

#define SCALING_WINDS 16					//Scaling winch needs to turn 360 degrees exactly 16 times
#define FW_WIND_SPEED 1
#define BW_WIND_SPEED -1
#define RELEASE_TIME 1.0

//Constructor
jankyScaling::jankyScaling(int encoderChannelA, int encoderChannelB, int motorChannelA, int motorChannelB, int pistonChannel)
{
	piston = new Solenoid(19, pistonChannel);
	printf("About to set piston to false\n");
	//piston->Set(false);

	motorEncoder = new JankyEncoder(encoderChannelA, encoderChannelB, motorChannelA, motorChannelB);
	printf("Created new motorEncoder\n");
	//motorEncoder->setSpeed(WIND_SPEED);
	printf("Set revolution\n");

	//motorEncoder->Reset();
	//motorEncoder->Start();
	ScalingStart();

}

//Destructor
jankyScaling::~jankyScaling()
{
	piston->Set(false);
	delete piston;
	delete motorEncoder;

	piston = NULL;
	motorEncoder = NULL;
}

void jankyScaling::ScalingStart(){
	piston->Set(false);
	motorEncoder->Reset();
	//motorEncoder->setSpeed(WIND_SPEED);
	motorEncoder->setRevolution(SCALING_WINDS);
	motorEncoder->Start();
	motorEncoder->motorGo();
}

void jankyScaling::Release(){
	printf("Releasing piston now\n");
	piston->Set(true);												//Pushing out piston to hit the CAM; no need to bring it back in we believe
}

void jankyScaling::WindUp()											//pulling the robot up
{
	motorEncoder->setSpeed(FW_WIND_SPEED);
	motorEncoder->motorGo();
	motorEncoder->Go();
	//StopWU();
	/*if (motorEncoder->typeWind() == true || motorEncoder->isStopButtonPressed() == true)
	{
		motorEncoder->setSpeed(FW_WIND_SPEED);
		motorEncoder->motorGo();
		motorEncoder->Go();
		motorEncoder->setWind(true);

	}*/

}

void jankyScaling::WindDn()											//extend the robot towards down
{
	motorEncoder->setSpeed(BW_WIND_SPEED);
	motorEncoder->motorGo();
	motorEncoder->ReverseGo();
	//StopWU();
	/*if (motorEncoder->typeWind() == false || motorEncoder->isStopButtonPressed() == true)
		{
			motorEncoder->setSpeed(BW_WIND_SPEED);
			motorEncoder->motorGo();
			motorEncoder->ReverseGo();
			motorEncoder->setWind(false);
		}*/

}

void jankyScaling::StopWU(){
	motorEncoder->Stop();											//Find a way to stop the motor without having to reset——stopMotor isn't working bc Go is overriding it?
	//motorEncoder->stopCheck();
	printf("Stopping wind\n");
}


