/*
 * AniTwoTransmissions.h
 *
 *  Created on: Feb 6, 2016
 *      Author: AnishaKabir
 */

#ifndef INCLUDE_ANITWOTRANSMISSIONS_H_
#include "frc/WPILib.h"
#define INCLUDE_ANITWOTRANSMISSIONS_H_

class AniTwoTransmissions
{
public:
	AniTwoTransmissions(int LPistonChannel, int RPistonChannel, int LPistonModNumber, int RPistonModNumber); // constructing the class AniTwoTransmissions
	virtual ~AniTwoTransmissions(); //destructing the class AniTwoTransmissions

//Declaring the objects needed in the class
	frc::Solenoid*LPiston;
	frc::Solenoid*RPiston;
//Declaring the Functions
	void SwitchGear();
	bool GetPistonL();
	bool GetPistonR();
};




#endif /* INCLUDE_ANITWOTRANSIMISSIONS_H_ */
