#include "WPILib.h"
#include "jankyRobot.h"
/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class JankyRobotStarter : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	Joystick AGH;
	//Jaguar leopard;
	Victor leftMotor;
	Victor rightMotor;
	
public:
	JankyRobotStarter(void):
		leftMotor(1),
		rightMotor(2),
		myRobot(leftMotor,rightMotor),
		//myRobot(3,1,4,2),	// these must be initialized in the same order 3,1,4,2 Joe Jr.; NEW ROBOT = 2,1,3,4
		stick(1),		// as they are declared above.
		AGH(2)
		//leopard(7)
	{
		myRobot.SetExpiration(0.1);
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		myRobot.SetSafetyEnabled(false);
		myRobot.Drive(0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		myRobot.Drive(0.0, 0.0); 	// stop robot
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		Watchdog &dog = this->GetWatchdog();
		dog.SetEnabled(true);
		dog.SetExpiration(0.1);
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			dog.Feed();
			//myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
			//myRobot.SetSafetyEnabled(false);
			//myRobot.SetExpiration(0.1);
			float leftYaxis = stick.GetY();
			float rightYaxis = stick.GetTwist();//RawAxis(5);
			myRobot.TankDrive(leftYaxis,rightYaxis); // drive with arcade style (use right stick)for joystick 1
			SmartDashboard::PutNumber("Left Axis",leftYaxis);
			SmartDashboard::PutNumber("Right Axis",rightYaxis);
			//printf("leftAxis=%d, rightAxis= %d\n", leftYaxis, rightYaxis);
			Wait(0.005);				// wait for a motor update time
			//float pleasework = AGH.GetY();
		//	leopard.Set(pleasework);
		}
	}
};

START_ROBOT_CLASS(JankyRobotStarter);

