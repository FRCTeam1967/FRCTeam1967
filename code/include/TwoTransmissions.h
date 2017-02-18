/*
 * TwoTransmissions.h
 *
 *  Created on: Feb 10, 2017
 *      Author: AnishaKabir
 */

#ifndef INCLUDE_TWOTRANSMISSIONS_H_
#include "WPILib.h"
#define INCLUDE_TWOTRANSMISSIONS_H_

class TwoTransmissions
{
public:
	TwoTransmissions(int LPistonChannel, int RPistonChannel, int LPistonModNumber, int RPistonModNumber); // constructing the class AniTwoTransmissions
	virtual ~TwoTransmissions(); //destructing the class AniTwoTransmissions

//Declaring the objects needed in the class
	Solenoid*lPiston;
	Solenoid*rPiston;
//Declaring the Functions
	void HighGear();
	void LowGear();
	bool GetPistonL();
	bool GetPistonR();
};




#endif /* INCLUDE_TWOTRANSIMISSIONS_H_ */
