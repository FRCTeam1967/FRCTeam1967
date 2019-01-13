
#ifndef INCLUDE_SOLENOIDTESTJIG_H_
//#include "jankyTask.h"
#include "frc/WPILib.h"
#define INCLUDE_SOLENOIDTESTJIG_H_

class NatSolenoidTestJig {
public:
    NatSolenoidTestJig(int pistonOneChannel);
    virtual ~NatSolenoidTestJig(void);

    //Member functions
    frc::Solenoid * GetSolenoid(void){
        return pPistonOne;
    }
    void OverrideEnable(void);
    void OverrideDisable(void);

    //Member variables
    frc::Solenoid * pPistonOne;
};
#endif
