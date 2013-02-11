#include "WPILib.h"
#include "jankyRobotTemplate.h"
#include "jankyXboxJoystick.h"
#include "jankyActuator.h"

#define JOYSTICK_PORT 1
#define PISTON_CHANNEL 1
#define COMPRESSOR_RELAY_CHANNEL 3
#define COMPRESSOR_PRESSURE_SWITCH 1
#define LOADING_RELAY_CHANNEL 4

//shooter
#define REAL_CYCLE_TIME 10
#define REAL_ACTUATION_TIME 12
#define SHOOTING_CHANNEL 6

/*
 * Team 1967's main robot code for 2013's game Ultimate Ascent. Includes some of our own basic classes:
 * JankyRobotTemplate, JankyTask, JankyActuator, JankyXboxJoystick. 
 * Test mode can be used to test the drive motors, piston, and light sensors.
 */

class UltimateAscent2013 : public JankyRobotTemplate
{
	jankyXboxJoystick stick;
	//JankyActuator shooterPiston;
	Compressor compressor;
	Solenoid loadingSolenoid;
	Victor shooterMotor;
    
public:
	UltimateAscent2013(void):
		stick(JOYSTICK_PORT),
		//shooterPiston(PISTON_CHANNEL),	//relay channel that the solenoid is connected to
		compressor(COMPRESSOR_PRESSURE_SWITCH,COMPRESSOR_RELAY_CHANNEL),	// (UINT32 pressureSwitchChannel, UINT32 compressorRelayChannel)
		loadingSolenoid(LOADING_RELAY_CHANNEL),	// relay number - used to actuate the shooter piston
		shooterMotor(SHOOTING_CHANNEL)
		
	{
		
	}

	void RobotInit(void)
	{
		LiveWindow *lw = LiveWindow::GetInstance();
		lw->AddActuator("Shooting", "Shooter Motor", &shooterMotor);
		//lw->AddActuator("Shooting", "Shooter Piston", shooterPiston.GetSolenoid());
		lw->SetEnabled(true);
		
		compressor.Start();
	}
	
	void Autonomous(void)
	{
		AutonomousInit();
		//MUST be called - DO NOT TAKE OUT!
		
	}

	void OperatorControl(void)
	{
		OperatorControlInit(); 
		//MUST be called - DO NOT TAKE OUT!
		
		while (IsOperatorControl())
		{
			//Safety precaution
			ProgramIsAlive();
			//No need to do waits at the end of teleop because ProgramIsAlive function does a wait. 
			//Wait(0.005);
			
			//Driving
			TankDrive(stick.GetLeftYAxis(),stick.GetRightYAxis());
			
			//Loading
			bool isButtonAPressed = stick.GetButtonA();
			if(isButtonAPressed == true)
			{
				
			}
			
			//Shooting
			bool isButtonBPressed = stick.GetButtonB();
			if(isButtonBPressed == true)
			{
				//shooterPiston.Start();
				/*shooterPiston.Reset();
				shooterPiston.SetFullCycleTime(REAL_CYCLE_TIME);
				shooterPiston.SetActuationTime(REAL_ACTUATION_TIME);
				shooterPiston.Go();
				shooterPiston.Run();
				*/
			}	
				
			
		}
	}
	
	void Test(void) 
	{
		printf("In Test Mode\n");
		
		while (IsTest())
		{
			ProgramIsAlive();
			pRobot->TankDrive(0.0,0.0); //make function in JRT to GetRobotDrive()
		}
		
	}

};

START_ROBOT_CLASS(UltimateAscent2013);

