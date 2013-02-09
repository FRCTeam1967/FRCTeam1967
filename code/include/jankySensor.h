///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankySensor for a simple robot for 2013 - Ultimate Ascent

#include "WPILib.h"

#ifndef _JANKYSENSOR_H
#define _JANKYSENSOR_H

#define THRESHOLD 10

class JankySensor : public DigitalInput {
public:
	JankySensor(UINT32 channel);
	JankySensor(UINT8 moduleNumber, UINT32 channel);
	virtual ~JankySensor(void);
	
	//member functions
	void AutoDebounce();
	UINT32 Get();
	UINT32 GetRaw();
	
	//member variables
	int cSensor;
	int cContiguous;
	bool rReporting;

};

#endif 