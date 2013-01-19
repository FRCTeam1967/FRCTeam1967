#include "WPILib.h"
#include "stdio.h"
#include "iostream"
#include "math.h"	
//#include "SmartDashboard/SmartDashboardData.h"
#include <map>
#include <string>
#include "ErrorBase.h"
//#include "NetworkTables/NetworkTableChangeListener.h"
//#include "SmartDashboard/SmartDashboardNamedData.h"
#include <set>
#include "NetworkTables/NetworkTable.h"
#include "WPIErrors.h"
#include "jankyTurret.h"
#include "jankyShooter.h"
#include "jankyTargeting.h"
//#define GYRO_PORT
//#define GYRO_Port 2
//uncomment USE_HARDWIRED_RPM to disable calculating RPM from distance function
#define USE_HARDWIRED_RPM 
#define HARDWIRED_RPM 1300
#define LMOTOR_PORT 1
#define RMOTOR_PORT 2
/***************************************************************************************************
 * This is a demo program showing the use of the RobotBase class.                                  *
 * The SimpleRobot class is the base of a robot application that will automatically call your      *
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on       *
 * the driver station or the field controls.                                                       *
 ***************************************************************************************************/ 
class RobotDemo : public SimpleRobot // Class = Begin
{
	/* Class is the same thing as Begin. Code also beging w/ parenthesis and end w/ parenthesis */
/*	Victor fr; //front right drive motor
	Victor fl; //front left drive motor
	Victor rr; //rear right drive motor
	Victor rl; //rear left drive motor
*/
	Victor leftMotor; //motor for test chassis
	Victor rightMotor; //motor for test chassis
	RobotDrive myRobot; // robot drive system   
	Joystick driver;    // driving Joystick for driver
	Joystick gamecomponent; //joystick for game component
	//Jaguar cheetah; // motor for shooting; 
	Jaguar lynx; //motor for elevator
	Compressor compressor; // Compsressor for the pneumatics system
	Gyro gyro; //the gyro for use during the balancing
	Solenoid solenoid; // the solenoid for bridge tipper
	Solenoid shepard;  // the solenoid for the ball pusher thing
	//SL Encoder Earth; // encoder for measuring speed
	//DigitalInput limitswitchright; // limit switch for the right side of the turret
	//DigitalInput limitswitchleft;  // limit switch for the left side of the turret
	DigitalInput bridge; //switch for bridge AUTONOMOUS
	DigitalInput high; //switch for high hoop AUTONOMOUS
	DigitalInput middle; //switch for middle hoop AUTONOMOUS
	DigitalInput twosec; //switch for 2 second delay AUTONOMOUS
	DigitalInput fivesec; //switch for 5 second DELAY AUTONOMOUS
	JoystickButton Gandalf; //button for automatic calculation of velocity for shooting + shooting
	JoystickButton Smeagol; //button for balancing
	JoystickButton Frodo;   // button for moving the elevator
	JoystickButton joystickbutton; // button for pushing balls through turret
	JoystickButton julesverne; // button for pushing the bridge down during the endgame for point
	//JoystickButton Pippin; //TEST BUTTON FOR ANYTHING!!!!
	JoystickButton button_H;
	JoystickButton button_M;
	JoystickButton autotarget; 
	JoystickButton button_L;
	Relay flashring; //turning on and off flashring
	JankyTurret turret; 
	JankyShooter shooter;
	JankyTargeting targeting;
//	Watchdog baddog;
	Timer ShooterTimer;
	
