#include "WPILib.h"
#include "jankyShooter.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	Joystick gamecomponent;
    SmartDashboard* smarty;
    JoystickButton button;
    JoystickButton ballbutton;
    Solenoid ballpusher;
    Jaguar conveyor;
    Compressor compressor;
    //InternalButton originalfire;
    
public:
	RobotDemo(void):
		myRobot(1, 2),	// these must be initialized in the same order
		stick(1),
		gamecomponent(2),// as they are declared above.
		button(&stick, 1),
		ballbutton(&gamecomponent,6),
		ballpusher (6),
		conveyor(8),
		compressor(8,2)
		
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
		//InternalButton* pFire;
		myRobot.SetSafetyEnabled(true);
		
		JankyShooter shoot(SHOOTER_JAGUAR_CHANNEL,SHOOTER_ENCODER_A,SHOOTER_ENCODER_B);
		
		SmartDashboard* smarty = SmartDashboard::GetInstance();
				
		smarty->PutString("setRPM","0");
		
		compressor.Start();
		
		//smarty->PutData("Fire", &originalfire);

		while (IsOperatorControl())
		{
			
			float nrpm;
			
			string rpm= smarty->GetString("setRPM");
			
			sscanf(rpm.c_str(), "%f", &nrpm);
			
			smarty->PutInt("Integer RPM", (int)nrpm);
									
			shoot.setTargetRPM((int)nrpm);
												
			if (ballbutton.Get()==true)
				
				ballpusher.Set(true);
			
			else
				ballpusher.Set(false);
			
			conveyor.Set(gamecomponent.GetY());
			
			/* internal button test-fails
			
			pFire = (InternalButton*)smarty->GetData("Fire");
			if (pFire->Get()==true)
			{ 
				
				smarty->PutString("Test1", "pressed");
			}	
			
			else
				smarty->PutString("Test1", "not pressed");
			
			
			*/
			
			myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
			Wait(0.05);				// wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

