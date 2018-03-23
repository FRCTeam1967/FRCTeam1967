/*
 * DriveSegment.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: AnishaKabir
 */
#include "WPILib.h"
#include "DriveSegment.h"
#include "JankyAutoEntry.h"
#include "ctre/Phoenix.h"
#include "JankyAutoEntry.h"
#include <math.h>

#define SCALE_FACTOR 1.25
#define MEASURED_DIST_PER_PULSE 0.0912
#define ENCODER_UNITS_PER_ROTATION 4096
#define DIAMETER 6.25
#define CIRCUMFERENCE_INCHES DIAMETER*M_PI*SCALE_FACTOR

double lEncoderCount;
double rEncoderCount;
double lEncoderDistance;
double rEncoderDistance;
double testEncoderCount;
double testEncoderDistance;

DriveSegment::DriveSegment(frc::ADXRS450_Gyro*gyro, RobotDrive*drive, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*leftmotor, WPI_TalonSRX*rightmotor, int inchDistance, double speed, double p, double i, double d) {
//DriveSegment::DriveSegment(RobotDrive*drive, Encoder*testEncoder, int inchDistance, double speed) {
	distance = inchDistance;
	chassis = drive;
	_leftEncoder = leftEncoder;
	_rightEncoder = rightEncoder;
	_leftmotor = leftmotor;
	_rightmotor = rightmotor;
	//_encoder=testEncoder;
	_speed = speed;
	_gyro=gyro;
	kP = p;
	kI = i;
	kD = d;
	pid = new PIDController(kP,kI,kD,_gyro,this);
	encoderTimer = new Timer();
	// TODO Auto-generated constructor stub
}

DriveSegment::~DriveSegment() {
	// TODO Auto-generated destructor stub
	delete pid;
	delete encoderTimer;
}

bool DriveSegment::JobDone(){
	lEncoderCount = -_leftEncoder->GetQuadraturePosition();
	rEncoderCount = _rightEncoder->GetQuadraturePosition();
	lEncoderDistance = (lEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES;
	rEncoderDistance = (rEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES;
	//remove in final code:
	printf("Left Encoder count %f \n", lEncoderCount);
	printf("Right Encoder count %f \n", rEncoderCount);
	if(encoderTimer->Get()>=0.15){
		encoderReset = true;
	}
	if(encoderReset){
		if((lEncoderDistance>=distance)&&(rEncoderDistance>=distance)){
			printf("job done \n");
			encoderReset = false;
			return true;
		}
	}
	if(encoderTimer->Get()>=maxTime){
		return true;
	}
	/*testEncoderCount=-_encoder->Get();
	testEncoderDistance=(testEncoderCount*MEASURED_DIST_PER_PULSE);
	SmartDashboard::PutNumber("Encoder Count", testEncoderCount);
	//printf("Test Encoder Count %f \n", testEncoderCount);
	SmartDashboard::PutNumber("Encoder Distance", testEncoderDistance);
	if(testEncoderDistance>=distance){
		printf("job done \n");
		return true;
	}*/
	return false;
}

void DriveSegment::RunAction(){
	//chassis->TankDrive(_speed, _speed);
}

void DriveSegment::Start(){
	//int leftError;
	//maybe it's taking so long bc its being double reset?
	_leftEncoder->SetQuadraturePosition(0, 10);
	_rightEncoder->SetQuadraturePosition(0, 10);
	//leftError=_leftmotor->SetSelectedSensorPosition(0, 0, 10);
	//printf("Encoder error %ld \n", leftError);
	//_leftmotor->SetSelectedSensorPosition(0, 0, 10);
	//_rightmotor->SetSelectedSensorPosition(0, 0, 10);
	_gyro->Reset();
	pid->SetInputRange(-180.0, 180.0);
	pid->SetOutputRange(-1.0, 1.0);
	pid->SetSetpoint(0.0);
	pid->Enable();
	encoderReset = false;
	encoderTimer->Start();
	maxTime = distance/28.0;
	//_encoder->Reset();
}

void DriveSegment::End(){
	pid->Disable();
	chassis->TankDrive(0.0, 0.0);
	encoderReset = false;
	printf("drive time: %f \n", encoderTimer->Get());
	encoderTimer->Stop();
	encoderTimer->Reset();
}

void DriveSegment::PIDWrite(double output)
{
	chassis->Drive(_speed, output);
	//chassis->CurvatureDrive(_speed, output, true); //IF CHANGING TO ARCADE DRIVE REMEMBER THAT NEGATIVE NUMBERS MEAN FORWARD
}
