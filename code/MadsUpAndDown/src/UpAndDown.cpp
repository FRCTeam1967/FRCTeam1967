#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "UpAndDown.h"

#define R_MOTOR_F_SPEED 1.0
#define L_MOTOR_F_SPEED 1.0
#define R_MOTOR_R_SPEED -1.0
#define L_MOTOR_R_SPEED -1.0

#define PULSES_PER_REVOLUTION 500
#define CIRCUMFERENCE 0.399 * 3.14
#define DISTANCE_PER_PULSE CIRCUMFERENCE/PULSES_PER_REVOLUTION


UpAndDown::UpAndDown(int lMotorChannel, int rMotorChannel, int limSwitchOneChannel, int limSwitchTwoChannel, int gameMotorEncoderChannel1, int gameMotorEncoderChannel2) {
	lMotor = new WPI_TalonSRX(lMotorChannel);
	rMotor = new WPI_TalonSRX(rMotorChannel);
	limSwitchOne = new DigitalInput(limSwitchOneChannel);
	limSwitchTwo = new DigitalInput(limSwitchTwoChannel);
	gameMotorEncoder = new Encoder(gameMotorEncoderChannel1, gameMotorEncoderChannel2);
	gameMotorEncoder -> SetDistancePerPulse(DISTANCE_PER_PULSE);
}

UpAndDown::~UpAndDown() {
	delete lMotor;
	delete rMotor;
	delete limSwitchOne;
	delete limSwitchTwo;
	delete gameMotorEncoder;
}

void UpAndDown::RLMotorForward() {
	lMotor->Set(L_MOTOR_F_SPEED);
	rMotor->Set(R_MOTOR_F_SPEED);
}

void UpAndDown::RLMotorReverse() {
	lMotor->Set(L_MOTOR_R_SPEED);
	rMotor->Set(R_MOTOR_R_SPEED);
}

void UpAndDown::RLMotorStop() {
	lMotor->Set(0.0);
	rMotor->Set(0.0);
}

bool UpAndDown::GetLimSwitchOne() {
	return limSwitchOne -> Get();
}

bool UpAndDown::GetLimSwitchTwo() {
	return limSwitchTwo -> Get();
}

double UpAndDown::GetEncoderDistance(){
	return gameMotorEncoder->GetDistance();
}

void UpAndDown::ResetEncoder(){
	gameMotorEncoder -> Reset();
}
void UpAndDown::EmergencyStopMechanism(){
	if (GetLimSwitchOne()) {
		RLMotorStop();
	}
	else if (GetLimSwitchTwo()) {
		RLMotorStop();
		ResetEncoder();
	}
}

void UpAndDown::SwitchHeight() {
	newDistance = 19;
}

void UpAndDown::ScaleLowHeight() {
	newDistance = 48;
}

void UpAndDown::ScaleMedHeight() {
	newDistance = 60;
}

void UpAndDown::ScaleHight() {
	newDistance = 72;
}

void UpAndDown::RegularHeight() {
	newDistance = 0;
}

void UpAndDown::MoveToNewHeight() {
	amountToMove = newDistance - GetEncoderDistance(); //This finds how far (forward or backward) the motor will have to turn in order to get to a certain height

	if (GetEncoderDistance() > amountToMove) {
		RLMotorReverse(); //spin the motor backward
	}
	else if (GetEncoderDistance() < amountToMove) {
		RLMotorForward(); //spin the motor forward

	}
	else if (GetEncoderDistance() == amountToMove) {
		RLMotorStop(); //stop spinning the motor
	}
}
