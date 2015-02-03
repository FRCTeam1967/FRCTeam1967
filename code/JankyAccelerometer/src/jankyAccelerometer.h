///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

#ifndef _JANKYACCELEROMETER_H
#include "jankyTask.h"
#include "WPILib.h"
#define _JANKYACCELEROMETER_H

class JankyAccelerometer : public JankyTask {
public:
	JankyAccelerometer();
	virtual ~JankyAccelerometer(void);

	//Member functions
	void Reset(void);
	bool isDone();
	bool Go();
	void Run();
	double returnX();
	double returnY();

	//Member variables
	Accelerometer * pAccel;
	double xVal;
	double yVal;

};

#endif
