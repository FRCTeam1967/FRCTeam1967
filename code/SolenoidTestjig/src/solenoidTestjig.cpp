#include "WPILib.h"
#include "solenoidTestjig.h"
#include "jankyTask.h"
#include "Robot.cpp"

solenoidTestjig::solenoidTestjig(int pistonOneChannel/*, int pistonTwoChannel*/)
{
	bActuating = false;
	cycleTimer.Reset();
	dFullCycleTime = 3.5;
	dFullActuationTime = 1.2;

	pPistonOne = new Solenoid(pistonOneChannel);
	//pPistonTwo = new Solenoid(pistonTwoChannel);

}


/*
 * Destructor
 */
solenoidTestjig::~solenoidTestjig()
{
	delete pPistonOne;
	//delete pPistonTwo;
}

void solenoidTestjig::Reset()
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
void solenoidTestjig::SetFullCycleTime(double cycleTime)
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
void solenoidTestjig::SetActuationTime(double actuationTime)
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

bool solenoidTestjig::Go()
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
void solenoidTestjig::Run()
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
				//pPistonTwo->Set(true);
			}

			else
			{
				pPistonOne->Set(false);
				//pPistonTwo->Set(false);
			}
		}
	}
}

void solenoidTestjig::OverrideEnable()
{
	pPistonOne->Set(true);
	//pPistonTwo->Set(true);
}

void solenoidTestjig::OverrideDisable()
{
	pPistonOne->Set(false);
	//pPistonTwo->Set(false);
}

START_ROBOT_CLASS(Robot);
