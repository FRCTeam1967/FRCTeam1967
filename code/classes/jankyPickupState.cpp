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
	SetName(Passing,"Spin rollers to pass");
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
	lowerTimer = new Timer();
	passTimer = new Timer();
	
	lowerTimer->Reset();
	rollersTimer->Reset();
	pickupTimer->Reset();
	passTimer->Reset();
	
	kickMachine = NULL;
	
	//Starting JankyTask at end of constructor
	Start();
	
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
	delete lowerTimer;
	delete passTimer;
	delete kickMachine;
}

void JankyPickupState::SetKickerMachine(JankyKickerState * kickState)
{
	kickMachine = kickState;
	printf("Set kick machine\n");
}

bool JankyPickupState::IsPickupUp()
{
	if(GetCurrentState() == Up || GetCurrentState() == Raise)
	{
		return true;
	}
	else
		return false;
}

bool JankyPickupState::IsPickupDown()
{
	if(GetCurrentState() == ReadyToKick)
	{
		return true;
	}
	else
		return false;
}

void JankyPickupState::LowerForKick()
{
	if(GetCurrentState() == Idle || GetCurrentState() == PrimedForKick)
	{
		lowerTimer->Reset();
		lowerTimer->Start();
		NewState(Lower,"Primed for kick done");
	}
}

void JankyPickupState::LowerToPickup()
{
	NewState(LowerArmPickup,"Button to lower arm to pick ball up");
}

void JankyPickupState::UnLowerExit()
{
	if(GetCurrentState() == LowerArmPickup)
	{
		if(pickupTimer)
		{
			//Starting pickupTimer for new state
			pickupTimer->Reset();
			pickupTimer->Start();
		}
		NewState(Up,"After lowering and picking up");
	}
}

void JankyPickupState::Pass()
{
	if(GetCurrentState() == PrimedForKick)
	{
		passTimer->Reset();
		passTimer->Start();
		NewState(Passing,"Button pressed");
	}
}

void JankyPickupState::StateEngine(int curState)
{
	switch(curState){
		case Idle:
			//Idle handled by LowerToPickup
			//printf("In Pickup Idle\n");
			if(pickupPistonOne)
				pickupPistonOne->Set(true);
			if(pickupPistonTwo)
				pickupPistonTwo->Set(false);
			break;
		case LowerArmPickup:
			//printf("In LowerArmPickup\n");
			//Engaging/lowering pickup
			if(pickupPistonOne)
				pickupPistonOne->Set(false);
			if(pickupPistonTwo)
				pickupPistonTwo->Set(true);
			rollersMotor->Set(FAST_ROLLERS_SPEED);
			break;
		case Up:
			//printf("In Up\n");
			rollersMotor->Set(FAST_ROLLERS_SPEED);
			if(pickupPistonOne)
				pickupPistonOne->Set(true);
			if(pickupPistonTwo)
				pickupPistonTwo->Set(false);
			if (pickupTimer->Get() >= RAISE_ARM_WAIT)
			{
				pickupTimer->Stop();
				rollersTimer->Reset();
				rollersTimer->Start();
				rollersMotor->Set(0.0);
				NewState(PrimedForKick,"Done raising arm up"); //Used to go to slow rollers but not using that any more
			}
			break;
		case SlowRollers:
			//printf("In SlowRollers\n");
			rollersMotor->Set(SLOW_ROLLERS_SPEED);
			if(rollersTimer->Get() >= ROLLERS_WAIT)
			{
				rollersTimer->Stop();
				NewState(PrimedForKick,"Done with slow rollers");
			}
			break;
		case PrimedForKick:
			//PrimedForKick handled by LowerForKick()
			//printf("In PrimedForKick\n");
			break;
		//Add state with pass capability (set motor to negative, timer after)
		//if button still pressed - will go back into state
		case Passing:
			if(rollersMotor)
			{
				rollersMotor->Set(PASS_ROLLERS_SPEED);
				if(passTimer->Get() >= PASS_TIME)
				{
					rollersMotor->Set(0.0);
					NewState(PrimedForKick,"Done passing");
				}
			}
			break;
		case Lower:
			//printf("In Lower\n");
			rollersMotor->Set(SLOW_ROLLERS_SPEED);
			if(lowerTimer->Get() >= ROLLING_TIME)
			{
				if(pickupPistonOne)
					pickupPistonOne->Set(false);
				if(pickupPistonTwo)
					pickupPistonTwo->Set(true);
			}
			if(lowerTimer->Get() >= LOWERING_TIME)
			{
				rollersMotor->Set(0.0);
				NewState(ReadyToKick,"Lowered arm to kick");
			}
			break;
		case ReadyToKick:
			//printf("In ReadyToKick\n");
			if(kickMachine->HasKicked() == true)
			{
				pickupTimer->Reset();
				pickupTimer->Start();
				NewState(Raise,"Kicking done");
			}
			break;
		case Raise:
			//printf("In Raise after kick\n");
			if(pickupPistonOne)
				pickupPistonOne->Set(true);
			if(pickupPistonTwo)
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



