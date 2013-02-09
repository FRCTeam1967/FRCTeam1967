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
	JankyActuator(int pistonChannel);
	virtual ~JankyActuator(void);
	
	//Member functions
	void Go();
	virtual void Run();
	void Reset(void);
	void SetFullCycleTime(int cycleTime);
	void SetActuationTime(int actuationTime);
	
	//Member variables
	int iFullCycleTime;
	int iFullActuationTime;
	bool bActuating;
	Timer cycleTimer;
	Solenoid * pPiston;
	
};

#endif
