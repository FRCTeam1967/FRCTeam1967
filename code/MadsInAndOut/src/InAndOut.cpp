#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "InAndOut.h"

//Motor speeds
#define MOTOR_CLAW_F_SPEED 0.5
#define MOTOR_CLAW_R_SPEED -0.5
#define MOTOR_ROLL_F_SPEED 0.5
#define MOTOR_ROLL_R_SPEED -0.5
#define MOTOR_STOP_SPEED 0.0

//For distance per pulse in in/out mechanism's encoder
#define CLAW_PULSES_PER_REVOLUTION 497
#define CLAW_CIRCUMFERENCE 0.399 * 3.14
#define CLAW_DISTANCE_PER_PULSE CLAW_CIRCUMFERENCE/CLAW_PULSES_PER_REVOLUTION

//Hysteresis for in/out mechanism
#define IO_HYSTERESIS_POS 1
#define IO_HYSTERESIS_NEG -1

InAndOut::InAndOut(int motorClawChannel, int pistonDoorLeftChannel, int pistonDoorRightChannel, int limSwitchInsideChannel, int limSwitchOutsideChannel, int motorRollChannel, int clawEncoderChannel1, int clawEncoderChannel2){
	motorClaw = new WPI_TalonSRX(motorClawChannel);
	pistonDoorLeft = new Solenoid(9, pistonDoorLeftChannel);
	pistonDoorRight = new Solenoid(9, pistonDoorRightChannel);
	limSwitchInside = new DigitalInput(limSwitchInsideChannel);
	limSwitchOutside = new DigitalInput(limSwitchOutsideChannel);
	motorRoll = new WPI_TalonSRX(motorRollChannel);
	clawEncoder = new Encoder(clawEncoderChannel1, clawEncoderChannel2);
	clawEncoder -> SetDistancePerPulse(CLAW_DISTANCE_PER_PULSE);

}
InAndOut::~InAndOut(){
	delete motorClaw;
	delete pistonDoorLeft;
	delete pistonDoorRight;
	delete limSwitchInside;
	delete limSwitchOutside;
}
void InAndOut::MotorClawOutOfRobot(){
	motorClaw->Set(MOTOR_CLAW_F_SPEED); ///Spin the motors on the claw mechanism forward, making the claw go out of the robot
}
void InAndOut::MotorClawIntoRobot(){
	motorClaw->Set(MOTOR_CLAW_R_SPEED); //Spin the motors on the claw mechanism backward, making the claw go out of the robot
}

void InAndOut::PistonDoorOpen(){
	pistonDoorLeft->Set(false); //Retract both pistons simultaneously
	pistonDoorRight->Set(false);

}

void InAndOut::PistonDoorClose(){
	pistonDoorLeft->Set(true); //Engage both pistons simultaneously
	pistonDoorRight->Set(true);

}
bool InAndOut::GetLimSwitchInside(){
	return limSwitchInside->Get(); //get value (true/false) of limit switch
}
bool InAndOut::GetLimSwitchOutside(){
	return limSwitchOutside->Get(); //get value (true/false) of limit switch
}

void InAndOut::MotorClawStop() {
	motorClaw -> Set(MOTOR_STOP_SPEED); //Stop the motors on the claw mechanism
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

double InAndOut::GetClawEncoderDistance(){
	return clawEncoder->GetDistance();
}

void InAndOut::ResetClawEncoder(){
	clawEncoder -> Reset();
}
double InAndOut::GetClawEncoderDistancePerPulse() {
	return clawEncoder -> GetDistancePerPulse();
}
void InAndOut::OutsideDistance() {
	desiredDistanceToMove = 0.25;
}
void InAndOut::InsideDistance() {
	desiredDistanceToMove = 0.0;

}

void InAndOut::MoveClawMechanism() {
	amountToMoveClaw = desiredDistanceToMove - GetClawEncoderDistance();

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
