/*
 * AutoDrive.cpp
 *
 *  Created on: Jan 21, 2019
 *      Author: AnishaKabir
 */

#include "AutoDrive.h"
#include "frc/WPILib.h"
#include "JankyAutoEntry.h"

#define START_TIME 1 //time for camera to begin capturing TODO: reduce this
#define DISTANCE_TO_STOP_DRIVING 23 //need to change for this year
#define VISION_DISTANCE "averaged distance to tape"
#define VISION_OFFSET "horizontal offset"
#define HORIZONTAL_OFFSET_UPPER_BOUND 100
#define HORIZONTAL_OFFSET_LOW_BOUND (-100)

AutoDrive::AutoDrive(frc::DifferentialDrive*drive, double speed, double p, double i, double d) {
	// TODO Auto-generated constructor stub
	kP=p;
	kI=i;
	kD=d;
	chassis=drive;
	_speed=speed;
	pid = new frc::PIDController(kP,kI,kD,this,this);
	visionTimer = new frc::Timer();
}

AutoDrive::~AutoDrive() {
	// TODO Auto-generated destructor stub
	delete pid;
	delete visionTimer;
}

void AutoDrive::Start(){
	badDataCounter=0;
	frc::SmartDashboard::PutNumber(VISION_DISTANCE, -100);

    //might have to be changed this year depending on vision data
	pid->SetInputRange(HORIZONTAL_OFFSET_LOW_BOUND, HORIZONTAL_OFFSET_UPPER_BOUND); //bounds for the horizontal offset
	pid->SetOutputRange(-1.0, 1.0);
	pid->SetSetpoint(0.0);
	pid->Enable();
	visionTimer->Start();
}

bool AutoDrive::JobDone(){
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
		printf("vision speed %f \n", _speed);

		if(distance==-100 || distance==-1){ //-1 is default value when vision has bad data
			//^ maybe this should be distance<=-1
			//TODO: make sure these values are bad data tht vision sends
			badDataCounter++;
			printf("Bad Data Count: %d \n", badDataCounter);
		}
		
		if(badDataCounter==5){ //TODO: add printf with reason why vision quit
			return true;
		}

		if(distance<DISTANCE_TO_STOP_DRIVING && distance>0){ //TODO: add printf with reason for completed drive
			return true;
		}
	}
	return false;

}

void AutoDrive::RunAction(){

}

void AutoDrive::End(){
	pid->Disable();
	chassis->TankDrive(0.0, 0.0);
	visionTimer->Stop();
	visionTimer->Reset();
}

double AutoDrive::PIDGet(){
    //make sure these are the right names for what vision is sending to the dashboard
	distance=frc::SmartDashboard::GetNumber(VISION_DISTANCE, -100); 
	horizontalOffset=frc::SmartDashboard::GetNumber(VISION_OFFSET, -100);

	//filtering out bad horizontal offset data here outside of the (-100, 100) range before returning
	if(horizontalOffset>HORIZONTAL_OFFSET_HIGH_BOUND || horizontalOffset<HORIZONTAL_OFFSET_LOW_BOUND){
		return 0; //this should make the robot not turn
	}
	else{
		return horizontalOffset;
	}
}

void AutoDrive::PIDWrite(double output){
    //have to test this since it uses ArcadeDrive which is different than last year
    chassis->ArcadeDrive(_speed, output); 
}
