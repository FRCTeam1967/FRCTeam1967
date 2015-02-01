/*
 * solenoidtest.h
 *
 *  Created on: Jan 31, 2015
 *      Author: Natasha
 */

#ifndef INCLUDE_SOLENOIDTESTJIG_H_
//#include "jankyTask.h"
#include "WPILib.h"
#define INCLUDE_SOLENOIDTESTJIG_H_

class SolenoidTestJig {
public:
	SolenoidTestJig(int pistonOneChannel);
	virtual ~SolenoidTestJig(void);

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
