/*
 * jankyActuator.cpp
 *
 *  Created on: Feb 2, 2013
 *      Author: Team1967
 */

#include "jankyActuator.h"
#include "frc/Solenoid.h"
#include "frc/Timer.h"


/*
 * @brief This program is written by Team 1967 (the Janksters) and uses out JankyTask class to build
 * an actuator class. If used, the class will run the solenoids/pistons on its own without having to 
 * hold down a button. Functions that must be called are SetFullCycleTime, SetActuationTime, 
 * and Reset. 
 */

/*
 * Construct instance of an actuator that runs in its own task.
 * 
 * @param pistonChannel The port/channel to which the actuator/solenoid is connected to. 
 */
JankyActuator::JankyActuator(int pistonOneChannel, int pistonTwoChannel)
{
	bActuating = false;
	pistonSync = true;
	cycleTimer.Reset();
	dFullCycleTime = 3.5;
	dFullActuationTime = 1.2;
	
	pPistonOne = new frc::Solenoid(pistonOneChannel);
	if (pistonTwoChannel == -1){
		pPistonTwo = NULL;
	}
	else{
		pPistonTwo = new frc::Solenoid(pistonTwoChannel);
	}

	Start();
}


/*
 * Destructor
 */
JankyActuator::~JankyActuator()
{
	if (pPistonOne != NULL){
		delete pPistonOne;
		pPistonOne = NULL;
	}
	if (pPistonTwo != NULL){
		delete pPistonTwo;
		pPistonTwo = NULL;
	}
}

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
void JankyActuator::SetFullCycleTime(double cycleTime)
{
	dFullCycleTime = cycleTime; //integer in seconds
}


/*
 * Sets actuation time or however long it takes for the piston to extend.
 * MUST be called in OperatorControl or else the default (1.2 seconds) will be used.
 * Default declared in the constructor.
 * 
 * @param actuationTime The time in seconds it takes for the piston to go out.
 * 
 */
void JankyActuator::SetActuationTime(double actuationTime)
{
	dFullActuationTime = actuationTime;	//integer in seconds
}


/*
 * Sets actuation to true so that the Run function begins.
 */

//
// @brief Cause the beginning of a shot if one isn't already in
//        progress. It's an edge-trigger mechanism.
// @return true - if a shot is started.
//         false - if a shot is already in progress..
//

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
	if(cycleTimer.Get() > dFullCycleTime)
	{
		bActuating = false;
		cycleTimer.Stop();
		cycleTimer.Reset();
	}
	else
	{
		if (bActuating == true)
		{
			if (cycleTimer.Get() < dFullActuationTime)
			{
				pPistonOne->Set(true);
				if (pPistonTwo != NULL){
					pPistonTwo->Set(pistonSync);
				}
			}
			
			else
			{
				pPistonOne->Set(false);
				if (pPistonTwo != NULL){
					pPistonTwo->Set(!pistonSync);
				}
			}	
		}	
	}
}

void JankyActuator::OverrideEnable()
{
	pPistonOne->Set(true);
	if (pPistonTwo != NULL){
		pPistonTwo->Set(!pistonSync);
	}

}

void JankyActuator::OverrideDisable()
{
	pPistonOne->Set(false);
	if (pPistonTwo != NULL){
		pPistonTwo->Set(pistonSync);
	}
}

void JankyActuator::SetDualPistonActuationSync(bool dualPistonSync){pistonSync = dualPistonSync;}



