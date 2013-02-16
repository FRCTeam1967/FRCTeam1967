#include "WPILib.h"

#define DRIVE_JOYSTICK_PORT 1
#define GC_JOYSTICK_PORT 2

/************************************************************************************************
 * This is a demo program showing the use of the RobotBase class.                            	*
 * The SimpleRobot class is the base of a robot application that will automatically call your	*
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on	*
 * the driver station or the field controls.													*
 ************************************************************************************************/ 
class RobotDemo : public SimpleRobot
{

	Compressor compressor;
	RobotDrive myRobot; // robot drive system
	Joystick driveJoystick;
	Victor leftdrive;
	Victor rightdrive;
	
public:
	RobotDemo(void):
		compressor(2,2),
		myRobot(1, 2),	// these must be initialized in the same order
		driveJoystick(1),		// as they are declared above.
		leftdrive (1),
		rightdrive (2)
	{
		myRobot.SetExpiration(0.1);
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		compressor.Start();
		myRobot.SetSafetyEnabled(false);
		myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		myRobot.Drive(0.0, 0.0); 	// stop robot
	}

	void OperatorControl(void)
	{
		printf("In Teleop\n");
		myRobot.SetSafetyEnabled(true);
		compressor.Start();
		
		while (IsOperatorControl())
		{
			//this code will probably work on the new game console and does should with the old joystick
			myRobot.TankDrive(driveJoystick.GetY(), driveJoystick.GetTwist()); 
			Wait(0.005);				// wait for a motor update time
			
			
			
		}
	}
	
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);
