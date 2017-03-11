/*
 * JankyFuelDoor.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: AnishaKabir
 */
#include "WPILib.h"
#include <JankyFuelDoor.h>
//States
#define QUIET 1
#define STARTCOLLECTING 2
#define ISCOLLECTING 3
#define ISCLOSINGAFTERCOLLECTING 4
#define ISCLOSINGAFTERDUMPING 5
#define DUMPING 6


JankyFuelDoor::JankyFuelDoor(int vertical_piston_channel, int horizontal_piston_channel) {
	doorPiston = new Solenoid(9, horizontal_piston_channel);
	blockPiston = new Solenoid(9, vertical_piston_channel);
	fuelDoorState = QUIET;
	// TODO Auto-generated constructor stub
// need timer + all the pistons
}

JankyFuelDoor::~JankyFuelDoor() {
	// TODO Auto-generated destructor stub
	delete doorPiston;
	delete blockPiston;
}

void JankyFuelDoor::OpenDoor(){
	doorPiston->Set(true);
}

void JankyFuelDoor::PushDownBlock(){
	blockPiston->Set(true);
}

void JankyFuelDoor::CloseDoor(){
	doorPiston->Set(false);
}

void JankyFuelDoor::PullUpBlock(){
	blockPiston->Set(false);
}

void JankyFuelDoor::Command(int currentCommand){
	switch(fuelDoorState){
			case QUIET:
				if (currentCommand==1){
					fuelDoorState=STARTCOLLECTING;
				}
				if (currentCommand==3){
					fuelDoorState=DUMPING;
				}
				break;
			case STARTCOLLECTING:
				JankyFuelDoor::OpenDoor();
				Wait(2);
				JankyFuelDoor::PushDownBlock();
				Wait(2);
				JankyFuelDoor::CloseDoor();
				fuelDoorState=ISCOLLECTING;
				break;
			case ISCOLLECTING:
				if (currentCommand==2){
					fuelDoorState=ISCLOSINGAFTERCOLLECTING;
				}
				break;
			case ISCLOSINGAFTERCOLLECTING:
				JankyFuelDoor::PullUpBlock();
				Wait(2);
				fuelDoorState=QUIET;
				break;
			case ISCLOSINGAFTERDUMPING:
				JankyFuelDoor::CloseDoor();
				Wait(2);
				fuelDoorState=QUIET;
				break;
			case DUMPING:
				JankyFuelDoor::OpenDoor();
				if (currentCommand==2){
					fuelDoorState=ISCLOSINGAFTERDUMPING;
				}
				break;
	/*if (currentCommand==1){
	}
	else if (currentCommand==2){

	}
	else if(currentCommand==3){

	}*/
	}
}
void JankyFuelDoor::Run(){

}

