#include "WPILib.h"
#include "ctre/Phoenix.h"

#ifndef INANDOUT_H_
#define INANDOUT_H_

class InAndOut {
public:
	double desiredDistanceToMove = 0.0;
	double amountToMoveClaw = 0.0;

	InAndOut(int motorClawChannel, int pistonDoorLeftChannel, int pistonDoorRightChannel, int limSwitchInsideChannel, int limSwitchOutsideChannel, int motorRollChannel, int clawEncoderChannel1, int clawEncoderChannel2);
	virtual ~InAndOut();

	bool GetLimSwitchOutside(); //Get the value of the limit switch for when the claw goes outside the robot(true/false)
	bool GetLimSwitchInside(); //Get the value of the limit switch for when the claw goes inside the robot(true/false)

	void PistonDoorOpen(); //Open the "doors" of the claw by retracting the piston
	void PistonDoorClose(); //Close the "doors" of the claw by engaging the piston

	void MotorClawOutOfRobot(); //Make the claw mechanism extend forward out of the robot
	void MotorClawIntoRobot(); //Make the claw mechanism extend backward into the robot
	void MotorClawStop(); //Stop the claw mechanism

	void MotorRollForward(); //Make the rollers go forward to push out the cube
	void MotorRollReverse(); //Make the rollers go backwards to pull in the cube
	void MotorRollStop(); //Make the rollers stop

	double GetClawEncoderDistance();
	void ResetClawEncoder();
	double GetClawEncoderDistancePerPulse();

	void  OutsideDistance();
	void InsideDistance();
	void MoveClawMechanism();

private:
	WPI_TalonSRX*motorClaw;
	WPI_TalonSRX*motorRoll;
	Solenoid*pistonDoorRight;
	Solenoid*pistonDoorLeft;
	DigitalInput*limSwitchOutside;
	DigitalInput*limSwitchInside;
	Encoder*clawEncoder;
};

#endif
