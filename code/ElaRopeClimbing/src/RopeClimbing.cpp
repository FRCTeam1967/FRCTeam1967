/*
 * RopeClimbing.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Elaine
 */

#include "WPILib.h"
#include "CANTalon.h"
#include "RopeClimbing.h"

#define IDLE 0
#define ARMED 1
#define STARTING 2
#define CLIMBING 3
#define FINISH_CLIMB 4
#define STOP 5

#define CLIMB_SPEED -0.5 // set it negative so electronics can have it standard

#define CIRCUMFERENCE 0.375 * 3.14
#define PULSES_PER_REVOLUTION 500 //1440
#define ENCODER_DISTANCE 5
#define DISTANCE_PER_PULSE CIRCUMFERENCE / PULSES_PER_REVOLUTION

RopeClimbing::RopeClimbing(int motorAChannel, int motorBChannel, int encoderChannelA, int encoderChannelB, int limitSwitchChannel) {
	// TODO Auto-generated constructor stub
	motorA = new CANTalon(motorAChannel);
	motorB = new CANTalon(motorBChannel);
	encoder = new Encoder(encoderChannelA, encoderChannelB);
	limitSwitch = new DigitalInput(limitSwitchChannel);
	climbState = IDLE;
	encoder->SetDistancePerPulse(DISTANCE_PER_PULSE);
//	Start();
}

RopeClimbing::~RopeClimbing() {
	// TODO Auto-generated destructor stub
	delete motorA;
	delete motorB;
	delete encoder;
	delete limitSwitch;
}

void RopeClimbing::StartClimbing()
{
	// Set state to STARTING
	climbState = STARTING;
}

void RopeClimbing::StopClimbing()
{
	// Set state to STOP
	climbState = STOP;
}

void RopeClimbing::SetArmed()
{
	// Set state to ARMED
	climbState = ARMED;
}

void RopeClimbing::StartClimbingMotors()
{
	// Make both motors move at climbing speed
	motorA->Set(CLIMB_SPEED);
	motorB->Set(CLIMB_SPEED);
}

void RopeClimbing::StopClimbingMotors()
{
	// Make both motors stop
	motorA->Set(0.0);
	motorB->Set(0.0);
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

int RopeClimbing::GetState()
{
	return climbState;
}

void RopeClimbing::SwitchStates()
{
	switch (climbState)
	{
		case IDLE:
			printf("IDLE\n");
			break;
		case ARMED:
			printf("ARMED\n");
			break;
		case STARTING: // starts the climbing motors and goes to CLIMBING
			printf("STARTING\n");
			StartClimbingMotors();
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
			if (encoder->Get() >= 1440 * 6) // jankyBot encoder pulses per revolution
			{
				climbState = STOP;
			}
			break;
		case STOP: // motor is going and encoder has reached stop value; stop climbing
			printf("STOP\n");
			StopClimbingMotors();
			break;
	}
}

/*void RopeClimbing::Run()
{
	SwitchStates();
	printf("I’m alive in run\n");
}*/
