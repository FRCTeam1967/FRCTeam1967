/*
 * JankyAutoEntry.cpp
 *
 *  Created on: Feb 4, 2018
 *      Author: Maxine
 */

#include <JankyAutoEntry.h>
#include "frc/WPILib.h"
bool active;

JankyAutoEntry::JankyAutoEntry() {
	// TODO Auto-generated constructor stub
	active = false;
}

JankyAutoEntry::~JankyAutoEntry() {

}

bool JankyAutoEntry::IsComplete(){
	if(!active){
		active = true;
		Start();
		RunAction();
	}
	if(JobDone()){
		active = false;
		End();
		return true;
	}
	return false;
}

void JankyAutoEntry::Abort(){
	active=false;
	End();
}
