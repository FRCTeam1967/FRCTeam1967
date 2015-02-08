#include "WPILib.h"

/*******************************
 * RobotDemo.cpp               *
 *                             *
 *  Created on: Jan 20, 2013   *
 *  Author: Team1967           *
 *******************************/

/**********************************************************************************************
 * This is a program to test the JankyXboxJoystick class.                                     *                                         *
 **********************************************************************************************/ 

class RobotDemo : public SampleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	JoystickButton button1;
	JoystickButton button2;
	JoystickButton button3;
	JoystickButton button4;
	JoystickButton button5;
	JoystickButton button6;
	JoystickButton button7;
	JoystickButton button8;
	JoystickButton button9;
	JoystickButton button10;
	JoystickButton button11;
	JoystickButton button12;
	
	/*
	Notes for the XBox controller: 
	Button A: 1
	Button B: 2
	Button X: 3
	Button Y: 4
	Button LB: 5
	Button RB: 6
	Button LT: Range 0 to 1.000 (stick.GetTrigger())
	Button RT: Range 0 to -1.000 (stick.GetTrigger())
	Button Back: 7
	Button Start: 8
	Left Axis press: 9
			  X-Axis: -1.000 to 1.000 (stick.GetX())
			  Y-Axis: -1.000 to 1.000 (stick.GetY())
	Right Axis press: 10
			  X-Axis: -1.000 to 1.000 (stick.GetTwist())
			  Y-Axis: 			   
	 */ 
	
public:
	RobotDemo(void):
		myRobot(1,2),	// these must be initialized in the same order
		stick(1),		// as they are declared above.
		button1(&stick, 1),
        button2(&stick, 2),
        button3(&stick, 3),
        button4(&stick, 4),
        button5(&stick, 5),
        button6(&stick, 6),
        button7(&stick, 7),
        button8(&stick, 8),
        button9(&stick, 9),
        button10(&stick, 10),
        button11(&stick, 11),
        button12(&stick, 12)
	{
		myRobot.SetExpiration(0.1);
	}

	void Autonomous(void)
	{
		/*
		myRobot.SetSafetyEnabled(false);
		myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		myRobot.Drive(0.0, 0.0); 	// stop robot
		*/
	}

	void OperatorControl(void)
	{

		bool button1Bool;
		bool button2Bool;
		bool button3Bool;
		bool button4Bool;
		bool button5Bool;
		bool button6Bool;
		bool button7Bool;
		bool button8Bool;
		bool button9Bool;
		bool button10Bool;
		bool button11Bool;
		bool button12Bool;
		float LeftaxisYValue;
		float LeftaxisXValue;
		float LeftaxisZValue;
	
		float RightaxisXValue;
		float RightaxisYValue;
		
		float TriggerValue;

		myRobot.SetSafetyEnabled(true);
		
		SmartDashboard::PutNumber("Number of Axis values:", stick.GetAxisCount());

		while (IsOperatorControl())
		{

			button1Bool = button1.Get();
			SmartDashboard::PutNumber("button1",button1Bool);
			button2Bool = button2.Get();
			SmartDashboard::PutNumber("button2",button2Bool);
			button3Bool = button3.Get();
			SmartDashboard::PutNumber("button3",button3Bool);
			button4Bool = button4.Get();
			SmartDashboard::PutNumber("button4",button4Bool);
			button5Bool = button5.Get();
			SmartDashboard::PutNumber("button5",button5Bool);
			button6Bool = button6.Get();
			SmartDashboard::PutNumber("button6",button6Bool);
			button7Bool = button7.Get();
			SmartDashboard::PutNumber("button7",button7Bool);
			button8Bool = button8.Get();
			SmartDashboard::PutNumber("button8",button8Bool);
			button9Bool = button9.Get();
			SmartDashboard::PutNumber("button9",button9Bool);
			button10Bool = button10.Get();
			SmartDashboard::PutNumber("button10",button10Bool);
			button11Bool = button11.Get();
			SmartDashboard::PutNumber("button11",button11Bool);
			button12Bool = button12.Get();
			SmartDashboard::PutNumber("button12",button12Bool);
			
			
			LeftaxisXValue = stick.GetX();
			SmartDashboard::PutNumber("Joystick1 X Axis",LeftaxisXValue);
			LeftaxisYValue = 0 - stick.GetY();
			SmartDashboard::PutNumber("Joystick1 Y Axis",LeftaxisYValue);
			LeftaxisZValue = stick.GetZ();
			SmartDashboard::PutNumber("Joystick1 Z Axis",LeftaxisZValue);
			
			
			RightaxisXValue = stick.GetTwist();
			SmartDashboard::PutNumber("Joystick2 X Axis", RightaxisXValue);
			RightaxisYValue = 0 - stick.GetRawAxis(5);
			SmartDashboard::PutNumber("Joystick2 Y Axis", RightaxisYValue);
			
			TriggerValue = stick.GetThrottle();
			SmartDashboard::PutNumber("Trigger value", TriggerValue);

			Wait(0.005);				// wait for a motor update time
		}
	}

	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

