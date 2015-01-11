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
		
		autonomous->SetAutoPickupMachine(pickup);
		autonomous->SetAutoKickerMachine(kicker);
		
		printf("End of RobotInit\n");
	}
	
	void Autonomous(void)
	{
		AutonomousInit();
		ProgramIsAlive();
		//MUST be called - DO NOT TAKE OUT!
		
		//Supposed to drive forward for 2.0 seconds
		//autonomous->GoForward();
		
		//Supposed to drive forward, wind and kick
		autonomous->StartAuto();
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
			
			//PICKUP
			//When button pressed arm will lower and motors will spin (should automatically raise arm up/slow rollers after button up)
			if(gameComponent->GetButtonA())
			{
				pickup->LowerToPickup();
			}
			if(gameComponent->GetButtonB())
			{
				pickup->UnLowerExit();
			}
			//When button pressed should just lower arm (extend piston) - get ready to kick
			if(gameComponent->GetButtonY())
			{
				pickup->LowerForKick();
			}
			//When button pressed, rollers will spin out and pass/shoot in low goal
			if(gameComponent->GetButtonX())
			{
				pickup->Pass();
			}
			
			//KICKER
			//When button pressed should automatically spin motor and wind up
			if(gameComponent->GetButtonRB())
			{
				printf("Button X and calling automated windup\n");
				kicker->WindUp();
			}
			//When button pressed pawl should actuate to kick
			if(gameComponent->GetButtonLB())
			{
				kicker->Kick();
			}
			/*
			if(gameComponent->GetButtonY())
			{
				//printf("Calling kicker's PreWind\n");
				kicker->PreWind();
			}
			else
			{
				kicker->PreWindExit();
			}
			*/
		}
		
	}

};

START_ROBOT_CLASS(AerialAssist2014);


