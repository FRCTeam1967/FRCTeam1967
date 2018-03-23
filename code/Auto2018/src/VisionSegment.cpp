/*
 * VisionSegment.cpp
 *
 *  Created on: Feb 24, 2018
 *      Author: AnishaKabir
 */

#include "VisionSegment.h"
#include "WPILib.h"
#include "JankyAutoEntry.h"

#define START_TIME 1
#define DISTANCE_TO_STOP_DRIVING 14 //need to change

VisionSegment::VisionSegment(RobotDrive*drive, double speed, double p, double i, double d) {
	// TODO Auto-generated constructor stub
	chassis=drive;
	_speed=speed;
	pid = new PIDController(kP,kI,kD,this,this);
	visionTimer = new Timer();
}

VisionSegment::~VisionSegment() {
	// TODO Auto-generated destructor stub
	delete pid;
	delete visionTimer;
}

void VisionSegment::Start(){
	badDataCounter=0;
	pid->SetInputRange(-100, 100); //not sure what this should be talk to vision
	pid->SetOutputRange(-1.0, 1.0);
	pid->SetSetpoint(0.0);
	pid->Enable();
	visionTimer->Start();
}

bool VisionSegment::JobDone(){
	//CHANGE ROBOT SPEED BASED ON DISTANCE AWAY
	/*if(distance>55){
		_speed=0.5;
	}
	else if(distance>30){
		_speed=0.4;
		//add setP, I, D
	}
	else if(distance>15){
		_speed=0.3;
	}
	else{
		_speed=0.25;
	}*/

	//Handle Bad Data
	if(visionTimer->Get()>=START_TIME){
		printf("distance %f \n", distance);
		if(distance==-100 || distance==-1){ //0 is default value when vision doesn't send data
			badDataCounter++;
			printf("Bad Data Count: %d \n", badDataCounter);
		}
		if(badDataCounter==5){
			return true;
		}

		if(distance<DISTANCE_TO_STOP_DRIVING && distance>0){
			return true;
		}
	}
	return false;

}

void VisionSegment::RunAction(){

}

void VisionSegment::End(){
	pid->Disable();
	chassis->TankDrive(0.0, 0.0);
	visionTimer->Stop();
	visionTimer->Reset();
}

double VisionSegment::PIDGet(){
	distance=SmartDashboard::GetNumber("averaged distance to tape", -100); //make sure these are the right names for what vision is sending to the dashboard
	horizontalOffset=SmartDashboard::GetNumber("horizontal offset", -100);
	return horizontalOffset;
}

void VisionSegment::PIDWrite(double output){
	chassis->Drive(_speed, output);
}
