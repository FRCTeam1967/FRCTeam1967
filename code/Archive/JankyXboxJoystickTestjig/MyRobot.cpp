#include "WPILib.h"
#include "jankyXboxJoystick.h"

/*******************************
 * RobotDemo.cpp               *
 *                             *
 *  Created on: Jan 20, 2013   *
 *  Author: Team1967           *
 *******************************/

/**********************************************************************************************
 * This is a program to test the JankyXboxJoystick class.                                     *                                         *
 **********************************************************************************************/ 

class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	jankyXboxJoystick newStick;
	
public:
	RobotDemo(void):
		myRobot(1,2),	// these must be initialized in the same order
		newStick(1)
		
	{
		myRobot.SetExpiration(0.1);
	}

	void Autonomous(void)
	{
		
	}

	void OperatorControl(void)
	{
		
		bool buttonABool;
		bool buttonBBool;
		bool buttonXBool;
		bool buttonYBool; 
		bool buttonLBBool;
		bool buttonRBBool;
		bool buttonBackBool;
		bool buttonStartBool;
		
		float leftTrigger;
		float rightTrigger;
		float leftYAxis;
		float rightYAxis;
		float leftXAxis;
		float rightXAxis;
		
		myRobot.SetSafetyEnabled(true);
		
		while (IsOperatorControl())
		{
			leftYAxis = newStick.GetLeftYAxis();
			rightYAxis = newStick.GetRightYAxis();
			SmartDashboard::PutNumber("Left Y Axis", leftYAxis);
			SmartDashboard::PutNumber("Right Y Axis", rightYAxis);
			
			leftXAxis = newStick.GetLeftXAxis();
			rightXAxis = newStick.GetRightXAxis();
			SmartDashboard::PutNumber("Left X Axis", leftXAxis);
			SmartDashboard::PutNumber("Right X Axis", rightXAxis);
						
			leftTrigger = newStick.GetLeftThrottle();
			SmartDashboard::PutNumber("left trigger", leftTrigger);

			rightTrigger = newStick.GetRightThrottle();
			SmartDashboard::PutNumber("right trigger", rightTrigger);
			
			buttonABool = newStick.GetButtonA();
			SmartDashboard::PutNumber("buttonA",buttonABool);
			buttonBBool = newStick.GetButtonB();
			SmartDashboard::PutNumber("buttonB",buttonBBool);
			buttonXBool = newStick.GetButtonX();
			SmartDashboard::PutNumber("buttonX",buttonXBool);
			buttonYBool = newStick.GetButtonY();
			SmartDashboard::PutNumber("buttonY",buttonYBool);
			
			buttonLBBool = newStick.GetButtonLB();
			SmartDashboard::PutNumber("buttonLB",buttonLBBool);
			buttonRBBool = newStick.GetButtonRB();
			SmartDashboard::PutNumber("buttonRB",buttonRBBool);
			buttonBackBool = newStick.GetButtonBack();
			SmartDashboard::PutNumber("buttonBack",buttonBackBool);
			buttonStartBool = newStick.GetButtonStart();
			SmartDashboard::PutNumber("buttonStart",buttonStartBool);
			
			Wait(0.005);				
		}
	}

	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);


//TODO float random=driver.Get(BUTTON_Y);
