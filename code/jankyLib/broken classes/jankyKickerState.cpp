/*
 * jankyKickerState.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: Team1967
 */

#include "jankyKickerState.h"

/*
 * Constructor
 * 
*/
JankyKickerState::JankyKickerState()
{
	printf("Beginning KickMachine constructor\n");
	SetMachineName("JankyKickerStateMachine");
	
	SetName(Idle,"Idle");
	SetName(ManualWindUp,"ManualWindUp");
	SetName(AutomatedWindUp,"AutomatedWindUp");
	SetName(PostWindUpWait,"PostWindUpWait");
	SetName(IdleReadyForKick,"IdleReadyForKick");
	SetName(AddWindUp,"AddWindUp");
	SetName(MotorDisengage,"MotorDisengage");
	SetName(PawlActuate,"PawlActuate");
	SetName(MotorEngage,"MotorEngage");
	
	kickEncoder = new JankyEncoder(ENCODER_ONE_CHANNEL, ENCODER_TWO_CHANNEL, ENCODER_MOTOR_CHANNEL);
	dogGearPistonOne = new Solenoid(DOG_GEAR_PISTON_TWO_CHANNEL);
	dogGearPistonTwo = new Solenoid(DOG_GEAR_PISTON_ONE_CHANNEL);
	pawlPistonOne = new Solenoid(PAWL_PISTON_ONE_CHANNEL);
	pawlPistonTwo = new Solenoid(PAWL_PISTON_TWO_CHANNEL);
	dogGearTimer = new Timer();
	postWindUpTimer = new Timer();
	pawlTimer = new Timer();
	addWindTimer = new Timer();
	disengageTimer = new Timer();
	
	kickEncoder->setSpeed(AUTOMATED_WINDUP_SPEED);
	kickEncoder->Reset();
	kickEncoder->SetMaxTime(20.0);
	kickEncoder->Start();
	
	/*
	dogGearPiston->SetFullCycleTime(DOG_GEAR_PISTON_CYCLE);
	dogGearPiston->SetActuationTime(DOG_GEAR_PISTON_ACTUATION);
	dogGearPiston->Start();
	pawlPiston->SetFullCycleTime(PAWL_PISTON_CYCLE);
	pawlPiston->SetActuationTime(PAWL_PISTON_ACTUATION);
	pawlPiston->Start();
	*/
	
	dogGearTimer->Reset();
	postWindUpTimer->Reset();
	pawlTimer->Reset();
	addWindTimer->Reset();
	disengageTimer->Reset();
	
	bHasKicked = false;
	pickupMachine = NULL;
	
	pWindMotor = kickEncoder->returnMotor();
	
	//Starting JankyTask at end of constructor
	Start();
	
	printf("End of kicker machine constructor\n");
}

/*
 * Destructor
 */
JankyKickerState::~JankyKickerState()
{
	delete kickEncoder;
	delete dogGearPistonOne;
	delete dogGearPistonTwo;
	delete pawlPistonOne;
	delete pawlPistonTwo;
	delete pWindMotor;
	delete dogGearTimer;
	delete postWindUpTimer;
	delete pawlTimer;
	delete pickupMachine;
}

void JankyKickerState::SetPickupMachine(JankyPickupState * pickupState)
{
	pickupMachine = pickupState;
}

void JankyKickerState::WindUp()
{
	if(GetCurrentState() == Idle)
	{
		NewState(AutomatedWindUp,"Going from Idle to automated winding");
	}
}

void JankyKickerState::PreWind()
{
	if(GetCurrentState() == Idle)
	{
		NewState(ManualWindUp,"Going from Idle to manually winding with button");
	}
	if(GetCurrentState() == IdleReadyForKick)
	{
		addWindTimer->Reset();
		addWindTimer->Start();
		NewState(AddWindUp,"Adding extra wind up");
	}
}

void JankyKickerState::PreWindExit()
{
	if(GetCurrentState() == ManualWindUp || GetCurrentState() == AddWindUp)
	{
		NewState(PostWindUpWait,"Button manually wound motor");
	}	
}

void JankyKickerState::Kick()
{
	if(GetCurrentState() == IdleReadyForKick)
	{
		if(pickupMachine && pickupMachine->IsPickupDown())
		{
			if(dogGearTimer)
			{
				dogGearTimer->Reset();
				dogGearTimer->Start();
			}
			if(kickEncoder)
				kickEncoder->Reset();
			if(disengageTimer)
			{
				disengageTimer->Reset();
				disengageTimer->Start();
			}
			NewState(MotorDisengage,"Pick-up down and ready to kick");
			return;
		}
		if(pickupMachine && pickupMachine->IsPickupUp() == true)
		{
			printf("Pickup up\n");
		}
		else if(pickupMachine == NULL)
			printf("Pick up machine NULL\n");
		else
			printf("Neither up nor down\n");
	}
}

bool JankyKickerState::HasKicked()
{
	return bHasKicked;
}

