#include "WPILib.h"
#include "jankyRobotTemplate.h"


class JankyRobotStarter : public JankyRobotTemplate
{
	Joystick stick; // only joystick
	
public:
	JankyRobotStarter(void):
		stick(1)		// as they are declared above.

	{
	
	}

	void Autonomous(void)
	{
		AutonomousInit();
		
	}

	void OperatorControl(void)
	{
		OperatorControlInit();
		
		while (IsOperatorControl())
		{
			ProgramIsAlive();
			//No need to do waits because ProgramIsAlive function does a wait. //Wait(0.005);
			
			float leftYaxis = stick.GetY();
			float rightYaxis = stick.GetRawAxis(5);	//RawAxis(5);
			TankDrive(leftYaxis,rightYaxis); 	// drive with arcade style (use right stick)for joystick 1
			SmartDashboard::PutNumber("Left Axis",leftYaxis);
			SmartDashboard::PutNumber("Right Axis",rightYaxis);	
		}
	}

};

START_ROBOT_CLASS(JankyRobotStarter);

