/*
 * RopeClimbing.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Elaine
 */

#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "RopeClimbing.h"

#define STOP 0
#define START 1
#define CLIMB 2
#define START_FAST 3
//#define FINISH_CLIMB 4
//#define STOP 5

#define CLIMB_SPEED -0.5 // set it negative so electronics can have it standard
#define FAST_SPEED -0.75 //faster for less time

#define CIRCUMFERENCE 0.375 * 3.14
#define PULSES_PER_REVOLUTION 500 //1440
#define ENCODER_DISTANCE 5
#define DISTANCE_PER_PULSE CIRCUMFERENCE / PULSES_PER_REVOLUTION

#define MAX_CURRENT 40 // max number of amps motors can go

RopeClimbing::RopeClimbing(int motorAChannel, int motorBChannel, int encoderChannelA, int encoderChannelB, int limitSwitchChannel) {
	// TODO Auto-generated constructor stub
	motorA = new WPI_TalonSRX(motorAChannel);
	motorB = new WPI_TalonSRX(motorBChannel);
	encoder = new Encoder(encoderChannelA, encoderChannelB);
	limitSwitch = new DigitalInput(limitSwitchChannel);
	climbState = STOP;
	//climbSpeed= CLIMB_SPEED;
	encoder->SetDistancePerPulse(DISTANCE_PER_PULSE);

	Start();
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
	climbState = START;
//	climbSpeed = speed;
}

void RopeClimbing::StartFastClimbing()
{
	// Set state to STARTING
	climbState = START_FAST;
//	climbSpeed = speed;
}

void RopeClimbing::StopClimbing()
{
	// Set state to STOP
	climbState = STOP;
}


void RopeClimbing::StartClimbingMotors()
{
	// Make both motors move at climbing speed
	if (!AboveMaxCurrent())
	{
		motorA->Set(CLIMB_SPEED);
		motorB->Set(CLIMB_SPEED);
	}
}

void RopeClimbing::StartFastClimbingMotors()
{
	// Make both motors move at fast climbing speed
	if (!AboveMaxCurrent())
	{
		motorA->Set(FAST_SPEED);
		motorB->Set(FAST_SPEED);
	}
}

void RopeClimbing::StopClimbingMotors()
{
	// Make both motors stop
	motorA->Set(0.0);
	motorB->Set(0.0);
}

bool RopeClimbing::AboveMaxCurrent()
{
	// Returns true if either motor current is above MAX_CURRENT
	if (motorA->GetOutputCurrent() >= MAX_CURRENT || motorB->GetOutputCurrent() >= MAX_CURRENT)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void RopeClimbing::StopAboveMaxCurrent()
{
	// Stops when talons reach current above 40 amps
	if (AboveMaxCurrent())
	{
		climbState = STOP;
	}
}

double RopeClimbing::GetMotorACurrent()
{
	// returns current of motor A
	return motorA->GetOutputCurrent();
}

double RopeClimbing::GetMotorBCurrent()
{
	return motorB->GetOutputCurrent();
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
		case STOP: // stop climbing motors
//			printf("STOP\n");
			StopClimbingMotors();
			break;
		case START: // starts climbing motors
//			printf("START\n");
			StartClimbingMotors();
			climbState = CLIMB;
			break;
		case CLIMB: // stop climbing motors if currents above MAX_CURRENT (40 amps)
//			printf("CLIMB\n");
			StopAboveMaxCurrent();
			break;
		case START_FAST: // starts climbing motors
	//			printf("START\n");
			StartFastClimbingMotors();
			climbState = CLIMB;
			break;
		/*case IDLE:
			printf("IDLE\n");
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
			if (encoder->GetDistance() >= ENCODER_DISTANCE)
			{
				climbState = STOP;
			}*/
			/*if (encoder->Get() >= 1440 * 6) // jankyBot encoder pulses per revolution
			{
				climbState = STOP;
			}
			break;
		case STOP: // motor is going and encoder has reached stop value; stop climbing
			printf("STOP\n");
			StopClimbingMotors();
			break;*/
	}
}

void RopeClimbing::Run()
{
	SwitchStates();

	//printf("I’m alive in run\n");
}
