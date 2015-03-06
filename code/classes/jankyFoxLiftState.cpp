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
	SetName(Safety, "Starting State");
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
	SetName(ManualOverride, "Overriding the State Machine");



	lSwitchTop = new DigitalInput(LIMIT_SWITCH_TOP);
	lSwitchDown = new DigitalInput(LIMIT_SWITCH_BOTTOM);
	//lSwitch6 = new DigitalInput(LIMIT_SWITCH_6);
	//lSwitch2 = new DigitalInput(LIMIT_SWITCH_2);
	motorLift = new Talon(TALON_LIFT);
	motorRoller1 = new Talon(TALON_ROLLER1);
	motorRoller2 = new Talon(TALON_ROLLER2);
	brake = new Solenoid(BRAKE);
	//toteIn = new AnalogInput(SONAR_SENSOR);
	//toteIn = new DigitalInput(LIMIT_SWITCH_TOTE);
	//toteIn = new Ultrasonic(ULTRASONIC_PING, ULTRASONIC_ECHO); (not using sonar sensor)
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
	//delete toteIn; (not using sonar sensor)
	delete rollerPistons;
	delete reorientation;
	delete singulationOne;
	delete singulationTwo;
	delete preRollerTimer;
	delete rollerInTimer;
	delete rollerOutTimer;
}
//ROLLERS
void JankyFoxliftState::SuckInTote(){
	motorRoller1->Set(ROLLER_SPEED_BACKWARD);
	motorRoller2->Set(ROLLER_SPEED_BACKWARD);
}
void JankyFoxliftState::PushOutTote(){
	motorRoller1->Set(ROLLER_SPEED_FORWARD);
	motorRoller2->Set(ROLLER_SPEED_FORWARD);
}
void JankyFoxliftState::StopRollers(){
	motorRoller1->Set(0);
	motorRoller2->Set(0);
}
//Arms
void JankyFoxliftState::ExtendArms(){
	rollerPistons -> Set(true);
}
void JankyFoxliftState::RetractArms(){
	rollerPistons -> Set(false);
}
bool JankyFoxliftState::AreArmsClosed(){
	if(rollerPistons -> Get() == true){
		return true;
	}
	return false;
}
//Manual Override
void JankyFoxliftState::ManualOverrideOn(){
	if(GetCurrentState() != ManualOverride){
	NewState(ManualOverride, "Pressed the button and it is now overriding");
	}
}
void JankyFoxliftState::ManualOverrideOff(){
	if(GetCurrentState() == ManualOverride){
	NewState(Init, "Stopped overriding-went into init");
	}
}
//Limit Switches
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
//Reorientation
void JankyFoxliftState::ExtendReorientation() {
	reorientation->Set(false);
}
void JankyFoxliftState::RetractReorientation() {
	reorientation->Set(true);
}
//Singulation
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
//Tote In (not using sonar sensor)
/*bool JankyFoxliftState::ToteIn(){
	//if (toteIn ->GetVoltage() > TOTE_SENSOR_PRESENT_IF_SMALLERTHAN){
		//return false;
	//}
	//if(toteIn->Get()== false){
		//return true;
	//}
	if(toteIn->GetRangeInches() < TOTE_SENSOR_PRESENT_IF_SMALLERTHAN){
		return true;
	}
	return false;
}*/
//Setting the robot
void JankyFoxliftState::SetFoxlift(){
	NewState(Safety,"Starting and going into safety");
}
//Stop For Lift
void JankyFoxliftState::StopLift(){
	if(GetCurrentState() == Up ||GetCurrentState() == Down){
		NewState(Init, "Stopped Boxlift");
	}
}
void JankyFoxliftState::GoUp(){
	if (GetCurrentState() == Init && !IsLSwitchTopClosed()){
		NewState(Up,"Button up pressed");
	}
	else if(GetCurrentState() == Down && !IsLSwitchTopClosed()){
		NewState(Up,"Button up pressed!");
	}
	else if(GetCurrentState() == ManualOverride){
			motorLift->Set(LIFT_UP_SPEED);
		}
}
void JankyFoxliftState::GoDown(){
	if(GetCurrentState() == Braking && !IsLSwitchDownClosed()){
		NewState(Down, "button down pressed");
	}
	else if (GetCurrentState() == Init && !IsLSwitchDownClosed()){
		NewState(Down, "button down pressed");
	}
	else if(GetCurrentState() == Up && !IsLSwitchDownClosed()){
		NewState(Down,"Button down pressed");
	}
	else if(GetCurrentState() == ManualOverride){
		motorLift->Set(LIFT_DOWN_SPEED);
	}
}
void JankyFoxliftState::SingulateOne(){
	if (GetCurrentState() == MoveRollersIn && rollerInTimer->Get() >= ROLLER_TIME){
		NewState(SingulationUp, "going to sing w/ 4-hook up");
	}
	else if (GetCurrentState() == MoveRollersOut){
		rollerInTimer -> Reset();
		NewState(MoveRollersIn, "Moving rollers in for sing up");
	}
	else if (GetCurrentState() == WaitForUndo){
		NewState(SingulationUp, "going to sing up");
	}
	else if (GetCurrentState() == Braking && ToteIn() == false){
		rollerInTimer->Reset();
		NewState(MoveRollersIn,"bringing rollers in for sing up");
	}
	else if(GetCurrentState() == ManualOverride){
		this->ExtendSingulation();
		this->RaiseSingulation();
    }
	else if(GetCurrentState() == SingulationDown){
		NewState(SingulationUp, "let go of trigger");
	}
}
void JankyFoxliftState::SingulateTwo(){
	if (GetCurrentState() == MoveRollersIn && rollerInTimer->Get() >= ROLLER_TIME){
		NewState(SingulationDown, "Rollers in- sing going down");
	}
	else if (GetCurrentState() == MoveRollersOut){
		rollerInTimer -> Reset();
		NewState(MoveRollersIn, "Pressed sing down");
	}
	else if (GetCurrentState() == WaitForUndo){
		NewState(SingulationDown, "Done waiting");
	}
	else if (GetCurrentState() == Braking){
		 rollerInTimer->Reset();
		 NewState(MoveRollersIn,"bringing rollers infor sing down");
	}
	else if(GetCurrentState() == ManualOverride){
			this->ExtendSingulation();
			this->LowerSingulation();
    }
	else if(GetCurrentState() == SingulationUp){
			NewState(SingulationDown, "pressed trigger");
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
		NewState(Reorientation, "starting to reorient");
	}
	else if (GetCurrentState() == Braking){
		rollerInTimer->Reset();
		NewState(MoveRollersIn,"for reorienting");
	}
	else if(GetCurrentState() == ManualOverride){
		this->ExtendReorientation();
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
	else if(GetCurrentState() == ManualOverride){
		this->RetractSingulation();
		this->RaiseSingulation();
        this->RetractReorientation();
	}
}

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
		case Safety:
			if(!AreArmsClosed() && IsLSwitchTopClosed()){
				NewState(Braking, "lift is up");
			}
			else if(!AreArmsClosed() && !IsLSwitchTopClosed()){
				NewState(Init, "lift is not up");
			}
			else if(AreArmsClosed()){
				NewState(WaitForUndo, " arms are closed");
			}
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
			/*if (ToteIn() == true){
				NewState(Up, "Tote in and now going up");
			}
			else{(not using sonar sensor)*/
			NewState(Init, "No tote so going to Init state");
			// }
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
				NewState(BottomStop, "All the way down");
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
				NewState(MoveRollersOut, "Waiting period is over");
			}
			break;
		case MoveRollersOut:
				this->ExtendArms();
			if(rollerOutTimer->Get() >= ROLLER_TIME && IsLSwitchTopClosed()){
				NewState(Braking, "because lift is up");
			}
			else if(rollerOutTimer->Get() >= ROLLER_TIME && !IsLSwitchTopClosed()){
				NewState(Init, "because lift is not up");
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
		case ManualOverride:
			break;
		default:
			printf("ERROR: ILLEGAL STATE - Value=%d\n",curState);
			break;
	}
}





