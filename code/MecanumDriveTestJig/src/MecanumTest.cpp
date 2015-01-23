
#include "WPILib.h"
/*
 * TestMecTest.cpp
 *
 *  Created on: Jan 19, 2015
 *      Author: meghamallya
 */
#define DRIVE_JOYSTICK_PORT 1

class MecanumTest: public SampleRobot
{
	const static int fLeft=1;
	const static int rLeft=2;
	const static int fRight=3;
	const static int rRight=4;
	Joystick*stick;
	double yValue;
	double xValue;
	double rotation;

	RobotDrive myRobot;

public:
	MecanumTest(void): myRobot(fLeft, rLeft, fRight, rRight)
		{
		  stick = NULL;
		//myRobot.SetExpiration(0.1);-Commented in Case
		}



	~MecanumTest(void)
		{
			delete stick;
		}

	void RobotInit(void){
		stick = new Joystick(DRIVE_JOYSTICK_PORT);
	}

	/**
	 * Leaving Autonomous empty until we know what we want
	 * to program for Autonomous.
	 */
	void Autonomous()
	{

	}
	/**
	 * Testing out mecanum drive with joysick
	 */

	void OperatorControl()
	{
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl() && IsEnabled())
		{
			//MECANUM DRIVE
			float yValue = stick->GetY();
			float xValue = stick->GetX();
			float rotation = stick->GetTwist();
			myRobot.MecanumDrive_Cartesian(xValue, yValue, rotation, 0.0);
			Wait(0.005);
		}
	}

};

START_ROBOT_CLASS(MecanumTest);
