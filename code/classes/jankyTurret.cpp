
#include "jankyTurret.h"

JankyTurret::JankyTurret(int JagPort, int LimLeft, int LimRight):
	//TurretMotor(JagPort),
	Victor(JagPort),
	TurretLeft(LimLeft),
	TurretRight(LimRight)
{
	Deadband = 0.056;
	SmartDashboard::PutNumber("Turret Motor Value", 0.0);
	SmartDashboard::PutString("Left Limit Switch", "Unpressed");
	SmartDashboard::PutString("Right Limit Switch", "Unpressed");
}

JankyTurret::~JankyTurret(void)
{
	
}

void JankyTurret::Set(float DesiredMotorValue)
{

	//Victor::Set(DesiredMotorValue);
	bool LeftLimPress= TurretLeft.Get();
	bool RightLimPress= TurretRight.Get();

	SmartDashboard::PutNumber("Turret Motor Value", DesiredMotorValue);
	//TurretMotor.Set(DesiredMotorValue);
	
	if (DesiredMotorValue>0.0 && RightLimPress==false)
	{
		Victor::Set(0.0);
		SmartDashboard::PutString("Right Limit Switch", "Pressed");
	}
	else if(DesiredMotorValue<0.0 && LeftLimPress==false)
	{
		Victor::Set(0.0);
		SmartDashboard::PutString("Left Limit Switch", "Pressed");
	}
	else
	{
		SmartDashboard::PutString("Left Limit Switch", "Unpressed");
		SmartDashboard::PutString("Right Limit Switch", "Unpressed");
		if (DesiredMotorValue < Deadband && DesiredMotorValue > Deadband * -1.0)
			Victor::Set(0.0);
		else
			Victor::Set(DesiredMotorValue);
			//printf("Turret Motor Value=%f\n",DesiredMotorValue);
	} 
} 

void JankyTurret::SetDeadband(float DB)
{
	Deadband = DB;
}
