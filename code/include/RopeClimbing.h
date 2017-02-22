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

#include "jankyTask.h"

//class RopeClimbing:public JankyTask  {
class RopeClimbing  {
public:
	RopeClimbing(int motorAChannel, int motorBChannel, int encoderChannelA, int encoderChannelB, int limitSwitchChannel);
	virtual ~RopeClimbing();

	void StartClimbing();
	void StopClimbing();
	void SetArmed();

	void StartClimbingMotors();
	void StopClimbingMotors();

	void StopAboveMaxCurrent();

	// Only being made public for testing purposes
	bool GetLimitSwitch();
	double GetEncoder();
	int GetState();

	double GetMotorACurrent();
	double GetMotorBCurrent();

//	void Run();

	int climbState;

private:
	CANTalon * motorA;
	CANTalon * motorB;
    Encoder * encoder;
	DigitalInput * limitSwitch;

	//void StartClimbingMotors();


	void SwitchStates();
	bool LimitSwitchPressed();
};

#endif /* ROPECLIMBING_H_ */
