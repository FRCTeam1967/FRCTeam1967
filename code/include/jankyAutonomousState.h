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

#define DRIVE_SIDEWAYS_TIME 1.0
#define DRIVE_FORWARD_TIME 0.2
#define TURN_TIME 0.75
#define DRIVE_TO_AUTO_TIME 1.2
#define DRIVE_BACKWARD_TIME 0.2
#define BINGULATE_TIME 2.0
#define BIN_PISTON 5

class JankyFoxliftState;

class JankyAutonomousState : public JankyStateMachine	{
public:
	JankyAutonomousState(RobotDrive * pt, JankyFoxliftState * foxlift);
	virtual ~JankyAutonomousState(void);
	
	//Member variables
	enum StateValue {
		Idle,
		BinIdle,
		Bingulate,
		DriveBackToAuto,
		Unbingulate,
		DriveSideways,
		DriveForward,
		DownTote,
		LiftTote,
		TurnToAuto,
		DriveToAuto,
		End
		
	};
	Timer * driveForwardTimer;
	Timer * driveSidewaysTimer;
	Timer * binTimer;
	Timer * driveBackTimer;
	Timer * turnTimer;
	Timer * driveToAutoTimer;
	RobotDrive * ptRobot;
	JankyFoxliftState * ptFoxLift;
	bool driveOnce;
	bool goDownOnce;
	Solenoid * binPiston;
	
	//Member functions
	void StateEngine(int curState);
	void Go(void);
	void GoSideways(void);
	void GoForward(void);
	void GoLiftTote(void);
	void StartAuto(void);
	void GoTurnToAuto(void);
    void GoForwardToAuto(void);
    void ExtendBingulate();
    void RetractBingulate();
};

#endif

