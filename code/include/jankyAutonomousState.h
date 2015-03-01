///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyAutonomous State Machine - 2014 example for state machine

#ifndef _JANKYAUTONOMOUSSTATE_H
#define _JANKYAUTONOMOUSSTATE_H
#include "WPILib.h"
#include "jankyStateMachine.h"
#include "jankyRobotTemplate.h"
#include "jankyFoxLiftState.h"

#define DRIVE_SIDEWAYS_TIME 1.5
#define DRIVE_FORWARD_TIME 1

class JankyFoxliftState;

class JankyAutonomousState : public JankyStateMachine	{
public:
	JankyAutonomousState(RobotDrive * pt, JankyFoxliftState * foxlift);
	virtual ~JankyAutonomousState(void);
	
	//Member variables
	enum StateValue {
		Idle,
		DriveForward,
		DriveSideways,
		End
		
	};
	Timer * driveForwardTimer;
	Timer * driveSidewaysTimer;
	RobotDrive * ptRobot;
	bool driveOnce;
	
	//Member functions
	void StateEngine(int curState);
	void Go(void);
	void GoForward(void);
	void GoSideways(void);
	void StartAuto(void);

};

#endif
