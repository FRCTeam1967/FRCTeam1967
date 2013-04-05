#include "WPILib.h"
#include "jankyRobotTemplate.h"
#include "jankyXboxJoystick.h"
#include "jankyActuator.h"

#define DRIVE_JOYSTICK_PORT 1
#define GC_JOYSTICK_PORT 2
#define PISTON_ONE_CHANNEL 1
#define PISTON_TWO_CHANNEL 2
#define BLOCKER_PISTON_ONE_CHANNEL 3
#define BLOCKER_PISTON_TWO_CHANNEL 4
#define COMPRESSOR_RELAY_CHANNEL 3
#define COMPRESSOR_PRESSURE_SWITCH 2
//#define LOADING_RELAY_CHANNEL 4

//shooter
#define SHOOTING_SPEED 1.0
#define REAL_CYCLE_TIME 1.5
#define REAL_ACTUATION_TIME 0.5
#define LOW_SHOOTING_SPEED 0.7
#define LOW_REAL_CYCLE_TIME 1.0
#define LOW_ACTUATION_TIME 0.5
#define LOW_BUTTON_TIMER 1.0

#define SHOOTER_TIMER_THRESHOLD 3
#define SHOOTER_MOTOR_ONE_CHANNEL 3 
#define SHOOTER_MOTOR_TWO_CHANNEL 4
#define SHOOTER_SOLENOID_CHANNEL 1

//driving system
#define LEFT_AUTONOMOUS 0.3
#define RIGHT_AUTONOMOUS 0.3
#define LEFT_HANGING -1.0
#define RIGHT_HANGING -1.0

#define AUTONOMOUS_SPIN_UP_TIME 3.0
#define AUTONOMOUS_MEDIUM_DRIVE_TIMER 2.0
#define AUTONOMOUS_SHOOT_SPEED 1.0
#define AUTONOMOUS_LOFT_SPEED 0.55

/*********************************************************************************************************
 * Team 1967's main robot code for 2013's game Ultimate Ascent. Includes some of our own basic classes:  *
 * JankyRobotTemplate, JankyTask, JankyActuator, JankyXboxJoystick.                                      *
 * Test mode can be used to test the drive motors, piston, and light sensors.                            *
 *********************************************************************************************************/

class UltimateAscent2013 : public JankyRobotTemplate
{
	jankyXboxJoystick * gameComponent;
	jankyXboxJoystick * driveJoystick;
	JankyActuator * shooterPiston;
	Solenoid * blockerPistonOne;
	Solenoid * blockerPistonTwo;
	Compressor * compressor;
	Victor * shooterMotorOne; //Talon class DNE
	Victor * shooterMotorTwo;
	//Solenoid * shooterSolenoid;
	Timer * onTimer;
	Timer * offTimer;
	Timer * lowTimer;
	Timer * AutonomousTimer;
	SendableChooser * ChooseAutonomousMode;
	const char * h;			//Normal positioning next to the pyramid - robot angled to shoot high
	const char * m;			//Normal positioning next to the pyramid - robot straight to shoot medium
	const char * loftH; 	//Positioned next to back pole of pyramid and angled to shoot high
							//Will the pyramid pole's block the frisbees?
	const char * loftM; 	//Positioned next to back pole of pyramid and straight to shoot medium
	const char * driveM;	//Angled against back pole of pyramid and drive to shoot medium
	const char * driveH;	//Angled against back pole of pyramid and drive to shoot high

public:
	UltimateAscent2013(void)
	/*	
		gameComponent(GC_JOYSTICK_PORT),
		driveJoystick(DRIVE_JOYSTICK_PORT),
		shooterPiston(PISTON_CHANNEL),	//relay channel that the solenoid is connected to
		compressor(COMPRESSOR_PRESSURE_SWITCH,COMPRESSOR_RELAY_CHANNEL),	// (UINT32 pressureSwitchChannel, UINT32 compressorRelayChannel)
		shooterMotorOne(SHOOTER_MOTOR_ONE_CHANNEL),
		shooterMotorTwo(SHOOTER_MOTOR_TWO_CHANNEL),
		shooterSolenoid(SHOOTER_SOLENOID_CHANNEL)
	*/	
	{
		gameComponent = NULL;
		driveJoystick = NULL;
		shooterPiston = NULL;
		blockerPistonOne = NULL;
		blockerPistonTwo = NULL;
		compressor = NULL;
		shooterMotorOne = NULL; //Talon class DNE
		shooterMotorTwo = NULL;
		//shooterSolenoid = NULL;
		onTimer = NULL;
		offTimer = NULL;
		lowTimer = NULL;
		AutonomousTimer = NULL;
		ChooseAutonomousMode = NULL;

		h = "High Goal";
		m = "Medium Goal";
		driveM = "Drive & Medium Goal";
		driveH = "Drive & High Goal";
		loftM = "Loft Medium Goal";
		loftH = "Loft High Goal";
	}

