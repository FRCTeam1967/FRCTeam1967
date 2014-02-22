#include "WPILib.h"
#include "jankyRobotTemplate.h"
#include "jankyXboxJoystick.h"
#include "jankyEncoder.h"

class EncoderTestjig : public JankyRobotTemplate
{
	//jankyXboxJoystick stick; // only joystick
	JankyEncoder encodertest;
	Joystick joystick;
	static const float Kp=0;
	 
	
public:
	EncoderTestjig(void):
		//stick(1),		// as they are declared above.
		encodertest(3,4,5),   //5=PWM
		joystick (1)
	{
		/*AutonomousInit();
		pRobot->MecanumDrive_Cartesian(0.1, 0.0, 0.0, 0.0);*/
		
	}

	void OperatorControl(void)
	{
		OperatorControlInit();
		encodertest.Reset();
		encodertest.setSpeed (.5);
		//encodertest.setRevolution (5.5);
		encodertest.Start();
		//encodertest.Go();
		
		printf("OPERATORCONTROLLL\n");
		
		
		while (IsOperatorControl())
		{
			ProgramIsAlive(); 
			//encodertest.getRevolution();
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
			
			/*
			if (stick.GetButtonA())
			{
				encodertest.setRevolution (5);
				encodertest.Go();
			}
			
			if (stick.GetButtonB())
			{
				encodertest.setRevolution (10);
				encodertest.Go();
			}
			
			if (stick.GetButtonY())
			{
				encodertest.setRevolution (15);
				encodertest.Go();
			}
			if (encodertest.isDone())
			{
				encodertest.Reset();
			}
			//No need to do waits because ProgramIsAlive does it
			 */
		}
		
	}

	void Autonomous(void)
	{

	}
	
};

START_ROBOT_CLASS(EncoderTestjig);


