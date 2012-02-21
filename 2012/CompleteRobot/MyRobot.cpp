#include "WPILib.h"
#include "stdio.h"
#include "iostream"
#include "math.h"
#include "SmartDashboard/SmartDashboardData.h"
#include <map>
#include <string>
#include "ErrorBase.h"
#include "NetworkTables/NetworkTableChangeListener.h"
#include "SmartDashboard/SmartDashboardNamedData.h"
#include <set>
#include "NetworkTables/NetworkTable.h"
#include "WPIErrors.h"
#include "jankyTurret.h"
#define GYRO_PORT
#define GYRO_Port 2
/***************************************************************************************************
 * This is a demo program showing the use of the RobotBase class.                                  *
 * The SimpleRobot class is the base of a robot application that will automatically call your      *
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on       *
 * the driver station or the field controls.                                                       *
 ***************************************************************************************************/ 
class RobotDemo : public SimpleRobot // Class = Begin
{
	/* Class is the same thing as Begin. Code also beging w/ parenthesis and end w/ parenthesis */
	Victor fr; //front right drive motor
	Victor fl; //front left drive motor
	Victor rr; //rear right drive motor
	Victor rl; //rear left drive motor
	RobotDrive myRobot; // robot drive system   
	Joystick driver;    // driving Joystick for driver
	Joystick gamecomponent; //joystick for game component
	//Jaguar leopard; // motor for the lazy susan
	Jaguar cheetah; // motor for shooting; 
	Jaguar lynx; //motor for elevator
	Jaguar panther; //motor for pushing basketball into turret
	Compressor compressor; // Compsressor for the pneumatics system
	Gyro gyro; //the gyro for use during the balancing
	Solenoid solenoid; // the first solenoid for the piston for the lazy susan
	Solenoid shepard;  // the second solenoid for pushing down the bridge in the endgame
	Encoder Earth; // encoder for measuring speed
	// Encoder encoder; //encoder for measuring distance
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
	JoystickButton Pippin; //TEST BUTTON FOR ANYTHING!!!!
	Relay flashring; //turning on and off flashring
	JankyTurret turret; 
	
public:
	RobotDemo(void):
		fr(2),
		fl(3),
		rr(1),
		rl(4),
		myRobot(fl,rl,fr,rr),	// these must be initialized in the same order (#s in parenthesis refer to port numbers) 3,4,2,1
		driver(1),              // as they are declared above. joystick #1 for driving
		gamecomponent(2),       // joystick #2 for game component
		//leopard(7),	 // Victor 7; for the lazy susan
		cheetah(5),	 //Victor 9; Not actually wired; for the shooter
		lynx(8),     //Victor 8; for elevator
		panther(10), //Victor 10; for pusing ball into turret
		compressor(8, 2), // (UINT32 pressureSwitchChannel, UINT32 compressorRelayChannel)
		gyro(1),          // port #; gyro is analog
		solenoid(5), // relay # = 5
		shepard(6),  // relay # = 6
		Earth(6, 7), //check for a&b; order really does matter
		// encoder(4, 3), // stuffstuffstuff
		//limitswitchright(10), // something about that number
		//limitswitchleft(11),  // something about this number
		bridge(1),
		high(2),
		middle(3), //switch for middle hoop AUTONOMOUS
		twosec(4), //switch for 2 second delay AUTONOMOUS
		fivesec(5), //switch for 5 second DELAY AUTONOMOUS
		Gandalf(&gamecomponent,1),
		Smeagol(&driver, 2),      // 
		Frodo(&gamecomponent, 4), // Frodo is controlled using button #4 on the second joystick
		joystickbutton(&gamecomponent, 7), // joystickbutton is controlled using button #7 on the second joystick
		julesverne(&driver, 4),    // julesverne is controlled using button #4 on the first joystick; I may change this later
		Pippin(&gamecomponent,5),
		flashring(1),
		turret(7,11,10)
	{
		myRobot.SetExpiration(0.1);
	}
	/******************************************************
	 * Drive left & right motors for 2 seconds then stop  *
	 ******************************************************/
	void Autonomous(void)
	{
		//printf("Hello World. Printf\n");
		//fprintf(stderr, "Hello stderr.\n");
		//cerr << "Hello cerr" << endl;
		compressor.Start(); // starts the compressor; though not necessary to start during competition, this means that you will have to start with autonomous during testing
		bool BRIDGE = bridge.Get();
		bool HIGH = high.Get();
		bool MIDDLE = middle.Get();
		bool TWOSEC = twosec.Get();
		bool FIVESEC = fivesec.Get();
		if (BRIDGE = 0, HIGH = 0, MIDDLE = 0, TWOSEC = 0, FIVESEC = 0)
			{
				myRobot.Drive(0.0, 0.0);
				Wait(10.0);
			}
		if (BRIDGE = 1, HIGH = 0, MIDDLE = 0, TWOSEC = 0, FIVESEC = 0)
			{
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);			
			}
		if (BRIDGE = 1, HIGH = 1, MIDDLE = 0, TWOSEC = 0, FIVESEC = 0)
			{
				//Robot aims 
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);			
			}
		if (BRIDGE = 1, HIGH = 1, MIDDLE = 0, TWOSEC = 1, FIVESEC = 0)
			{
				Wait(2.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}
		if (BRIDGE = 1, HIGH = 1, MIDDLE = 0, TWOSEC = 0, FIVESEC = 1)
			{
				Wait(5.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}		
		if (BRIDGE = 1, HIGH = 0, MIDDLE = 1, TWOSEC = 0, FIVESEC = 0)
			{
				//Robot aims 
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);			
			}
		if (BRIDGE = 1, HIGH = 0, MIDDLE = 1, TWOSEC = 1, FIVESEC = 0)
			{
				Wait(2.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}
		if (BRIDGE = 1, HIGH = 0, MIDDLE = 1, TWOSEC = 0, FIVESEC = 5)
			{
				Wait(5.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}				
		if (BRIDGE = 0, HIGH = 1, MIDDLE = 0, TWOSEC = 0, FIVESEC = 0)
			{
				//Robot aims 
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);			
			}
		if (BRIDGE = 0, HIGH = 1, MIDDLE = 0, TWOSEC = 1, FIVESEC = 0)
			{
				Wait(2.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}
		if (BRIDGE = 0, HIGH = 1, MIDDLE = 0, TWOSEC = 0, FIVESEC = 1)
			{
				Wait(5.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}		
		if (BRIDGE = 0, HIGH = 0, MIDDLE = 1, TWOSEC = 0, FIVESEC = 0)
			{
				//Robot aims 
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);			
			}
		if (BRIDGE = 0, HIGH = 0, MIDDLE = 1, TWOSEC = 1, FIVESEC = 0)
			{
				Wait(2.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}
		if (BRIDGE = 0, HIGH = 0, MIDDLE = 1, TWOSEC = 0, FIVESEC = 5)
			{
				Wait(5.0);
				//Robot aims
				//Robot shoots
				myRobot.Drive(-0.5, 0.0);
				Wait(3.0);
			}						
		myRobot.SetSafetyEnabled(false);
	}

	/****************************************
	 * Runs the motors with arcade steering.*
	 ****************************************/
	void OperatorControl(void)
	{
		myRobot.SetSafetyEnabled(true);
		Earth.Start(); // turns on Earth
		// encoder.Start(); // turns on encoder
		SmartDashboard *smarty = SmartDashboard::GetInstance();
		DriverStationLCD *dslcd = DriverStationLCD::GetInstance(); // don't press SHIFT 5 times; this line starts up driver station messages (in theory)
		char debugout [100];
		compressor.Start();
		//SmartDashboard *superdash = SmartDashboard::GetInstance(); // starts up smart dashboard instances (in theory)
		//superdash->PutString("KeyName-Here","Hello World");  // what about those strings yeaaah
		gyro.Reset(); // resets gyro angle
	//	int count = 0; // makes it start at zero

		while (IsOperatorControl()) // while is the while loop for stuff; this while loop is for "while it is in Teleop"
		{ 
			//count++;
			float leftYaxis = driver.GetY();
			float rightYaxis = driver.GetTwist();//RawAxis(5);
			myRobot.TankDrive(leftYaxis,rightYaxis); // drive with arcade style (use right stick)for joystick 1
			Wait(0.005);				// wait for a motor update time ORIGINALLY 0.005
			//float shooter = gamecomponent.GetY();
			float random = gamecomponent.GetY();
			float lazysusan = gamecomponent.GetZ();
			// Make deadband.
			//if (lazysusan < 0.05 && lazysusan > -0.05)
			//	lazysusan = 0;
			bool elevator = Frodo.Get();
			//bool tangledwires = limitswitchright.Get();
			//bool mixedwires = limitswitchleft.Get();
			float angle = gyro.GetAngle();
			bool balance = Smeagol.Get();
			smarty->PutDouble("Gyro Value",angle);
			// bool statusofjoystick = Frodo.Get();
			bool light = Pippin.Get();
			float speed = Earth.GetRate();
			//float TARGETspeed = 500.0;
			//float NEWESTspeed = leopard.Get();
			//float gravity = 39.3700787;
			//int LaunchAngle = 60;
			// int LaunchHeight = 45;
			// float Velocity = (sqrt(gravity/(distance*(tan(LaunchAngle)) - HoopHeight + LaunchHeight)))/(sqrt(2)*cos(LaunchAngle));
			//float aspeed = TARGETspeed - speed;
			//float YUPspeed = NEWESTspeed + 0.0025*aspeed;
			//float dspeed = speed - TARGETspeed;
			//float newspeed = NEWESTspeed - 0.005*dspeed;
			//Wait(1.0);	        // wait for a motor update time-not a "tight loop"
			turret.Set(lazysusan);	// the lazy susan would turn right & left based on how far the person moves the right joystick#2 side to side
			//cheetah.Set(shooter);   //shooter would move forward and backwards based on how far the person moves the left joystick#2 up & down
			lynx.Set(random);
			//if (elevator)           //elevator would move at full speed if button is pressed
			//	lynx.Set(-0.2);
			//	else
			//		lynx.Set(0.0);
			if (elevator)           //shooter would move at full speed if button is pressed
				cheetah.Set(1.0);
			else
				cheetah.Set(0.0);
			
			if (balance)						// this is the start of the balancing code
			{
				angle = gyro.GetAngle();
				myRobot.Drive(-0.03*angle, 0.0);
				Wait(0.005);
			}
			if (light)
			     flashring.Set(Relay::kForward);
			     else
			            flashring.Set(Relay::kOff);
			/*
			if (tangledwires)					// this is the start of the limit switch code; 10
			{
				//leopard.Set(0.0);
				if (lazysusan<0.0)
				{
					leopard.Set(lazysusan);
				}
				else
					leopard.Set(0.0);
			}	
		   if (mixedwires)					// this is the start of the limit switch code; 10
			{
				//leopard.Set(0.0);
				if (lazysusan>0.0)
				{
					leopard.Set(lazysusan);
				}
				else
					leopard.Set(0.0);
			}	
			*/
			//if (statusofjoystick)		 			// this is the start of the sameple encoder code
				//lynx.Set(0.5);
		//	else
		//		cheetah.Set(0);
		/*if (testbutton)								// this is the start of moving the shooting motors
		{
			if (speed < TARGETspeed)
			{
				NEWESTspeed = cheetah.Get();
				aspeed = TARGETspeed - speed;
				YUPspeed = NEWESTspeed + 0.00025*aspeed; //originally 0.0025
				cheetah.Set(YUPspeed);
			}
			else
			{
				NEWESTspeed = cheetah.Get();
				dspeed = speed - TARGETspeed;
				newspeed = NEWESTspeed - 0.00025*dspeed; //originally 0.005
				cheetah.Set(newspeed); // 
		
			}
		}*/
		//	while (statusofjoystick)
		//	{
		//		if((TARGETspeed+100.0)> speed > (TARGETspeed-100.0))
		//      solenoid.Set(true); // then the first solenoid is on
		//      Wait(0.5); // and then the first solenoid waits for 0.5 seconds
		//      solenoid.Set(false); //and then the first solenoid turns off
		//	}
			dslcd->Clear();
					sprintf(debugout,"Number=%f",leftYaxis); 
					dslcd->Printf(DriverStationLCD::kUser_Line2,2,debugout);
					sprintf(debugout,"Number=%f",speed);
					dslcd->Printf(DriverStationLCD::kUser_Line3,3,debugout);
			//		sprintf(debugout,"Number=%f",speed);
			//		dslcd->Printf(DriverStationLCD::kUser_Line4,4,debugout);	
		//	dslcd->Printf(DriverStationLCD::kUser_Line5,5,debugout); // "prints" driver station the driver station
			dslcd->UpdateLCD(); // update the Driver Station with the information in the code
			
		    // sprintf(debugout,"Number=%u",maxi);
			// dslcd->Printf(DriverstationLCD::kUser_Line6,5,debugout)
					
		//	float angle2 = gyro.GetAngle(); // get the angle that the gyro registers onto the Driver Station
			
		//	superdash->PutDouble("Gyro Angle", angle2);
			//superdash->PutInt("Iterations", count);
			
			if (joystickbutton.Get()==true) // if joystick button 2 is pressed (is true)
			{	
				solenoid.Set(true); // then the first solenoid is on
				Wait(0.5); // and then the first solenoid waits for 0.5 seconds
				solenoid.Set(false); //and then the first solenoid turns off
			}
			if (julesverne.Get()==true) // if joystick button 4 is pressed (is true)
			{
				shepard.Set(true); // then shepard is on the run
				Wait(0.5); // and shepard waits for 0.5 seconds
				shepard.Set(false); // and then shepard turns off
			} //10.19.67.9 IP address of computer;255.0.0.0 subnet mask ALL FOR WIRELESS CONNECTION #2
		}
	}
};

START_ROBOT_CLASS(RobotDemo);