	~UltimateAscent2013(void)
{
	delete gameComponent;
	delete driveJoystick;
	delete shooterPiston;
	delete blockerPistonOne;
	delete blockerPistonTwo;
	delete compressor;
	delete shooterMotorOne;
	delete shooterMotorTwo;
	//delete shooterSolenoid;
	delete onTimer;
	delete offTimer;
	delete lowTimer;
	delete AutonomousTimer;
	delete ChooseAutonomousMode;
}
	
	void RobotInit(void)
	{
		printf("RobotInit\n");
		
		if(IsTest())
		{
			printf("RobotInit:: Test mode found. Adding LiveWindow items.\n");
			LiveWindow *lw = LiveWindow::GetInstance();

			//lw->AddActuator("Shooting", "Shooter Piston", shooterPiston.GetSolenoid());
			//lw->AddActuator("Shooting", "Shooter Motor One", shooterMotorOne);
			//lw->AddActuator("Shooting", "Shooter Motor Two", shooterMotorTwo);
			//lw->AddActuator("Driving","Left Motor",pL);
			//lw->AddActuator("Driving","Right Motor",pR);

			lw->SetEnabled(true);
		}

		Wait (2.0);

		gameComponent = new jankyXboxJoystick(GC_JOYSTICK_PORT);
		driveJoystick = new jankyXboxJoystick(DRIVE_JOYSTICK_PORT);
		shooterPiston = new JankyActuator(PISTON_ONE_CHANNEL, PISTON_TWO_CHANNEL);
		blockerPistonOne = new Solenoid(BLOCKER_PISTON_ONE_CHANNEL);
		blockerPistonTwo = new Solenoid(BLOCKER_PISTON_TWO_CHANNEL);
		compressor = new Compressor(COMPRESSOR_PRESSURE_SWITCH,COMPRESSOR_RELAY_CHANNEL);
		shooterMotorOne = new Victor(SHOOTER_MOTOR_ONE_CHANNEL);
		shooterMotorTwo = new Victor(SHOOTER_MOTOR_TWO_CHANNEL);
		//shooterSolenoid = new Solenoid(SHOOTER_SOLENOID_CHANNEL);

		onTimer = new Timer;	//this is activated when shooter motor is spinning
		offTimer = new Timer;
		
		/*when fire button is pressed and spin button is not && both buttons not pressed. 
		offTimer is useful in the case that the person lifts the button
		*/
		
		lowTimer = new Timer;
		AutonomousTimer = new Timer;
		ChooseAutonomousMode = new SendableChooser;

		SmartDashboard::PutString("Status","Choose Autonomous Mode");
		
		ChooseAutonomousMode->AddDefault(m, ((void*)m));
		ChooseAutonomousMode->AddObject(h, ((void*)h));
		ChooseAutonomousMode->AddObject(driveM, ((void*)driveM));
		ChooseAutonomousMode->AddObject(driveH, ((void*)driveH));
		ChooseAutonomousMode->AddObject(loftM, ((void*)loftM));
		ChooseAutonomousMode->AddObject(loftH, ((void*)loftH));
		
		SmartDashboard::PutData("Autonomous Modes", ChooseAutonomousMode);

		compressor->Start();
		JankyRobotTemplate::RobotInit();
	}
	
