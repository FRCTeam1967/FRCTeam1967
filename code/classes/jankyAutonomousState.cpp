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
	
	driveOnce = false;
	
	if(pt)
		ptRobot = pt;
	else
		printf("No pt\n");
	
	autoPickupMachine = NULL;
	autoKickerMachine = NULL;
	
	SetMachineName("JankyAutonomousStateMachine");
	printf("Set machine name\n");
	
	SetName(Idle,"Idle");
	SetName(DriveForward,"Just drive forward");
	SetName(InitialLower,"Lowering pickup with ball");
	SetName(DriveForwardWindUp,"Driving forward and winding");
	SetName(StopDriveWind,"Stopping drive but winding");
	SetName(DriveStopWind,"Driving but done winding");
	SetName(PrepKick,"Wound up and drove forward");
	SetName(Kick,"Kicking");
	SetName(PostKick,"Done kicking");
	SetName(End,"Done in auto");
	printf("Set state names\n");
	
	driveTimer = new Timer();
	lowerTimer = new Timer();
	windTimer = new Timer();
	driveTimer->Reset();
	lowerTimer->Reset();
	printf("Newed/reset drive timer\n");
	
	//Starting JankyTask at end of constructor
	Start();
}

/*
 * Destructor
 */
JankyAutonomousState::~JankyAutonomousState()
{
	delete driveTimer;
	delete lowerTimer;
	delete autoPickupMachine;
	delete autoKickerMachine;
	delete ptRobot;
}

void JankyAutonomousState::SetAutoPickupMachine(JankyPickupState * pickupState)
{
	if(pickupState)
		autoPickupMachine = pickupState;
}

void JankyAutonomousState::SetAutoKickerMachine(JankyKickerState * kickState)
{
	if(kickState)
		autoKickerMachine = kickState;
}

void JankyAutonomousState::StartAuto()
{
	if(GetCurrentState() == Idle)
	{
		lowerTimer->Reset();
		lowerTimer->Start();
		NewState(InitialLower,"Starting autonomous");
	}
}

void JankyAutonomousState::GoForward()
{
	printf("GoForward() called\n");
	if(GetCurrentState() == Idle)
	{
		if(driveTimer)
		{
			driveTimer->Reset();
			driveTimer->Start();
		}
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
			//Idle handled by GoForward() or StartAuto()
			//printf("In Idle\n");
			if(driveTimer)
			{
				driveTimer->Stop();
				driveTimer->Reset();
			}
			break;
		case InitialLower:
			autoPickupMachine->LowerToPickup();
			if(lowerTimer->Get() >= LOWER_TIME)
			{
				driveTimer->Reset();
				driveTimer->Start();
				if(ptRobot)
					ptRobot->MecanumDrive_Cartesian(-1.0,0.0,0.0,0.0);
				windTimer->Reset();
				windTimer->Start();
				autoKickerMachine->WindUp();
				NewState(DriveForwardWindUp,"Pickup lowered");
			}
			break;
		case DriveForward:
			printf("In state DriveForward\n");
			if(ptRobot)
				ptRobot->MecanumDrive_Cartesian(-1.0,0.0,0.0,0.0);
			driveOnce = true;
			if(driveTimer->Get() >= DRIVE_TIME)
				NewState(End,"Done driving in auto");
			break;
		case DriveForwardWindUp:
			if(driveTimer->Get() >= DRIVE_TIME)
			{
				if(autoKickerMachine->GetCurrentState() == JankyKickerState::PostWindUpWait)
					NewState(PrepKick,"Drive and wind done");
				else
					NewState(StopDriveWind,"Drive done, wind not");
			}
			else
			{
				if(autoKickerMachine->GetCurrentState() == JankyKickerState::PostWindUpWait)
					NewState(DriveStopWind,"Wind done, drive not");
			}
			break;
		case StopDriveWind:
			//autoPickupMachine->LowerToPickup();
			if(ptRobot)
			{
				ptRobot->MecanumDrive_Cartesian(0.0,0.0,0.0,0.0);
				driveOnce = false;
			}
			if(autoKickerMachine->GetCurrentState() == JankyKickerState::PostWindUpWait)
			{
				autoPickupMachine->UnLowerExit();
			}
			if(autoPickupMachine->GetCurrentState() == JankyPickupState::PrimedForKick)
			{
				printf("Checked PrimedForKick\n");
				NewState(PrepKick,"Drive and wind done");
			}
			break;
		case DriveStopWind:
			if(driveTimer->Get() >= DRIVE_TIME)
			{
				if(ptRobot)
				{
					ptRobot->MecanumDrive_Cartesian(0.0,0.0,0.0,0.0);
					driveOnce = false;
				}
				autoPickupMachine->UnLowerExit();
				if(autoPickupMachine->GetCurrentState() == JankyPickupState::PrimedForKick)
					NewState(PrepKick,"Drive and wind done");
			}
			break;
		case PrepKick:
			autoPickupMachine->LowerForKick();
			if(autoPickupMachine->GetCurrentState() == JankyPickupState::ReadyToKick)
				NewState(Kick,"Pickup lowered, kicking");
			break;
		case Kick:
			if(autoPickupMachine->IsPickupDown() == true)
			{	
				autoKickerMachine->Kick();
				if(autoKickerMachine->HasKicked() == true)
					NewState(PostKick,"Have kicked");
			}
			break;
		case PostKick:
			if(autoKickerMachine->GetCurrentState() == JankyKickerState::Idle)
			{
				if(autoPickupMachine->GetCurrentState() == JankyPickupState::Idle)
					NewState(End,"Done in auto");
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



