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
	SetName(WaitForUndo, "Waiting for driver to see if she wants to change her mind");



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

	preRollerTimer->Start();
	rollerInTimer->Start();
	rollerOutTimer->Start();
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

void JankyFoxliftState::ExtendReorientation() {
	reorientation->Set(true);
}
void JankyFoxliftState::RetractReorientation() {
	reorientation->Set(false);
}
void JankyFoxliftState::RetractSingulation() {
	singulationOne->Set(false);
}
void JankyFoxliftState::ExtendSingulation(){
	singulationOne->Set(true);
}
void JankyFoxliftState::LowerSingulation(){
	singulationTwo->Set(true);
}
void JankyFoxliftState::RaiseSingulation(){
	singulationTwo->Set(false);
}
void JankyFoxliftState::GoUp(){
	if (GetCurrentState() == Init && lSwitchTop->Get() == false){
		NewState(Up,"Button up pressed-starting to go up!");
	}
}
void JankyFoxliftState::GoDown(){
	if(GetCurrentState() == Braking && lSwitchDown->Get() == false){
		NewState(Down, "Tote is Up and button down pressed-Going Down!");
	}
	else if (GetCurrentState() == Init && lSwitchDown->Get() == false){
		NewState(Down, "In Init and button down pressed-Going Down!");
	}
}
void JankyFoxliftState::SingulateOne(){
	if (GetCurrentState() == MoveRollersIn && rollerInTimer->Get() >= ROLLER_TIME){
		NewState(SingulationUp, "Moved Rollers In- going to Singulation w/ 4-hook up");
	}
	if (GetCurrentState() == MoveRollersOut){
		rollerInTimer -> Reset();
		NewState(MoveRollersIn, "Moving rollers in for singulation up");
	}
	if (GetCurrentState() == WaitForUndo){
		NewState(SingulationUp, "Was in waiting period- going to singulation up");
	}
	if (GetCurrentState() == Braking && toteIn ->GetVoltage()> TOTE_SENSOR_PRESENT_IF_SMALLERTHAN){
		rollerInTimer->Reset();
		NewState(MoveRollersIn,"Up and no tote- bringing rollers in for singulation up");
	}
}
void JankyFoxliftState::SingulateTwo(){
	if (GetCurrentState() == MoveRollersIn && rollerInTimer->Get() >= ROLLER_TIME){
		NewState(SingulationDown, "Rollers in- singulation going down");
	}
	if (GetCurrentState() == MoveRollersOut){
		rollerInTimer -> Reset();
		NewState(MoveRollersIn, "Pressed singulation down- rollers going in for singulation down");
	}
	if (GetCurrentState() == WaitForUndo){
		NewState(SingulationDown, "In waiting period but now going to singulation down");
	}
	if (GetCurrentState() == Braking && toteIn ->GetVoltage()> TOTE_SENSOR_PRESENT_IF_SMALLERTHAN){
		 rollerInTimer->Reset();
		 NewState(MoveRollersIn,"Up and no tote- bringing rollers in for singulation down");
	}
}
void JankyFoxliftState::Reorient(){
	if (GetCurrentState() == MoveRollersIn && rollerInTimer->Get() >= ROLLER_TIME){
			NewState(Reorientation, " Arms in, starting to reorient");
		}
		if (GetCurrentState() == MoveRollersOut){
			rollerInTimer -> Reset();
			NewState(MoveRollersIn, "Putting rollers in to reorient");
		}
		if (GetCurrentState() == WaitForUndo){
			NewState(Reorientation, "Was in waiting period-starting to reorient");
		}
		if (GetCurrentState() == Braking && toteIn ->GetVoltage()> TOTE_SENSOR_PRESENT_IF_SMALLERTHAN){
			rollerInTimer->Reset();
			NewState(MoveRollersIn," Up and no tote in- bringing arms in");
		}
}
void JankyFoxliftState::DoneSingulating(){
	if(GetCurrentState() == SingulationDown || GetCurrentState() == SingulationUp){
		preRollerTimer->Reset();
		NewState(WaitForUndo, " Done Singulating-going to waiting period");
	}
	//if the arms are going in, and the driver is not pressing the button for singulation anymore, then go back to arms out
	if( GetCurrentState() == MoveRollersIn && rollerInTimer->Get() <= ROLLER_TIME){
		rollerOutTimer->Reset();
		NewState(MoveRollersOut, "Done singulating -moving rollers out");
	}
}
void JankyFoxliftState::DoneReorienting(){
	if (GetCurrentState()== Reorientation ){
		this->RetractReorientation();
		preRollerTimer->Reset();
		NewState(WaitForUndo, " Done Reorienting-going to waiting period");
	}
	//if the arms are going in, and the driver is not pressing the button for reorientation anymore, then go back to arms out
	if( GetCurrentState() == MoveRollersIn && rollerInTimer->Get() <= ROLLER_TIME){
		rollerOutTimer->Reset();
		NewState(MoveRollersOut, " Done reorienting-moving rollers out");
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
			break;
		case WaitForUndo:
			if(preRollerTimer ->Get() >= PREROLLER_TIME){
				NewState(MoveRollersOut, "Waiting period is over-Moving rollers out for real now");
			}
			break;
		case MoveRollersOut:
				rollerPistons->Set(true);
			if(rollerOutTimer->Get() >= ROLLER_TIME){
				NewState(Braking, "Rollers are out now-breaking");
			}
			break;
		case SingulationDown:
			this->ExtendSingulation();
			this->LowerSingulation();
			break;
		case SingulationUp:
			this->ExtendSingulation();
			this->RaiseSingulation();
			break;
		case Reorientation:
			this->ExtendReorientation();
			break;
	}
}





