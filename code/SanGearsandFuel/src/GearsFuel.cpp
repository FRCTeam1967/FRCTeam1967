/*
 * GearsFuel.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: Sana
 */
#include "WPILib.h"
#include "GearsFuel.h"



#define BALL_MOTOR_SPEED 0.75
#define XBOX_CHANNEL 0
#define BALL_MOTOR_CHANNEL 1
#define DOOR_PISTON_CHANNEL 5//our chassis
#define GEAR_BOX_CHANNEL 1 //their chassis
#define PUSH_GEAR_CHANNEL 2
#define PCM_MOD_NUMBER 9

GearsFuel::GearsFuel(int ballMotorChannel, int doorPistonChannel, int gearBoxChannel, int pushGearChannel){
	ballMotor= new CANTalon(ballMotorChannel);
	solenoid1 = new Solenoid(PCM_MOD_NUMBER,doorPistonChannel);
	solenoid2 = new Solenoid(PCM_MOD_NUMBER,gearBoxChannel);
	solenoid3 = new Solenoid(PCM_MOD_NUMBER,pushGearChannel);


}

GearsFuel::~GearsFuel(void) {
	delete ballMotor;
	delete solenoid1;
	delete solenoid2;
	delete solenoid3;
}

void GearsFuel::FuelIntake(void){
	ballMotor->Set(BALL_MOTOR_SPEED);



}
void GearsFuel::StopBallMotor(void) {
		// stop the ball motor
		ballMotor->Set(0.0);
}

void GearsFuel::DoorUp(void) {
	//move door up
	solenoid1->Set(true);
}
void GearsFuel::DoorDown(void){
	//move door down
	solenoid1->Set(false);
}

void GearsFuel::BoxOut(void){
	//move box out
	solenoid2->Set(true);
}
void GearsFuel::BoxIn(void){
	//move box in
	solenoid2->Set(false);
}

void GearsFuel::GearOut(void){
	solenoid3->Set(true);
}
void GearsFuel::GearIn(void){
	solenoid3->Set(false);
}
