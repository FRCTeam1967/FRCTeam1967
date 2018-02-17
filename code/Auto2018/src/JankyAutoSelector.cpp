/*
 * JankyAutoSelector.cpp
 *
 *  Created on: Jan 18, 2018
 *      Author: AnishaKabir
 */

#include "JankyAutoSelector.h"
#include "WPILib.h"

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
#define SAME 5

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
int same = SAME;

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
	position.AddDefault("Left", &left);
	position.AddObject("Middle", &middle);
	position.AddObject("Right", &right);
	SmartDashboard::PutData("Position", &position);

	delay.AddDefault("0 seconds", &zero_sec);
	delay.AddObject("1 second", &one_sec);
	delay.AddObject("2 seconds", &two_sec);
	delay.AddObject("3 seconds", &three_sec);
	delay.AddObject("4 seconds", &four_sec);
	SmartDashboard::PutData("Delay", &delay);

	action.AddDefault("Default (does nothing)", &defaultAuto);
	action.AddObject("Cross Auto Line", &crossLine);
	action.AddObject("Our Side Switch", &sameSwitch);
	action.AddObject("Either Switch", &eitherSwitch);
	SmartDashboard::PutData("Action", &action);

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
	printf("left: %p \n", (void*)left);
	printf("middle: %p \n", (void*)middle);
	printf("right: %p \n", (void*)right);
	printf("zero_sec: %p \n", (void*)zero_sec);
	printf("one_sec: %p \n", (void*)one_sec);
	printf("two_sec: %p \n", (void*)two_sec);
	printf("three_sec: %p \n", (void*)three_sec);
	printf("four_sec: %p \n", (void*)four_sec);
	printf("defaultAuto: %p \n", (void*)defaultAuto);
	printf("crossLine: %p \n", (void*)crossLine);
	printf("sameSwitch: %p \n", (void*)sameSwitch);
	printf("eitherSwitch: %p \n", (void*)eitherSwitch);
}

int JankyAutoSelector::GetAutoMode(char switchPos){
	int* selectedPosition = position.GetSelected();
	int* selectedAction = action.GetSelected();

	printf("selected position: %p \n", (void*)selectedPosition);
	printf("selected action: %p \n", (void*)selectedAction);
	if(switchPos == 'E'){
		printf("default auto \n");
		autoMode = DEFAULT_MODE;
	}
	else if(&defaultAuto==selectedAction){
		printf("default auto \n");
		autoMode = DEFAULT_MODE;
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
			printf("left start + auto line \n");
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
