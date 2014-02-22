#include "WPILib.h"
#include "jankyRobotTemplate.h"
#include "jankyXboxJoystick.h"
#include "jankyTask.h"
#include "jankyEncoder.h"
#include "jankyActuator.h"
#include "jankyKickerState.h"
#include "jankyPickupState.h"
#include "jankyAutonomousState.h"

#define DRIVE_JOYSTICK_PORT 1
#define GC_JOYSTICK_PORT 2

class AerialAssist2014 : public JankyRobotTemplate
{
	Joystick * joystick;
	jankyXboxJoystick * gameComponent; 
	JankyKickerState * kicker;
	JankyPickupState * pickup;
	JankyAutonomousState * autonomous;
	
public:
	AerialAssist2014(void)
	{
		joystick = NULL;
		gameComponent = NULL;
		kicker = NULL;
		pickup = NULL;
		autonomous = NULL;
	}

	~AerialAssist2014(void)
	{
		delete joystick;
		delete gameComponent;
		delete kicker;
		delete pickup;
		delete autonomous;
	}
	
	void RobotInit(void)
	{
		//MUST call JankyRobotTemplate's RobotInit function
		JankyRobotTemplate::RobotInit();
		
		joystick = new Joystick(DRIVE_JOYSTICK_PORT);
		gameComponent = new jankyXboxJoystick(GC_JOYSTICK_PORT);
		kicker = new JankyKickerState();
		pickup = new JankyPickupState();
		autonomous = new JankyAutonomousState(pRobot);
		
		kicker->SetPickupMachine(pickup);
		pickup->SetKickerMachine(kicker);
		
		printf("End of RobotInit\n");
	}
	
	void Autonomous(void)
	{
		AutonomousInit();
		ProgramIsAlive();
		//MUST be called - DO NOT TAKE OUT!
		
		//Supposed to drive forward for 2.0 seconds
		autonomous->GoForward();
	}
	
	void OperatorControl(void)
	{
		OperatorControlInit();
		printf("OPERATORCONTROLLL\n");
		
		while (IsOperatorControl())
		{
			//LOOP ALIVE - CANNOT TAKE OUT
			ProgramIsAlive(); 
			
			
			//MECANUM DRIVE
			float LeftaxisYValue = joystick->GetY();
			float LeftaxisXValue = joystick->GetX();
			float RightaxisXValue = joystick->GetTwist();
			pRobot->MecanumDrive_Cartesian(LeftaxisYValue, LeftaxisXValue, RightaxisXValue, 0.0);
			
			
			//PICKUP & KICKER
			//When button pressed arm will lower (extend piston) and motors will spin
			//Should automatically raise arm up and slow rollers after button released
			if(gameComponent->GetButtonA())
			{
				pickup->LowerToPickup();
			}
			//When button pressed should lower arm (extend piston)
			if(gameComponent->GetButtonB())
			{
				pickup->LowerForKick();
			}
			//When button pressed should automatically spin motor and wind up
			if(gameComponent->GetButtonX())
			{
				kicker->WindUp();
			}
			//When button pressed pawl should actuate to kick
			if(gameComponent->GetButtonY())
			{
				kicker->Kick();
			}
			
		}
		
	}

};

START_ROBOT_CLASS(AerialAssist2014);


