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
	piston->Set(false);

	motorEncoder = new JankyEncoder(encoderChannelA, encoderChannelB, motorChannel);
	printf("Created new motorEncoder\n");
	motorEncoder->setSpeed(WIND_SPEED);
	printf("Set revolution\n");
	//windUpCancel = false;
	//printf("Resetting encoder now\n");
	//motorEncoder->Reset();

	encoderReset = false;
	motorEncoder->Reset();
	motorEncoder->Start();

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

void jankyScaling::EncoderReset(){
	motorEncoder->Reset();
	//motorEncoder->setSpeed(WIND_SPEED);
	//motorEncoder->setRevolution(SCALING_WINDS);
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
	//if (encoderReset == true){
	motorEncoder->setRevolution(SCALING_WINDS);
	motorEncoder->Go();
	//motorEncoder->Reset();
	//motorEncoder->Go();
	//}

	/*motorEncoder->setRevolution(SCALING_WINDS);

	printf("a motorEncoder->getRevolution() abs(%d)", abs(motorEncoder->getRevolution()));
	if (SCALING_WINDS > abs(motorEncoder->getRevolution())){
	}
	else{
		motorEncoder->Reset();
		printf("Winch done winding up\n");
	}

	printf("Made it to WindUp\n");
	encoder->Reset();												//Reseting encoder to zero
	printf("Reset encoder\n");
	printf("About to start WindUp motor\n");

	while(encoder->Get() < (SCALING_WINDS*360)){					//While the encoder is below 360 times the amount of revolutions, the motor will be set to -0.25 speed
		motor->Set(-0.25);
		SmartDashboard::PutNumber("Scaling Winds", encoder->Get());	//Printing encoder number on SmartDashboard
		if(windUpCancel == true){
			motor->Set(0);
			windUpCancel = false;
			encoder->Reset();
			break;
		}

	}

	motor->Set(0);													//After the encoder has hit 360, the motor will stop
	printf("WindUp motor stopped\n");

	if (encoder->Get() == 360){
		motor->Set(0);
		printf("Stopping WindUp motor\n");
	}
	else{
		printf("Starting WindUp motor\n");
		motor->Set(0.25);
		Wait(10);
	}*/
}

void jankyScaling::StopWU(){
	motorEncoder->stopMotor();
	motorEncoder->Reset();
	//motorEncoder->setRevolution(SCALING_WINDS);
	printf("Canceling all\n");
}


/*float jankyScaling::GetWrap()
{
	encoder->Get();
}*/
