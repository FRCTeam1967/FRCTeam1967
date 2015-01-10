///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankySensor for a simple robot for 2013 - Ultimate Ascent

#include "WPILib.h"
#include "DigitalInput.h"

#ifndef _JANKYSENSOR_H
#define _JANKYSENSOR_H

#define THRESHOLD 10

class JankySensor : public DigitalInput {
public:
	JankySensor(uint32_t channel);
	virtual ~JankySensor(void);
	
	//member functions
	void AutoDebounce();
	uint32_t Get();
	uint32_t GetRaw();
	
	//member variables
	int cSensor;
	int cContiguous;
	bool rReporting;

};

#endif 
