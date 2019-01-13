#include "WPILib.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 

Preferences *prefs = Preferences::GetInstance();
SmartDashboard *smarty = SmartDashboard::GetInstance();

void GrabIt(const char *pName)
{
	float tempdouble;
	string tempstring = smarty->GetString(pName);
	sscanf(tempstring.c_str(),"%f",&tempdouble);
	prefs->PutDouble(pName,(double)tempdouble);
}

class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	
public:
	RobotDemo(void):
		myRobot(1, 2),	// these must be initialized in the same order
		stick(1)		// as they are declared above.
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
		char *names[9] = {"P-turret","I-turret","D-turret","P-shooter","I-shooter","D-shooter","P-gyro","I-gyro","D-gyro"};
		
		int i;
		
		for(i=0; i<9; i++)
		{
			smarty->PutString(names[i],"0.00");	
		}

		smarty->PutString("Message","When PID values ready to save, press DISABLE");
		
		
/*		std::string test = prefs->GetString("hollytest","noValue");
		double DoubleTest = prefs->GetDouble("hollytestDouble");
		int IntTest = prefs->GetInt("hollytestInt");
		smarty->PutDouble("SaveDoubleTest",DoubleTest);
		smarty->PutInt("SaveIntTest",IntTest);
		smarty->PutString("SaveTest",test);
		prefs->PutDouble("hollytestDouble",1.0);
		prefs->PutInt("hollytestInt",2);
		prefs->PutString("hollytest","TestSucceed");
		Wait(1.0);
		prefs->Save();
		smarty->PutBoolean("BoolTest",false);
*/		

		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
/*			bool Test = smarty->GetBoolean("BoolTest");
			if(Test)
				smarty->PutString("Boolean","yes");
			else
				smarty->PutString("Boolean","no");
*/
			
			
			i = 0;
			for(i=0; i<9; i++)
				GrabIt(names[i]);
			
			
			
			myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
			Wait(0.005);				// wait for a motor update time
		}
		
		prefs->Save();
		Wait(0.5);
		
	}
};

START_ROBOT_CLASS(RobotDemo);

