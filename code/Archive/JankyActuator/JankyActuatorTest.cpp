#include "WPILib.h"
#include "jankyActuator.h"
#include "jankyRobotTemplate.h"


class JankyActuatorTest : public JankyRobotTemplate
{
	Joystick stick; // only joystick
	JankyActuator testActuator;
	Compressor compressor;
	
public:
	JankyActuatorTest(void):
		stick(1),	
		testActuator(6, 7),		//JankyActuator (solenoid or piston channel)
		compressor(5,4)
		
	{
	
	}

	void Autonomous(void)
	{
		AutonomousInit();
		
	}

	void OperatorControl(void)
	{
		OperatorControlInit();
		compressor.Start();
		testActuator.Start();
		
		while (IsOperatorControl())
		{
			ProgramIsAlive();
			//No need to do waits because ProgramIsAlive function does a wait. //Wait(0.005);
			
			bool isButtonPressed = stick.GetRawButton(3);
			SmartDashboard::PutNumber("Actuator Button Status",isButtonPressed);
			if (isButtonPressed)
			{
				testActuator.Go();
			}
			
			
			float leftYaxis = stick.GetY();
			float rightYaxis = stick.GetRawAxis(5);	//RawAxis(5);
			TankDrive(leftYaxis,rightYaxis); 	// drive with arcade style (use right stick)for joystick 1
			SmartDashboard::PutNumber("Left Axis",leftYaxis);
			SmartDashboard::PutNumber("Right Axis",rightYaxis);	
		}
	}
};

START_ROBOT_CLASS(JankyActuatorTest);

