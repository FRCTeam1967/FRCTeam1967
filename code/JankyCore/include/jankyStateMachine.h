///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyStateMachine for a simple robot for 2013

#ifndef _JANKYSTATEMACHINE_H
#define _JANKYSTATEMACHINE_H
#include "jankyTask.h"
#include "WPILib.h"
#define MAX_NAMES 32
#define MAX_NAME_LENGTH 32

class JankyStateMachine : public JankyTask {
public:
	JankyStateMachine();
	virtual ~JankyStateMachine(void);
	
	//Member Functions
	virtual void Run();
	virtual void StateEngine(int curState) = 0;
	int GetCurrentState();
	void NewState(int newState, const char*reason);
	void SetName(int state, const char*name);
	void SetMachineName(const char* machine);

	//Member Variables
private:	
	int currentState;
	char names[MAX_NAMES][MAX_NAME_LENGTH];
	char machineName [MAX_NAME_LENGTH];
};

#endif
