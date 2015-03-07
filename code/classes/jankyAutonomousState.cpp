/*
 *
 * jankyAutonomousState.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: Team1967
 */

#include "WPILib.h"
#include "jankyAutonomousState.h"
#include "jankyFoxLiftState.h"

/*
 * Constructor
 * 
*/
JankyAutonomousState::JankyAutonomousState(RobotDrive * pt, JankyFoxliftState * foxLift)
{
	//printf("JankyAutonomousState() constructor\n");
	ptRobot = pt;
	ptFoxLift = foxLift;
	driveOnce = false;
	
	SetMachineName("JankyAutonomousStateMachine");
	printf("Set machine name\n");
	
	SetName(Idle,"Idle");
	SetName(DriveSideways, "Just drive sideways");
	SetName(DriveForward,"Drive forward");
	SetName(LiftTote, "Lift tote");
	SetName(End, "End - STOPPPPPP");
	printf("Set state names\n");
	
	driveForwardTimer = new Timer();
	driveSidewaysTimer = new Timer();
	driveForwardTimer->Reset();
	driveSidewaysTimer->Reset();
	printf("Newed/reset drive timer\n");
	
	//Starting JankyTask at end of constructor
	Start();
}

/*
 * Destructor
 */
JankyAutonomousState::~JankyAutonomousState()
{
	delete driveSidewaysTimer;
	delete driveForwardTimer;
}

void JankyAutonomousState::StartAuto()
{
	if(GetCurrentState() == Idle)
	{
		driveSidewaysTimer->Reset();
		driveForwardTimer->Reset();
		NewState(Idle,"Starting autonomous");
	}
}
void JankyAutonomousState::Go()
{
	printf("Go() called\n");
	GoSideways();
}

void JankyAutonomousState::GoSideways()
{
	printf("GoSideways() called\n");
	if (GetCurrentState() == Idle)
	{
		if(driveSidewaysTimer)
		{
			driveSidewaysTimer->Reset();
			driveSidewaysTimer->Start();
		}
		NewState(DriveSideways,"Start driving sideways");
	}
	else
		printf("Can't go sideways\n");
}

void JankyAutonomousState::GoForward()
{
	printf("GoForward() called\n");
	if (GetCurrentState() == DriveSideways)
	{
		if (driveForwardTimer)
		{
			driveForwardTimer->Reset();
			driveForwardTimer->Start();
		}
		NewState(DriveForward,"Start driving forward");
	}
	else
		printf("Can't go forward\n");
}

void JankyAutonomousState::GoLiftTote()
{
	printf("LiftTote() called\n");
	if (GetCurrentState() == DriveForward)
	{
		NewState(StateValue::LiftTote,"Start lifting tote");
	}
	else
		printf("Can't go sideways\n");
}
void JankyAutonomousState::StateEngine(int curState)
{
	switch(curState)
	{
		case Idle:
			//Idle handled by GoForward() or StartAuto()
			//printf("In Idle\n");
			if(driveSidewaysTimer)
			{
				driveSidewaysTimer->Stop();
				driveSidewaysTimer->Reset();
			}
			if(driveForwardTimer)
			{
				driveForwardTimer->Stop();
				driveForwardTimer->Reset();
			}
			break;
		case DriveSideways:
			printf("In state DriveSideways\n");
			if(driveSidewaysTimer->Get() >= DRIVE_SIDEWAYS_TIME)
			{
				GoForward();
			}
			else
			{
				ptRobot->MecanumDrive_Cartesian(1.0,0.0,0.0,0.0);
			}
			break;
		case DriveForward:
			printf("In state DriveForward\n");
			if(driveForwardTimer->Get() >= DRIVE_FORWARD_TIME)
			{
				NewState(End,"Done driving forward");
			}
			else
			{
				ptRobot->MecanumDrive_Cartesian(0.0,-1.0,0.0,0.0);
			}
			break;
		case LiftTote:
			ptFoxLift->GoDown();
			ptFoxLift->GoUp();
			break;
		case End:
			if(ptRobot)
			{
				ptRobot->MecanumDrive_Cartesian(0.0,0.0,0.0,0.0);
			}
			break;
		default:
			printf("error current state = %d\n",curState);
			break;
	}
}
