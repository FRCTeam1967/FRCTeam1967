///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyAutonomous State Machine - 2014 example for state machine

#ifndef _JANKYAUTONOMOUSSTATE_H
#define _JANKYAUTONOMOUSSTATE_H
#include "WPILib.h"
#include "jankyStateMachine.h"
#include "jankyRobotTemplate.h"
#include "jankyPickupState.h"
#include "jankyKickerState.h"

#define DRIVE_TIME 1.9
#define LOWER_TIME 0.8
#define WIND_TIME 5.0

class JankyPickupState; //Forward declaration due to reference below
class JankyKickerState; //Forward declaration due to reference below
class JankyAutonomousState : public JankyStateMachine	{
public:
	JankyAutonomousState(RobotDrive * pt);
	virtual ~JankyAutonomousState(void);
	
	//Member variables
	enum StateValue {
		Idle,
		DriveForward,
		InitialLower,
		DriveForwardWindUp,
		StopDriveWind,
		DriveStopWind,
		PrepKick,
		Kick,
		PostKick,
		End
		
	};
	Timer * driveTimer;
	Timer * lowerTimer;
	Timer * windTimer;
	RobotDrive * ptRobot;
	JankyPickupState * autoPickupMachine;
	JankyKickerState * autoKickerMachine;
	bool driveOnce;
	
	//Member functions
	void StateEngine(int curState);
	void GoForward(void);
	void StartAuto(void);
	void SetAutoPickupMachine(JankyPickupState * pickupState);
	void SetAutoKickerMachine(JankyKickerState * kickState);

};

#endif
