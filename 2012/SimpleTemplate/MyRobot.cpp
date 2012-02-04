#include "WPILib.h"
#include "stdio.h"
#include "iostream"
/***************************************************************************************************
 * This is a demo program showing the use of the RobotBase class.                                  *
 * The SimpleRobot class is the base of a robot application that will automatically call your      *
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on       *
 * the driver station or the field controls.                                                       *
 ***************************************************************************************************/ 
class RobotDemo : public SimpleRobot // Class = Begin
{
	/* Class is the same thing as Begin. Code also beging w/ parenthesis and end w/ parenthesis */
	RobotDrive myRobot; // robot drive system   
	Joystick stick; // driving Joystick for teleop control
	Joystick controller; //joystick for game component
	Jaguar lynx; //jaguar or motor for the elevator
	Jaguar leopard; // motor for the lazy susan
	Jaguar cheetah; // motor for shooting; NOT ACTUALLY WIRED
	Compressor compressor; // Compsressor for the pneumatics system
	Gyro gyro; //the gyro for use during the balancing
	Gyro grayson; //the gyro for the turret
	Solenoid solenoid; // the first solenoid for the piston for the lazy susan
	Solenoid shepard; // the second solenoid for pushing down the bridge in the endgame
	Encoder Earth; // encoder for measuring speed
	// Encoder encoder; //encoder for measuring distance
	DigitalInput limitswitchright; // limit switch for the right side of the turret
	DigitalInput limitswitchleft; // limit switch for the left side of the turret
	JoystickButton Smeagol; //button for balancing
	JoystickButton Frodo; // button for moving the shooting motor
	JoystickButton joystickbutton; // button for the piston for the lazy susan
	JoystickButton julesverne; // button for pushing the bridge down during the endgame for points
	

public:
	RobotDemo(void):
		myRobot(3, 1, 4, 2),	// these must be initialized in the same order (#s in parenthesis refer to port numbers)
		stick(1), // as they are declared above. joystick #1 for driving
		controller(2), // joystick #2 for game component
		lynx(5), // Jaguar 5; for the elevator; these will be changed to victors later.
		leopard(7),	// Jaguar 7; for the lazy susan
		cheetah(9),	 //Jaguar 9; Not actually wired; for the shooter
		compressor(4, 2), // (UINT32 pressureSwitchChannel, UINT32 compressorRelayChannel)
		gyro(1), // port #; gyro is analog
		grayson(9), //
		solenoid(5), // relay # = 5
		shepard(6), // relay # = 6
		Earth(3, 2), //check for a&b; order really does matter
		// encoder(4, 3), // stuffstuffstuff
		limitswitchright(10), // something about that number
		limitswitchleft(11), // something about this number
		Smeagol(&stick, 2), // 
		Frodo(&controller, 4), // Frodo is controlled using button #4 on the second joystick
		joystickbutton(&stick, 5), // joystickbutton is controlled using button #5 on the first joystick
		julesverne(&stick, 6) // julesverne is controlled using button #6 on the first joystick; I may change this later
	{
		myRobot.SetExpiration(0.1);
	}
	/******************************************************
	 * Drive left & right motors for 2 seconds then stop  *
	 ******************************************************/
	void Autonomous(void)
	{
		printf("Hello World. Printf\n");
		fprintf(stderr, "Hello stderr.\n");
		cerr << "Hello cerr" << endl;
		compressor.Start(); // starts the compressor; though not necessary to start during competition, this means that you will have to start with autonomous during testing
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
		
		SmartDashboard *superdash = SmartDashboard::GetInstance(); // starts up smart dashboard instances (in theory)
		superdash->PutString("KeyName-Here","Hello World");  // what about those strings yeaaah
		
		
		gyro.Reset(); // resets gyro angle
		int count = 0; // makes it start at zero
		
		while (IsOperatorControl()) // while is the while loop for stuff; this while loop is for "while it is in Teleop"
		{ 
			count++;
			myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)for joystick 1
			float elevator = controller.GetY();
			float lazysusan = controller.GetZ();
			bool tangledwires = limitswitchright.Get();
			bool mixedwires = limitswitchleft.Get();
			float angle = gyro.GetAngle();
			bool balance = Smeagol.Get();
			smarty->PutDouble("Gyro Value",angle);
			bool statusofjoystick = Frodo.Get();
			float speed = Earth.GetRate();
			float TARGETspeed = 500.0;
			float NEWESTspeed = leopard.Get();
			float aspeed = TARGETspeed - speed;
			float YUPspeed = NEWESTspeed + 0.0025*aspeed;
			float dspeed = speed - TARGETspeed;
			float newspeed = NEWESTspeed - 0.005*dspeed;
			Wait(1.0);	// wait for a motor update time-not a "tight loop"
			lynx.Set(lazysusan);	// the lazy susan would turn right & left based on how far the person moves the right joystick#2 side to size
			leopard.Set(elevator); // The elevator would move up & down based on how far the person moves the left joystick#2 up or down 
			if (balance);
			{
				angle = gyro.GetAngle();
				myRobot.Drive(-0.03*angle, 0.0);
				Wait(0.005);
			}
			if (tangledwires)
				cheetah.Set(0.0);
			if (mixedwires)
				cheetah.Set(0.0);
			if (statusofjoystick)
				cheetah.Set(1);
			else
				cheetah.Set(0);
			if (speed < TARGETspeed)
			{
				NEWESTspeed = leopard.Get();
				aspeed = TARGETspeed - speed;
				YUPspeed = NEWESTspeed + 0.00025*aspeed; //originally 0.0025
				leopard.Set(YUPspeed);
			}
			else
			{
				NEWESTspeed = leopard.Get();
				dspeed = speed - TARGETspeed;
				newspeed = NEWESTspeed - 0.00025*dspeed; //originally 0.005
				leopard.Set(newspeed); // 
			}
			
			dslcd->Clear();
			
					sprintf(debugout,"Number=%f",aspeed);
					dslcd->Printf(DriverStationLCD::kUser_Line2,2,debugout);
					sprintf(debugout,"Number=%f",dspeed);
					dslcd->Printf(DriverStationLCD::kUser_Line3,3,debugout);
					sprintf(debugout,"Number=%f",speed);
					dslcd->Printf(DriverStationLCD::kUser_Line4,4,debugout);
			
			
			dslcd->Printf(DriverStationLCD::kUser_Line5,5,debugout); // "prints" driver station the driver station
			dslcd->UpdateLCD(); // update the Driver Station with the information in the code
			// sprintf(debugout,"Number=%u",maxi);
			// dslcd->Printf(DriverstationLCD::kUser_Line6,5,debugout)
					
			float angle2 = gyro.GetAngle(); // get the angle that the gyro registers onto the Driver Station
			
			superdash->PutDouble("Gyro Angle", angle2);
			superdash->PutInt("Iterations", count);
			
			if (joystickbutton.Get()==true) // if joystick button 5 is pressed (is true)
			{				
				solenoid.Set(true); // then the first solenoid is on
				Wait(0.5); // and then the first solenoid waits for 0.5 seconds
				solenoid.Set(false); //and then the first solenoid turns off
			}
			if (julesverne.Get()==true) // if joystick button 6 is pressed (is true)
			{
				shepard.Set(true); // then shepard is on the run
				Wait(0.5); // and shepard waits for 0.5 seconds
				shepard.Set(false); // and then shepard turns off
			}
			Wait(0.005);				// wait for a motor update time
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

