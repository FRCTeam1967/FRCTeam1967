/*
 * jankyPickupState.cpp
 *
 *  Created on: Feb 15, 2014
 *      Author: Team1967
 */

#include "jankyPickupState.h"

/*
 * Constructor
 * 
*/
JankyPickupState::JankyPickupState()
{
	printf("Beginning PickupMachine constructor\n");

	SetMachineName("JankyPickupStateMachine");
	SetName(Idle,"Idle");
	SetName(LowerArmPickup,"Lower arm, spin motors");
	SetName(Up,"Pick-up goes up");
	SetName(SlowRollers,"Slow down rollers");
	SetName(PrimedForKick,"Primed for kick");
	SetName(Lower,"Lower the pick-up");
	SetName(ReadyToKick,"Ready to kick");
	SetName(Raise,"Raise pick-up");
	
	pickupPistonOne = new Solenoid(PICKUP_PISTON_ONE_CHANNEL);
	pickupPistonTwo = new Solenoid(PICKUP_PISTON_TWO_CHANNEL);
	rollersMotor = new Talon(ROLLERS_MOTOR_CHANNEL);
	
	rollersTimer = new Timer();	
	pickupTimer = new Timer();
	
	rollersTimer->Reset();
	pickupTimer->Reset();
	
	kickMachine = NULL;
	
	printf("End of pickup machine constructor\n");
}

/*
 * Destructor
 */
JankyPickupState::~JankyPickupState()
{
	delete pickupPistonOne;
	delete pickupPistonTwo;
	delete rollersMotor;
	delete rollersTimer;
	delete pickupTimer;
	delete kickMachine;
}

void JankyPickupState::SetKickerMachine(JankyKickerState * kickState)
{
	kickMachine = kickState;
	printf("Set kick machine\n");
}

bool JankyPickupState::IsPickupUp()
{
	if(GetCurrentState() == Up || Raise)
	{
		return true;
	}
	else
		return false;
}

bool JankyPickupState::IsPickupDown()
{
	if(GetCurrentState() == Lower)
	{
		return true;
	}
	else
		return false;
}

void JankyPickupState::LowerForKick()
{
	if(GetCurrentState() == PrimedForKick)
	{
		NewState(Lower,"Primed for kick done");
	}
}

void JankyPickupState::LowerToPickup()
{
	NewState(LowerArmPickup,"Button to lower arm to pick ball up");
}

void JankyPickupState::StateEngine(int curState)
{
	switch(curState){
		case Idle:
			//Idle handled by LowerToPickup
			//printf("In Pickup Idle\n");
			break;
		case LowerArmPickup:
			printf("In LowerArmPickup\n");
			pickupPistonOne->Set(false);
			pickupPistonTwo->Set(true);
			rollersMotor->Set(FAST_ROLLERS_SPEED);
			NewState(Up,"After lowering and picking up");
			break;
		case Up:
			printf("In Up\n");
			pickupTimer->Reset();
			pickupTimer->Start();
			rollersMotor->Set(0.0);
			pickupPistonOne->Set(true);
			pickupPistonTwo->Set(false);
			if (pickupTimer->Get() >= RAISE_ARM_WAIT)
			{
				pickupTimer->Stop();
				NewState(SlowRollers,"Done raising arm up");
			}
			break;
		case SlowRollers:
			printf("In SlowRollers\n");
			rollersTimer->Reset();
			rollersTimer->Start();
			rollersMotor->Set(SLOW_ROLLERS_SPEED);
			if(rollersTimer->Get() >= ROLLERS_WAIT)
			{
				rollersTimer->Stop();
				NewState(PrimedForKick,"Done with slow rollers");
			}
			break;
		case PrimedForKick:
			//PrimedForKick handled by LowerForKick()
			printf("In PrimedForKick\n");
			break;
		case Lower:
			printf("In Lower\n");
			pickupPistonOne->Set(false);
			pickupPistonTwo->Set(true);
			NewState(ReadyToKick,"Lowered arm to kick");
			break;
		case ReadyToKick:
			printf("In ReadyToKick\n");
			if(kickMachine->HasKicked() == true)
			{
				NewState(Raise,"Kicking done");
			}
			break;
		case Raise:
			printf("In Raise after kick\n");
			pickupTimer->Reset();
			pickupTimer->Start();
			pickupPistonOne->Set(true);
			pickupPistonTwo->Set(false);
			if(pickupTimer->Get() >= RAISE_ARM_WAIT)
			{
				NewState(Idle,"Raised arm up");
			}
			break;
		default:
			printf("error current state = %d\n",curState);
			break;
	}
}



