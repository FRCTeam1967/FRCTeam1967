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
	//SetName(UpSingle,"Raise tote if !tote+ ");
	//SetName(UpFinish,"Raise tote if tote+");
	SetName(Braking,"Put brake and stop motor");
	//SetName(Down6,"Lower for step");
	//SetName(Down2,"Lower for platform");
	//SetName(DownStack,"Lower if tote in lift");
	SetName(BottomStop,"Ready to kick");
	SetName(Up,"Going Up");
	SetName(Down, "Going Down");


	lSwitchTop = new DigitalInput(LIMIT_SWITCH_TOP);
	lSwitchDown = new DigitalInput(LIMIT_SWITCH_BOTTOM);
	//lSwitch6 = new DigitalInput(LIMIT_SWITCH_6);
	//lSwitch2 = new DigitalInput(LIMIT_SWITCH_2);
	motorLift = new Talon(TALON_LIFT);
	motorRoller1 = new Talon(TALON_ROLLER1);
	motorRoller2 = new Talon(TALON_ROLLER2);
	brake = new Solenoid(BRAKE);
	toteIn = new AnalogInput(SONAR_SENSOR);

	//Starting JankyTask at end of constructor
	Start();

	printf("End of foxlift machine constructor\n");
}

/*
 * Destructor
 */
JankyFoxliftState::~JankyFoxliftState()
{
	delete lSwitchTop;
	delete lSwitchDown;
	//delete lSwitch6;
	//delete lSwitch2;
	delete motorLift;
	delete motorRoller1;
	delete motorRoller2;
	delete brake;
	delete toteIn;
}


void JankyFoxliftState::GoUp(){
	if (lSwitchTop == false){
		NewState(Up,"Starting to go up!");
	}
}
void JankyFoxliftState::GoDown(){
	if (lSwitchDown == false){
		NewState(Down, "Going Down!");
	}
}


void JankyFoxliftState::StateEngine(int curState)
{
	switch(curState){
		case Init:
			printf("In Init\n");
			motorRoller1->Set(0);
			motorRoller2->Set(0);
			brake->Set(false);
			motorLift->Set(0);
		break;
		case Braking:
			printf("In Braking\n");
			motorLift->Set(0);
			brake->Set(true);
		break;
		case BottomStop:
			printf("In BottomStop\n");
			motorLift->Set(0);
			brake->Set(false);
			if (toteIn->GetVoltage() < .7){
				NewState(Up, "Tote in and now going up");
			}
			else{
				NewState(Init, "No tote so going to Init state");
			}
		break;
		case Up:
			printf("In Up\n");
			motorLift->Set(.5);
			brake->Set(false);
			if(lSwitchTop->Get() == true){
				NewState(Braking, "All the way up, so braking");
			}
		break;
		case Down:
			printf("In Down");
			motorLift->Set(-.5);
			brake->Set(false);
			if(lSwitchDown->Get() == true){
				NewState(BottomStop, "All the way down, so stopping for now");
			}
		break;

	}
}