	//Problems: ports from DIO-8 and 9 are BAD
	//Might be in cRIO or 9403 or digital sidecar or ribbon cable--do not know which)
	
public:
	RobotDemo(void):
		//DIO 8 does not work
/*		fr(2),
		fl(3),
		rr(1),
		rl(4),
*/
		leftMotor(LMOTOR_PORT),
		rightMotor(RMOTOR_PORT),
//		myRobot(fl,rl,fr,rr),	// these must be initialized in the same order (#s in parenthesis refer to port numbers) 3,4,2,1
		myRobot(leftMotor,rightMotor), 
		driver(1),              // as they are declared above. joystick #1 for driving
		gamecomponent(2),       // joystick #2 for game component
		//cheetah(7),	 //Victor 7; for the turret
		lynx(6),     //Victor 8; for elevator
		//panther(10), //Victor 10; for pusing ball into turret
		compressor(5, 4), // (UINT32 pressureSwitchChannel, UINT32 compressorRelayChannel)
		gyro(1),          // port #; gyro is analog
		solenoid(5), // relay # = 5; for the bridge tipper
		shepard(6),  // relay # = 6; for the ball kicker that pushes the ball into the turret
		//SL Earth(6,7),
		bridge(1),
		high(2),
		middle(3), //switch for middle hoop AUTONOMOUS
		twosec(4), //switch for 2 second delay AUTONOMOUS
		//fivesec(8), //switch for 5 second DELAY AUTONOMOUS
		fivesec(13),
		Gandalf(&gamecomponent,1),
		Smeagol(&driver, 2),      // 
		Frodo(&gamecomponent, 4), // Frodo is controlled using button #4 on the second joystick
		joystickbutton(&driver, 7), // joystickbutton is controlled using button #7 on the second joystick
		julesverne(&gamecomponent, 6),    // julesverne is controlled using button #4 on the first joystick; I may change this later
		//Pippin(&gamecomponent,7),
		button_H(&gamecomponent,1),
		button_M(&gamecomponent,2),
		autotarget(&gamecomponent,5),
		button_L(&gamecomponent,3),
		flashring(2),
		turret(7,11,10),
		shooter(5,6,7),
		targeting(&turret),
		// baddog(),
		ShooterTimer()
		
