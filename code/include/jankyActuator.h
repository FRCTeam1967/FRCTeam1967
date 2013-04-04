///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyActuator for a simple robot for 2013

#ifndef _JANKYACTUATOR_H
#include "jankyTask.h"
#include "WPILib.h"
#define _JANKYACTUATOR_H

#define CYCLE_TIME 3.5
#define ACTUATION_TIME 1.2

class JankyActuator : public JankyTask	{
public:
	JankyActuator(int pistonOneChannel, int pistonTwoChannel);
	virtual ~JankyActuator(void);
	
	//Member functions
	bool Go();
	virtual void Run();
	void Reset(void);
	void SetFullCycleTime(double cycleTime);
	void SetActuationTime(double actuationTime);
	void OverrideEnable(void);
	void OverrideDisable(void);
	Solenoid * GetSolenoid(void){
		return pPistonOne;
	}
	
	//Member variables
	double dFullCycleTime;
	double dFullActuationTime;
	bool bActuating;
	Timer cycleTimer;
	Solenoid * pPistonOne;
	Solenoid * pPistonTwo;
	
};

#endif
