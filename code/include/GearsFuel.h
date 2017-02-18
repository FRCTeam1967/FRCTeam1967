/*
 * GearsFuel.h
 *
 *  Created on: Jan 28, 2017
 *      Author: Sana
 */

#ifndef GEARSFUEL_H_
#include "WPILib.h"
#include "CANTalon.h"
#define GEARSFUEL_H_
#include "Solenoid.h"



class GearsFuel{
public:
	GearsFuel(int ballMotorChannel, int doorPistonChannel, int gearBoxChannel, int pushGearChannel);

		virtual ~GearsFuel(void);

		void FuelIntake(void);
		void StopBallMotor(void);
		void DoorUp(void);
		void DoorDown(void);
		void BoxOut(void);
		void BoxIn(void);
		void GearOut(void);
		void GearIn(void);


		CANTalon * ballMotor;
		Solenoid * solenoid1;
		Solenoid * solenoid2;
		Solenoid * solenoid3;






};

#endif /* GEARSFUEL_H_ */
