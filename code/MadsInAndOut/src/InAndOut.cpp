#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "InAndOut.h"
#define MOTOR_CLAW_F_SPEED 1.0
#define MOTOR_CLAW_R_SPEED -1.0
#define MOTOR_ROLL_F_SPEED 1.0
#define MOTOR_ROLL_R_SPEED -1.0

InAndOut::InAndOut(int motorClawChannel, int pistonDoorLeftChannel, int pistonDoorRightChannel, int limSwitchUpChannel, int limSwitchDownChannel, int motorRollChannel){
	motorClaw = new WPI_TalonSRX(motorClawChannel);
//	pistonPush = new Solenoid(9, pistonPushChannel);
	pistonDoorLeft = new Solenoid(9, pistonDoorLeftChannel);
	pistonDoorRight = new Solenoid(9, pistonDoorRightChannel);
	limSwitchUp = new DigitalInput(limSwitchUpChannel);
	limSwitchDown = new DigitalInput(limSwitchDownChannel);
	motorRoll = new WPI_TalonSRX(motorRollChannel);
}
InAndOut::~InAndOut(){
	delete motorClaw;
//	delete pistonPush;
	delete pistonDoorLeft;
	delete pistonDoorRight;
	delete limSwitchUp;
	delete limSwitchDown;
}
void InAndOut::MotorClawForward(){
	motorClaw->Set(MOTOR_CLAW_F_SPEED); //make motor go forward @ full speed
}
void InAndOut::MotorClawReverse(){
	motorClaw->Set(MOTOR_CLAW_R_SPEED); //make motor go backwards @ full speed
}
//void InAndOut::PistonPushOpen(){ //We might not need this, depending on what the game component people say
//	pistonPush->Set(true); //make piston go out
//}
void InAndOut::PistonDoorOpen(){
	pistonDoorLeft->Set(true); //make piston go out
	pistonDoorRight->Set(true);
	//Check w/game component group to see how pistons are
	//pistonDoor->Set(false);
}
//void InAndOut::PistonPushClose(){ //We might not need this, depending on what the game component people say
//	pistonPush->Set(false); //make piston go in
//}
void InAndOut::PistonDoorClose(){
	pistonDoorLeft->Set(false); //make piston go in
	pistonDoorRight->Set(false); //make piston go in

	//Check w/game component group to see how pistons are
	//pistonDoor->Set(false);

}
bool InAndOut::GetLimSwitchUp(){
	return limSwitchUp->Get(); //get value (true/false) of limit switch
}
bool InAndOut::GetLimSwitchDown(){
	return limSwitchDown->Get(); //get value (true/false) of limit switch
}

void InAndOut::MotorClawStop() {
	motorClaw -> Set(0.0);
}

void InAndOut::MotorRollForward() {
	motorRoll -> Set(MOTOR_ROLL_F_SPEED);
}

void InAndOut::MotorRollReverse() {
	motorRoll -> Set(MOTOR_ROLL_R_SPEED);
}

void InAndOut::MotorRollStop() {
	motorRoll -> Set(0.0);
}

