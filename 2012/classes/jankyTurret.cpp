
#include "jankyTurret.h"

JankyTurret::JankyTurret(int JagPort, int LimLeft, int LimRight):
	Jaguar(JagPort),
	TurretLeft(LimLeft),
	TurretRight(LimRight)
{
	Deadband = 0.05;
}

JankyTurret::~JankyTurret(void)
{
	
}

void JankyTurret::Set(float DesiredMotorValue)
{
	bool LeftLimPress= TurretLeft.Get();
	bool RightLimPress= TurretRight.Get();

	
	if (DesiredMotorValue>0.0 && RightLimPress==true)
	{
		Jaguar::Set(0.0);
	}
	else if(DesiredMotorValue<0.0 && LeftLimPress==true)
	{
		Jaguar::Set(0.0);
	}
	else 
	{
		if (DesiredMotorValue < Deadband && DesiredMotorValue > Deadband * -1.0)
			Jaguar::Set(0.0);
		else
			Jaguar::Set(DesiredMotorValue);
	}
}

void JankyTurret::SetDeadband(float DB)
{
	Deadband = DB;
}
