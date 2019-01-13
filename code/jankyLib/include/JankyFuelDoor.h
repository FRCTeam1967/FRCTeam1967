/*
 * JankyFuelDoor.h
 *
 *  Created on: Mar 4, 2017
 *      Author: AnishaKabir
 */
#include "frc/WPILib.h"
#include "jankyTask.h"
#ifndef SRC_JANKYFUELDOOR_H_
#define SRC_JANKYFUELDOOR_H_

class JankyFuelDoor:public JankyTask {

public:
	JankyFuelDoor(int vertical_piston_channel, int horizontal_piston_channel);
	virtual ~JankyFuelDoor();
	frc::Solenoid*doorPiston;
	frc::Solenoid*blockPiston;
	void OpenDoor();
	void PushDownBlock();
	void CloseDoor();
	void PullUpBlock();
	void SetToQuiet();
	void Command(int currentCommand);
	void Run();

	int fuelDoorState;

};

#endif /* SRC_JANKYFUELDOOR_H_ */
