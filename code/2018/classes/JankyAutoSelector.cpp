/*
 * JankyAutoSelector.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: AnishaKabir
 */

#include "JankyAutoSelector.h"
#include "frc/WPILib.h"

//selection criteria
#define LEFT 1
#define MIDDLE 2
#define RIGHT 3
#define ZERO_SEC 1
#define ONE_SEC 2
#define TWO_SEC 3
#define THREE_SEC 4
#define FOUR_SEC 5
#define DEFAULT 1
#define CROSS_AUTO_LINE 2
#define SAME_SIDE_SWITCH 3
#define EITHER_SWITCH 4
#define SAME_SIDE_SCALE 5
#define EITHER_SCALE 6

//auto modes
#define DEFAULT_MODE 1
#define L_CROSS_AUTOLINE 2
#define L_SAME_SWITCH 3
#define L_OPPOSITE_SWITCH 4
#define M_LEFT_SWITCH 5
#define M_RIGHT_SWITCH 6
#define R_CROSS_AUTOLINE 7
#define R_SAME_SWITCH 8
#define R_OPPOSITE_SWITCH 9
#define L_SAME_SCALE 10
#define R_SAME_SCALE 11
#define L_OPPOSITE_SCALE 12
#define R_OPPOSITE_SCALE 13

int delayTime = 0;

int left = LEFT;
int middle = MIDDLE;
int right = RIGHT;
int zero_sec = ZERO_SEC;
int one_sec = ONE_SEC;
int two_sec = TWO_SEC;
int three_sec = THREE_SEC;
int four_sec = FOUR_SEC;
int defaultAuto = DEFAULT;
int crossLine = CROSS_AUTO_LINE;
int sameSwitch = SAME_SIDE_SWITCH;
int eitherSwitch = EITHER_SWITCH;
int sameScale = SAME_SIDE_SCALE;
int eitherScale = EITHER_SCALE;

frc::SendableChooser<int*>position;
frc::SendableChooser<int*>delay;
frc::SendableChooser<int*>action;

int autoMode = DEFAULT_MODE;

JankyAutoSelector::JankyAutoSelector() {

}

JankyAutoSelector::~JankyAutoSelector() {
	// TODO Auto-generated destructor stub
}

void JankyAutoSelector::Init(){
	position.SetDefaultOption("Left", &left);
	position.AddOption("Middle", &middle);
	position.AddOption("Right", &right);
	frc::SmartDashboard::PutData("Position", &position);

	delay.SetDefaultOption("0 seconds", &zero_sec);
	delay.AddOption("1 second", &one_sec);
	delay.AddOption("2 seconds", &two_sec);
	delay.AddOption("3 seconds", &three_sec);
	delay.AddOption("4 seconds", &four_sec);
	frc::SmartDashboard::PutData("Delay", &delay);

	action.SetDefaultOption("Default (does nothing)", &defaultAuto);
	action.AddOption("Cross Auto Line", &crossLine);
	action.AddOption("Our Side Switch", &sameSwitch);
	action.AddOption("Either Switch", &eitherSwitch);
	action.AddOption("Our Side Scale", &sameScale);
	//action.AddObject("Either Scale", &eitherScale); //removing because this is not ready yet
	frc::SmartDashboard::PutData("Action", &action);

}

int JankyAutoSelector::GetDelayTime(){
	if(delay.GetSelected()==&zero_sec){
		return 0;
	}
	else if(delay.GetSelected()==&one_sec){
		return 1;
	}
	else if(delay.GetSelected()==&two_sec){
		return 2;
	}
	else if(delay.GetSelected()==&three_sec){
		return 3;
	}
	else if(delay.GetSelected()==&four_sec){
		return 4;
	}
	else{
		return 0;
	}
}

void JankyAutoSelector::PrintValues(){
	printf("left: %d \n", left);
	printf("middle: %d \n", middle);
	printf("right: %d \n", right);
	printf("zero_sec: %d \n", zero_sec);
	printf("one_sec: %d \n", one_sec);
	printf("two_sec: %d \n", two_sec);
	printf("three_sec: %d \n", three_sec);
	printf("four_sec: %d \n", four_sec);
	printf("defaultAuto: %d \n", defaultAuto);
	printf("crossLine: %d \n", crossLine);
	printf("sameSwitch: %d \n", sameSwitch);
	printf("eitherSwitch: %d \n", eitherSwitch);
	printf("sameScale: %d \n", sameScale);
	printf("eitherScale: %d \n", eitherScale);
}

