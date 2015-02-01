/*
 * jankyFoxLiftState.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: Team1967
 */

#include "jankyFoxLiftState.h"

/*
 * Constructor
 *
*/
JankyFoxliftState::JankyFoxliftState()
{
	printf("Beginning PickupMachine constructor\n");

	SetMachineName("JankyPickupStateMachine");
	SetName(Init,"Init");
	SetName(UpSingle,"Raise tote if !tote+ ");
	SetName(UpFinish,"Raise tote if tote+");
	SetName(Braking,"Put brake and stop motor");
	SetName(Down6,"Lower for step");
	SetName(Down2,"Lower for platform");
	SetName(DownStack,"Lower if tote in lift");
	//SetName(BottomStack,"Ready to kick");



	/*rollersMotor = new Talon(ROLLERS_MOTOR_CHANNEL);

	rollersTimer = new Timer();
	pickupTimer = new Timer();
	lowerTimer = new Timer();
	passTimer = new Timer();

	lowerTimer->Reset();
	rollersTimer->Reset();
	pickupTimer->Reset();
	passTimer->Reset();*/

	//kickMachine = NULL;

	//Starting JankyTask at end of constructor
	Start();

	printf("End of foxlift machine constructor\n");
}

/*
 * Destructor
 */
JankyFoxliftState::~JankyFoxliftState()
{
	/*delete pickupPistonOne;
	delete pickupPistonTwo;
	delete rollersMotor;
	delete rollersTimer;
	delete pickupTimer;
	delete lowerTimer;
	delete passTimer;
	delete kickMachine;*/
}


/*
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

void JankyPicState::UnLowerExit()
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

void JankyFoxliftState::Pass()
{
	if(GetCurrentState() == PrimedForKick)
	{
		passTimer->Reset();
		passTimer->Start();
		NewState(Passing,"Button pressed");
	}
 */

void JankyFoxliftState::StateEngine(int curState)
{
	switch(curState){
		case Init:
			printf(GetName(0));
			printf("\n");
		break;
		case UpSingle:
			printf(GetName(1));
			printf("\n");
		break;
		case UpFinish:
			printf(GetName(2));
			printf("\n");
		break;
		case Braking:
			printf(GetName(3));
			printf("\n");
		break;
		case Down6:
			printf(GetName(4));
			printf("\n");
		break;
		case Down2:
			printf(GetName(5));
			printf("\n");
		break;
		case DownStack:
			printf(GetName(6));
			printf("\n");
		break;
	}
}





