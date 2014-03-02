///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyKicker State Machine - 2014 use of state machine for kicking ball

#ifndef _JANKYKICKERSTATE_H
#define _JANKYKICKERSTATE_H
#include "WPILib.h"
#include "jankyStateMachine.h"
#include "jankyPickupState.h"
#include "jankyEncoder.h"
//#include "jankyActuator.h"

#define ENCODER_ONE_CHANNEL 9
#define ENCODER_TWO_CHANNEL 10
#define ENCODER_MOTOR_CHANNEL 6
#define ENCODER_WINDUP_REVOLUTION 2.0
#define DISENGAGE_SPEED -0.2
#define MOTOR_DISENGAGE_TIME 0.7		//Was 1.0
#define AUTOMATED_WINDUP_SPEED 1.0
#define MANUAL_WINDUP_SPEED 0.3

#define DOG_GEAR_PISTON_ONE_CHANNEL 1
#define DOG_GEAR_PISTON_TWO_CHANNEL 2
//#define DOG_GEAR_PISTON_CYCLE 3.0
//#define DOG_GEAR_PISTON_ACTUATION 1.5

#define PAWL_PISTON_ONE_CHANNEL 3
#define PAWL_PISTON_TWO_CHANNEL 4
//#define PAWL_PISTON_CYCLE 3.0
//#define PAWL_PISTON_ACTUATION 1.5

#define POST_WINDUP_WAIT 0.5			//Was 1.0
#define DOG_GEAR_DISENGAGE_WAIT 0.5		//Was 1.0
#define DOG_GEAR_ENGAGE_WAIT 1.5		//Was 3.0
#define PAWL_ACTUATE_WAIT 1.0			//Was 1.0
#define ADD_WIND_TIME 0.5

class JankyPickupState; //Forward declaration due to reference below
class JankyKickerState : public JankyStateMachine	{
public:
	JankyKickerState();
	virtual ~JankyKickerState(void);
	
	//Member variables
	enum StateValue {
		Idle,
		ManualWindUp,
		AutomatedWindUp,
		PostWindUpWait,
		IdleReadyForKick,
		AddWindUp,
		MotorDisengage,
		PawlActuate,
		MotorEngage
	};
	JankyEncoder * kickEncoder;
	Solenoid * dogGearPistonOne;
	Solenoid * dogGearPistonTwo;
	Solenoid * pawlPistonOne;
	Solenoid * pawlPistonTwo;
	Talon * pWindMotor;
	Timer * dogGearTimer;
	Timer * postWindUpTimer;
	Timer * pawlTimer;
	Timer * addWindTimer;
	Timer * disengageTimer;
	JankyPickupState * pickupMachine;
	bool bHasKicked;
	
	//Member functions
	void StateEngine(int curState);
	void SetPickupMachine(JankyPickupState * pickupState);
	void PreWind(void);
	void PreWindExit(void);
	void Kick(void);
	void WindUp(void);
	bool HasKicked(void);

};

#endif
