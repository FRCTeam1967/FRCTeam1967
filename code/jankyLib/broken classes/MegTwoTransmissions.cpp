#include "WPIlib.h"
#include "MegTwoTransmissions.h"

MegTwoTransmissions::MegTwoTransmissions(int lPistonChannel, int rPistonChannel, int lPistonModNumber, int rPistonModNumber ){
 lPiston = new Solenoid(lPistonModNumber, lPistonChannel);
 rPiston = new Solenoid(rPistonModNumber, rPistonChannel);
}
MegTwoTransmissions::~MegTwoTransmissions(){
	delete lPiston;
	delete rPiston;
}
void MegTwoTransmissions::SwitchGear(){
	if(lPiston->Get() == false && rPiston->Get() == false){
		lPiston->Set(true);
		rPiston->Set(true);
	}
	else{
		lPiston->Set(false);
		rPiston->Set(false);
	}
}
void MegTwoTransmissions::HighGear()
{
	if(lPiston->Get() == true && rPiston->Get() == true)
	{
		lPiston->Set(false);
		rPiston->Set(false);
	}
	else if(lPiston->Get() == true && rPiston->Get() == false)
	{
		lPiston->Set(false);
		rPiston->Set(false);
	}
	else if(lPiston->Get() == false && rPiston->Get() == true)
	{
		lPiston->Set(false);
		rPiston->Set(false);
	}
}
void MegTwoTransmissions::LowGear()
{
	if(lPiston->Get() == false && rPiston->Get() == false)
	{
		lPiston->Set(true);
		rPiston->Set(true);
	}
	else if(lPiston->Get() == true && rPiston->Get() == false)
	{
		lPiston->Set(true);
		rPiston->Set(true);
	}
	else if(lPiston->Get() == false && rPiston->Get() == true)
	{
		lPiston->Set(true);
		rPiston->Set(true);
	}
}
bool MegTwoTransmissions::GetPistonL(){
	bool lValue = lPiston->Get();
	return lValue;
}
bool MegTwoTransmissions::GetPistonR(){
	bool rValue = rPiston->Get();
	return rValue;
}
