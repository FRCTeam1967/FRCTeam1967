#include "WPILib.h"
#include "ctre/Phoenix.h"

#ifndef INANDOUT_H_
#define INANDOUT_H_

class InAndOut {
public:
	InAndOut(int motorClawChannel, int pistonDoorLeftChannel, int pistonDoorRightChannel, int limSwitchUpChannel, int limSwitchDownChannel, int motorRollChannel);
	virtual ~InAndOut();

	bool GetLimSwitchDown(); //Get the value of the lower limit switch (true/false)
	bool GetLimSwitchUp(); //Get the value of the upper limit switch (true/false)

	void PistonDoorOpen(); //Open the "doors" of the claw by bringing in the piston
	void PistonDoorClose(); //Close the "doors" of the claw by pushing out the piston

	void MotorClawForward(); //Make the claw mechanism extend forward 180º out of the robot
	void MotorClawReverse(); //Make the claw mechanism extend forward 180º into the robot
	void MotorClawStop();

	void MotorRollForward(); //Make the rollers go forward to push out the cube
	void MotorRollReverse(); //Make the rollers go backwards to pull in the sube
	void MotorRollStop(); //Make the rollers stop

private:
	WPI_TalonSRX*motorClaw;
	WPI_TalonSRX*motorRoll;
	Solenoid*pistonDoorRight;
	Solenoid*pistonDoorLeft;
	DigitalInput*limSwitchDown;
	DigitalInput*limSwitchUp;
};

#endif
