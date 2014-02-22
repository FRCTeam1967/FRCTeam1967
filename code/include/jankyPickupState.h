///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyPickup State Machine - 2014 picker upper class

#ifndef _JANKYPICKUPSTATE_H
#define _JANKYPICKUPSTATE_H
#include "WPILib.h"
#include "jankyStateMachine.h"
#include "jankyKickerState.h"

#define PICKUP_PISTON_ONE_CHANNEL 1
#define PICKUP_PISTON_TWO_CHANNEL 2

#define ROLLERS_MOTOR_CHANNEL 5
#define FAST_ROLLERS_SPEED 0.8
#define SLOW_ROLLERS_SPEED 0.3
#define ROLLERS_WAIT 2.0

#define RAISE_ARM_WAIT 1.0

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
	JankyKickerState * kickMachine;
	
	//Member functions
	void StateEngine(int curState);
	void SetKickerMachine(JankyKickerState * kickState);
	void LowerForKick(void);
	void LowerToPickup(void);
	bool IsPickupUp(void);
	bool IsPickupDown(void);
	
};

#endif
