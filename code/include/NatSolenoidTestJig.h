
#ifndef INCLUDE_SOLENOIDTESTJIG_H_
//#include "jankyTask.h"
#include "WPILib.h"
#define INCLUDE_SOLENOIDTESTJIG_H_

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
