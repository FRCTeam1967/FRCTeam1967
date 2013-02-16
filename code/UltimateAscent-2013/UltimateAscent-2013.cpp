#include "WPILib.h"
#include "jankyRobotTemplate.h"
#include "jankyXboxJoystick.h"
//#include "jankyActuator.h"

#define DRIVE_JOYSTICK_PORT 1
#define GC_JOYSTICK_PORT 2
//#define PISTON_CHANNEL 1
#define COMPRESSOR_RELAY_CHANNEL 5
#define COMPRESSOR_PRESSURE_SWITCH 2
#define LOADING_RELAY_CHANNEL 4

//shooter
#define REAL_CYCLE_TIME 3
#define REAL_ACTUATION_TIME 1.5
#define SHOOTING_SPEED 1.0
#define SHOOTER_TIMER_THRESHOLD 3
#define SHOOTER_MOTOR_ONE_CHANNEL 3
#define SHOOTER_MOTOR_TWO_CHANNEL 4
#define SHOOTER_SOLENOID_CHANNEL 1

//driving system
#define LEFT_AUTONOMOUS 0.2
#define RIGHT_AUTONOMOUS 0.2
#define LEFT_HANGING 0.3
#define RIGHT_HANGING 0.3

//Autonomous 
#define AUTONOMOUS_A  //drive up and ramp up motor simultaneously.
#define AUTONOMOUS_B  //Turn on shooter motor, wait, then shoot. 
#define AUTONOMOUS_TIMER 2.5

/*********************************************************************************************************
 * Team 1967's main robot code for 2013's game Ultimate Ascent. Includes some of our own basic classes:  *
 * JankyRobotTemplate, JankyTask, JankyActuator, JankyXboxJoystick.                                      *
 * Test mode can be used to test the drive motors, piston, and light sensors.                            *
 *********************************************************************************************************/


class UltimateAscent2013 : public JankyRobotTemplate
{
	jankyXboxJoystick gameComponent;
	jankyXboxJoystick driveJoystick;
	//JankyActuator shooterPiston;
	Compressor compressor;
	//Solenoid loadingSolenoid;
	Victor shooterMotorOne; //Talon class DNE
	Victor shooterMotorTwo;
	Solenoid shooterSolenoid;
	Timer onTimer;
	Timer offTimer;

public:
	UltimateAscent2013(void):
		gameComponent(GC_JOYSTICK_PORT),
		driveJoystick(DRIVE_JOYSTICK_PORT),
		//shooterPiston(PISTON_CHANNEL),	//relay channel that the solenoid is connected to
		compressor(COMPRESSOR_PRESSURE_SWITCH,COMPRESSOR_RELAY_CHANNEL),	// (UINT32 pressureSwitchChannel, UINT32 compressorRelayChannel)
		//loadingSolenoid(LOADING_RELAY_CHANNEL),	// relay number - used to actuate the shooter piston
		shooterMotorOne(SHOOTER_MOTOR_ONE_CHANNEL),
		shooterMotorTwo(SHOOTER_MOTOR_TWO_CHANNEL),
		shooterSolenoid(SHOOTER_SOLENOID_CHANNEL)
		
	{

	}

	void RobotInit(void)
	{
		LiveWindow *lw = LiveWindow::GetInstance();

		//lw->AddActuator("Shooting", "Shooter Piston", shooterPiston.GetSolenoid());
		//lw->AddActuator("Shooting", "Shooter Motor One", shooterMotorOne);
		//lw->AddActuator("Shooting", "Shooter Motor Two", shooterMotorTwo);
		//lw->AddActuator("Driving","Left Motor",pL);
		//lw->AddActuator("Driving","Right Motor",pR);

		lw->SetEnabled(true);
		
//TODO Test multiple button functions 
		//(diff scenarios-print outs,etc) & remaining time-release timer
		
		compressor.Start();
	}
	
	
	void Autonomous(void)
	{
		//shooterPiston.SetFullCycleTime(REAL_CYCLE_TIME);
		//shooterPiston.SetActuationTime(REAL_ACTUATION_TIME);
		
		Timer AutonomousTimer;
		AutonomousTimer.Reset();
		AutonomousInit();
		//MUST be called - DO NOT TAKE OUT!
		
#ifdef AUTONOMOUS_A
		
		AutonomousTimer.Start();
		shooterMotorOne.Set(SHOOTING_SPEED);
		shooterMotorTwo.Set(SHOOTING_SPEED);
		
		if (AutonomousTimer.Get()<AUTONOMOUS_TIMER)
		{	
			TankDrive(LEFT_AUTONOMOUS, RIGHT_AUTONOMOUS);
		}
		else if (AutonomousTimer.Get()> AUTONOMOUS_TIMER)
		{
			TankDrive(0.0,0.0);
			shooterSolenoid.Set(true);
			Wait(2.0);
			shooterSolenoid.Set(false);
			Wait(2.0);
			shooterSolenoid.Set(true);
			Wait(2.0);
			shooterSolenoid.Set(false);
			Wait(2.0);
			//shooterPiston.Go();
		}	
		else
		{
			shooterSolenoid.Set(false);
		}	
		
#endif
		
#ifdef AUTONOMOUS_B
		
		AutonomousTimer.Start();
		shooterMotorOne.Set(SHOOTING_SPEED);
		shooterMotorTwo.Set(SHOOTING_SPEED);	
		
		if (AutonomousTimer.Get()>AUTONOMOUS_TIMER)
			//shooterPiston.Go();
			{
				shooterSolenoid.Set(true);
				Wait(2.0);
				shooterSolenoid.Set(false);
				Wait(2.0);
				shooterSolenoid.Set(true);
				Wait(2.0);
				shooterSolenoid.Set(false);
				Wait(2.0);
			}	
		else
			shooterSolenoid.Set(false);
		
#endif	
		
	}
	 


