/*
 * JankyFoxLiftStateDemo.cpp
 *
 *  Created on: Feb 14, 2015
 *      Author: Natasha
 */

#include "JankyFoxLiftStateDemo.h"

JankyFoxLiftStateDemo::JankyFoxLiftStateDemo() : JankyFoxliftState() {

	printf("I'm in the Demo Constructor");
	//Terminate();
	printf("got past the terminate");
}

JankyFoxLiftStateDemo::~JankyFoxLiftStateDemo() {

}
void JankyFoxLiftStateDemo::GoUp(){
	motorLift->Set(LIFT_UP_SPEED);
				brake->Set(false);
	}

void JankyFoxLiftStateDemo::GoDown(){
	motorLift->Set(LIFT_DOWN_SPEED);
			brake->Set(false);
	}

void JankyFoxLiftStateDemo::SpinIn(){

	//rollerPistons->Set(false);
	motorRoller1->Set(-1.0);
	motorRoller2->Set(1.0);

}
void JankyFoxLiftStateDemo:: SpinOut()
{
	//rollerPistons->Set(true);
	motorRoller1->Set(1.0);
	motorRoller2->Set(-1.0);
}
/*void JankyFoxliftState::SingulateTwo(){
	if(singulationTwo){
				singulationTwo -> Set(true);
				}
	if (GetCurrentState() == MoveRollersOut && preRollerTimer->Get() <= PREROLLER_TIME){
		rollerInTimer -> Reset();
		NewState(MoveRollersIn, "Singulation w/ 4-hook down");
	}
}*/

/*void JankyFoxLiftStateDemo::DoneSingulating(){
	rollerInTimer->Reset();
				motorLift->Set(0);
				brake->Set(true);
}
void JankyFoxLiftStateDemo::Reorient()
{
	reorientation->Set(true);
}
void JankyFoxLiftStateDemo::DoneReorienting(){

		preRollerTimer->Reset();
		rollerOutTimer->Reset();
		reorientation->Set(false);

		}*/
