/*
 * JankyAutoEntry.cpp
 *
 *  Created on: Feb 4, 2018
 *      Author: Maxine
 */

#include <JankyAutoEntry.h>
#include "WPILib.h"
bool active;

JankyAutoEntry::JankyAutoEntry() {
	// TODO Auto-generated constructor stub
	active = false;
}

JankyAutoEntry::~JankyAutoEntry() {
	// TODO Auto-generated destructor stub

}

bool JankyAutoEntry::IsComplete(){
	if(!active){
		active = true;
		RunAction();
		Start();
	}
	if(JobDone()){
		active = false;
		End();
		return true;
	}
	return false;
}

void JankyAutoEntry::Abort(){
	End();
}
