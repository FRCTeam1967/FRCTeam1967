/*
 * jankyFoxLiftState.cpp
 *
 *  Created on: Jan 31, 2015
 *      Author: Team1967
 */
//2/10
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
	SetName(MoveRollersIn, "Closing arms");
	SetName(MoveRollersOut, "Opening arms");
	SetName(SingulationDown, "The 4-hook down is pressed");
	SetName(SingulationUp, "The 4-hook up is pressed");
	SetName(Reorientation, "Reorienting is pressed");



	lSwitchTop = new DigitalInput(LIMIT_SWITCH_TOP);
	lSwitchDown = new DigitalInput(LIMIT_SWITCH_BOTTOM);
	//lSwitch6 = new DigitalInput(LIMIT_SWITCH_6);
	//lSwitch2 = new DigitalInput(LIMIT_SWITCH_2);
	motorLift = new Talon(TALON_LIFT);
	motorRoller1 = new Talon(TALON_ROLLER1);
	motorRoller2 = new Talon(TALON_ROLLER2);
	brake = new Solenoid(BRAKE);
	toteIn = new AnalogInput(SONAR_SENSOR);
	rollerPistons = new Solenoid(ROLLER_PISTON);
	reorientation = new Solenoid(REORIENTATION);
	singulationOne = new Solenoid(SINGULATION_ONE);
	singulationTwo = 	new Solenoid(SINGULATION_TWO);
	preRollerTimer = new Timer();
	rollerInTimer = new Timer();
	rollerOutTimer = new Timer();
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
	delete rollerPistons;
	delete reorientation;
	delete singulationOne;
	delete singulationTwo;
	delete preRollerTimer;
	delete rollerInTimer;
	delete rollerOutTimer;
}

