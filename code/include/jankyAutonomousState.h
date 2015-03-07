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

#define DRIVE_SIDEWAYS_TIME 0.4
#define DRIVE_FORWARD_TIME 0.5
#define TURN_TIME 0.5
#define DRIVE_TO_AUTO_TIME 1.2

class JankyFoxliftState;

class JankyAutonomousState : public JankyStateMachine	{
public:
	JankyAutonomousState(RobotDrive * pt, JankyFoxliftState * foxlift);
	virtual ~JankyAutonomousState(void);
	
	//Member variables
	enum StateValue {
		Idle,
		DriveSideways,
		DriveForward,
		LiftTote,
		TurnToAuto,
		DriveToAuto,
		End
		
	};
	Timer * driveForwardTimer;
	Timer * driveSidewaysTimer;
	Timer * turnTimer;
	Timer * driveToAutoTimer;
	RobotDrive * ptRobot;
	JankyFoxliftState * ptFoxLift;
	bool driveOnce;
	
	//Member functions
	void StateEngine(int curState);
	void Go(void);
	void GoSideways(void);
	void GoForward(void);
	void GoLiftTote(void);
	void StartAuto(void);
	void GoTurnToAuto(void);
    void GoForwardToAuto(void);

};

#endif

