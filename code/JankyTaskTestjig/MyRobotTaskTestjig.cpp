#include "WPILib.h"
#include "jankyRobotTemplate.h"
#include "CounterTaskTestjig.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 

class MyRobotTaskTestjig : public SimpleRobot
{
	Joystick stick; // only joystick
	
public:
	MyRobotTaskTestjig(void):
		stick(1)		// as they are declared above.

	{
	
	}

	void Autonomous(void)
	{
		//AutonomousInit();
		
	}

	void OperatorControl(void)
	{
		//OperatorControlInit();
		
		while (IsOperatorControl())
		{
			//ProgramIsAlive();
			//No need to do waits because ProgramIsAlive function does a wait. //Wait(0.005);
			
			bool isButtonPressed = stick.GetRawButton(3);
			if (isButtonPressed)
			{
				int taskCounter = this.ReturnCounter();
				SmartDashboard::PutNumber("Task Counter",taskCounter);
			}			
			
			float leftYaxis = stick.GetY();
			float rightYaxis = stick.GetRawAxis(5);	//RawAxis(5);
			TankDrive(leftYaxis,rightYaxis); 	// drive with arcade style (use right stick)for joystick 1
			SmartDashboard::PutNumber("Left Axis",leftYaxis);
			SmartDashboard::PutNumber("Right Axis",rightYaxis);	
		}
	}
};

START_ROBOT_CLASS(JankyRobotStarter);

