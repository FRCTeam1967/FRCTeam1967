#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "InAndOut.h"
#include "math.h"
#include "jankyTask.h"
#include "Constants.h"

//Motor speeds
#define MOTOR_CLAW_F_SPEED 0.4

#define MOTOR_CLAW_R_SPEED -0.4
#define MOTOR_ROLL_F_SPEED 1.0
#define MOTOR_ROLL_R_SPEED -1.0
#define MOTOR_STOP_SPEED 0.0

//Encoder pulses
#define CLAW_PULSES_PER_REVOLUTION 4096

//Values to move motor w/ encoder
#define FORWARD_NUMBER_OF_PULSES 750
#define BACKWARD_NUMBER_OF_PULSES -750

//Hysteresis for in/out mechanism
#define IO_HYSTERESIS_POS 10
#define IO_HYSTERESIS_NEG -10

InAndOut::InAndOut(int pistonDoorLeftChannel, int pistonDoorRightChannel, int motorRollChannel, int motorClawChannel){
	pistonDoorLeft = new Solenoid(10, pistonDoorLeftChannel);
	pistonDoorRight = new Solenoid(10, pistonDoorRightChannel);

	motorRoll = new WPI_TalonSRX(motorRollChannel);

	motorClaw = new WPI_TalonSRX(motorClawChannel);
	motorClaw -> ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	motorClaw -> SetSelectedSensorPosition(0, 0, 10);
	motorClaw -> GetSensorCollection().SetQuadraturePosition(0,10);

	//	motorClaw->ConfigForwardLimitSwitchSource(RemoteLimitSwitchSource_RemoteTalonSRX , LimitSwitchNormal_NormallyOpen , 7, 0);
	//	motorClaw->ConfigReverseLimitSwitchSource(RemoteLimitSwitchSource_RemoteTalonSRX , LimitSwitchNormal_NormallyOpen , 7, 0);


	//UNUSED
	//	clawEncoder = new Encoder(clawEncoderChannel1, clawEncoderChannel2);
	//	clawEncoder -> SetDistancePerPulse(CLAW_DISTANCE_PER_PULSE);
	//	pistonInOut1 = new Solenoid(9, pistonInOut1Channel);
	//	pistonInOut2 = new Solenoid(9, pistonInOut2Channel);
	//	limSwitchInside = new DigitalInput(limSwitchInsideChannel);
	//	limSwitchOutside = new DigitalInput(limSwitchOutsideChannel);

}
InAndOut::~InAndOut(){
	delete motorClaw;
	delete motorRoll;
	delete pistonDoorLeft;
	delete pistonDoorRight;

	//  Unused
	//	delete limSwitchInside;
	//	delete limSwitchOutside;
}

void InAndOut::PistonDoorOpen(){
	pistonDoorLeft->Set(false); //Retract both pistons simultaneously
	pistonDoorRight->Set(false);
}

void InAndOut::PistonDoorClose(){
	pistonDoorLeft->Set(true); //Engage both pistons simultaneously
	pistonDoorRight->Set(true);
}

void InAndOut::MotorRollForward() {
	motorRoll -> Set(MOTOR_ROLL_F_SPEED); //Make the rollers go forwards
}

void InAndOut::MotorRollReverse() {
	motorRoll -> Set(MOTOR_ROLL_R_SPEED); //Make the rollers go backwards
}

void InAndOut::MotorRollStop() {
	motorRoll -> Set(MOTOR_STOP_SPEED); //Stop the rollers
}

void InAndOut::OutsideDistance() {
	desiredDistanceToMove = 0.25;
}

void InAndOut::InsideDistance() {
	desiredDistanceToMove = 0.0;
}

int InAndOut::GetLimSwitchInside(){
	//	return limSwitchInside->Get(); //get value (true/false) of limit switch
	return motorClaw ->GetSensorCollection().IsFwdLimitSwitchClosed();
}

int InAndOut::GetLimSwitchOutside(){
	//	return limSwitchOutside->Get(); //get value (true/false) of limit switch
	return motorClaw ->GetSensorCollection().IsRevLimitSwitchClosed();
}

void InAndOut::MotorClawStop() {
	motorClaw -> Set(MOTOR_STOP_SPEED); //Stop the motors on the claw mechanism
	clawGoingForward = false;
	clawGoingBackward = false;
}

void InAndOut::MotorClawOutOfRobot(){
	motorClaw->Set(MOTOR_CLAW_F_SPEED); ///Spin the motors on the claw mechanism forward, making the claw go out of the robot
	clawGoingForward = true;
	clawGoingBackward = false;
	clawPositionIsOut = true;
}

void InAndOut::MotorClawIntoRobot(){
	motorClaw->Set(MOTOR_CLAW_R_SPEED); //Spin the motors on the claw mechanism backward, making the claw go out of the robot
	clawGoingBackward = true;
	clawGoingForward = false;
	clawPositionIsOut = false;
}

