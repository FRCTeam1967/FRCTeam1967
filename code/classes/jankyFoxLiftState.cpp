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
	printf("Beginning FoxliftMachine constructor\n");

	SetMachineName("JankyFoxlift");
	SetName(Init,"Init");
	//SetName(UpSingle,"Raise tote if !tote+ ");
	//SetName(UpFinish,"Raise tote if tote+");
	SetName(Braking,"Put brake and stop motor");
	//SetName(Down6,"Lower for step");
	//SetName(Down2,"Lower for platform");
	//SetName(DownStack,"Lower if tote in lift");
	SetName(BottomStop,"All the way down");
	SetName(Up,"Going Up");
	SetName(Down, "Going Down");
	SetName(MoveRollersIn, "Closing arms");
	SetName(MoveRollersOut, "Opening arms");
	SetName(SingulationDown, "The 4-hook down is pressed");
	SetName(SingulationUp, "The 4-hook up is pressed");
	SetName(Reorientation, "Reorienting is pressed");
	SetName(WaitForUndo, "Waiting 4 sing/reor 2 retract");



	lSwitchTop = new DigitalInput(LIMIT_SWITCH_TOP);
	lSwitchDown = new DigitalInput(LIMIT_SWITCH_BOTTOM);
	//lSwitch6 = new DigitalInput(LIMIT_SWITCH_6);
	//lSwitch2 = new DigitalInput(LIMIT_SWITCH_2);
	motorLift = new Talon(TALON_LIFT);
	motorRoller1 = new Talon(TALON_ROLLER1);
	motorRoller2 = new Talon(TALON_ROLLER2);
	brake = new Solenoid(BRAKE);
	//toteIn = new AnalogInput(SONAR_SENSOR);
	toteIn = new DigitalInput(LIMIT_SWITCH_TOTE);
	rollerPistons = new Solenoid(ROLLER_PISTON);
	reorientation = new Solenoid(REORIENTATION);//double solenoid on mentor bot
	singulationOne = new Solenoid(SINGULATION_ONE);//double solenoid on mentor bot
	singulationTwo = 	new Solenoid(SINGULATION_TWO);//double solenoid on mentor bot
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
bool JankyFoxliftState::IsLSwitchTopClosed(){
	if(lSwitchTop->Get() == false){
		return true;
	}
	return false;
}
bool JankyFoxliftState::IsLSwitchDownClosed(){
	if(lSwitchDown->Get() == false){
		return true;
	}
	return false;
}
void JankyFoxliftState::ExtendReorientation() {
	reorientation->Set(false);
}
void JankyFoxliftState::RetractReorientation() {
	reorientation->Set(true);
}
void JankyFoxliftState::RetractSingulation() {
	singulationOne->Set(true);
}
void JankyFoxliftState::ExtendSingulation(){
	singulationOne->Set(false);
}
void JankyFoxliftState::LowerSingulation(){
	singulationTwo->Set(false);
}
void JankyFoxliftState::RaiseSingulation(){
	singulationTwo->Set(true);
}
bool JankyFoxliftState::ToteIn(){
	//if (toteIn ->GetVoltage() > TOTE_SENSOR_PRESENT_IF_SMALLERTHAN){
		//return false;
	//}
	if(toteIn->Get()== false){
		return true;
	}
	return false;
}
void JankyFoxliftState::ExtendArms(){
	rollerPistons -> Set(true);
}
void JankyFoxliftState::RetractArms(){
	rollerPistons -> Set(false);
}
void JankyFoxliftState::SetFoxlift(){
	NewState(Init,"Starting the robot and going into init");
}
void JankyFoxliftState::GoUp(){
	if (GetCurrentState() == Init && !IsLSwitchTopClosed()){
		NewState(Up,"In init-Button up pressed-starting to go up!");
	}
	else if(GetCurrentState() == Down && !IsLSwitchTopClosed()){
		NewState(Up,"In down-Button up pressed-starting to go up!");
	}
}
void JankyFoxliftState::GoDown(){
	if(GetCurrentState() == Braking && !IsLSwitchDownClosed()){
		NewState(Down, "Tote is Up and button down pressed-Going Down!");
	}
	else if (GetCurrentState() == Init && !IsLSwitchDownClosed()){
		NewState(Down, "In Init and button down pressed-Going Down!");
	}
	else if(GetCurrentState() == Up && !IsLSwitchDownClosed()){
		NewState(Down,"In up-Button down pressed-starting to go down!");
	}
}
void JankyFoxliftState::SingulateOne(){
	if (GetCurrentState() == MoveRollersIn && rollerInTimer->Get() >= ROLLER_TIME){
		NewState(SingulationUp, "Arms in- going to sing w/ 4-hook up");
	}
	else if (GetCurrentState() == MoveRollersOut){
		rollerInTimer -> Reset();
		NewState(MoveRollersIn, "Moving rollers in for sing up");
	}
	else if (GetCurrentState() == WaitForUndo){
		NewState(SingulationUp, "Was in waiting period- going to sing up");
	}
	else if (GetCurrentState() == Braking && ToteIn() == false){
		rollerInTimer->Reset();
		NewState(MoveRollersIn,"Up and no tote- bringing rollers in for sing up");
	}
	else if(GetCurrentState() == SingulationDown){
		NewState(SingulationUp, "let go of trigger-going to sing up");
	}
}
void JankyFoxliftState::SingulateTwo(){
	if (GetCurrentState() == MoveRollersIn && rollerInTimer->Get() >= ROLLER_TIME){
		NewState(SingulationDown, "Rollers in- sing going down");
	}
	else if (GetCurrentState() == MoveRollersOut){
		rollerInTimer -> Reset();
		NewState(MoveRollersIn, "Pressed sing down- rollers going in");
	}
	else if (GetCurrentState() == WaitForUndo){
		NewState(SingulationDown, "Done waiting-going to sing down");
	}
	else if (GetCurrentState() == Braking && ToteIn() == false){
		 rollerInTimer->Reset();
		 NewState(MoveRollersIn,"Up w/ no tote- bringing rollers in for sing down");
	}
	else if(GetCurrentState() == SingulationUp){
			NewState(SingulationDown, "pressed trigger-going to sing down");
		}
}
void JankyFoxliftState::Reorient(){
	if (GetCurrentState() == MoveRollersIn && rollerInTimer->Get() >= ROLLER_TIME){
		NewState(Reorientation, " Arms in, starting to reorient");
	}
	else if (GetCurrentState() == MoveRollersOut){
			rollerInTimer -> Reset();
		NewState(MoveRollersIn, "Putting rollers in to reorient");
	}
	else if (GetCurrentState() == WaitForUndo){
		NewState(Reorientation, "Was in waiting period-starting to reorient");
	}
	else if (GetCurrentState() == Braking && ToteIn() == false){
		rollerInTimer->Reset();
		NewState(MoveRollersIn," Up and no tote in- bringing arms in");
	}
}
void JankyFoxliftState::DoneSingReor(){
	if(GetCurrentState() == SingulationDown || GetCurrentState() == SingulationUp
			|| GetCurrentState()== Reorientation){
		preRollerTimer->Reset();
		NewState(WaitForUndo, " Done sing-going to waiting period");
	}
	//if the arms are going in, and the driver is not pressing the button for singulation anymore, then go back to arms out
	else if( GetCurrentState() == MoveRollersIn && rollerInTimer->Get() <= ROLLER_TIME){
		rollerOutTimer->Reset();
		NewState(MoveRollersOut, "Done sing -moving rollers out");
	}
}
/*void JankyFoxliftState::DoneReorienting(){
	if (GetCurrentState()== Reorientation ){
		printf("done reorienting\n");
		preRollerTimer->Reset();
		NewState(WaitForUndo, " Done reor-going to waiting period");
	}
	//if the arms are going in, and the driver is not pressing the button for reorientation anymore, then go back to arms out
	else if( GetCurrentState() == MoveRollersIn && rollerInTimer->Get() <= ROLLER_TIME){
		rollerOutTimer->Reset();
		NewState(MoveRollersOut, " Done reor-moving rollers out");
	}
}*/
void JankyFoxliftState::StateEngine(int curState)
{
	switch(curState){
		case Init:
			//printf("In Init\n");
			/*motorRoller1->Set(0); Commented because bottomStop goes to init and we might want to
			motorRoller2->Set(0); use rollers during init.*/
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
			if (ToteIn() == true){
				NewState(Up, "Tote in and now going up");
			}
			else{
				NewState(Init, "No tote so going to Init state");
			}
			break;

		case Up:
			//printf("In Up\n");
			motorLift->Set(LIFT_UP_SPEED);
			motorRoller1->Set(0);
			motorRoller2->Set(0);
			brake->Set(false);
			if(IsLSwitchTopClosed()){
				NewState(Braking, "All the way up, so braking");
			}
			break;

		case Down:
			//printf("In Down\n");
			motorLift->Set(LIFT_DOWN_SPEED);
			motorRoller1->Set(0);
			motorRoller2->Set(0);
			brake->Set(false);
			if(IsLSwitchDownClosed()){
				NewState(BottomStop, "All the way down, so stopping for now");
			}
			break;
		case MoveRollersIn:
			this->RetractArms();
			break;
		case WaitForUndo:
			this->RetractReorientation();
			this->RetractSingulation();
			this->RaiseSingulation();
			if(preRollerTimer ->Get() >= PREROLLER_TIME){
				NewState(MoveRollersOut, "Waiting period is over-Moving rollers out");
			}
			break;
		case MoveRollersOut:
				this->ExtendArms();
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