	void OperatorControl(void)
	{
		printf("In Teleop\n");
		
		OperatorControlInit(); 
	//MUST be called - DO NOT TAKE OUT!
		onTimer.Reset();
		offTimer.Reset();
		
		/*shooterPiston.Start();
		shooterPiston.SetFullCycleTime(REAL_CYCLE_TIME);
		shooterPiston.SetActuationTime(REAL_ACTUATION_TIME);
		*/
		
		bool isSpinOn = false;
		bool isFireOn = false;
		
		int iShotsRemaining = 4;
		SmartDashboard::PutNumber("Shots Remaining", iShotsRemaining);
		
		bool spinButton = gameComponent.GetButtonRB();
		bool fireButton = gameComponent.GetButtonX();
		
		//bool isSpinButtonPressed = gameComponent.GetButtonY();
		//bool isFireButtonPressed = gameComponent.GetButtonLB();
		
		while (IsOperatorControl())
		{
			//Safety precaution
			ProgramIsAlive();
			//No need to do waits at the end of teleop because ProgramIsAlive function does a wait. 
		
			
			/*
				Button A: Spin up Shooting Motor
				Button B: 
				Button X: 
				Button Y: Hanging 
				Button LB: Loading
				Button RB: Actuator
				Button LT: 
				Button RT: 
				Button Back: 
				Button Start: 
			*/		  
			//Driving
			TankDrive(driveJoystick.GetLeftYAxis(),driveJoystick.GetRightYAxis());
			
			// Hanging
			bool isButtonYPressed = driveJoystick.GetButtonY();
			if (isButtonYPressed == true)
			{	
				TankDrive(LEFT_HANGING, RIGHT_HANGING);
			}	
			
			//Loading (to block frisbees)
			
			
			//Shooting			
			
		/*	
			if(isSpinButtonPressed)
			{
				printf("Spin Button Pressed\n");
				shooterMotorOne.Set(0.7);
				shooterMotorTwo.Set(0.7);
			}
			else if (!isSpinButtonPressed)
			{
				printf("Spin Button Not Pressed\n");
				shooterMotorOne.Set(0.0);
				shooterMotorTwo.Set(0.0);			
				loadingSolenoid.Set(true);
			}
			else
				loadingSolenoid.Set(false);
			
			if (isFireButtonPressed)
			{
				printf("Fire Button Pressed\n");
				shooterSolenoid.Set(true);
			}
			else
		

			
			bool isButtonRBPressed = stick.GetButtonRB();
			bool isButtonAPressed = stick.GetButtonA();
			
			if (isButtonAPressed == true)

			{	
				printf("Fire Button Not Pressed\n");
				shooterSolenoid.Set(false);
			}
			
		*/	
		
		
			 if(spinButton)
			{
				if(!isSpinOn)
				{
					printf("In Spin Button Loop\n");
					onTimer.Reset();
					onTimer.Start();	
					//For the timing of how long the shooter motor should be on to reach desired motor speed
					
					shooterMotorOne.Set(SHOOTING_SPEED);
					shooterMotorTwo.Set(SHOOTING_SPEED);
					isSpinOn = true;
				}
			} //What happens when it goes back through this loop and isSpinOn is false?
			  //Shooter motor won't be set to the shooting speed we want?
			
			
			if(fireButton)
			{
				if(!isFireOn)
				{
					printf("Fire On\n");
					if(isSpinOn) //This is evaluated if the spinButton is pressed
					{
						printf("Fire and Spin On\n");
						isFireOn = true;
					}
					if(!isSpinOn) //This is evaluated if the spinButton is not pressed
					{
						printf("Fire On, Spin Off\n");
						onTimer.Reset();
						onTimer.Start();
						shooterMotorOne.Set(SHOOTING_SPEED);
						shooterMotorTwo.Set(SHOOTING_SPEED);
						isSpinOn = true;
						isFireOn = true;
					}
				}
				if(offTimer.Get() > SHOOTER_TIMER_THRESHOLD)
				{
					printf("Fire button - Off Timer greater than threshold & stopped\n");
					offTimer.Stop();
					offTimer.Reset();
				}
				if(onTimer.Get() + offTimer.Get() > SHOOTER_TIMER_THRESHOLD)
				{
					printf("Piston Go");
					//shooterPiston.Go();
					/*if(shooterPiston.Go() == true)
					{
						iShotsRemaining--;
					}
					 */	
				}
				if(!spinButton)
				{
					if(offTimer.Get() < SHOOTER_TIMER_THRESHOLD)
					{
						printf("Off timer less than threshold\n");
					}
				}
										
			}
			
			
			else if(!spinButton && !fireButton)
			{
				printf("NOT spin nor fire");
				isSpinOn = false;
				isFireOn = false;
				
				offTimer.Start();
				//For the timing of how long it has been since the buttons were pressed
				
				onTimer.Stop();
				onTimer.Reset();
				
				shooterMotorOne.Set(0.0);
				shooterMotorTwo.Set(0.0);
									
			} 
			
		}
		
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

