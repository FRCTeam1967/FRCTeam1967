/*
 /*
 * NatSolenoidtest.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: Natasha
 */

#include "NatSolenoidTestJig.h"
#include "WPILib.h"

NatSolenoidTestJig::NatSolenoidTestJig(int pistonOneChannel)
{
    pPistonOne = new Solenoid(pistonOneChannel);
}

/*
 * Destructor
 */
NatSolenoidTestJig::~NatSolenoidTestJig()
{
    delete pPistonOne;
}

void NatSolenoidTestJig::OverrideEnable(void)
{
    pPistonOne ->Set(true);
    printf("did this\n");
}

void NatSolenoidTestJig::OverrideDisable(void)
{
    pPistonOne ->Set(false);
}