	void Autonomous(void)
	{
		printf("In Autonomous\n");
		shooterPiston->Start();
		shooterPiston->SetFullCycleTime(REAL_CYCLE_TIME);
		shooterPiston->SetActuationTime(REAL_ACTUATION_TIME);
		
		AutonomousTimer->Reset();
		
		AutonomousInit();
		ProgramIsAlive();
		//MUST be called - DO NOT TAKE OUT!

		SmartDashboard::PutString("Status","In Autonomous");
		
		bool RunMedium = false;
		bool RunHigh = false;
		bool RunDriveMedium = false;
		bool RunDriveHigh = false;
		bool RunLoftMedium = false;
		bool RunLoftHigh = false;
		
		int counter = 0;

		
		//Choosing which autonomous mode from SmartDashboard
		if(ChooseAutonomousMode->GetSelected() == ((void*)m))
		{
			RunMedium = true;
			printf("Autonomous-shooting at medium goal\n");
		}
		if(ChooseAutonomousMode->GetSelected() == ((void*)h))
		{
			RunHigh = true;
			printf("Autonomous-shooting at high goal\n");
		}
		
		if(ChooseAutonomousMode->GetSelected() == ((void*)driveM))
		{
			RunDriveMedium = true;
			printf("Autonomous-driving & shooting at medium goal\n");
		}
		if(ChooseAutonomousMode->GetSelected() == ((void*)driveH))
		{
			RunDriveHigh = true;
			printf("Autonomous-driving & shooting at high goal\n");
		}
		
		if(ChooseAutonomousMode->GetSelected() == ((void*)loftH))
		{
			RunLoftHigh = true;
			printf("Autonomous-lofting at high goal\n");
		}
		if(ChooseAutonomousMode->GetSelected() == ((void*)loftM))
		{
			RunLoftMedium = true;
			printf("Autonomous-lofting at medium goal\n");
		}
		
		
		//Different autonomous run modes
		if(RunMedium)
		{
			AutonomousTimer->Start();
			while(IsAutonomous())
			{
				ProgramIsAlive();
				shooterMotorOne->Set(AUTONOMOUS_SHOOT_SPEED);
				shooterMotorTwo->Set(AUTONOMOUS_SHOOT_SPEED);
				
				if (AutonomousTimer->Get() > AUTONOMOUS_SPIN_UP_TIME)
				{
					if(counter <= 3 && shooterPiston->Go() == true)
					{
						counter++;
					}
					else
					{
						printf("Setting motors to zero in autonomous else\n");
						shooterMotorOne->Set(0.0);
						shooterMotorTwo->Set(0.0);
					}
				}
			}
		}
		else if(RunDriveMedium)
		{
			AutonomousTimer->Start();
			while(IsAutonomous())
			{
				ProgramIsAlive();
				shooterMotorOne->Set(AUTONOMOUS_SHOOT_SPEED);
				shooterMotorTwo->Set(AUTONOMOUS_SHOOT_SPEED);
				
				if (AutonomousTimer->Get()<AUTONOMOUS_MEDIUM_DRIVE_TIMER)
				{	
					TankDrive(LEFT_AUTONOMOUS, RIGHT_AUTONOMOUS);
				}
				else if (AutonomousTimer->Get()> AUTONOMOUS_MEDIUM_DRIVE_TIMER)
				{
					if(counter <= 4 && shooterPiston->Go() == true)
					{
						counter++;
					}
					else
					{
						printf("Setting motors to zero in autonomous else\n");
						shooterMotorOne->Set(0.0);
						shooterMotorTwo->Set(0.0);
					}
				}	
			}
		}
		else if(RunLoftMedium)
		{
			AutonomousTimer->Start();
			while(IsAutonomous())
			{
				ProgramIsAlive();
				shooterMotorOne->Set(AUTONOMOUS_LOFT_SPEED);
				shooterMotorTwo->Set(AUTONOMOUS_LOFT_SPEED);
				
				if (AutonomousTimer->Get() > AUTONOMOUS_SPIN_UP_TIME)
				{
					if(counter <= 4 && shooterPiston->Go() == true)
					{
						counter++;
					}
					else
					{
						printf("Setting motors to zero in autonomous else\n");
						shooterMotorOne->Set(0.0);
						shooterMotorTwo->Set(0.0);
					}
				}
			}
		}
		
		else if(RunHigh)
		{
			AutonomousTimer->Start();
			while(IsAutonomous())
			{
				ProgramIsAlive();
				shooterMotorOne->Set(AUTONOMOUS_SHOOT_SPEED);
				shooterMotorTwo->Set(AUTONOMOUS_SHOOT_SPEED);
				
				if (AutonomousTimer->Get() > AUTONOMOUS_SPIN_UP_TIME)
				{
					if(counter <= 3 && shooterPiston->Go() == true)
					{
						counter++;
					}
					else
					{
						printf("Setting motors to zero in autonomous else\n");
						shooterMotorOne->Set(0.0);
						shooterMotorTwo->Set(0.0);
					}
				}
			}
		}
		else if(RunDriveHigh)
		{
			AutonomousTimer->Start();
			while(IsAutonomous())
			{
				ProgramIsAlive();
				shooterMotorOne->Set(AUTONOMOUS_SHOOT_SPEED);
				shooterMotorTwo->Set(AUTONOMOUS_SHOOT_SPEED);
				
				if (AutonomousTimer->Get() < 2.2)
				{	
					TankDrive(-0.6, -0.6);
				}
				else if (AutonomousTimer->Get() < 2.4)
				{
					TankDrive(0.0, -0.55);
				}
				else if (AutonomousTimer->Get() > 4.0)
				{
					TankDrive(0.0,0.0);
					if(counter <= 4 && shooterPiston->Go() == true)
					{
						counter++;
					}
					else
					{
						printf("Setting motors to zero in autonomous else\n");
						shooterMotorOne->Set(0.0);
						shooterMotorTwo->Set(0.0);
					}
				}	
			}
		}
		else if(RunLoftHigh)
		{
			AutonomousTimer->Start();
			while(IsAutonomous())
			{
				ProgramIsAlive();
				shooterMotorOne->Set(AUTONOMOUS_LOFT_SPEED);
				shooterMotorTwo->Set(AUTONOMOUS_LOFT_SPEED);
				
				if (AutonomousTimer->Get() > AUTONOMOUS_SPIN_UP_TIME)
				{
					if(counter <= 4 && shooterPiston->Go() == true)
					{
						counter++;
					}
					else
					{
						printf("Setting motors to zero in autonomous else\n");
						shooterMotorOne->Set(0.0);
						shooterMotorTwo->Set(0.0);
					}
				}
			}
		}
		else
		{
			printf("No autonomous mode chosen\n");
		}
	}
	


