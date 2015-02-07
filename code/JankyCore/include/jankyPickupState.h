///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyPickup State Machine - 2014 picker upper class

#ifndef _JANKYPICKUPSTATE_H
#define _JANKYPICKUPSTATE_H
#include "WPILib.h"
#include "jankyStateMachine.h"
#include "jankyKickerState.h"

#define PICKUP_PISTON_ONE_CHANNEL 5
#define PICKUP_PISTON_TWO_CHANNEL 6

#define ROLLERS_MOTOR_CHANNEL 5
#define FAST_ROLLERS_SPEED 1.0
#define SLOW_ROLLERS_SPEED 0.7
#define PASS_ROLLERS_SPEED -1.0
#define ROLLERS_WAIT 2.0
#define ROLLING_TIME 1.0	//Was 1.0, changed to 0.8, changed back
#define LOWERING_TIME 1.0	//Was 2.5
#define PASS_TIME 1.0

#define RAISE_ARM_WAIT 1.0	//Was 2.0

class JankyKickerState; //Forward declaration due to reference below
class JankyPickupState : public JankyStateMachine	{
public:
	JankyPickupState();
	virtual ~JankyPickupState(void);
	
	//Member variables
	enum StateValue {
		Idle,
		LowerArmPickup,
		Up,
		Passing,
		SlowRollers,
		PrimedForKick,
		Lower,
		ReadyToKick,
		Raise
	};
	Solenoid * pickupPistonOne;
	Solenoid * pickupPistonTwo;
	Talon * rollersMotor;
	Timer * rollersTimer;
	Timer * pickupTimer;
	Timer * lowerTimer;
	Timer * passTimer;
	JankyKickerState * kickMachine;
	
	//Member functions
	void StateEngine(int curState);
	void SetKickerMachine(JankyKickerState * kickState);
	void LowerForKick(void);
	void LowerToPickup(void);
	void UnLowerExit(void);
	bool IsPickupUp(void);
	bool IsPickupDown(void);
	void Pass(void);
	
};

#endif
