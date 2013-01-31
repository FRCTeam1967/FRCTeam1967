///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyRobotTemplate for a simple robot for 2013

#ifndef _COUNTERTASK_H
#include "jankyTask.h"
#define _COUNTERTASK_H

class CounterTask : public JankyTask	{
public:
	CounterTask(void);
	virtual ~CounterTask(void);
	
	virtual void Run();
	int ReturnCounter();
	int counter;

};

#endif
