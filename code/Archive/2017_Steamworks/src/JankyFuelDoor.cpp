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
	doorPiston = new Solenoid(9, horizontal_piston_channel); //9 on robot and 19 on test robot
	blockPiston = new Solenoid(9, vertical_piston_channel);
	fuelDoorState = QUIET;
	Start();

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

void JankyFuelDoor::SetToQuiet(){
	fuelDoorState=QUIET;
}

void JankyFuelDoor::Command(int currentCommand){
	if (currentCommand==1){
		if (fuelDoorState==QUIET){
	//		printf("State Changed to Start Collecting");
			fuelDoorState=STARTCOLLECTING;
		}
	}
	else if(currentCommand==2){
		if (fuelDoorState==ISCOLLECTING){
			fuelDoorState=ISCLOSINGAFTERCOLLECTING;
		}
		else if(fuelDoorState==DUMPING){
			fuelDoorState=ISCLOSINGAFTERDUMPING;
		}
	}
	else if(currentCommand==3){
		if(fuelDoorState==QUIET){
		fuelDoorState=DUMPING;
		}
	}
}
void JankyFuelDoor::Run(){
	switch(fuelDoorState){
		case QUIET:
			CloseDoor();
			PullUpBlock();
	//		printf("In Quiet State\n");
			break;
		case STARTCOLLECTING:
				OpenDoor();
				printf("Door Opened, Begin Wait\n");
				Wait(2);
				printf("End of Wait\n");
				PushDownBlock();
				printf("Block Pushed Down, Begin Wait\n");
				Wait(2);
				printf("End of Wait\n");
				CloseDoor();
				printf("Door Closed\n");
				fuelDoorState=ISCOLLECTING;
				break;
		case ISCOLLECTING:
			printf("In IsCollecting State\n");
			break;
		case ISCLOSINGAFTERCOLLECTING:
			PullUpBlock();
			printf("Block Pulled Up, Begin Wait\n");
			Wait(2);
			printf("End of Wait\n");
			fuelDoorState=QUIET;
			break;
		case ISCLOSINGAFTERDUMPING:
			CloseDoor();
			printf("Door Closed, Begin Wait\n");
			Wait(2);
			printf("End of Wait\n");
			fuelDoorState=QUIET;
			break;
		case DUMPING:
			OpenDoor();
			printf("Door Opened\n");
		break;
	}
}

