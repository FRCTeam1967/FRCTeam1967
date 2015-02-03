/*
 * NatSolenoidTestJig.h
 *
 *  Created on: Feb 1, 2015
 *      Author: Natasha
 */

/*
 * NatSolenoidtest.h
 *
 *  Created on: Jan 31, 2015
 *      Author: Natasha
 */

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





#endif /* INCLUDE_SOLENOIDTESTJIG_H_ */
