///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyPickup State Machine - 2014 picker upper class

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
		Down
	};

	//Member functions
	void GoUp();
	void GoDown();
	void StateEngine(int curState);


	//Member Variables
	DigitalInput*lSwitchTop;
	DigitalInput*lSwitchDown;
	AnalogInput*toteIn;//sonar sensor
	//DigitalInput*lSwitch6;
	//DigitalInput*lSwitch2;
	Talon*motorLift;
	Talon*motorRoller1;
	Talon*motorRoller2;
	Solenoid*brake;
};

#endif
