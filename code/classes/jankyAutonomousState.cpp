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
JankyAutonomousState::JankyAutonomousState(RobotDrive * pt) 
{
	//printf("JankyAutonomousState() constructor\n");
	
	if(pt)
		ptRobot = pt;
	else
		printf("No pt\n");
	
	SetMachineName("JankyAutonomousStateMachine");
	printf("Set machine name\n");
	
	SetName(Idle,"Idle");
	SetName(DriveForward,"Driving forward");
	printf("Set state names\n");
	
	
	driveTimer = new Timer();
	driveTimer->Reset();
	printf("Newed/reset drive timer\n");
}

/*
 * Destructor
 */
JankyAutonomousState::~JankyAutonomousState()
{
	delete driveTimer;
	//delete ptRobot;
}

void JankyAutonomousState::GoForward()
{
	printf("GoForward() called\n");
	if(GetCurrentState() == Idle)
	{
		NewState(DriveForward,"Start driving");
	}
	else
		printf("Can't go forward\n");
}

void JankyAutonomousState::StateEngine(int curState)
{
	switch(curState)
	{
		case Idle:
			//Idle handled by GoForward()
			//printf("In Idle\n");
			driveTimer->Stop();
			driveTimer->Reset();
			break;
		case DriveForward:
			printf("In state DriveForward\n");
			driveTimer->Reset();
			driveTimer->Start();
			ptRobot->MecanumDrive_Cartesian(0.0,0.5,0.0,0.0);
			if(driveTimer->Get() >= DRIVE_TIME)
				NewState(Idle,"Done driving in auto");
			break;
		default:
			printf("error current state = %d\n",curState);
			break;
	}
}



