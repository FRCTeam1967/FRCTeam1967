/*
 * AutoDrive.cpp
 *
 *  Created on: Jan 21, 2019
 *      Author: AnishaKabir
 */
#include "AutoDrive.h"
#include "frc/WPILib.h"
#include "JankyAutoEntry.h"
#include "ctre/Phoenix.h"
#include "Settings.h"

#define STARTING_TIME 0.1 //time for camera to begin capturing 
#define STOP_DRIVING_DISTANCE 10 
#define VISION_DISTANCE "Distance to Tape"
#define VISION_OFFSET "Offset"
#define HORIZONTAL_OFFSET_UPPER_BOUND 100
#define HORIZONTAL_OFFSET_LOW_BOUND (-100)
#define MAX_BAD_DATA 10
#define BAD_DATA_DEFAULT (-1)
#define NO_DATA_DEFAULT (-100)

AutoDrive::AutoDrive(frc::DifferentialDrive*drive, double speed, double p, double i, double d) {
	// TODO Auto-generated constructor stub
	kP=p;
	kI=i;
	kD=d;
	chassis=drive;
	_speed=speed;
	pid = new frc::PIDController(kP,kI,kD,this,this);
	visionTimer = new frc::Timer();

	frc::SmartDashboard::PutNumber(VISION_DISTANCE, NO_DATA_DEFAULT);
	frc::SmartDashboard::PutNumber(VISION_OFFSET, NO_DATA_DEFAULT);
	frc::SmartDashboard::PutString("Reason for AutoDrive exit", "Default name");

}

AutoDrive::~AutoDrive() {
	// TODO Auto-generated destructor stub
	pid->Disable();
	delete pid;
	delete visionTimer;
}

void AutoDrive::Start(){
	badDataCounter=0;
	noDataCounter=0;

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
	if(visionTimer->Get()>=STARTING_TIME){ 
		//printf("distance %f \n", distance);
		//printf("vision speed %f \n", _speed);

		if(distance==BAD_DATA_DEFAULT){ //this is default value when vision is unable to detect the tape
			badDataCounter++;
			frc::SmartDashboard::PutNumber("Bad Data Count:", badDataCounter);
			//printf("Bad Data Count: %f \n", badDataCounter);
		}
		else if(distance==NO_DATA_DEFAULT){
			noDataCounter++;
			frc::SmartDashboard::PutNumber("No Data Count:", noDataCounter);
			//printf("No Data Sent Count: %f \n", noDataCounter);
		}
		else if(distance<STOP_DRIVING_DISTANCE){
			printf("AutoDrive complete! Distance reached! \n");
			return true;
		}
		//TODO: add more bad data cases (ex. if distance is negative or infinite)
		
		if(badDataCounter>=MAX_BAD_DATA){
			frc::SmartDashboard::PutString("Reason for AutoDrive exit", "AutoDrive exited because unable to detect the tape");
			//printf("AutoDrive exited because unable to detect the tape \n");
			return true;
		}
		else if(noDataCounter>=MAX_BAD_DATA){
			frc::SmartDashboard::PutString("Reason for AutoDrive exit", "AutoDrive exited because no data being sent; using default value of -100");
			//printf("AutoDrive exited because no data being sent; using default value of -100 \n");
			return true;
		}

	}
	return false;

}

void AutoDrive::RunAction(){

}

void AutoDrive::End(){
	pid->Disable();
	pid->Reset();
	chassis->TankDrive(0.0, 0.0);
	printf("end \n");
	visionTimer->Stop();
	visionTimer->Reset();
}

double AutoDrive::PIDGet(){
    //TODO: make sure these are the right names for what vision is sending to the dashboard
	distance=frc::SmartDashboard::GetNumber(VISION_DISTANCE, NO_DATA_DEFAULT); 
	horizontalOffset=(frc::SmartDashboard::GetNumber(VISION_OFFSET, NO_DATA_DEFAULT)+2); //positive is to the left

	//filtering out bad horizontal offset data here outside of the (-100, 100) range before returning
	if(horizontalOffset>HORIZONTAL_OFFSET_UPPER_BOUND || horizontalOffset<HORIZONTAL_OFFSET_LOW_BOUND){
		printf("Horizontal Offset out of bounds: %f \n", horizontalOffset);		
		return 0; //this should make the robot not turn
	}
	else if(horizontalOffset==NO_DATA_DEFAULT){
		printf("Horizontal Offset not changing from default: %f \n", horizontalOffset);		
		return 0;		
	}
	else{
		return horizontalOffset;
	}
}

void AutoDrive::PIDWrite(double output){
    //have to test this since it uses ArcadeDrive which is different than last year
    chassis->ArcadeDrive(_speed, output); 
}
