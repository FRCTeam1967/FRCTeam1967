/*
 * MegTwoTransmissions.h
 *
 *  Created on: Feb 13, 2016
 *      Author: meghamallya
 */

#ifndef SRC_MEGTWOTRANSMISSIONS_H_
#include "frc/WPILib.h"
#define SRC_MEGTWOTRANSMISSIONS_H_
class MegTwoTransmissions {
 	 public:

	MegTwoTransmissions(int lPistonChannel, int rPistonChannel, int lPistonModNumber, int rPistonModNumber );
	virtual ~MegTwoTransmissions();

	//Variables of MegTwoTransmissions
	frc::Solenoid*lPiston;
	frc::Solenoid*rPiston;

	//Functions of MegTwoTransmissions
	void SwitchGear();
	void LowGear();
	void HighGear();
	bool GetPistonL();
	bool GetPistonR();
};
#endif /* SRC_MEGTWOTRANSMISSIONS_H_ */
