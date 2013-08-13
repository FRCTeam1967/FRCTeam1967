#include "WPILib.h"
#include "jankyXboxJoystick.h"

/**
 * This is a demo program used as a starting point for new programmers. 
 */ 
class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	jankyXboxJoystick stick; // only joystick
	Relay flashring;
	DigitalInput limitButton;

public:
	RobotDemo(void):
		myRobot(2, 4, 5, 1),		//RobotDrive (frontLeft, rearLeft, frontRight, rearRight)
		stick(1),
		flashring(1),
		limitButton(1)
				
	{
		myRobot.SetExpiration(0.1);
	}

	
	void Autonomous(void)
	{
		myRobot.SetSafetyEnabled(false);
		myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		myRobot.Drive(0.0, 0.0); 	// stop robot
	}


	void OperatorControl(void)
	{
		myRobot.SetSafetyEnabled(false);
		printf("Going into teleop loop");
		
		//Beginning Smart Dashboard
		SmartDashboard::PutString("Test Button","Not pressed");
		SmartDashboard::PutString("Limit Button Status","Light Off");
		
		while (IsOperatorControl())
		{
			//Driving the robot
			myRobot.TankDrive(stick.GetLeftYAxis(),stick.GetRightYAxis()); 
			
			
			//Test button - print out to Smart Dashboard
			if(stick.GetButtonA())
			{
				SmartDashboard::PutString("Test Button","Pressed");
			}
			
			
			flashring.Set(Relay::kForward);
			
			
			//Wait at end of loop
			Wait(0.005);
		}
	}
	

	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

