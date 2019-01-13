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
	Joystick gameComponent;
	Victor leftdrive;
	Victor rightdrive;
	Victor shooterMotorOne;
	Victor shooterMotorTwo;
	Solenoid shooterSolenoid;
	
public:
	RobotDemo(void):
		compressor(2,2),
		myRobot(1, 2),	// these must be initialized in the same order
		driveJoystick(1), // as they are declared above.
		gameComponent (2),
		leftdrive (1),
		rightdrive (2),
		shooterMotorOne (3),
		shooterMotorTwo (4),
		shooterSolenoid (1)
		
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
			myRobot.TankDrive(driveJoystick.GetY(), driveJoystick.GetRawAxis(5)); 
			Wait(0.005);				// wait for a motor update time
			
			//shooting
			bool isSpinButtonPressed = gameComponent.GetRawButton(1);
			bool isFireButtonPressed = gameComponent.GetRawButton(6);
			
						if(isSpinButtonPressed)
						{
							printf("Spin Button Pressed\n");
							shooterMotorOne.Set(0.7);
							shooterMotorTwo.Set(0.7);
						}
						else
						{
							printf("Spin Button Not Pressed\n");
							shooterMotorOne.Set(0.0);
							shooterMotorTwo.Set(0.0);			
							
						}
						
						
						if (isFireButtonPressed)
						{
							printf("Fire Button Pressed\n");
							shooterSolenoid.Set(true);
						}
						else
							shooterSolenoid.Set(false);
			
			
			
			// Hanging
			bool isHangingButtonPressed = driveJoystick.GetRawButton(4);
			if (isHangingButtonPressed == true)
			{	
				myRobot.TankDrive(-1.0,-1.0);
			}	
			else
				myRobot.TankDrive(driveJoystick.GetY(), driveJoystick.GetRawAxis(5));
		}
	}
	
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);
