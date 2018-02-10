#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "InAndOut.h"
#include "math.h"

//Motor speeds
#define MOTOR_CLAW_F_SPEED 0.5
#define MOTOR_CLAW_R_SPEED -0.5
#define MOTOR_ROLL_F_SPEED 0.5
#define MOTOR_ROLL_R_SPEED -0.5
#define MOTOR_STOP_SPEED 0.0

//For distance per pulse in in/out mechanism's encoder
#define CLAW_PULSES_PER_REVOLUTION 497
#define CLAW_CIRCUMFERENCE 0.399 * M_PI
#define CLAW_DISTANCE_PER_PULSE CLAW_CIRCUMFERENCE/CLAW_PULSES_PER_REVOLUTION

//Hysteresis for in/out mechanism
#define IO_HYSTERESIS_POS 0.001
#define IO_HYSTERESIS_NEG -0.001

InAndOut::InAndOut(int pistonDoorLeftChannel, int pistonDoorRightChannel, int motorRollChannel, int motorClawChannel, int limSwitchInsideChannel, int limSwitchOutsideChannel){
	pistonDoorLeft = new Solenoid(9, pistonDoorLeftChannel);
	pistonDoorRight = new Solenoid(9, pistonDoorRightChannel);

	motorRoll = new WPI_TalonSRX(motorRollChannel);

	motorClaw = new WPI_TalonSRX(motorClawChannel);
	motorClaw ->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	motorClaw->SetSelectedSensorPosition(0, 0, 10);

	limSwitchInside = new DigitalInput(limSwitchInsideChannel);
	limSwitchOutside = new DigitalInput(limSwitchOutsideChannel);

	//UNUSED
	//	clawEncoder = new Encoder(clawEncoderChannel1, clawEncoderChannel2);
	//	clawEncoder -> SetDistancePerPulse(CLAW_DISTANCE_PER_PULSE);
	//	pistonInOut1 = new Solenoid(9, pistonInOut1Channel);
	//	pistonInOut2 = new Solenoid(9, pistonInOut2Channel);
	//	limSwitchOutside ->ConfigSelectedFeedbackSensor(Analog, 0, 0);
	//	limSwitchOutside->SetSelectedSensorPosition(0, 0, 10);
	//	limSwitchInside ->ConfigSelectedFeedbackSensor(Analog, 0, 0);
	//	limSwitchInside->SetSelectedSensorPosition(0, 0, 10);
}
InAndOut::~InAndOut(){
	delete motorClaw;
	delete motorRoll;
	delete pistonDoorLeft;
	delete pistonDoorRight;
	delete limSwitchInside;
	delete limSwitchOutside;
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

void InAndOut::MoveClawMechanism() {
	amountToMoveClaw = desiredDistanceToMove - GetEncoderDistance();

	if (amountToMoveClaw > IO_HYSTERESIS_POS) {
		MotorClawOutOfRobot();
	}
	else if (amountToMoveClaw < IO_HYSTERESIS_NEG) {
		MotorClawIntoRobot();
	}
	else if ((amountToMoveClaw < IO_HYSTERESIS_POS) && (amountToMoveClaw > IO_HYSTERESIS_NEG)) {
		MotorClawStop();
	}

}

bool InAndOut::GetLimSwitchInside(){
	return limSwitchInside->Get(); //get value (true/false) of limit switch
	//	return limSwitchInside ->GetSensorCollection().IsFwdLimitSwitchClosed();
}

bool InAndOut::GetLimSwitchOutside(){
	return limSwitchOutside->Get(); //get value (true/false) of limit switch
	//	return limSwitchOutside ->GetSensorCollection().IsFwdLimitSwitchClosed();
}

void InAndOut::MotorClawStop() {
	motorClaw -> Set(MOTOR_STOP_SPEED); //Stop the motors on the claw mechanism
}

void InAndOut::MotorClawOutOfRobot(){
	motorClaw->Set(MOTOR_CLAW_F_SPEED); ///Spin the motors on the claw mechanism forward, making the claw go out of the robot
	clawGoingForward = true;
}

void InAndOut::MotorClawIntoRobot(){
	motorClaw->Set(MOTOR_CLAW_R_SPEED); //Spin the motors on the claw mechanism backward, making the claw go out of the robot
	clawGoingBackward = true;
}

void InAndOut::MotorClawStopWithLimSwitches(){
	if (GetLimSwitchOutside() && clawGoingForward) {
		MotorClawStop();
		clawGoingForward = false;
	}
	else if (GetLimSwitchInside() && clawGoingBackward) {
		MotorClawStop();
		clawGoingBackward = false;
	}
}

double InAndOut::GetEncoderDistance() {
	clawEncoderCount = motorClaw->GetSensorCollection().GetQuadraturePosition();
	clawEncoderDistance = (clawEncoderCount/CLAW_PULSES_PER_REVOLUTION)*CLAW_CIRCUMFERENCE;
	return clawEncoderDistance;
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

