///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// Janky Foxlift State Machine - 2015

#ifndef _JANKYFOXLIFTSTATE_H
#define _JANKYFOXLIFTSTATE_H
#include "WPILib.h"
#include "jankyStateMachine.h"

#define LIMIT_SWITCH_TOP 0
#define LIMIT_SWITCH_BOTTOM 1
//#define LIMIT_SWITCH_6 4
//#define LIMIT_SWITCH_2 3
#define TALON_ROLLER1 5
#define TALON_ROLLER2 6
#define TALON_LIFT 7
#define BRAKE 0
#define SONAR_SENSOR 2
#define LIMIT_SWITCH_TOTE 2
#define LIFT_UP_SPEED .75
#define LIFT_DOWN_SPEED -.75
//#define TOTE_SENSOR_PRESENT_IF_SMALLERTHAN .7
#define ROLLER_PISTON 1
#define PREROLLER_TIME 3.0
#define ROLLER_TIME 2.0
#define REORIENTATION 2
#define SINGULATION_ONE 3
#define SINGULATION_TWO 4

class JankyFoxliftState : public JankyStateMachine	{
public:
	JankyFoxliftState();
	virtual ~JankyFoxliftState(void);

	//Member variables
	enum StateValue {
		Init,
		//UpSingle,
		//UpFinish,
		Braking,
		//Down6,
		//Down2,
		//DownStack,
		BottomStop,
		Up,
		Down,
		MoveRollersIn,
		MoveRollersOut,
		SingulationDown,
		SingulationUp,
		Reorientation,
		WaitForUndo,
		ManualOverride
	};

	//Member functions
	void ManualOverrideOn();
	void ManualOverrideOff();
	bool IsLSwitchTopClosed();
	bool IsLSwitchDownClosed();
	void ExtendReorientation();
	void RetractReorientation();
	void ExtendSingulation();
	void RetractSingulation();
	void LowerSingulation();
	void RaiseSingulation();
	void ExtendArms();
	void RetractArms();
	bool ToteIn();
	void SetFoxlift();
	void GoUp();
	void GoDown();
	void Reorient();
	void SingulateOne();
	void SingulateTwo();
	void DoneSingulating();
	void DoneReorienting();
	void StateEngine(int curState);


	//Member Variables
	DigitalInput*lSwitchTop;
	DigitalInput*lSwitchDown;
	DigitalInput*toteIn;
	//AnalogInput*toteIn;//sonar sensor
	//DigitalInput*lSwitch6;
	//DigitalInput*lSwitch2;
	Talon*motorLift;
	Talon*motorRoller1;
	Talon*motorRoller2;
	Solenoid*brake;
	Solenoid*rollerPistons;
	Solenoid*reorientation;
	Solenoid*singulationOne;
	Solenoid*singulationTwo;
	Timer*preRollerTimer;
	Timer*rollerInTimer;
	Timer*rollerOutTimer;
};

#endif