	{
		myRobot.SetExpiration(0.1);
	}
	/******************************************************
	 * Drive left & right motors for 2 seconds then stop  *
	 ******************************************************/
	void Autonomous(void)
	{
		//increase 2nd RPM
		compressor.Start(); // starts the compressor; 
		bool BRIDGE = bridge.Get();
		bool HIGH = high.Get();
		bool MIDDLE = middle.Get();
		bool TWOSEC = twosec.Get();
		bool FIVESEC = fivesec.Get();
		int highRPM = 1800; // 1st 1800 short about 5 ft
		int secondhighRPM = 1800; //1st 1400 (did not fire)
		int rpmForShooter;
		DriverStationLCD *dslcd = DriverStationLCD::GetInstance(); // don't press SHIFT 5 times; this line starts up driver station messages (in theory
		char debugout [100];
	//	baddog.SetExpiration(30.0);
	//	baddog.Feed();
		dslcd->Clear(); 
		sprintf(debugout,"Bridge=%u",BRIDGE); 
		dslcd->Printf(DriverStationLCD::kUser_Line1,1,debugout);
		sprintf(debugout,"High=%u",HIGH); 
		dslcd->Printf(DriverStationLCD::kUser_Line2,2,debugout);
		sprintf(debugout,"Middle=%u",MIDDLE); 
		dslcd->Printf(DriverStationLCD::kUser_Line3,3,debugout);
		sprintf(debugout,"TwoSec=%u",TWOSEC); 
		dslcd->Printf(DriverStationLCD::kUser_Line4,4,debugout);
		sprintf(debugout,"FiveSec=%u",FIVESEC); 
		dslcd->Printf(DriverStationLCD::kUser_Line5,5,debugout);
		dslcd->UpdateLCD(); // update the Driver Station with the information in the code */
		if (BRIDGE == 0 && HIGH == 0 && MIDDLE == 0 && TWOSEC == 0 && FIVESEC == 0)
			{
				myRobot.Drive(0.0, 0.0);
				Wait(10.0);
			}
		if (BRIDGE == 1 && HIGH == 0 && MIDDLE == 0 && TWOSEC == 0 && FIVESEC == 0) //1700 RPM
			{
				/*myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
				*/
			
							flashring.Set(Relay::kForward);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<7)
							{	
								shooter.setTargetRPM(1700);
								//wait-0.01
								Wait(0.005);
							}

							lynx.Set(-1.0);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<1)
							{	
								shooter.setTargetRPM(1700);
								Wait(0.005);
							}
							turret.Set(-0.05);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<0.2)
							{	
								shooter.setTargetRPM(1700);
								Wait(0.005);
							}
							turret.Set(0.0);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<2.0)
							{	
								shooter.setTargetRPM(1700);
								Wait(0.005);
							}
			//				lynx.Set(0.0);
							shepard.Set(true);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<2.0)
							{	 
								shooter.setTargetRPM(1700);
								Wait(0.005);
							}
							shepard.Set(false);
							shooter.setTargetRPM((int)0);
							flashring.Set(Relay::kOff);
							lynx.Set(0.0);
			}
		if (BRIDGE == 1 && HIGH == 1 && MIDDLE == 0 && TWOSEC == 0 && FIVESEC == 0) //main autonomous code-default
			{
		    	flashring.Set(Relay::kForward);
				ShooterTimer.Reset();
				ShooterTimer.Start();
				while(ShooterTimer.Get()<7)
				{	
					shooter.setTargetRPM((int)highRPM);
					//wait-0.01
					Wait(0.005);
				}

				lynx.Set(-1.0);
				ShooterTimer.Reset();
				ShooterTimer.Start();
				while(ShooterTimer.Get()<1)
				{	
					shooter.setTargetRPM((int)highRPM);
					Wait(0.005);
				}
				turret.Set(-0.05);
				ShooterTimer.Reset();
				ShooterTimer.Start();
				while(ShooterTimer.Get()<0.2)
				{	
					shooter.setTargetRPM((int)secondhighRPM);
					Wait(0.005);
				}
				turret.Set(0.0);
				ShooterTimer.Reset();
				ShooterTimer.Start();
				while(ShooterTimer.Get()<2.0)
				{	
					shooter.setTargetRPM((int)secondhighRPM);
					Wait(0.005);
				}
//				lynx.Set(0.0);
				shepard.Set(true);
				ShooterTimer.Reset();
				ShooterTimer.Start();
				while(ShooterTimer.Get()<2.0)
				{	 
					shooter.setTargetRPM((int)secondhighRPM);
					Wait(0.005);
				}
				shepard.Set(false);
				shooter.setTargetRPM((int)0);
				flashring.Set(Relay::kOff);
				lynx.Set(0.0);
			//	baddog.Feed();
			}
		if (BRIDGE == 1 && HIGH == 1 && MIDDLE == 0 && TWOSEC == 1 && FIVESEC == 0)
			{
				//Wait(2.0);
				//Robot aims
				//Robot shoots
				//myRobot.Drive(-0.5, 0.0);
				//Wait(3.0);
			
							flashring.Set(Relay::kForward);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<7)
							{	
								shooter.setTargetRPM(1900);
								//wait-0.01
								Wait(0.005);
							}

							lynx.Set(-1.0);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<1)
							{	
								shooter.setTargetRPM(1900);
								Wait(0.005);
							}
							turret.Set(-0.05);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<0.2)
							{	
								shooter.setTargetRPM(1800);
								Wait(0.005);
							}
							turret.Set(0.0);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<2.0)
							{	
								shooter.setTargetRPM(1800);
								Wait(0.005);
							}
			//				lynx.Set(0.0);
							shepard.Set(true);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<2.0)
							{	 
								shooter.setTargetRPM(1800);
								Wait(0.005);
							}
							shepard.Set(false);
							shooter.setTargetRPM((int)0);
							flashring.Set(Relay::kOff);
							lynx.Set(0.0);
			}
		if (BRIDGE == 1 && HIGH == 1 && MIDDLE == 0 && TWOSEC == 0 && FIVESEC == 1)
			{
				Wait(5.0);
				//Robot aims
				//Robot shoots
				//myRobot.Drive(-0.5, 0.0);
				myRobot.Drive(0.0,0.0);
				Wait(3.0);
			}		
		if (BRIDGE == 1 && HIGH == 0 && MIDDLE == 1 && TWOSEC == 0 && FIVESEC == 0)
			{
				//Robot aims 
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);			
			}
		if (BRIDGE == 1 && HIGH == 0 && MIDDLE == 1 && TWOSEC == 1 && FIVESEC == 0)
			{
				Wait(2.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}
		if (BRIDGE == 1 && HIGH == 0 && MIDDLE == 1 && TWOSEC == 0 && FIVESEC == 5)
			{
				Wait(5.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}				
		if (BRIDGE == 0 && HIGH == 1 && MIDDLE == 0 && TWOSEC == 0 && FIVESEC == 0) //position robot-front of key-low RPMs
			{
				//Robot aims 
				//Robot shoots
				//myRobot.Drive(-0.5, 0.0);
				//Wait(3.0);	
			
							flashring.Set(Relay::kForward);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<7)
							{	
								shooter.setTargetRPM(1600);
								//wait-0.01
								Wait(0.005);
							}

							lynx.Set(-1.0);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<1)
							{	
								shooter.setTargetRPM(1600);
								Wait(0.005);
							}
							turret.Set(-0.05);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<0.2)
							{	
								shooter.setTargetRPM(1600);
								Wait(0.005);
							}
							turret.Set(0.0);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<2.0)
							{	
								shooter.setTargetRPM(1600);
								Wait(0.005);
							}
			//				lynx.Set(0.0);
							shepard.Set(true);
							ShooterTimer.Reset();
							ShooterTimer.Start();
							while(ShooterTimer.Get()<2.0)
							{	 
								shooter.setTargetRPM(1600);
								Wait(0.005);
							}
							shepard.Set(false);
							shooter.setTargetRPM((int)0);
							flashring.Set(Relay::kOff);
							lynx.Set(0.0);
			}
		if (BRIDGE == 0 && HIGH == 1 && MIDDLE == 0 && TWOSEC == 1 && FIVESEC == 0)
			{
				Wait(2.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}
		if (BRIDGE == 0 && HIGH == 1 && MIDDLE == 0 && TWOSEC == 0 && FIVESEC == 1)
			{
				Wait(5.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}		
		if (BRIDGE == 0 && HIGH == 0 && MIDDLE == 1 && TWOSEC == 0 && FIVESEC == 0)
			{
				//Robot aims 
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);			
			}
		if (BRIDGE == 0 && HIGH == 0 && MIDDLE == 1 && TWOSEC == 1 && FIVESEC == 0)
			{
				Wait(2.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}
		if (BRIDGE == 0 && HIGH == 0 && MIDDLE == 1 && TWOSEC == 0 && FIVESEC == 5)
			{
				Wait(5.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
	}
		if (BRIDGE == 0 && HIGH == 0 && MIDDLE == 1 && TWOSEC == 1 && FIVESEC == 1)
		{
		
			flashring.Set(Relay::kForward);
			ShooterTimer.Reset();
			ShooterTimer.Start();
			targeting.SetLMHTarget(BOGEY_H);
			while(ShooterTimer.Get()<2)
			{
				if (targeting.ProcessOneImage())
				{
					targeting.ChooseBogey();
					targeting.MoveTurret();
					rpmForShooter = targeting.GetCalculatedRPM();
					shooter.setTargetRPM(rpmForShooter);
					Wait(0.01);
				}
			}
			targeting.StopPID();
			ShooterTimer.Reset();
			ShooterTimer.Start();
			while(ShooterTimer.Get() < 7)
			{
				shooter.setTargetRPM(rpmForShooter);
				Wait(0.005);
			}

			lynx.Set(-1.0);
			ShooterTimer.Reset();
			ShooterTimer.Start();
			while(ShooterTimer.Get()<1)
			{	
				shooter.setTargetRPM(rpmForShooter);
				Wait(0.005);
			}
			turret.Set(-0.05);
			ShooterTimer.Reset();
			ShooterTimer.Start();
			while(ShooterTimer.Get()<0.2)
			{	
				shooter.setTargetRPM(rpmForShooter);
				Wait(0.005);
			}
			turret.Set(0.0);
			ShooterTimer.Reset();
			ShooterTimer.Start();
			while(ShooterTimer.Get()<2.0)
			{	
				shooter.setTargetRPM(rpmForShooter);
				Wait(0.005);
			}
		//	lynx.Set(0.0);
			shepard.Set(true);
			ShooterTimer.Reset();
			ShooterTimer.Start();
			while(ShooterTimer.Get()<2.0)
			{	 
				shooter.setTargetRPM(rpmForShooter);
				Wait(0.005);
			}
			shepard.Set(false);
			shooter.setTargetRPM((int)0);
			flashring.Set(Relay::kOff);
			lynx.Set(0.0);
			
		}
		
		if (BRIDGE == 1 && HIGH == 1 && MIDDLE == 1 && TWOSEC == 0 && FIVESEC == 0)
				{
									flashring.Set(Relay::kForward);
									ShooterTimer.Reset();
									ShooterTimer.Start();
									while(ShooterTimer.Get()<7)
									{	
										shooter.setTargetRPM(2000); //high RPM
										//wait-0.01
										Wait(0.005);
									}

									lynx.Set(-1.0);
									ShooterTimer.Reset();
									ShooterTimer.Start();
									while(ShooterTimer.Get()<1)
									{	
										shooter.setTargetRPM(2000); //high RPM
										Wait(0.005);
									}
									turret.Set(-0.05);
									ShooterTimer.Reset();
									ShooterTimer.Start();
									while(ShooterTimer.Get()<0.2)
									{	
										shooter.setTargetRPM(1800); //low RPM
										Wait(0.005);
									}
									turret.Set(0.0);
									ShooterTimer.Reset();
									ShooterTimer.Start();
									while(ShooterTimer.Get()<2.0)
									{	
										shooter.setTargetRPM(1800); //low RPM
										Wait(0.005);
									}
					//				lynx.Set(0.0);
									shepard.Set(true);
									ShooterTimer.Reset();
									ShooterTimer.Start();
									while(ShooterTimer.Get()<2.0)
									{	 
										shooter.setTargetRPM(1800); //low RPM
										Wait(0.005);
									}
									shepard.Set(false);
									shooter.setTargetRPM((int)0);
									flashring.Set(Relay::kOff);
									lynx.Set(0.0);
				}		
		//baddog.Feed();
		myRobot.SetSafetyEnabled(false);
	}

	/****************************************
	 * Runs the motors with arcade steering.*
	 ****************************************/
	void OperatorControl(void)
	{
//TODO put in servo for lower camera--look in WPI to set	
//		Watchdog baddog;
		
	//	baddog.Feed();
		myRobot.SetSafetyEnabled(true);
		//SL Earth.Start(); // turns on Earth
//		SmartDashboard *smarty = SmartDashboard::GetInstance();
		//DriverStationLCD *dslcd = DriverStationLCD::GetInstance(); // don't press SHIFT 5 times; this line starts up driver station messages (in theory)
		//char debugout [100];
		compressor.Start();
		gyro.Reset(); // resets gyro angle
		int rpmForShooter;

		
		while (IsOperatorControl()) // while is the while loop for stuff; this while loop is for "while it is in Teleop"
		{ 
//			baddog.Feed();
			//myRobot.SetSafetyEnabled(true);
			//myRobot.SetExpiration(0.1);
			float leftYaxis = driver.GetY();
			float rightYaxis = driver.GetTwist();//RawAxis(5);
			myRobot.TankDrive(leftYaxis,rightYaxis); // drive with arcade style (use right stick)for joystick 1
			float random = gamecomponent.GetY();
			float lazysusan = gamecomponent.GetZ();
			//bool elevator = Frodo.Get();
			float angle = gyro.GetAngle();
			bool balance = Smeagol.Get();
			SmartDashboard::PutNumber("Gyro Value",angle);
			int NumFail = -1;
			//bool light = Pippin.Get();
			//SL float speed = Earth.GetRate();
			//float number = shooter.Get();
			//bool highspeed = button1.Get()
			//bool mediumspeed = button2.Get();
			//bool slowspeed = button3.Get();
			bool finder = autotarget.Get();
			//bool targetandspin = autodistanceandspin.Get();
			SmartDashboard::PutString("Targeting Activation","");
			//dslcd->Clear();
			//sprintf(debugout,"Number=%f",angle); 
			//dslcd->Printf(DriverStationLCD::kUser_Line2,2,debugout);
			//SL sprintf(debugout,"Number=%f",speed);
			//SL dslcd->Printf(DriverStationLCD::kUser_Line4,4,debugout);
			//sprintf(debugout,"Number=%f",number);
			//dslcd->Printf(DriverStationLCD::kUser_Line1,1,debugout);
			//sprintf(debugout,"Finder=%u",finder);
			//dslcd->Printf(DriverStationLCD::kUser_Line5,5,debugout);
			//dslcd->UpdateLCD(); // update the Driver Station with the information in the code
		    // sprintf(debugout,"Number=%u",maxi);
			// dslcd->Printf(DriverstationLCD::kUser_Line6,5,debugout)
						bool basketballpusher = julesverne.Get();
						bool bridgetipper = joystickbutton.Get();
						if (bridgetipper) // if joystick button 7 is pressed (is true)
						{	
							solenoid.Set(true); // then the first solenoid is on
						}
						
							else
							{
							//Wait(0.5); // and then the first solenoid waits for 0.5 seconds
							solenoid.Set(false); //and then the first solenoid turns off
						}
						if (basketballpusher) // if joystick button 6 is pressed (is true)
						{
							shepard.Set(true); // then shepard is on the run
							//Wait(0.5); // and shepard waits for 0.5 seconds
						}
							else
							{		
							shepard.Set(false); // and then shepard turns off
						} //10.19.67.9 IP address of computer;255.0.0.0 subnet mask ALL FOR WIRELESS CONNECTION #2			

			//}	
			//cheetah.Set(0.3*lazysusan);
//			smarty->PutDouble("pre-elevator",lynx.Get());
			lynx.Set(random);
//			smarty->PutDouble("elevator",lynx.Get());
			
//			smarty->PutDouble("joystick elevator",random);
			
			
			if (balance)						// this is the start of the balancing code
			{
				angle = gyro.GetAngle();
				myRobot.Drive(-0.03*angle, 0.0);
				Wait(0.005);
			}
			/*if (light)							//button 5 turns light on oand off on game controller
			     flashring.Set(Relay::kForward);
			     else
			            flashring.Set(Relay::kOff);
			 */           
			if (finder)
			{
				flashring.Set(Relay::kForward);
				if (button_H.Get()==true)
				{
					targeting.SetLMHTarget(BOGEY_H);
					SmartDashboard::PutString("Targeting","High Button Pressed");
				}
				if (button_M.Get()==true)
				{
					targeting.SetLMHTarget(BOGEY_M);
					SmartDashboard::PutString("Targeting","Medium Button Pressed");
				}
				if (button_L.Get()==true)
				{
					targeting.SetLMHTarget(BOGEY_L);
					SmartDashboard::PutString("Targeting","Low Button Pressed");
				}
				if (button_H.Get()==true || button_M.Get()==true || button_L.Get()==true)
				{
					if (targeting.ProcessOneImage())
					{
						NumFail = 0;
						SmartDashboard::PutString("Targeting Activation","YES");
						targeting.ChooseBogey();
						targeting.MoveTurret();
#ifdef USE_HARDWIRED_RPM
						shooter.setTargetRPM(HARDWIRED_RPM);
#else				
						rpmForShooter = targeting.GetCalculatedRPM();
						shooter.setTargetRPM(rpmForShooter);
#endif
						
						targeting.InteractivePIDSetup();
					}
					
					else
					{
						NumFail++;
						if (NumFail > 10)
							targeting.StopPID();
						
					}
					SmartDashboard::PutNumber("Numfail", NumFail);
					
					
					shooter.setTargetRPM(rpmForShooter);
				}	
				
				else 
				{	
					SmartDashboard::PutString("Targeting Activation","NO");
					shooter.setTargetRPM(0);
					targeting.StopPID();
				}	
			}
			else
			{	
				flashring.Set(Relay::kOff);
				targeting.StopPID();
				turret.Set(lazysusan);	// the lazy susan would turn right & left based on how far the person moves the right joystick#2 side to side
				
				//targeting.StopPID();
				//if (elevator)           //shooter would move at full speed if button is pressed

//TODO Change RPM values
//TODO Disable calculation of RPM values 

				
				SmartDashboard::PutNumber("CurrentRPM",shooter.GetCurrentRPM());
				
				if (button_H.Get() == true)
					shooter.setTargetRPM((int)2100);
					//From front of free throw line, should hit the backboard and go in
					//used to be 2700 RPMs
				
				else if (button_M.Get() == true)
					 shooter.setTargetRPM((int)1900);
					//From front of free throw line, should go in the net--can shoot the next ball on the overshoot?
					//Used to be 2250 RPMs
				
				else if (button_L.Get() == true)
					shooter.setTargetRPM((int)1350);
					//From fender, should hit the backboard
					//Used to be 2000 RPMs
				
					//shooter.Set(0.5);
				
				else
					shooter.setTargetRPM(0);
					 
				 //               else if (mediumspeed)
								//shooter.setTargetRPM((int)0);
		       
					
				 
				 //else if (slowspeed)
								//shooter.setTargetRPM((int)0);
				           
								
				
				
							/*if (targetandspin)									//code for autotargeting and speed will go here
							{
								shooter.setTargetRPM((int)1800);
							}
							else
							{*/
								
							//}
				myRobot.TankDrive(leftYaxis,rightYaxis);
			}	
		//Wait(0.005);
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
