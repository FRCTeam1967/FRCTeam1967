#include "WPILib.h"
#include "jankyRobotTemplate.h"
#include "jankyXboxJoystick.h"
#include "jankyAccelerometer.h"

class Robot : public JankyRobotTemplate
{
	//jankyXboxJoystick stick; // only joystick
	JankyAccelerometer accelTest;
	Joystick joystick;

public:
	Robot(void):
		accelTest(),
		joystick (1)
	{
		/*AutonomousInit();
		pRobot->MecanumDrive_Cartesian(0.1, 0.0, 0.0, 0.0);*/

	}

	void OperatorControl(void)
	{
		OperatorControlInit();

		printf("OPERATORCONTROLLL\n");

		while (IsOperatorControl())
		{
			ProgramIsAlive();
			//accelTest.getRevolution();
			//float leftYaxis = stick.GetLeftYAxis();
			//float rightXaxis = stick.GetRightXAxis();
			float LeftaxisYValue = joystick.GetY();
			float LeftaxisXValue = joystick.GetX();
			SmartDashboard::PutNumber("Get Y joystick",LeftaxisYValue);
			SmartDashboard::PutNumber("Get X joystick",LeftaxisXValue);

			//GetTwist does rotate the robot but only with the accelerator part
			float RightaxisXValue = joystick.GetTwist();
			SmartDashboard::PutNumber("Get Twist",RightaxisXValue);

			//Does not work to rotate the robot
			//float RightaxisXValue = joystick.GetTrigger();

			//pRobot->MecanumDrive_Cartesian(leftYaxis, rightXaxis, 0.0, 0.0);
			pRobot->MecanumDrive_Cartesian(LeftaxisYValue, LeftaxisXValue, RightaxisXValue, 0.0);

			accelTest.Run();
		}

	}

	void Autonomous(void)
	{

	}

};

START_ROBOT_CLASS(Robot);
