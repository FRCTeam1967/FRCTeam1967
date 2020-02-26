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

AutoSequencer::AutoSequencer(RobotDrive*drive, ADXRS450_Gyro*gyro, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*leftmotor, WPI_TalonSRX*rightmotor) { //add shooter as a parameter later
}

AutoSequencer::~AutoSequencer() {
	
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
	
}
