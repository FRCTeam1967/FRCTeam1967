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
	
	kickEncoder->setSpeed(AUTOMATED_WINDUP_SPEED);
	kickEncoder->Reset();
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
	
	bHasKicked = false;
	pickupMachine = NULL;
	
	pWindMotor = kickEncoder->returnMotor();
	
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
		NewState(AddWindUp,"Adding extra wind up");
	}
}

void JankyKickerState::Kick()
{
	if(GetCurrentState() == IdleReadyForKick)
	{
		if(pickupMachine->IsPickupDown() == true)
		{
			NewState(MotorDisengage,"Pick-up down and ready to kick");
		}
		if(pickupMachine->IsPickupUp() == true)
		{
			printf("Pickup up\n");
		}
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
			kickEncoder->Reset();
			bHasKicked = false;
			//Engaging dog gear
			dogGearPistonOne->Set(true);
			dogGearPistonTwo->Set(false);
			break;
		case ManualWindUp:
			printf("In ManualWindUp\n");
			//Disengaging pawl
			pawlPistonOne->Set(false);
			pawlPistonTwo->Set(true);
			//Manually winding motor up
			pWindMotor->Set(MANUAL_WINDUP_SPEED);
			SmartDashboard::PutNumber("Kick Motor Revolutions",kickEncoder->getRevolution());
			NewState(PostWindUpWait,"Button manually wound motor");
			break;
		case AutomatedWindUp:
			printf("In AutomatedWindUp\n");
			//Disengaging pawl
			pawlPistonOne->Set(false);
			pawlPistonTwo->Set(true);
			//Automated wind up with encoder
			kickEncoder->setRevolution(ENCODER_WINDUP_REVOLUTION);
			kickEncoder->Go();
			SmartDashboard::PutNumber("Kick Motor Revolutions",kickEncoder->getRevolution());
			if (kickEncoder->isDone() == true)
			{
				NewState(PostWindUpWait,"Automated wind up done");
			}
			break;
		case PostWindUpWait:
			printf("In PostWindUpWait\n");
			postWindUpTimer->Reset();
			postWindUpTimer->Start();
			if (postWindUpTimer->Get() >= POST_WINDUP_WAIT)
			{
				//Engaging pawl after arming
				pawlPistonOne->Set(true);
				pawlPistonTwo->Set(false);
				postWindUpTimer->Stop();
				NewState(IdleReadyForKick,"Wait done after wind up");
			}
			break;
		case IdleReadyForKick:
			printf("In IdleReadyForKick\n");
			//PreWind function goes to new state of adding windup
			//Kick function goes to new state of disengaging motor with dog gear
			break;
		case AddWindUp:
			printf("In AddWindUp\n");
			//Disengaging pawl
			pawlPistonOne->Set(false);
			pawlPistonTwo->Set(true);
			//Adding to previous arming
			pWindMotor->Set(MANUAL_WINDUP_SPEED);
			SmartDashboard::PutNumber("Kick Motor Revolutions",kickEncoder->getRevolution());
			NewState(PostWindUpWait,"Button manually wound motor");
			break;
		case MotorDisengage:
			printf("MotorDisengage\n");
			dogGearTimer->Reset();
			dogGearTimer->Start();
			//Disengage dog gear
			dogGearPistonOne->Set(false);
			dogGearPistonTwo->Set(true);
			if(dogGearTimer->Get() >= DOG_GEAR_DISENGAGE_WAIT)
			{
				//Motor spinning backwards to fully disengage after dog gear goes
				kickEncoder->setRevolution(ENCODER_DISENGAGE_REVOLUTION);
				kickEncoder->Go();
				SmartDashboard::PutNumber("Kick Motor Revolutions",kickEncoder->getRevolution());
				dogGearTimer->Stop();
				if(kickEncoder->isDone())
				{
					NewState(PawlActuate,"Dog gear disengaged motor");
				}
			}
			break;
		case PawlActuate:
			printf("In PawlActuate - kick\n");
			//Disengaging pawl
			pawlPistonOne->Set(false);
			pawlPistonTwo->Set(true);
			if(pawlTimer->Get() >= PAWL_ACTUATE_WAIT)
			{
				bHasKicked = true;
				NewState(MotorEngage,"Done kicking - pawl actuated");
			}
			break;
		case MotorEngage:
			printf("In MotorEngage\n");
			dogGearTimer->Reset();
			dogGearTimer->Start();
			//Engage dog gear
			dogGearPistonOne->Set(true);
			dogGearPistonTwo->Set(false);
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