int JankyAutoSelector::GetAutoMode(char switchPos, char scalePos){
	int* selectedPosition = position.GetSelected();
	int* selectedAction = action.GetSelected();
	int* selectedDelay = delay.GetSelected();

	printf("selected position: %d \n", *selectedPosition);
	printf("selected action: %d \n", *selectedAction);
	printf("selected delay: %d \n", *selectedDelay);
	if((switchPos == 'E')&&(scalePos == 'E')){
		printf("default auto bc no valid data \n");
		autoMode = DEFAULT_MODE;
	}
	else if(&defaultAuto==selectedAction){
		printf("default auto \n");
		autoMode = DEFAULT_MODE;
	}
	else if((&left==selectedPosition)&&(&sameScale==selectedAction)){
		if(scalePos == 'L'){
			printf("left start + same side scale \n");
			autoMode = L_SAME_SCALE;
		}
		else{
			printf("left start + auto line bc scale is R \n");
			autoMode = L_CROSS_AUTOLINE;
		}
	}
	else if((&right==selectedPosition)&&(&sameScale==selectedAction)){
		if(scalePos == 'R'){
			printf("right start + same side scale \n");
			autoMode = R_SAME_SCALE;
		}
		else{
			printf("right start + auto line \n");
			autoMode = R_CROSS_AUTOLINE;
		}
	}
	else if((&left==selectedPosition)&&(&eitherScale==selectedAction)){
		if(scalePos == 'L'){
			printf("left start + same side scale \n");
			autoMode = L_SAME_SCALE;
		}
		else{
			printf("left start + opposite scale \n");
			autoMode = L_OPPOSITE_SCALE;
		}
	}
	else if((&right==selectedPosition)&&(&eitherScale==selectedAction)){
		if(scalePos == 'R'){
			printf("right start + same side scale \n");
			autoMode = R_SAME_SCALE;
		}
		else{
			printf("right start + opposite scale \n");
			autoMode = R_OPPOSITE_SCALE;
		}
	}
	else if((&left==selectedPosition)&&(&crossLine==selectedAction)){
		printf("left start + auto line \n");
		autoMode = L_CROSS_AUTOLINE;
	}
	else if((&left==selectedPosition)&&(&sameSwitch==selectedAction)){
		if(switchPos == 'L'){
			printf("left start + same side switch \n");
			autoMode = L_SAME_SWITCH;
		}
		else{
			printf("left start + auto line bc switch is R \n");
			autoMode = L_CROSS_AUTOLINE;
		}
	}
	else if((&left==selectedPosition)&&(&eitherSwitch==selectedAction)){
		if(switchPos == 'L'){
			printf("left start + same side switch \n");
			autoMode = L_SAME_SWITCH;
		}
		else{
			printf("left start + opposite side switch \n");
			autoMode = L_OPPOSITE_SWITCH;
		}
	}
	else if(((&middle==selectedPosition)&&(&eitherSwitch==selectedAction))||((&middle==selectedPosition)&&(&crossLine==selectedAction))||((&middle==selectedPosition)&&(&sameSwitch==selectedAction))){
		printf("middle start + either switch \n");
		if(switchPos == 'L'){
			autoMode = M_LEFT_SWITCH;
		}
		else{
			autoMode = M_RIGHT_SWITCH;
		}
	}
	else if((&right==selectedPosition)&&(&crossLine==selectedAction)){
		printf("right start + auto line \n");
		autoMode = R_CROSS_AUTOLINE;
	}
	else if((&right==selectedPosition)&&(&sameSwitch==selectedAction)){
		if(switchPos == 'R'){
			printf("right start + same side switch \n");
			autoMode = R_SAME_SWITCH;
		}
		else{
			printf("right start + auto line \n");
			autoMode = R_CROSS_AUTOLINE;
		}
	}
	else if((&right==selectedPosition)&&(&eitherSwitch==selectedAction)){
		if(switchPos == 'R'){
			printf("right start + same side switch \n");
			autoMode = R_SAME_SWITCH;
		}
		else{
			printf("right start + opposite side switch \n");
			autoMode = R_OPPOSITE_SWITCH;
		}
	}
	return autoMode;
}
