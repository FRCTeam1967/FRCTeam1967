///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyRobotTemplate for a simple robot for 2013

#ifndef _COUNTERTASKTESTJIG_H
#define _COUNTERTASKTESTJIG_H

class CounterTaskTestjig : public JankyTask	{
public:
	CounterTaskTestjig(void);
	virtual ~CounterTaskTestjig(void);
	
	virtual void Run();
	int ReturnCounter();
	int counter;

};

#endif
