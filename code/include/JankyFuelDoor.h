/*
 * JankyFuelDoor.h
 *
 *  Created on: Mar 4, 2017
 *      Author: AnishaKabir
 */
#include "WPILib.h"
#ifndef SRC_JANKYFUELDOOR_H_
#define SRC_JANKYFUELDOOR_H_

class JankyFuelDoor {

public:
	JankyFuelDoor(int vertical_piston_channel, int horizontal_piston_channel);
	virtual ~JankyFuelDoor();
	Solenoid*doorPiston;
	Solenoid*blockPiston;
	void OpenDoor();
	void PushDownBlock();
	void CloseDoor();
	void PullUpBlock();
	void Command(int command);
	void Run();

	int fuelDoorState;
	int currentCommand;

};

#endif /* SRC_JANKYFUELDOOR_H_ */