void JankyKickerState::StateEngine(int curState)
{
	switch(curState){
		case Idle:
			//Idle handled by PreWind or WindUp
			//printf("In Idle\n");
			if(kickEncoder)
				kickEncoder->Reset();
			bHasKicked = false;
			//Engaging dog gear
			if(dogGearPistonOne)
				dogGearPistonOne->Set(false);
			if(dogGearPistonTwo)
				dogGearPistonTwo->Set(true);
			//printf("Idle before break\n");
			break;
		case ManualWindUp:
			//printf("In ManualWindUp\n");
			//Disengaging pawl
			if(pawlPistonOne)
				pawlPistonOne->Set(true);
			if(pawlPistonTwo)
				pawlPistonTwo->Set(false);
			//Manually winding motor up
			if(pWindMotor)
				pWindMotor->Set(MANUAL_WINDUP_SPEED);
			SmartDashboard::PutNumber("Kick Motor Revolutions",kickEncoder->getRevolution());
			if(postWindUpTimer)
			{
				postWindUpTimer->Reset();
				postWindUpTimer->Start();
			}
			break;
		case AutomatedWindUp:
			//printf("In AutomatedWindUp\n");
			//Disengaging pawl
			if(pawlPistonOne)
				pawlPistonOne->Set(true);
			if(pawlPistonTwo)
				pawlPistonTwo->Set(false);
			//Automated wind up with encoder
			if(kickEncoder)
			{
				kickEncoder->setRevolution(ENCODER_WINDUP_REVOLUTION);
				kickEncoder->Go();
			}
			SmartDashboard::PutNumber("Kick Motor Revolutions",kickEncoder->getRevolution());
			if (kickEncoder->isDone() == true)
			{
				if(postWindUpTimer)
				{
					postWindUpTimer->Reset();
					postWindUpTimer->Start();
				}
				NewState(PostWindUpWait,"Automated wind up done");
			}
			break;
		case PostWindUpWait:
			//printf("In PostWindUpWait\n");
			if(pWindMotor)
				pWindMotor->Set(0.0);
			if (postWindUpTimer->Get() >= POST_WINDUP_WAIT)
			{
				printf("In post wind up wait - pawl actuate\n");
				//Engaging pawl after arming
				if(pawlPistonOne)
					pawlPistonOne->Set(false);
				if(pawlPistonOne)
					pawlPistonTwo->Set(true);
				postWindUpTimer->Stop();
				NewState(IdleReadyForKick,"Wait done after wind up");
			}
			break;
		case IdleReadyForKick:
			//printf("In IdleReadyForKick\n");
			//PreWind function goes to new state of adding windup
			//Kick function goes to new state of disengaging motor with dog gear
			break;
		case AddWindUp:
			//printf("In AddWindUp\n");
			//Disengaging pawl
			if(pawlPistonOne)
				pawlPistonOne->Set(true);
			if(pawlPistonTwo)
				pawlPistonTwo->Set(false);
			//Adding to previous arming
			if(addWindTimer->Get() <= ADD_WIND_TIME)
				pWindMotor->Set(MANUAL_WINDUP_SPEED);
			else
				pWindMotor->Set(0.0);
			
			SmartDashboard::PutNumber("Kick Motor Revolutions",kickEncoder->getRevolution());
			if(postWindUpTimer)
			{
				postWindUpTimer->Reset();
				postWindUpTimer->Start();
			}
			break;
		case MotorDisengage:
			//printf("MotorDisengage\n");
			//Disengage dog gear
			if(dogGearPistonOne)
				dogGearPistonOne->Set(true);
			if(dogGearPistonTwo)
				dogGearPistonTwo->Set(false);
			if(dogGearTimer->Get() >= DOG_GEAR_DISENGAGE_WAIT)
			{
				//Motor spinning backwards to fully disengage after dog gear goes
				if(disengageTimer)
				{
					if(disengageTimer->Get() <= (DOG_GEAR_DISENGAGE_WAIT + MOTOR_DISENGAGE_TIME))
					{
						pWindMotor->Set(DISENGAGE_SPEED);
					}
					else
					{
						pWindMotor->Set(0.0);
						pawlTimer->Reset();
						pawlTimer->Start();
						NewState(PawlActuate,"Dog gear disengaged motor");
					}
				}
				dogGearTimer->Stop();
			}
			break;
		case PawlActuate:
			//printf("In PawlActuate - kick\n");
			//Disengaging pawl
			if(pawlPistonOne)
				pawlPistonOne->Set(true);
			if(pawlPistonTwo)
				pawlPistonTwo->Set(false);
			if(pawlTimer->Get() >= PAWL_ACTUATE_WAIT)
			{
				bHasKicked = true;
				if(dogGearTimer)
				{
					dogGearTimer->Reset();
					dogGearTimer->Start();
				}
				NewState(MotorEngage,"Done kicking - pawl actuated");
			}
			break;
		case MotorEngage:
			//printf("In MotorEngage\n");
			//Engage dog gear
			if(dogGearTimer->Get() >= 2.0)
			{
				if(dogGearPistonOne)
					dogGearPistonOne->Set(false);
				if(dogGearPistonTwo)
					dogGearPistonTwo->Set(true);
			}
			if(dogGearTimer->Get() >= DOG_GEAR_ENGAGE_WAIT)
			{
				dogGearTimer->Stop();
				NewState(Idle,"Dog gear engaged motor");
			}
			break;
		default:
			printf("error current state = %d\n",curState);
			break;
	}
}



