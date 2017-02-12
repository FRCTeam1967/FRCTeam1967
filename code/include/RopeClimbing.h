/*
 * RopeClimbing.h
 *
 *  Created on: Jan 21, 2017
 *      Author: Elaine
 */

#ifndef ROPECLIMBING_H_
#define ROPECLIMBING_H_

#include "WPILib.h"
#include "CANTalon.h"

class RopeClimbing {
public:
	RopeClimbing(int motorAChannel, int motorBChannel, int encoderChannelA, int encoderChannelB, int limitSwitchChannel);
	virtual ~RopeClimbing();

	CANTalon * motorA;
	CANTalon * motorB;
    Encoder * encoder;
	DigitalInput * limitSwitch;

	void StartClimb();
	void StopClimb();
	void SwitchStates();
	bool LimitSwitchPressed();
	bool GetLimitSwitch();
	double GetEncoder();

	int climbState;
};

#endif /* ROPECLIMBING_H_ */
