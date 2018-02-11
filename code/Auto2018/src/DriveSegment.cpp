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



DriveSegment::DriveSegment(RobotDrive*drive, SensorCollection*leftEncoder, SensorCollection*rightEncoder, int inchDistance, double speed) {
	distance = inchDistance;
	chassis = drive;
	_leftEncoder = leftEncoder;
	_rightEncoder = rightEncoder;
	_speed = speed;
	// TODO Auto-generated constructor stub

}

DriveSegment::~DriveSegment() {
	// TODO Auto-generated destructor stub
}

bool DriveSegment::JobDone(){
	double lEncoderCount = _leftEncoder->GetQuadraturePosition();
	double rEncoderCount = _rightEncoder->GetQuadraturePosition();
	double lEncoderDistance = (lEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES;
	double rEncoderDistance = (rEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES;
	if((lEncoderDistance>=distance)&&(rEncoderDistance>=distance)){
		return true;
	}
	return false;
}

void DriveSegment::RunAction(){
	chassis->TankDrive(_speed, _speed);
}

void DriveSegment::Start(){
	_leftEncoder->SetQuadraturePosition(0, 10);
	_rightEncoder->SetQuadraturePosition(0, 10);
}

void DriveSegment::End(){
	chassis->TankDrive(0.0, 0.0);
}
