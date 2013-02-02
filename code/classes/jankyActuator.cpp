/*
 * jankyActuator.cpp
 *
 *  Created on: Jan 21, 2013
 *      Author: Team1967
 */

#include "WPILib.h"
#include "jankyActuator.h"

/*
 * Default constructor
 */

JankyActuator::JankyActuator(int pistonChannel)
{
	bActuating = false;
	cycleTimer.Reset();
	pPiston = new Solenoid(pistonChannel);
}

/*
 * Default destructor
 */
JankyActuator::~JankyActuator()
{
	delete pPiston;
}

void JankyActuator::Go()
{
	if(bActuating == false)
	{
		bActuating = true;
		cycleTimer.Reset();
		cycleTimer.Start();
	}
}

void JankyActuator::Run()
{
	if(cycleTimer.HasPeriodPassed(CYCLE_TIME))
	{
		bActuating = false;
		cycleTimer.Stop();
		cycleTimer.Reset();
	}
	else if(bActuating == true)
	{
		pPiston->Set(! cycleTimer.HasPeriodPassed(1200));
	}
	else
	{
		
	}
}
