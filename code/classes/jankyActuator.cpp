/*
 * jankyActuator.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: Team1967
 */

#include "WPILib.h"
#include "jankyActuator.h"


/*
 * @brief This program is written by Team 1967 (the Janksters) and uses out JankyTask class to build
 * an actuator class. If used, the class will run the solenoids/pistons on its own without having to 
 * hold down a button. Functions that must be called are Start, SetFullCycleTime, SetActuationTime, 
 * and Reset. 
 */


/*
 * Construct instance of an actuator that runs in its own task.
 * 
 * @param pistonChannel The port/channel to which the actuator/solenoid is connected to. 
 */
JankyActuator::JankyActuator(int pistonChannel)
{
	bActuating = false;
	cycleTimer.Reset();
	iFullCycleTime = 3.5;
	iFullActuationTime = 1.2;
	
	pPiston = new Solenoid(pistonChannel);
	
	LiveWindow *lw = LiveWindow::GetInstance();
	lw->SetEnabled(true);
	lw->AddActuator("Piston", "Piston", pPiston);
		
}


/*
 * Destructor
 */
JankyActuator::~JankyActuator()
{
	delete pPiston;
}

//
// @brief Cause the beginning of a shot if one isn't already in
//        progress. It's an edge-trigger mechanism.
// @return true - if a shot is started.
//         false - if a shot is already in progress..
//

void JankyActuator::Reset()
{
	bActuating = false;
	cycleTimer.Reset();
}


/*
 * Sets the full cycle time or however long it will take for the piston to go out and then retract.
 * MUST be called in OperatorControl or else the default (3.5 seconds) will be used.
 * Default declared in the constructor. 
 * 
 * @param cycleTime The time in seconds it takes for the piston to extend and retract.
 * 				Probably the actuationTime plus whatever time it takes for the piston to retract.
 */
void JankyActuator::SetFullCycleTime(int cycleTime)
{
	iFullCycleTime = cycleTime; //integer in seconds
}


/*
 * Sets actuation time or however long it takes for the piston to extend.
 * MUST be called in OperatorControl or else the default (1.2 seconds) will be used.
 * Default declared in the constructor.
 * 
 * @param actuationTime The time in seconds it takes for the piston to go out.
 * 
 */
void JankyActuator::SetActuationTime(int actuationTime)
{
	iFullActuationTime = actuationTime;	//integer in seconds
}


/*
 * Sets actuation to true so that the Run function begins.
 */
bool JankyActuator::Go()
{
	if(bActuating == false)
	{
		bActuating = true;
		cycleTimer.Reset();
		cycleTimer.Start();
		return true;
	}
	else
		return false;
}


/*
 * Called periodically every 2 ms (built on top of JankyTask Run function).n 
 * 
 * Runs actuator for certain amount of time if Go function has already been called. 
 * Piston goes in and out on its own without human control.
 */
void JankyActuator::Run()
{	
	if(cycleTimer.Get() > iFullCycleTime)
	{
		bActuating = false;
		cycleTimer.Stop();
		cycleTimer.Reset();
	}
	else
	{
		if (bActuating == true)
		{
			if (cycleTimer.Get() < iFullActuationTime)
			{
				pPiston->Set(true);
			}
			
			else
			{
				pPiston->Set(false);
			}	
		}	
	}
}

void JankyActuator::OverrideEnable()
{
	pPiston->Set(true);
}

void JankyActuator::OverrideDisable()
{
	pPiston->Set(false);
}




