#include "WPILib.h"
#include "AniTwoTransmissions.h"

AniTwoTransmissions::AniTwoTransmissions(int LPistonChannel, int RPistonChannel, int LPistonModNumber, int RPistonModNumber)
{
	RPiston = new Solenoid(RPistonModNumber, RPistonChannel);
	LPiston = new Solenoid(LPistonModNumber, LPistonChannel);
}
AniTwoTransmissions::~AniTwoTransmissions()
{
	delete RPiston;
	delete LPiston;
}
void AniTwoTransmissions::SwitchGear()
{
	if(LPiston->Get()==false&&RPiston->Get()==false)
	{
		LPiston->Set(true);
		RPiston->Set(true);
	}
	else
	{
		LPiston->Set(false);
		RPiston->Set(false);
	}
}

bool AniTwoTransmissions::GetPistonL()
{
	bool LValue=LPiston->Get();
	return LValue;
}

bool AniTwoTransmissions::GetPistonR()
{
	bool RValue=RPiston->Get();
	return RValue;
}

