#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "UpAndDown.h"
#include "math.h"

//Motor speeds
#define R_MOTOR_F_SPEED 0.7
#define L_MOTOR_F_SPEED 0.7
#define R_MOTOR_R_SPEED -0.5
#define L_MOTOR_R_SPEED -0.5

//For distance per pulse in up/down mechanism's encoder
#define UD_PULSES_PER_REVOLUTION 49152
//#define GEAR_RATIO 12
#define UD_CIRCUMFERENCE 5.5 //22 teeth & size 25 chain //1.8125 * M_PI
#define THIRD_STAGE_PRESENT 1
//#define UD_DISTANCE_PER_PULSE UD_CIRCUMFERENCE/PULSES_PER_SPROCKET


//Up down hysteresis values (1&-1 are good values for 1/2 speed motors under no load)
#define UD_HYSTERESIS_POS 1.0
#define UD_HYSTERESIS_NEG -1.0

//Field Element Heights
#define SWITCH_HEIGHT 19.0
#define SCALE_LOW_HEIGHT 6.0
#define SCALE_MED_HEIGHT 8.0
#define SCALE_HIGH_HEIGHT 10.0
#define REG_HEIGHT 0.0

UpAndDown::UpAndDown(int lMotorChannel, int rMotorChannel) {
	lMotor = new WPI_TalonSRX(lMotorChannel);
	rMotor = new WPI_TalonSRX(rMotorChannel);

	//ToDo: Set l&r motors to brake mode

	lMotor ->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	lMotor -> SetSelectedSensorPosition(0, 0, 10);
	lMotor -> GetSensorCollection().SetQuadraturePosition(0,10);

//	lMotor->ConfigForwardLimitSwitchSource(RemoteLimitSwitchSource_RemoteTalonSRX , LimitSwitchNormal_NormallyOpen , 6, 0);
//	lMotor->ConfigReverseLimitSwitchSource(RemoteLimitSwitchSource_RemoteTalonSRX , LimitSwitchNormal_NormallyOpen , 6, 0);

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

	//  UNUSED
	//	delete bottomLimSwitch;
	//	delete topLimSwitch;
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
	return lMotor->GetSensorCollection().IsFwdLimitSwitchClosed();
}

int UpAndDown::GetTopLimSwitch() {
	return lMotor->GetSensorCollection().IsRevLimitSwitchClosed();
	//	return topLimSwitch -> Get();
}

void UpAndDown::EmergencyStopMechanism(){
	if ((GetBottomLimSwitch()==1) && bottomLimSwitchHasNotBeenPressed) {
		RLMotorStop();
		isMechanismRunning = false;
		//		ResetEncoder();
		bottomLimSwitchHasNotBeenPressed = false;
	}
	else if (GetBottomLimSwitch()==0) {
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

void UpAndDown::SwitchHeight() {
	desiredHeight = SWITCH_HEIGHT;
	isMechanismRunning = true;
}

void UpAndDown::ScaleLowHeight() {
	desiredHeight = SCALE_LOW_HEIGHT;
	isMechanismRunning = true;
}

void UpAndDown::ScaleMedHeight() {
	desiredHeight = SCALE_MED_HEIGHT;
	isMechanismRunning = true;
}

void UpAndDown::ScaleHight() {
	desiredHeight = SCALE_HIGH_HEIGHT;
	isMechanismRunning = true;
}

void UpAndDown::RegularHeight() {
	desiredHeight = REG_HEIGHT;
	isMechanismRunning = true;
}

void UpAndDown::SmartDashboardComments() {
	SmartDashboard::PutNumber("Desired Height", desiredHeight);
	SmartDashboard::PutNumber("Amount To Move", amountToMove);
	SmartDashboard::PutBoolean("Limit switch top value", GetTopLimSwitch());
	SmartDashboard::PutBoolean("Limit switch bottom value", GetBottomLimSwitch());
	SmartDashboard::PutNumber("Encoder Dist:" ,GetGameMotorEncoderDistance());

	//  UNUSED
	//	SmartDashboard::PutNumber("Distance Per Pulse", GetEncoderDistancePerPulse());
	//	SmartDashboard::PutNumber("Game Component Encoder: ", GetEncoderDistance());
}

void UpAndDown::PutMechanismDown() {
	RLMotorReverse();
	if (GetBottomLimSwitch() == 1) {
		RLMotorStop();
		needsToPutDownMechanism = false;
		bottomLimSwitchHasNotBeenPressed = false;
		topLimSwitchHasNotBeenPressed = true;
	}
}

void UpAndDown::Run() {
//	if (needsToPutDownMechanism) {
//		PutMechanismDown();
//	}
//	else {
		//Display SmartDashboard Comments on the driver station
		SmartDashboardComments();

		EmergencyStopMechanism();

//		if (isMechanismRunning) {
			amountToMove = desiredHeight - (GetGameMotorEncoderDistance()*-1); //This finds how far (forward or backward) the motor will have to turn in order to get to a certain height

			if (amountToMove > UD_HYSTERESIS_POS) {
//				RLMotorForward();
				RLMotorReverse();
			}
			else if (amountToMove < UD_HYSTERESIS_NEG) {
//				RLMotorReverse();
				RLMotorForward();

			}
			else if ((amountToMove < UD_HYSTERESIS_POS) && (amountToMove > UD_HYSTERESIS_NEG)) {
				RLMotorStop();
				isMechanismRunning = false;
			}
//		}
//	}
}

double UpAndDown::GetGameMotorEncoderDistance() {
	lmotorEncoderCount = lMotor->GetSensorCollection().GetQuadraturePosition();
	lmotorEncoderDistance = ((lmotorEncoderCount/UD_PULSES_PER_REVOLUTION)*UD_CIRCUMFERENCE)*THIRD_STAGE_PRESENT;
	return lmotorEncoderDistance;
}

bool UpAndDown::GetIfMechIsRunning(){
	return isMechanismRunning;
}

void UpAndDown::StartUpInit() {
	isMechanismRunning = false;
	desiredHeight = 0.0;
	amountToMove = 0.0;

	reachedMaxHeight = false;
	reachedMinHeight = true;
	needsToPutDownMechanism = true;
	bottomLimSwitchHasNotBeenPressed = true;
	topLimSwitchHasNotBeenPressed = true;
}

//UNUSED
/*
 double UpAndDown::GetEncoderDistancePerPulse() {
	return gameMotorEncoder -> GetDistancePerPulse();
}

double UpAndDown::GetEncoderDistance(){
	return gameMotorEncoder->GetDistance();
}

void UpAndDown::ResetEncoder(){
	gameMotorEncoder -> Reset();
}
 */
