/*
 * NatSolenoidTestJig.h
 *
 *  Created on: Feb 3, 2015
 *      Author: Natasha
 */

#ifndef INCLUDE_SOLENOIDTESTJIG_H_
#define INCLUDE_SOLENOIDTESTJIG_H_
//#include "jankyTask.h"
#include "WPILib.h"

class NatSolenoidTestJig {
public:
    NatSolenoidTestJig(int pistonOneChannel);
    virtual ~NatSolenoidTestJig(void);

    //Member functions
    Solenoid * GetSolenoid(void){
        return pPistonOne;
    }
    void OverrideEnable(void);
    void OverrideDisable(void);

    //Member variables
    Solenoid * pPistonOne;
};

#endif
