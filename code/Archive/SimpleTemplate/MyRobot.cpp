#include "WPILib.h"
#include "jankyXboxJoystick.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	jankyXboxJoystick stick; // only joystick
	Relay flashring;
	DigitalInput limitButton;
	Timer time;

public:
	RobotDemo(void):
		myRobot(2, 4, 5, 1),	//   RobotDrive (frontLeft, rearLeft, frontRight, rearRight)
		stick(1),
		flashring(1),
		limitButton(1),
		time()
				
	{
		myRobot.SetExpiration(0.1);
		time.Reset();
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
		
		//Variables
		int numPressed = 0;
		int previousNumber = 0;
		bool bPressed = false;

		//Beginning Smart Dashboard
		SmartDashboard::PutString("Test Button","Not pressed");
		SmartDashboard::PutString("Limit Button Status","Light Off");
		SmartDashboard::PutNumber("Timer Status",time.Get());
		SmartDashboard::PutNumber("Number times pressed",numPressed);
		SmartDashboard::PutBoolean("Was button pressed",bPressed);
		SmartDashboard::PutNumber("Previous count",previousNumber);
		
		time.Start();
		//int numberpress;
		//numberpress = 0;

		while (IsOperatorControl())
		{
			//Driving the robot
			myRobot.TankDrive(stick.GetLeftYAxis(),stick.GetRightYAxis()); 			
			
			//If limit button pressed three times light turns on
			if(limitButton.Get() == false)
			{
				if(bPressed == false)
				{
					numPressed = numPressed + 1;
					SmartDashboard::PutNumber("Number times pressed",numPressed);
					bPressed = true;
				}
			}

			else if (limitButton.Get() == true)
			{
				if(numPressed > previousNumber && bPressed == true)
				{
					bPressed = false;
					
					previousNumber = numPressed;
					SmartDashboard::PutNumber("Previous count",previousNumber);
				}
			}
			if(numPressed >= 3)
			{
				flashring.Set(Relay::kForward);
				time.Stop();
			}
			/*
			 * if(numPressed > 3)
			{
				flashring.Set(Relay::kOff);
				numPressed = 0;
				previousNumber = 0;
			}
			*/
			
			
			/*
			//Test button - print out to Smart Dashboard
			if(stick.GetButtonA())
			{
				SmartDashboard::PutString("Test Button","Pressed");
			}
			
			//Working with variables and Smart Dashboard
			bool bTest = false;
			SmartDashboard::PutBoolean("Test Boolean",bTest);
			if(stick.GetButtonX())
			{
				bTest = true;
				SmartDashboard::PutBoolean("Test Boolean",bTest);
			}
			
			//Limit switch to turn light on
			bool isLimitButtonPressed = limitButton.Get();
			if (isLimitButtonPressed == false)
			{
				SmartDashboard::PutString("Limit Button Status","Light On");
				flashring.Set(Relay::kForward);
			}
			else if (isLimitButtonPressed == true)
			{
				flashring.Set(Relay::kOff);
				SmartDashboard::PutString("Limit Button Status","Light Off");
			}
			*/
			
			//Wait at end of loop
			Wait(0.005);
		}
	}

	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

