/*
 * DriveSegment.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: AnishaKabir
 */

#include "DriveSegment.h"
#include "JankyAutoEntry.h"
#include "ctre/Phoenix.h"
#include "JankyAutoEntry.h"
#include <math.h>

#define MEASURED_DIST_PER_PULSE 0.0912
#define ENCODER_UNITS_PER_ROTATION 4096
#define DIAMETER 6
#define CIRCUMFERENCE_INCHES DIAMETER*M_PI

double lEncoderCount;
double rEncoderCount;
double lEncoderDistance;
double rEncoderDistance;
double testEncoderCount;
double testEncoderDistance;

//DriveSegment::DriveSegment(RobotDrive*drive, SensorCollection*leftEncoder, SensorCollection*rightEncoder, Encoder*testEncoder, int inchDistance, double speed) {
DriveSegment::DriveSegment(RobotDrive*drive, Encoder*testEncoder, int inchDistance, double speed) {
	distance = inchDistance;
	chassis = drive;
	//_leftEncoder = leftEncoder;
	//_rightEncoder = rightEncoder;
	_encoder=testEncoder;
	_speed = speed;
	// TODO Auto-generated constructor stub

}

DriveSegment::~DriveSegment() {
	// TODO Auto-generated destructor stub
}

bool DriveSegment::JobDone(){
	/*lEncoderCount = -_leftEncoder->GetQuadraturePosition();
	rEncoderCount = _rightEncoder->GetQuadraturePosition();
	lEncoderDistance = (lEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES;
	rEncoderDistance = (rEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES;
	printf("Left Encoder dist %f \n", lEncoderDistance);
	printf("Right Encoder dist %f \n", rEncoderDistance);
	if((lEncoderDistance>=distance)&&(rEncoderDistance>=distance)){
		printf("job done \n");
		return true;
	}*/
	testEncoderCount=-_encoder->Get();
	testEncoderDistance=(testEncoderCount*MEASURED_DIST_PER_PULSE);
	SmartDashboard::PutNumber("Encoder Count", testEncoderCount);
	SmartDashboard::PutNumber("Encoder Distance", testEncoderDistance);
	if(testEncoderDistance>=distance){
		printf("job done \n");
		return true;
	}
	return false;
}

void DriveSegment::RunAction(){
	chassis->TankDrive(_speed, _speed);
}

void DriveSegment::Start(){
	//_leftEncoder->SetQuadraturePosition(0, 10);
	//_rightEncoder->SetQuadraturePosition(0, 10);
	_encoder->Reset();
}

void DriveSegment::End(){
	chassis->TankDrive(0.0, 0.0);
}
