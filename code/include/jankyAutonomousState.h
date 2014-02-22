///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyAutonomous State Machine - 2014 example for state machine

#ifndef _JANKYAUTONOMOUSSTATE_H
#define _JANKYAUTONOMOUSSTATE_H
#include "WPILib.h"
#include "jankyStateMachine.h"
#include "jankyRobotTemplate.h"

#define DRIVE_TIME 2.0

class JankyAutonomousState : public JankyStateMachine	{
public:
	JankyAutonomousState(RobotDrive * pt);
	virtual ~JankyAutonomousState(void);
	
	//Member variables
	enum StateValue {
		Idle,
		DriveForward
	};
	Timer * driveTimer;
	RobotDrive * ptRobot;
	
	//Member functions
	void StateEngine(int curState);
	void GoForward();

};

#endif
