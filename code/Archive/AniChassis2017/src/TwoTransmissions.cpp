#include "WPIlib.h"
#include <TwoTransmissions.h>

TwoTransmissions::TwoTransmissions(int lPistonChannel, int rPistonChannel, int lPistonModNumber, int rPistonModNumber ){
	 lPiston = new Solenoid(lPistonModNumber, lPistonChannel);
	 rPiston = new Solenoid(rPistonModNumber, rPistonChannel);
}

TwoTransmissions::~TwoTransmissions(){
	delete lPiston;
	delete rPiston;
}
void TwoTransmissions::LowGear() //used to say high
{
		lPiston->Set(false);
		rPiston->Set(false);
}
void TwoTransmissions::HighGear() //used to say low
{
		lPiston->Set(true);
		rPiston->Set(true);
}
bool TwoTransmissions::GetPistonL(){
	bool lValue = lPiston->Get();
	return lValue;
}
bool TwoTransmissions::GetPistonR(){
	bool rValue = rPiston->Get();
	return rValue;
}




