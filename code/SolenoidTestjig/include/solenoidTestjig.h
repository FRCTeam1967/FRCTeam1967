#ifndef _JANKYSOLENOID_H
#include "jankyTask.h"
#include "WPILib.h"
#define _JANKYSOLENOID_H

#define CYCLE_TIME 3.5
#define ACTUATION_TIME 1.2

class solenoidTestjig : public JankyTask {
public:
	solenoidTestjig(int pistonOneChannel/*, int pistonTwoChannel*/);
	virtual ~solenoidTestjig(void);

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
		//return pPistonTwo;
	}

	//Member variables
	double dFullCycleTime;
	double dFullActuationTime;
	bool bActuating;
	Timer cycleTimer;
	Solenoid * pPistonOne;
	//Solenoid * pPistonTwo;

};

#endif
