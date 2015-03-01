/*
 * jankyAutonomousState.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: Team1967
 */

#include "WPILib.h"
#include "jankyAutonomousState.h"

/*
 * Constructor
 * 
*/
JankyAutonomousState::JankyAutonomousState(RobotDrive * pt, JankyFoxliftState * foxLift)
{
	//printf("JankyAutonomousState() constructor\n");
	
	driveOnce = false;
	
	SetMachineName("JankyAutonomousStateMachine");
	printf("Set machine name\n");
	
	SetName(Idle,"Idle");
	SetName(DriveForward,"Just drive forward");
	SetName(DriveSideways, "Just drive sideways");
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
	delete driveForwardTimer;
	delete driveSidewaysTimer;
}

void JankyAutonomousState::StartAuto()
{
	if(GetCurrentState() == Idle)
	{
		driveForwardTimer->Reset();
		driveSidewaysTimer->Reset();
		NewState(Idle,"Starting autonomous");
	}
}
void JankyAutonomousState::Go()
{
	GoSideways();
}

void JankyAutonomousState::GoForward()
{
	printf("GoForward() called\n");
	if(GetCurrentState() == Idle)
	{
		if(driveForwardTimer)
		{
			driveForwardTimer->Reset();
			driveForwardTimer->Start();
		}
		NewState(DriveForward,"Start driving forward");
	}
	else
		printf("Can't go forward\n");
}

void JankyAutonomousState::GoSideways()
{
	printf("GoSideways() called\n");
	if(GetCurrentState() == Idle)
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

void JankyAutonomousState::StateEngine(int curState)
{
	switch(curState)
	{
		case Idle:
			//Idle handled by GoForward() or StartAuto()
			//printf("In Idle\n");
			if(driveForwardTimer)
			{
				driveForwardTimer->Stop();
				driveForwardTimer->Reset();
			}
			break;
		case DriveForward:
			printf("In state DriveForward\n");
			if(ptRobot)
				ptRobot->MecanumDrive_Cartesian(-1.0,0.0,0.0,0.0);
			driveOnce = true;
			if(driveForwardTimer->Get() >= DRIVE_FORWARD_TIME)
				NewState(End,"Done driving in auto");
			break;
		case DriveSideways:
			if(driveSidewaysTimer->Get() >= DRIVE_SIDEWAYS_TIME)
			{
				NewState(DriveForward,"Done driving in auto");
			}
			else
			{
				ptRobot->MecanumDrive_Cartesian(1.0,0.0,0.0,0.0);
			}
			break;
		case End:
			if(driveOnce == true)
			{
				if(ptRobot)
				{
					ptRobot->MecanumDrive_Cartesian(0.0,0.0,0.0,0.0);
				}
				driveOnce = false;
			}
			break;
		default:
			printf("error current state = %d\n",curState);
			break;
	}
}



