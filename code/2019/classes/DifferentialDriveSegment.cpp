/*
 * DriveSegment.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: AnishaKabir
 */
#include "Settings.h"
#include "frc/WPILib.h"
#include "DifferentialDriveSegment.h"
#include "JankyAutoEntry.h"
#include "ctre/Phoenix.h"
#include "JankyAutoEntry.h"
#include <math.h>
#include "ctre/Phoenix.h"

#define SCALE_FACTOR 1.25
#define MEASURED_DIST_PER_PULSE 0.0912
#define ENCODER_UNITS_PER_ROTATION 4096
#define DIAMETER 6.25
#define CIRCUMFERENCE_INCHES DIAMETER*M_PI*SCALE_FACTOR
#define DISTANCE_TO_RAISE_ARM 0
#define DISTANCE_TO_LOWER_ARM 40 //distance when robot arm will be above scale
#define ARM_DOWN_TIME 0.3

double lEncoderCount;
double rEncoderCount;
double lEncoderDistance;
double rEncoderDistance;
double testEncoderCount;
double testEncoderDistance;

bool armUp;
bool timerStart;

DifferentialDriveSegment::DifferentialDriveSegment(frc::ADXRS450_Gyro*gyro, frc::DifferentialDrive*drive, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*leftmotor, WPI_TalonSRX*rightmotor, int inchDistance, double speed, double p, double i, double d) {
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
	pid = new frc::PIDController(kP,kI,kD,_gyro,this);
	encoderTimer = new frc::Timer();
	armTimer = new frc::Timer();
	armUp=true;
	timerStart=false;
	// TODO Auto-generated constructor stub
}

DifferentialDriveSegment::~DifferentialDriveSegment() {
	// TODO Auto-generated destructor stub
	pid->Disable();
	delete pid;
	delete encoderTimer;
}

bool DifferentialDriveSegment::JobDone(){
	lEncoderCount = -_leftEncoder -> ctre::phoenix::motorcontrol::SensorCollection::GetQuadraturePosition();
	rEncoderCount = _rightEncoder-> ctre::phoenix::motorcontrol::SensorCollection::GetQuadraturePosition();
	lEncoderDistance = (lEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES;
	rEncoderDistance = (rEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES;
	//remove in final code:
	//printf("Left Encoder count %f Right Encoder count %f \n", lEncoderCount, rEncoderCount);
	/*if(encoderTimer->Get()>=0.15){
		encoderReset = true;
	}
	if(encoderReset){*/
		if(distance>=0){
			if(((lEncoderDistance-leftDist)>=distance)&&((rEncoderDistance-rightDist)>=distance)){
				printf("job done \n");
				encoderReset = false;
				return true;
			}
		}
		else{
			if(((lEncoderDistance-leftDist)<=distance)&&((rEncoderDistance-rightDist)<=distance)){
				printf("job done \n");
				encoderReset = false;
				return true;
			}
		}
	//}

	if(encoderTimer->Get()>=maxTime){
		printf("drive segment exited due to timeout \n");
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

void DifferentialDriveSegment::RunAction(){
	//chassis->TankDrive(_speed, _speed);
}

void DifferentialDriveSegment::Start(){
	//int leftError;
	//maybe it's taking so long bc its being double reset?
	//_leftEncoder->SetQuadraturePosition(0, 10);
	//_rightEncoder->SetQuadraturePosition(0, 10);
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
	//armTimer->Start();
	if(distance>=0){
		maxTime = distance/25.0;
	}
	else{
		maxTime = -distance/25.0;
	}
	leftDist=(-_leftEncoder->ctre::phoenix::motorcontrol::SensorCollection::GetQuadraturePosition()/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES;
	rightDist=(-_rightEncoder->ctre::phoenix::motorcontrol::SensorCollection::GetQuadraturePosition()/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES;
	//_encoder->Reset();
}

void DifferentialDriveSegment::End(){
	pid->Disable();
	chassis->TankDrive(0.0, 0.0);
	encoderReset = false;
	printf("drive time: %f \n", encoderTimer->Get());
	encoderTimer->Stop();
	encoderTimer->Reset();
	armTimer->Stop();
	armTimer->Reset();
}

void DifferentialDriveSegment::PIDWrite(double output)
{
	chassis->ArcadeDrive(_speed, output);
	//chassis->CurvatureDrive(_speed, output, true); //IF CHANGING TO ARCADE DRIVE REMEMBER THAT NEGATIVE NUMBERS MEAN FORWARD
}
