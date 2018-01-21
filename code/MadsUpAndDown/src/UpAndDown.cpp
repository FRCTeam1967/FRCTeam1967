#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "UpAndDown.h"

//Motor speeds
#define R_MOTOR_F_SPEED 0.5
#define L_MOTOR_F_SPEED 0.5
#define R_MOTOR_R_SPEED -0.5
#define L_MOTOR_R_SPEED -0.5

//For distance per pulse in up/down mechanism's encoder
#define UD_PULSES_PER_REVOLUTION 497
#define UD_CIRCUMFERENCE 0.399 * 3.14
#define UD_DISTANCE_PER_PULSE UD_CIRCUMFERENCE/UD_PULSES_PER_REVOLUTION

//Up down hysteresis values
#define UD_HYSTERESIS_POS 1
#define UD_HYSTERESIS_NEG -1

UpAndDown::UpAndDown(int lMotorChannel, int rMotorChannel, int bottomLimSwitchChannel, int topLimSwitchChannel, int gameMotorEncoderChannel1, int gameMotorEncoderChannel2) {
	lMotor = new WPI_TalonSRX(lMotorChannel);
	rMotor = new WPI_TalonSRX(rMotorChannel);
	bottomLimSwitch = new DigitalInput(bottomLimSwitchChannel);
	topLimSwitch = new DigitalInput(topLimSwitchChannel);
	gameMotorEncoder = new Encoder(gameMotorEncoderChannel1, gameMotorEncoderChannel2);
	gameMotorEncoder -> SetDistancePerPulse(UD_DISTANCE_PER_PULSE);
}

UpAndDown::~UpAndDown() {
	delete lMotor;
	delete rMotor;
	delete bottomLimSwitch;
	delete topLimSwitch;
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

bool UpAndDown::GetBottomLimSwitch() {
	return bottomLimSwitch -> Get();
}

bool UpAndDown::GetTopLimSwitch() {
	return topLimSwitch -> Get();
}

double UpAndDown::GetEncoderDistance(){
	return gameMotorEncoder->GetDistance();
}

void UpAndDown::ResetEncoder(){
	gameMotorEncoder -> Reset();
}
void UpAndDown::EmergencyStopMechanism(){
	if (GetBottomLimSwitch()) {
		RLMotorStop();
		isMechanismRunning = false;
		ResetEncoder();
	}
	else if (GetTopLimSwitch()) {
		RLMotorStop();
		isMechanismRunning = false;
	}
}
double UpAndDown::GetEncoderDistancePerPulse() {
	return gameMotorEncoder -> GetDistancePerPulse();
}

void UpAndDown::SwitchHeight() {
	desiredHeight = 19;
}

void UpAndDown::ScaleLowHeight() {
	desiredHeight = 48.0;
}

void UpAndDown::ScaleMedHeight() {
	desiredHeight = 60.0;
}

void UpAndDown::ScaleHight() {
	desiredHeight = 72.0;
}

void UpAndDown::RegularHeight() {
	desiredHeight = 0.0;
}

void UpAndDown::MoveToNewHeight() {
	amountToMove = desiredHeight - GetEncoderDistance(); //This finds how far (forward or backward) the motor will have to turn in order to get to a certain height

	if (amountToMove > UD_HYSTERESIS_POS) {
		RLMotorForward();
	}
	else if (amountToMove < UD_HYSTERESIS_NEG) {
		RLMotorReverse();
	}
	else if ((amountToMove < UD_HYSTERESIS_POS) && (amountToMove > UD_HYSTERESIS_NEG)) {
		RLMotorStop();
		isMechanismRunning = false;
	}
}
