#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "jankyTask.h"

#ifndef INANDOUT_H_
#define INANDOUT_H_

class InAndOut: public JankyTask {
public:
	virtual void Run();

	InAndOut(int pistonDoorLeftChannel, int pistonDoorRightChannel, int motorRollChannel, int motorClawChannel);
	virtual ~InAndOut();

	void PistonDoorOpen(); //Open the "doors" of the claw by retracting the piston
	void PistonDoorClose(); //Close the "doors" of the claw by engaging the piston

	void MotorRollForward(); //Make the rollers go forward to push out the cube
	void MotorRollReverse(); //Make the rollers go backwards to pull in the cube
	void MotorRollStop(); //Make the rollers stop

	void  OutsideDistance();
	void InsideDistance();

	void MotorClawOutOfRobot(); //Make the claw mechanism extend forward out of the robot
	void MotorClawIntoRobot(); //Make the claw mechanism extend backward into the robot
	void MotorClawStop(); //Stop the claw mechanism
	void MotorClawStopWithLimSwitches();
	double GetDesiredDistance();
	void MoveClawDownInAuto();

	int GetLimSwitchOutside(); //Get the value of the limit switch for when the claw goes outside the robot(true/false)
	int GetLimSwitchInside(); //Get the value of the limit switch for when the claw goes inside the robot(true/false)

	double GetEncoderCount();

	void StartUpInit();
	bool GetClawPosition();

	//PID
	void PIDSetup();

	//UNUSED
	//	void ClawDown();
	//	void ClawUp();
	//	double GetClawEncoderDistance();
	//	void ResetClawEncoder();
	//	double GetClawEncoderDistancePerPulse();
//	  void MoveClawMechanism();

private:
	double clawEncoderCount;
	double clawEncoderDistance;
	bool clawGoingForward;
	bool clawGoingBackward;
	bool needsToPutDownClaw;
	bool clawPositionIsOut; //true = forward & false = backward
	double desiredDistanceToMove;
	double amountToMoveClaw;

	WPI_TalonSRX*motorRoll;
	frc::Solenoid*pistonDoorRight;
	frc::Solenoid*pistonDoorLeft;
	WPI_TalonSRX*motorClaw;

	//UNUSED
	//	Encoder*clawEncoder;
	//	Solenoid*pistonInOut1;
	//	Solenoid*pistonInOut2;
	//	WPI_TalonSRX*limSwitchOutside;
	//	WPI_TalonSRX*limSwitchInside;
};

#endif
