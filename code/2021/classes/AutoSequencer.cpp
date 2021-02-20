/*
 * AutoSequencer.cpp
 *
 *  Created on: Feb 13, 2020
 *      Author: Maddie
 */

#include <frc/RobotDrive.h>
#include <frc/smartdashboard/SmartDashboard.h>
#include "ctre/Phoenix.h"
#include "jankyStateMachine.h"
#include "AutoSequencer.h"
#include "AutoSettings.h"
#include "AutoDriveSubsystems.h"
#include "JankyPathWeaver.h"
#include "AutoShoot.h"

using namespace std;
using namespace frc;

// pid values
float aMode;
float turn_kP = 0.05;
float turn_kI = 0.0;
float turn_kD = 0.06;

float drive_kP = 0.04;
float drive_kI = 0.0;
float drive_kD = 0.0;
int c;

// JankyPathWeaver * trajectory1;
AutoShoot * shoot;

AutoSequencer::AutoSequencer(AutoDriveSubsystem * m_drive, int autoMode) {
	for(int i = 0; i<MAX_NAMES; i++){
		entries[i]=NULL;
	}

	//trajectory1 = new JankyPathWeaver(m_drive, autoMode);
	shoot = new AutoShoot();

	SetMachineName("JankyAutoSequencer");
	//SetName(Trajectory1, "Trajectory1", trajectory1);
	SetName(Shoot, "Shoot", shoot);

	Start();
}

AutoSequencer::~AutoSequencer() {
	//delete trajectory1;
	delete shoot;
}

void AutoSequencer::SetName(int state, const char* name, AutoEntry*entry){
	JankyStateMachine::SetName(state, name);
	entries[state]=entry;
}

void AutoSequencer::SetMode(int mode){ //call set mode in autoPeriodic
	if(aMode!=DONE){
		aMode=mode;
	}
}

void AutoSequencer::EndSequence(){
	printf("Current Entry: %s \n", names[GetCurrentState()]);
	SmartDashboard::PutString("Current Entry", names[GetCurrentState()]);
	if(entries[GetCurrentState()]){
		entries[GetCurrentState()]->Abort();
		printf("Aborted Entry: %s \n", names[GetCurrentState()]);
	}
	Pause();
}

void AutoSequencer::StateEngine(int curState)
{
	switch(curState){
		//case Trajectory1: //TEST THS
				//if(trajectory1->IsComplete()){
				//	NewState(Shoot, "Shoot");
				//}
				//break;
		case Shoot: 
				if(shoot->IsComplete())
				{
					
				}
	}
}
