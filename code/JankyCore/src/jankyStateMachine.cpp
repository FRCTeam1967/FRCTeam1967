/*	
 * jankyStateMachine.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: Team1967
 */
#include "WPILib.h"
#include "jankyTask.h"
#include "jankyStateMachine.h"

JankyStateMachine::JankyStateMachine()
{
	printf("JankyStateMachine() constructor\n");
	
	char nametemp[MAX_NAME_LENGTH];
	for (int i=0; i< MAX_NAMES; i++)
	{
		sprintf(nametemp, "State-Default-%d", i);
		SetName(i, nametemp);
	}

	strcpy(machineName, "DefaultMachine");
	currentState = 0;		// Make a sane initial choice.
	
	Start(); //NEED TO TEST THIS!
}

JankyStateMachine::~JankyStateMachine()
{

}

void JankyStateMachine::SetMachineName(const char * machine)
{
	if (strlen(machine) < MAX_NAME_LENGTH -1)
	{
		strcpy(machineName, machine);
	}
	else
	{
		strcpy(machineName,"MACHINE NAME ERROR");
	}
}
int JankyStateMachine::GetCurrentState()
{
	return currentState;
}

void JankyStateMachine::Run(){
	StateEngine(currentState);
}

void JankyStateMachine::NewState(int newState, const char*reason){
	if ( newState < MAX_NAMES)
	{
		printf("STATECHANGE:%s:OLD:%s:NEW:%s:REASON:%s\n", machineName, names[currentState], names[newState], reason);
		currentState = newState;
	}
	else
	{
		printf ("JankyStateMachine::NewState ERROR invalid newState\n");
	}
	
}

void JankyStateMachine::SetName(int state, const char*stateName) {
	if (state < MAX_NAMES && strlen(stateName) < MAX_NAME_LENGTH -1)
	{
		strcpy(names[state], stateName);
	}
	else
	{
		printf ("JankyStateMachine::SetName Bad state value/name\n");
	}
}
