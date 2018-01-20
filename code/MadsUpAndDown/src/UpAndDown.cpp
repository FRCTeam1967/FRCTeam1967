#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "UpAndDown.h"

#define R_MOTOR_F_SPEED 0.5
#define L_MOTOR_F_SPEED 0.5
#define R_MOTOR_R_SPEED -0.5
#define L_MOTOR_R_SPEED -0.5

#define PULSES_PER_REVOLUTION 500
#define CIRCUMFERENCE 0.399 * 3.14
#define DISTANCE_PER_PULSE CIRCUMFERENCE/PULSES_PER_REVOLUTION

#define UD_HYSTERESIS_POS 1
#define UD_HYSTERESIS_NEG -1

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
		while (GetLimSwitchOne() == false) {
			RLMotorReverse();
		}
	}
	else if (GetLimSwitchTwo()) {
		while (GetLimSwitchTwo() == false) {
			RLMotorForward();
		}
	}
}

double UpAndDown::GetEncoderDistancePerPulse() {
	return gameMotorEncoder -> GetDistancePerPulse();
}

void UpAndDown::SwitchHeight() {
	newHeight = 19;
}

void UpAndDown::ScaleLowHeight() {
	newHeight = 48.0;
}

void UpAndDown::ScaleMedHeight() {
	newHeight = 60.0;
}

void UpAndDown::ScaleHight() {
	newHeight = 72.0;
}

void UpAndDown::RegularHeight() {
	newHeight = 0.0;
}

void UpAndDown::MoveToNewHeight() {
	amountToMove = newHeight - GetEncoderDistance(); //This finds how far (forward or backward) the motor will have to turn in order to get to a certain height

	if (amountToMove > UD_HYSTERESIS_POS) {
		RLMotorForward();
	}
	else if (amountToMove < UD_HYSTERESIS_NEG) {
		RLMotorReverse();
	}
	else if ((amountToMove < UD_HYSTERESIS_POS) && (amountToMove > UD_HYSTERESIS_NEG)) {
		RLMotorStop();
	}
}