	void OperatorControl(void)
	{
		printf("In Teleop\n");

		OperatorControlInit();
		//MUST be called - DO NOT TAKE OUT!
		printf("OperatorControlInit called\n");

		SmartDashboard::PutString("Status","In Operator Control");
				
		onTimer->Reset();
		offTimer->Reset();
		lowTimer->Reset();
		
		shooterPiston->Start();
		shooterPiston->SetFullCycleTime(REAL_CYCLE_TIME);
		shooterPiston->SetActuationTime(REAL_ACTUATION_TIME);
		
		bool isSpinOn = false;
		bool isFireOn = false;
		bool isLowOn = false;
		
		//bool isBlockOn = false;
		
		int iShotsRemaining = 4;
		
			
		printf("Entering while loop\n");
		SmartDashboard::PutString("Status","Entering Teleop loop");
		
		while (IsOperatorControl())
		{
			//Safety precaution
			ProgramIsAlive();
			//No need to do waits at the end of teleop because ProgramIsAlive function does a wait. 

			/*************
			*   Driving	 *			
			**************/
			int testButton = driveJoystick->GetButtonRB();
			SmartDashboard::PutNumber("Teleop Test Button", testButton);
			TankDrive(driveJoystick->GetLeftYAxis(),driveJoystick->GetRightYAxis());
			
			
			
			
			
			/*************
			*   Hanging	 *			
			**************/
			bool isButtonYPressed = driveJoystick->GetButtonY();
			if (isButtonYPressed == true)
			{	
				//TankDrive(LEFT_HANGING, RIGHT_HANGING);
				TankDrive(0.8, 0.8);
			}	
			
			
			
			/*************
			 * Blocking	 *
			*************/
			bool blockButtonUp = driveJoystick->GetButtonRB();
			bool blockButtonDown = driveJoystick->GetButtonLB();
			
			if (blockButtonUp)
			{
				blockerPistonOne->Set(true);
				blockerPistonTwo->Set(false);
			}
			if (blockButtonDown)
			{
				blockerPistonOne->Set(false);
				blockerPistonTwo->Set(true);
			}
			
			/*
			if (blockButton)
			{
				if (!isBlockOn)
				{
					blockerPistonOne->Set(true);
					blockerPistonTwo->Set(false);
					isBlockOn = true;
				}
				else if (isBlockOn)
				{
					blockerPistonOne->Set(false);
					blockerPistonTwo->Set(true);
					isBlockOn = false;
				}
			}
			else if(!blockButton && )
			{
				printf("Block button not being pressed\n");
			}
			*/
			
			
			
			/*************
			*  Shooting	 *			
			**************/
			 bool spinButton = gameComponent->GetButtonA();
			 bool fireButton = gameComponent->GetButtonRB();
			 bool lowButton = gameComponent->GetButtonY();
			 bool pistonButton = gameComponent->GetButtonLB();
			 
			 if (pistonButton)
				 shooterPiston->Go();
			 
			 if (lowButton)
			 {
				 
				 shooterPiston->SetFullCycleTime(LOW_REAL_CYCLE_TIME);
				 shooterPiston->SetActuationTime(LOW_ACTUATION_TIME);
				 if(!isLowOn)
				 {
					 printf("In Low Button Loop\n");
					 isLowOn = true;
					 lowTimer->Reset();
					 lowTimer->Start();
					 shooterMotorOne->Set(LOW_SHOOTING_SPEED);
					 shooterMotorTwo->Set(LOW_SHOOTING_SPEED);
				 }
				 if (lowTimer->Get() > LOW_BUTTON_TIMER)
				 {
					 shooterPiston->Go(); 
				 }
			 }
			 else if (isLowOn && !lowButton) 
			 {	 
				//printf("In low else statement\n");
				lowTimer->Stop();
				isLowOn = false;
				shooterMotorOne->Set(0.0);
				shooterMotorTwo->Set(0.0);
			 }

			 SmartDashboard::PutNumber("Shots Remaining", iShotsRemaining);
			 if(spinButton) 
			 {
				if(isSpinOn == false)
				{
					printf("In Spin Button Loop\n");
					onTimer->Reset();
					onTimer->Start();	
					//For the timing of how long the shooter motor should be on to reach desired motor speed
					
					shooterMotorOne->Set(SHOOTING_SPEED);
					shooterMotorTwo->Set(SHOOTING_SPEED);
					isSpinOn = true; //NOT TRUE-WHY?-if spinButton pressed, and if isSpin on is false-only then will shooterMotor run-B/C once its set-its set! no else statement
				}
			
			}
					
			  //What happens when it goes back through this loop and isSpinOn is false?
			  //Shooter motor won't be set to the shooting speed we want?
			
			if(fireButton)
			{
				if(!isFireOn) //isFireOn == false
				{
					printf("Fire On\n");
					
					if(isSpinOn) //This is evaluated if the spinButton is pressed-BOTH BUTTONS PRESSED
					{
						printf("Fire and Spin On\n");
						isFireOn = true;
					}			
					if(!isSpinOn) //This is evaluated if the spinButton is not pressed-ONLY FIREBUTTON
					{
						printf("Fire On, Spin Off\n");
						onTimer->Reset();
						onTimer->Start();
						shooterMotorOne->Set(SHOOTING_SPEED);
						shooterMotorTwo->Set(SHOOTING_SPEED);
						isSpinOn = true;
						isFireOn = true;
					}
				}
				if (onTimer->Get() > SHOOTER_TIMER_THRESHOLD) //piston activation is dependent on time that the shooter motor is spinning
				{
					printf("Piston Go");
					shooterPiston->Go();
				}
				if(offTimer->Get() > SHOOTER_TIMER_THRESHOLD) //if the offTimer is longer for certain time, reset-shooting system not being used
				{
					printf("Fire button - Off Timer greater than threshold & stopped\n");
					offTimer->Stop();
					offTimer->Reset();
				}		
				if(onTimer->Get() + offTimer->Get() > SHOOTER_TIMER_THRESHOLD) //offTimer and onTimer are used to activate shooting piston
				{
					printf("Piston Go");
					shooterPiston->Go();
				}
				if(!spinButton)
				{
					if(offTimer->Get() < SHOOTER_TIMER_THRESHOLD)
					{
						printf("Off timer less than threshold\n");
					}
				}
										
			} //end of if (fire button)
			
			else if(!spinButton && !fireButton) //when BOTH buttons are not pressed-edge transition
			{
				if (isSpinOn == true || isFireOn == true) //one mode is changed faster than the other mode
				{	
					printf("NOT spin nor fire\n");
				
					isSpinOn = false;
					isFireOn = false;
				
					offTimer->Reset();
					offTimer->Start();
					//For the timing of how long it has been since the buttons were pressed
				
					onTimer->Stop();
					onTimer->Reset();
				
					shooterMotorOne->Set(0.0);
					shooterMotorTwo->Set(0.0);
				}
									
			} 
			
		}
		SmartDashboard::PutString("Status","Left Teleop loop");
		
	}

	void Test(void) 
	
	{
		printf("In Test Mode\n");
		
		while (IsTest())
		{
			ProgramIsAlive();
			TankDrive(0.0,0.0); //make function in JRT to GetRobotDrive()
		}
		
	}
	
};

START_ROBOT_CLASS(UltimateAscent2013);

