/*
 * GearsFuel.h
 *
 *  Created on: Jan 28, 2017
 *      Author: Sana
 */

#ifndef GEARSFUEL_H_
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#define GEARSFUEL_H_
#include "Solenoid.h"



class GearsFuel{
public:
	GearsFuel( int gearChannel);

		virtual ~GearsFuel(void);


		void Vert(void);
		void Vert2(void);
		void GearOut(void);
		void GearIn(void);
		void Horz(void);
		void Horz2(void);

		frc::Solenoid * gearPiston;






};

#endif /* GEARSFUEL_H_ */
