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

//	void PistonPushOpen(); //Push the power cube out of the claw by pushing out the piston
	void PistonDoorOpen(); //Open the "doors" of the claw by pushing out the piston

//	void PistonPushClose(); //Bring the piston that pushed the cube back in so that it is ready to push another cube again
	void PistonDoorClose(); //Close the "doors" of the claw by bringing in the piston

	void MotorClawForward(); //Make the claw mechanism extend forward 180º out of the robot
	void MotorClawReverse(); //Make the claw mechanism extend forward 180º into the robot
	void MotorClawStop();

	void MotorRollForward();
	void MotorRollReverse();
	void MotorRollStop();

private:
	WPI_TalonSRX*motorClaw;
	WPI_TalonSRX*motorRoll;
	Solenoid*pistonDoorRight;
	Solenoid*pistonDoorLeft;
//	Solenoid*pistonPush;
	DigitalInput*limSwitchDown;
	DigitalInput*limSwitchUp;
};

#endif
