/*
 * GearsFuel.cpp
 *
 *  Created on: Jan 28, 2017
 *      Author: Sana
 */
#include "WPILib.h"
#include "GearsFuel.h"




#define XBOX_CHANNEL 1

#define GEAR_CHANNEL 1
#define INTAKE_CHANNEL 2
#define OUTTAKE_CHANNEL 3

GearsFuel::GearsFuel(int gearChannel){
	gearPiston = new Solenoid(9,gearChannel);


}

GearsFuel::~GearsFuel(void) {

	delete gearPiston;
}




void GearsFuel::GearOut(void){
	//move box out
	gearPiston->Set(true);
}
void GearsFuel::GearIn(void){
	//move box in
	gearPiston->Set(false);
}
//////