void JankyFoxliftState::GoUp(){
	if (GetCurrentState() == Init && lSwitchTop->Get() == false){
		NewState(Up,"Starting to go up!");
	}
}
void JankyFoxliftState::GoDown(){
	if(GetCurrentState() == Braking && lSwitchDown->Get() == false){
		NewState(Down, "Going Down!");
	}
	else if (GetCurrentState() == Init && lSwitchDown->Get() == false){
		NewState(Down, "Going Down!");
	}
}
void JankyFoxliftState::SingulateOne(){
	if (GetCurrentState() == MoveRollersIn && rollerInTimer->Get() >= ROLLER_TIME){
		NewState(SingulationUp, "Singulation w/ 4-hook up");
	}
	if (GetCurrentState() == MoveRollersOut && preRollerTimer->Get() <= PREROLLER_TIME){
		rollerInTimer -> Reset();
		NewState(MoveRollersIn, "Singulation w/ 4-hook up");
	}
	if (GetCurrentState() == Braking && toteIn ->GetVoltage()> TOTE_SENSOR_PRESENT_IF_SMALLERTHAN){
		NewState(MoveRollersIn,"Bringing Rollers In");
	}
}
void JankyFoxliftState::SingulateTwo(){
	if (GetCurrentState() == MoveRollersIn && rollerInTimer->Get() >= ROLLER_TIME){
		NewState(SingulationDown, "Singulation w/ 4-hook down");
	}
	if (GetCurrentState() == MoveRollersOut && preRollerTimer->Get() <= PREROLLER_TIME){
		rollerInTimer -> Reset();
		NewState(MoveRollersIn, "Singulation w/ 4-hook down");
	}
	if (GetCurrentState() == Braking && toteIn ->GetVoltage()> TOTE_SENSOR_PRESENT_IF_SMALLERTHAN){
			NewState(MoveRollersIn,"Bringing Rollers In");
		}
}
void JankyFoxliftState::Reorient(){
	if (GetCurrentState() == MoveRollersIn && rollerInTimer->Get() >= ROLLER_TIME){
			NewState(Reorientation, " Arms in, starting to reorient");
		}
		if (GetCurrentState() == MoveRollersOut && preRollerTimer->Get() <= PREROLLER_TIME){
			rollerInTimer -> Reset();
			NewState(MoveRollersIn, "starting to reorient");
		}
		if (GetCurrentState() == Braking && toteIn ->GetVoltage()> TOTE_SENSOR_PRESENT_IF_SMALLERTHAN){
					NewState(MoveRollersIn,"Bringing Rollers In");
				}
}
void JankyFoxliftState::DoneSingulating(){
	if(GetCurrentState() == SingulationDown || GetCurrentState() == SingulationUp){
		preRollerTimer->Reset();
		rollerOutTimer->Reset();
		if(singulationTwo){
			singulationTwo ->Set(false);
		}
		singulationOne ->Set(false);
		NewState(MoveRollersOut, " Done Singulating-moving rollers out");
	}
	//if the arms are going in, and the driver is not pressing the button for singulation anymore, then go back to arms out
	if( GetCurrentState() == MoveRollersIn && rollerInTimer->Get() <= ROLLER_TIME){
		preRollerTimer->Reset();
		rollerOutTimer->Reset();
		NewState(MoveRollersOut, " Done Singulating-moving rollers out");
	}
}
void JankyFoxliftState::DoneReorienting(){
	if (GetCurrentState()== Reorientation || GetCurrentState()==MoveRollersIn){
		preRollerTimer->Reset();
		rollerOutTimer->Reset();
		reorientation->Set(false);
		NewState(MoveRollersOut, " Done Reorienting-moving rollers out");
	}
	//if the arms are going in, and the driver is not pressing the button for reorientation anymore, then go back to arms out
	if( GetCurrentState() == MoveRollersIn && rollerInTimer->Get() <= ROLLER_TIME){
			preRollerTimer->Reset();
			rollerOutTimer->Reset();
			NewState(MoveRollersOut, " Done Singulating-moving rollers out");
		}
}
void JankyFoxliftState::StateEngine(int curState)
{
	switch(curState){
		case Init:
			//printf("In Init\n");
			motorRoller1->Set(0);
			motorRoller2->Set(0);
			brake->Set(false);
			motorLift->Set(0);
			break;

		case Braking:
			//printf("In Braking\n");
			rollerInTimer->Reset();
			motorLift->Set(0);
			brake->Set(true);
			break;

		case BottomStop:
			//printf("In BottomStop\n");
			motorLift->Set(0);
			brake->Set(false);
			if (toteIn->GetVoltage() < TOTE_SENSOR_PRESENT_IF_SMALLERTHAN){
				NewState(Up, "Tote in and now going up");
			}
			else{
				NewState(Init, "No tote so going to Init state");
			}
			break;

		case Up:
			//printf("In Up\n");
			motorLift->Set(LIFT_UP_SPEED);
			brake->Set(false);
			if(lSwitchTop->Get() == true){
				NewState(Braking, "All the way up, so braking");
			}
			break;

		case Down:
			//printf("In Down\n");
			motorLift->Set(LIFT_DOWN_SPEED);
			brake->Set(false);
			if(lSwitchDown->Get() == true){
				NewState(BottomStop, "All the way down, so stopping for now");
			}
			break;
		case MoveRollersIn:
			rollerPistons->Set(false);
			rollerInTimer->Start();
			break;
		case MoveRollersOut:
			preRollerTimer->Start();
			if(preRollerTimer ->Get() >= PREROLLER_TIME){
				rollerPistons->Set(true);
				rollerOutTimer->Start();
			}
			if(rollerOutTimer->Get() >= ROLLER_TIME){
				NewState(Braking, "Rollers are out now, can go down");
			}
			break;
		case SingulationDown:
			if (singulationOne){
			singulationOne -> Set(true);
			}
			if(singulationTwo){
			singulationTwo -> Set(true);
			}
			break;
		case SingulationUp:
			if(singulationOne){
			singulationOne ->Set(true);
			}
			if(singulationTwo){
				singulationTwo ->Set(false);
			}
			break;
		case Reorientation:
			reorientation->Set(true);
			break;
	}
}





