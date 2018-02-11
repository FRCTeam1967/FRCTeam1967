#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "UpAndDown.h"
#include "math.h"

//Motor speeds
#define R_MOTOR_F_SPEED 0.5
#define L_MOTOR_F_SPEED -0.5
#define R_MOTOR_R_SPEED -0.5
#define L_MOTOR_R_SPEED 0.5

//For distance per pulse in up/down mechanism's encoder
#define UD_PULSES_PER_REVOLUTION 4096
#define UD_CIRCUMFERENCE 0.399 * M_PI
#define UD_DISTANCE_PER_PULSE UD_CIRCUMFERENCE/UD_PULSES_PER_REVOLUTION

//Up down hysteresis values
#define UD_HYSTERESIS_POS 1
#define UD_HYSTERESIS_NEG -1

UpAndDown::UpAndDown(int lMotorChannel, int rMotorChannel, int gameMotorEncoderChannel1, int gameMotorEncoderChannel2) {
	lMotor = new WPI_TalonSRX(lMotorChannel);
	rMotor = new WPI_TalonSRX(rMotorChannel);

	lMotor ->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	lMotor -> SetSelectedSensorPosition(0, 0, 10);
	lMotor -> GetSensorCollection().SetQuadraturePosition(0,10);

	bottomLimSwitch->ConfigForwardLimitSwitchSource(RemoteLimitSwitchSource_RemoteTalonSRX , LimitSwitchNormal_NormallyOpen , 10, 0);
	topLimSwitch->ConfigForwardLimitSwitchSource(RemoteLimitSwitchSource_RemoteTalonSRX , LimitSwitchNormal_NormallyOpen , 10, 0);

	//  UNUSED
	//	rMotor ->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	//	rMotor -> SetSelectedSensorPosition(0, 0, 10);
	//	rMotor -> GetSensorCollection().SetQuadraturePosition(0,10);
	//	gameMotorEncoder = new Encoder(gameMotorEncoderChannel1, gameMotorEncoderChannel2);
	//	gameMotorEncoder -> SetDistancePerPulse(UD_DISTANCE_PER_PULSE);
	//	bottomLimSwitch = new DigitalInput(bottomLimSwitchChannel);
	//	topLimSwitch = new DigitalInput(topLimSwitchChannel);
}

UpAndDown::~UpAndDown() {
	delete lMotor;
	delete rMotor;
	delete bottomLimSwitch;
	delete topLimSwitch;
	//	delete gameMotorEncoder;
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

int UpAndDown::GetBottomLimSwitch() {
	//	return bottomLimSwitch -> Get();
	return bottomLimSwitch->GetSensorCollection().IsFwdLimitSwitchClosed();
}

int UpAndDown::GetTopLimSwitch() {
	return topLimSwitch->GetSensorCollection().IsFwdLimitSwitchClosed();
	//	return topLimSwitch -> Get();
}

//double UpAndDown::GetEncoderDistance(){
//	return gameMotorEncoder->GetDistance();
//}
//
//void UpAndDown::ResetEncoder(){
//	gameMotorEncoder -> Reset();
//}

void UpAndDown::EmergencyStopMechanism(){
	if ((GetBottomLimSwitch()==1) && bottomLimSwitchHasNotBeenPressed) {
		RLMotorStop();
		isMechanismRunning = false;
		//		ResetEncoder();
		bottomLimSwitchHasNotBeenPressed = false;
	}
	else if (!GetBottomLimSwitch()==0) {
		bottomLimSwitchHasNotBeenPressed = true;
	}

	if ((GetTopLimSwitch()==1) && topLimSwitchHasNotBeenPressed) {
		RLMotorStop();
		isMechanismRunning = false;
		topLimSwitchHasNotBeenPressed = false;
	}
	else if (GetTopLimSwitch()==0) {
		topLimSwitchHasNotBeenPressed = true;
	}
}

//double UpAndDown::GetEncoderDistancePerPulse() {
//	return gameMotorEncoder -> GetDistancePerPulse();
//}

void UpAndDown::SwitchHeight() {
	desiredHeight = 19;
	isMechanismRunning = true;

}

void UpAndDown::ScaleLowHeight() {
	desiredHeight = 48.0;
	isMechanismRunning = true;
}

void UpAndDown::ScaleMedHeight() {
	desiredHeight = 60.0;
	isMechanismRunning = true;
}

void UpAndDown::ScaleHight() {
	desiredHeight = 72.0;
	isMechanismRunning = true;
}

void UpAndDown::RegularHeight() {
	desiredHeight = 0.0;
	isMechanismRunning = true;
}

void UpAndDown::SmartDashboardComments() {
	//	SmartDashboard::PutNumber("Game Component Encoder: ", GetEncoderDistance());
	SmartDashboard::PutNumber("Desired Height", desiredHeight);
	SmartDashboard::PutNumber("Amount To Move", amountToMove);
	//	SmartDashboard::PutNumber("Distance Per Pulse", GetEncoderDistancePerPulse());
	SmartDashboard::PutBoolean("Limit switch top value", GetTopLimSwitch());
	SmartDashboard::PutBoolean("Limit switch bottom value", GetBottomLimSwitch());
	SmartDashboard::PutNumber("Encoder Dist:" ,GetGameMotorEncoderDistance());
}

void UpAndDown::PutMechanismDown() {

	RLMotorReverse();
	if (GetBottomLimSwitch()) {
		RLMotorStop();
		needsToPutDownMechanism = false;
		bottomLimSwitchHasNotBeenPressed = false;
		topLimSwitchHasNotBeenPressed = true;
	}
}

void UpAndDown::Run() {
	if (needsToPutDownMechanism) {
		PutMechanismDown();
	}

	SmartDashboardComments(); //Display SmartDashboard Comments on the driver station

	EmergencyStopMechanism();

	if (isMechanismRunning) {
		amountToMove = desiredHeight - GetGameMotorEncoderDistance(); //This finds how far (forward or backward) the motor will have to turn in order to get to a certain height

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
}

double UpAndDown::GetGameMotorEncoderDistance() {
	lmotorEncoderCount = lMotor->GetSensorCollection().GetQuadraturePosition();
	lmotorEncoderDistance = (lmotorEncoderCount/UD_PULSES_PER_REVOLUTION)*UD_CIRCUMFERENCE;
	return lmotorEncoderDistance;
}
