/*
 * RopeClimbing.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Elaine
 */

#include "WPILib.h"
#include "CANTalon.h"
#include "RopeClimbing.h"

#define STARTING 0
#define CLIMBING 1
#define FINISH_CLIMB 2
#define STOP 3

#define CLIMB_SPEED 0.5

#define CIRCUMFERENCE 2.0
#define PULSES_PER_REVOLUTION 1440
#define ENCODER_DISTANCE .5
#define DISTANCE_PER_PULSE CIRCUMFERENCE / PULSES_PER_REVOLUTION

RopeClimbing::RopeClimbing(int motorAChannel, int motorBChannel, int encoderChannelA, int encoderChannelB, int limitSwitchChannel) {
	// TODO Auto-generated constructor stub
	motorA = new CANTalon(motorAChannel);
	motorB = new CANTalon(motorBChannel);
	encoder = new Encoder(encoderChannelA, encoderChannelB);
	limitSwitch = new DigitalInput(limitSwitchChannel);
	climbState = STARTING;
	encoder->SetDistancePerPulse(DISTANCE_PER_PULSE);
}

RopeClimbing::~RopeClimbing() {
	// TODO Auto-generated destructor stub
	delete motorA;
	delete motorB;
	delete encoder;
	delete limitSwitch;
}

void RopeClimbing::StartClimb()
{
	// Make both motors move at climbing speed
	motorA->Set(CLIMB_SPEED);
	motorB->Set(CLIMB_SPEED);
}

void RopeClimbing::StopClimb()
{
	// Make both motors stop
	motorA->Set(0);
	motorB->Set(0);
}

bool RopeClimbing::LimitSwitchPressed()
{
	// returns true if limit switch is pressed
	return limitSwitch->Get();
}

bool RopeClimbing::GetLimitSwitch()
{
	return limitSwitch->Get();
}

double RopeClimbing::GetEncoder()
{
	return encoder->Get();
}

void RopeClimbing::SwitchStates()
{
	switch (climbState)
	{
		case STARTING: // starts the climb and goes to CLIMBING
			printf("STARTING\n");
			StartClimb();
			climbState = CLIMBING;
			break;
		case CLIMBING: // motor is going; when limit switch pressed, reset encoder and go to FINISH_CLIMB
			printf("CLIMBING\n");
			if (GetLimitSwitch())
			{
				printf("Limit switch pressed\n");
				encoder->Reset();
				climbState = FINISH_CLIMB;
			}
			break;
		case FINISH_CLIMB: // motor is going and encoder counting; when reaches top, go to STOP
			printf("FINISH_CLIMB\n");
			/*if (encoder->GetDistance() >= ENCODER_DISTANCE)
			{
				climbState = STOP;
			}*/
			if (encoder->Get() >= 1440 * 6)
			{
				climbState = STOP;
			}
			break;
		case STOP: // motor is going and encoder has reached stop value; stop climbing
			printf("STOP\n");
			StopClimb();
			break;
	}
}
