#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "UpAndDown.h"
#include "math.h"
#include "Constants.h"

//Motor speeds
#define R_MOTOR_F_SPEED 0.7
#define L_MOTOR_F_SPEED 0.7
#define R_MOTOR_R_SPEED -0.9
#define L_MOTOR_R_SPEED -0.9

//For distance per pulse in up/down mechanism's encoder
#define UD_PULSES_PER_REVOLUTION 4096
#define GEAR_RATIO 12
#define SPROCKET_TEETH 22
#define SPROCKET_INCHES_PER_TOOTH 0.25

#define UD_CIRCUMFERENCE 5.5 //22 teeth & size 25 chain
//1.8125 * M_PI
#define THIRD_STAGE_PRESENT 2

//Up down hysteresis values (1&-1 are good values for 1/2 speed motors under no load)
#define UD_HYSTERESIS_POS 0.5
#define UD_HYSTERESIS_NEG -0.5

//Field Element Heights
#define SWITCH_HEIGHT 30
#define IN_BETWEEN_HEIGHT 13.0
#define SCALE_LOW_HEIGHT 46.5
#define SCALE_MED_HEIGHT 58.5
#define SCALE_HIGH_HEIGHT 70.5
#define REG_HEIGHT 0.0

//PID
#define P_VAL 0.5
#define I_VAL 0.0
#define D_VAL 0.0

UpAndDown::UpAndDown(int lMotorChannel, int rMotorChannel) {
	lMotor = new WPI_TalonSRX(lMotorChannel);
	rMotor = new WPI_TalonSRX(rMotorChannel);
	rMotor->Set(ControlMode::Follower, lMotorChannel);

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
	//	rMotor->Set(R_MOTOR_F_SPEED);
}

void UpAndDown::RLMotorReverse() {
	lMotor->Set(L_MOTOR_R_SPEED);
	//	rMotor->Set(R_MOTOR_R_SPEED);
}

void UpAndDown::RLMotorStop() {
	lMotor->Set(0.0);
	//	rMotor->Set(0.0);
}

int UpAndDown::GetBottomLimSwitch() {
	//	return bottomLimSwitch -> Get();
	return lMotor->GetSensorCollection().IsFwdLimitSwitchClosed();
	lMotor->GetSensorCollection().SetQuadraturePosition(0,10);
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
void UpAndDown::InBetweenSwitchAndScale() {
	desiredHeight = IN_BETWEEN_HEIGHT;
	isMechanismRunning = true;
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

	//  PID
	//	SmartDashboard::PutNumber("Error", lMotor->GetClosedLoopError(kPIDLoopIdx));
	//	SmartDashboard::PutNumber("DesiredHeight: ", GetEncoderDistanceInPulses(desiredHeight));
	//	SmartDashboard::PutNumber("Current Height: " , lMotor->GetSensorCollection().GetQuadraturePosition());

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

double UpAndDown::GetGameMotorEncoderDistance() {
	lmotorEncoderCount = lMotor->GetSensorCollection().GetQuadraturePosition();
	lmotorEncoderDistance = ((lmotorEncoderCount/(UD_PULSES_PER_REVOLUTION*GEAR_RATIO))*UD_CIRCUMFERENCE)*THIRD_STAGE_PRESENT;
	return lmotorEncoderDistance;
}

double UpAndDown::GetEncoderDistanceInPulses(double desiredHeight) {
	sprocketRevolutions = (SPROCKET_TEETH*SPROCKET_INCHES_PER_TOOTH) * desiredHeight;
	encoderRevolutions = (sprocketRevolutions * GEAR_RATIO);
	encoderDistanceInPulses = (encoderRevolutions * UD_PULSES_PER_REVOLUTION);
	return encoderDistanceInPulses;
}

bool UpAndDown::GetIfMechIsRunning(){
	return isMechanismRunning;
}

void UpAndDown::StartUpInit() {
	lMotor -> GetSensorCollection().SetQuadraturePosition(0,10);
	isMechanismRunning = false;
	desiredHeight = 0.0;
	amountToMove = 0.0;

	reachedMaxHeight = false;
	reachedMinHeight = true;
	needsToPutDownMechanism = true;
	bottomLimSwitchHasNotBeenPressed = true;
	topLimSwitchHasNotBeenPressed = true;
}

void UpAndDown::PIDSetup() {
	int absolutePosition = lMotor->GetSelectedSensorPosition(0); /* mask out the bottom12 bits, we don't care about the wrap arounds */
	/* use the low level API to set the quad encoder signal */
	lMotor->SetSelectedSensorPosition(absolutePosition, kPIDLoopIdx,
			kTimeoutMs);

	/* choose the sensor and sensor direction */
	lMotor->ConfigSelectedFeedbackSensor(
			FeedbackDevice::CTRE_MagEncoder_Relative, kPIDLoopIdx,
			kTimeoutMs);
	lMotor->SetSensorPhase(false);

	/* set the peak and nominal outputs, 12V means full */
	lMotor->ConfigNominalOutputForward(0, kTimeoutMs);
	lMotor->ConfigNominalOutputReverse(0, kTimeoutMs);
	lMotor->ConfigPeakOutputForward(1, kTimeoutMs);
	lMotor->ConfigPeakOutputReverse(-1, kTimeoutMs);

	/* set closed loop gains in slot0 */
	lMotor->Config_kF(kPIDLoopIdx, 0.0, kTimeoutMs);
	lMotor->Config_kP(kPIDLoopIdx, P_VAL, kTimeoutMs);
	lMotor->Config_kI(kPIDLoopIdx, I_VAL, kTimeoutMs);
	lMotor->Config_kD(kPIDLoopIdx, D_VAL, kTimeoutMs);
}

void UpAndDown::Run() {
	//	if (needsToPutDownMechanism) {
	//		PutMechanismDown();
	//	}
	//	else {

	//Display SmartDashboard Comments on the driver station
	SmartDashboardComments();

	//Emergency stop the mechanism with the limit switches
	EmergencyStopMechanism();

	//Print-f statements for PID
	//	printf("Error %d \n", lMotor->GetClosedLoopError(kPIDLoopIdx));
	//	printf("DesiredHeight %f \n", GetEncoderDistanceInPulses(desiredHeight));
	//	printf("Current Height: %d \n" , lMotor->GetSensorCollection().GetQuadraturePosition());

	//	if (isMechanismRunning) {
	//	lMotor->Set(ControlMode::Position, GetEncoderDistanceInPulses(desiredHeight));
	//	isMechanismRunning = false;

	amountToMove = desiredHeight - (GetGameMotorEncoderDistance()*-1); //This finds how far (forward or backward) the motor will have to turn in order to get to a certain height
	if (isMechanismRunning) {
		if (amountToMove > UD_HYSTERESIS_POS) {
			RLMotorReverse();
		}
		else if (amountToMove < UD_HYSTERESIS_NEG) {
			RLMotorForward();

		}
		else if ((amountToMove < UD_HYSTERESIS_POS) && (amountToMove > UD_HYSTERESIS_NEG)) {
			RLMotorStop();
			isMechanismRunning = false;
		}
	}
}
//}
//}

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
