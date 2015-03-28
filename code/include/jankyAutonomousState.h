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
#define FORKLIFT_TIME 1.0
#define BINGULATE_PISTON 5
#define BINGULATE_SERVO 9
#define DRIVE_BACKWARD_TIME 2.3
#define BINGULATE_TIME 3.0

class JankyFoxliftState;

class JankyAutonomousState : public JankyStateMachine	{
public:
	JankyAutonomousState(RobotDrive * pt, JankyFoxliftState * foxlift);
	virtual ~JankyAutonomousState(void);
	
	//Member variables
	enum StateValue {
		Idle,
		HugIdle,
		BingulateDown,
		BingulateWait,
		BingulateUp,
		DriveBackward,
		RollersIn,
		DriveSideways,
		DriveForward,
		DownTote,
		LiftTote,
		TurnToAuto,
		DriveToAuto,
		BingulateEnd,
		End
		
	};
	Timer * driveForwardTimer;
	Timer * driveSidewaysTimer;
	Timer * turnTimer;
	Timer * driveToAutoTimer;
	Timer * driveBackwardTimer;
	Timer * forkliftTimer;
	Timer*bingulateTimer;
	Solenoid*binPiston;
	Servo*binServo;
	RobotDrive * ptRobot;
	JankyFoxliftState * ptFoxLift;
	bool driveOnce;
	bool goDownOnce;
	
	//Member functions
	void RetractBinPiston();
	void ExtendBinPiston();
	void BinServoSetStart();
	void BinServoSetEnd();
	void StateEngine(int curState);
	void GoForBox(void);
	void GoForHug(void);
	void StartBinAuto();
	void GoSideways(void);
	void GoForward(void);
	void GoLiftTote(void);
	void StartAuto(void);
	void GoTurnToAuto(void);
    void GoForwardToAuto(void);
    void BringInRollers(void);

};

#endif

