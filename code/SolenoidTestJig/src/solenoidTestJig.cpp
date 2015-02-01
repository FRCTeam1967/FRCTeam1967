/*
 * solenoidtest.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: Natasha
 */

#include "solenoidTestJig.h"
#include "WPILib.h"

SolenoidTestJig::SolenoidTestJig(int pistonOneChannel)
{
	pPistonOne = new Solenoid(pistonOneChannel);
}

/*
 * Destructor
 */
SolenoidTestJig::~SolenoidTestJig()
{
	delete pPistonOne;
}

void SolenoidTestJig::OverrideEnable(void)
{
	pPistonOne ->Set(true);
	printf("did this\n");
}

void SolenoidTestJig::OverrideDisable(void)
{
	pPistonOne ->Set(false);
}