void InAndOut::MotorClawStopWithLimSwitches(){
	if ((GetLimSwitchOutside()==1) && clawGoingForward) {
		MotorClawStop();
	}
	else if ((GetLimSwitchInside()==1) && clawGoingBackward) {
		MotorClawStop();
	}
}

double InAndOut::GetEncoderCount() {
	clawEncoderCount = motorClaw->GetSensorCollection().GetQuadraturePosition();
	return clawEncoderCount;
}

bool InAndOut::GetClawPosition() {
	return clawPositionIsOut;
}

double InAndOut::GetDesiredDistance() {
	if (clawPositionIsOut) {
		desiredDistanceToMove = BACKWARD_NUMBER_OF_PULSES;
	}
	else if (clawPositionIsOut == false){
		desiredDistanceToMove = FORWARD_NUMBER_OF_PULSES;
	}
	return desiredDistanceToMove;
}

void InAndOut::MoveClawDownInAuto(){
	desiredDistanceToMove = FORWARD_NUMBER_OF_PULSES;
	needsToPutDownClaw = false;
}

void InAndOut::StartUpInit() {
	motorClaw -> GetSensorCollection().SetQuadraturePosition(0,10);
	clawEncoderCount = 0.0;
	clawEncoderDistance = 0.0;
	desiredDistanceToMove = 0.0;
	amountToMoveClaw = 0.0;
	clawGoingForward = false;
	clawGoingBackward = false;
	needsToPutDownClaw = true;
	clawPositionIsOut = true;
}

void InAndOut::PIDSetup() {
	int absolutePositionClaw = motorClaw->GetSelectedSensorPosition(0); /* mask out the bottom12 bits, we don't care about the wrap arounds */
	/* use the low level API to set the quad encoder signal */
	motorClaw->SetSelectedSensorPosition(absolutePositionClaw, kPIDLoopIdx,
			kTimeoutMs);

	/* choose the sensor and sensor direction */
	motorClaw->ConfigSelectedFeedbackSensor(
			FeedbackDevice::CTRE_MagEncoder_Relative, kPIDLoopIdx,
			kTimeoutMs);
	motorClaw->SetSensorPhase(true);

	/* set the peak and nominal outputs, 12V means full */
	motorClaw->ConfigNominalOutputForward(0, kTimeoutMs);
	motorClaw->ConfigNominalOutputReverse(0, kTimeoutMs);
	motorClaw->ConfigPeakOutputForward(1, kTimeoutMs);
	motorClaw->ConfigPeakOutputReverse(-1, kTimeoutMs);

	/* set closed loop gains in slot0 */
	motorClaw->Config_kF(kPIDLoopIdx, 0.0, kTimeoutMs);
	motorClaw->Config_kP(kPIDLoopIdx, 0.1, kTimeoutMs);
	motorClaw->Config_kI(kPIDLoopIdx, 0.0, kTimeoutMs);
	motorClaw->Config_kD(kPIDLoopIdx, 0.0, kTimeoutMs);
}

void InAndOut::Run() {
	//	Put the claw mechanism down once during auto
	//	if (needsToPutDownClaw) {
	//			MoveClawDownInAuto();
	//	}

	//  Emergency stop the claw depending on what limit switches are pressed
	MotorClawStopWithLimSwitches();

	//Move motor claw with PID
	//	motorClaw->Set(ControlMode::Position, GetDesiredDistance());

	//	Moving the claw mechanism with encoders
	amountToMoveClaw = desiredDistanceToMove - (GetEncoderCount()*-1);
	SmartDashboard::PutNumber("InOut Encoder" , GetEncoderCount());
	if (amountToMoveClaw > IO_HYSTERESIS_POS) {
		//	MotorClawOutOfRobot();
		MotorClawIntoRobot();
	}
	else if (amountToMoveClaw < IO_HYSTERESIS_NEG) {
		//	MotorClawIntoRobot();
		MotorClawOutOfRobot();

	}
	else if ((amountToMoveClaw < IO_HYSTERESIS_POS) && (amountToMoveClaw > IO_HYSTERESIS_NEG)) {
		MotorClawStop();
	}
}

//UNUSED
/*
void InAndOut::ClawDown() {
	pistonInOut1 -> Set(true);
	pistonInOut2 -> Set(true);
}

void InAndOut::ClawUp() {
	pistonInOut1 -> Set(false);
	pistonInOut2 -> Set(false);
}
double InAndOut::GetClawEncoderDistance(){
	return clawEncoder->GetDistance();
}

void InAndOut::ResetClawEncoder(){
	clawEncoder -> Reset();
}

double InAndOut::GetClawEncoderDistancePerPulse() {
	return clawEncoder -> GetDistancePerPulse();
}
 */

