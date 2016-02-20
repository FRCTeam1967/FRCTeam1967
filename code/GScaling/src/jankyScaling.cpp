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
#define WIND_SPEED 1.0
#define RELEASE_TIME 1.0

//Constructor
jankyScaling::jankyScaling(int encoderChannelA, int encoderChannelB, int motorChannel, int pistonChannel)
{
	piston = new Solenoid(19, pistonChannel);
	printf("About to set piston to false\n");
	//piston->Set(false);

	motorEncoder = new JankyEncoder(encoderChannelA, encoderChannelB, motorChannel);
	printf("Created new motorEncoder\n");
	//motorEncoder->setSpeed(WIND_SPEED);
	printf("Set revolution\n");

	encoderReset = false;
	//motorEncoder->Reset();
	//motorEncoder->Start();

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
	motorEncoder->setSpeed(WIND_SPEED);
	motorEncoder->Reset();
	motorEncoder->Start();
}

void jankyScaling::EncoderReset(){
	motorEncoder->Reset();
}

void jankyScaling::MotorEncoderReset(){
	encoderReset = true;
}

void jankyScaling::Release(){
	printf("Releasing piston now\n");
	piston->Set(true);												//Pushing out piston to hit the CAM; no need to bring it back in we believe
}

void jankyScaling::WindUp()
{
	motorEncoder->setRevolution(SCALING_WINDS);
	motorEncoder->Go();

}

void jankyScaling::StopWU(){
	//motorEncoder->stopMotor();
	motorEncoder->Reset();
	printf("Canceling all\n");
}


