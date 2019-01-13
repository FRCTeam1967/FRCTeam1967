#include "WPILib.h"
#define LEFT_FRONT 3
#define RIGHT_FRONT 2
#define LEFT_REAR 4
#define RIGHT_REAR 1

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
	Joystick * stick; // only joystick
	RobotDrive * pRobot;
	Victor * pLF;
	Victor * pRF;
	Victor * pLR;
	Victor * pRR;
	Victor * pL;
	Victor * pR;
	Solenoid * shooterSolenoid;
	Victor * shooterMotor;
	static const char groupA = '/elmo/' ; 
	static const char groupB = '/cookiemonster/'; 
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
	RobotDemo(void)

	{
		stick = new Joystick(1);
		
		
		pLF = new Victor(LEFT_FRONT);
		//fourVictorChannels[0] = LEFT_FRONT;
		pRF = new Victor(RIGHT_FRONT);
		//fourVictorChannels[1] = RIGHT_FRONT;
		pLR = new Victor(LEFT_REAR);
		//fourVictorChannels[2] = LEFT_REAR;
		pRR = new Victor(RIGHT_REAR);
		//fourVictorChannels[3] = RIGHT_REAR;
		
		//pRobot = new RobotDrive(pLF, pRF, pLR, pRR);
		
		shooterMotor = new Victor(6);
		shooterSolenoid = new Solenoid(1);
	}

	void RobotInit (void)
	{
		LiveWindow *lw = LiveWindow::GetInstance();
		lw->AddActuator("Shooting", "Shooter Motor", shooterMotor);
		lw->AddActuator("Shooting", "Shooter Piston", shooterSolenoid);
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
		
		float axisChannel1Value;
		float axisChannel2Value;
		float axisChannel3Value;
		float axisChannel4Value;
		float axisChannel5Value;
		float axisChannel6Value;


		//pRobot->SetSafetyEnabled(true);
		
		Watchdog &dog = this->GetWatchdog();
		dog.SetEnabled(true);
		dog.SetExpiration(0.1);

		while (IsOperatorControl())
		{
			dog.Feed();
			button1Bool = stick->GetRawButton(1);
			SmartDashboard::PutNumber(groupA + "Button 1",button1Bool);
			button2Bool = stick->GetRawButton(2);
			SmartDashboard::PutNumber(groupA + "Button 2",button2Bool);
			button3Bool = stick->GetRawButton(3);
			SmartDashboard::PutNumber(groupA + "Button 3",button3Bool);
			button4Bool = stick->GetRawButton(4);
			SmartDashboard::PutNumber(groupA + "Button 4",button4Bool);
			button5Bool = stick->GetRawButton(5);
			SmartDashboard::PutNumber(groupA + "Button 5",button5Bool);
			button6Bool = stick->GetRawButton(6);
			SmartDashboard::PutNumber(groupA + "Button 6",button6Bool);
			button7Bool = stick->GetRawButton(7);
			SmartDashboard::PutNumber(groupB + "Button 7",button7Bool);
			button8Bool = stick->GetRawButton(8);
			SmartDashboard::PutNumber(groupB + "Button 8",button8Bool);
			button9Bool = stick->GetRawButton(9);
			SmartDashboard::PutNumber(groupB + "Button 9",button9Bool);
			button10Bool = stick->GetRawButton(10);
			SmartDashboard::PutNumber(groupB + "Button 10",button10Bool);
			button11Bool = stick->GetRawButton(11);
			SmartDashboard::PutNumber(groupB + "Button 11",button11Bool);
			button12Bool = stick->GetRawButton(12);
			SmartDashboard::PutNumber(groupB + "Button 12",button12Bool);
			

			axisChannel1Value = stick->GetRawAxis(1);
			SmartDashboard::PutNumber("Axis Channel 1", axisChannel1Value);
			axisChannel2Value = stick->GetRawAxis(2);
			SmartDashboard::PutNumber("Axis Channel 2", axisChannel2Value);
			axisChannel3Value = stick->GetRawAxis(3);
			SmartDashboard::PutNumber("Axis Channel 3", axisChannel3Value);
			axisChannel4Value = stick->GetRawAxis(4);
			SmartDashboard::PutNumber("Axis Channel 4", axisChannel4Value);
			axisChannel5Value = stick->GetRawAxis(5);
			SmartDashboard::PutNumber("Axis Channel 5", axisChannel5Value);
			axisChannel6Value = stick->GetRawAxis(6);
			SmartDashboard::PutNumber("Axis Channel 6", axisChannel6Value);

			Wait(0.005);				// wait for a motor update time
		}
	}

	void Test() {
	
		Watchdog &dog = this->GetWatchdog();
		dog.SetEnabled(false);
		dog.SetExpiration(0.1);
		
		printf("In Test Mode\n");
		
		while(IsTest())
		{
			printf("In Test While Loop\n");
			//pRobot->TankDrive(0.0,0.0);
			Wait(0.01);
		}
		
		

	}
};

START_ROBOT_CLASS(RobotDemo);

