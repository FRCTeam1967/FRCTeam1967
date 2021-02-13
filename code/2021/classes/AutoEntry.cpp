/*
 * AutoEntry.cpp
 *
 *  Created on: Feb 13, 2020
 *      Author: Maddie
 */

#include <AutoEntry.h>

bool active;

AutoEntry::AutoEntry() {
	// TODO Auto-generated constructor stub
	active = false;
}

AutoEntry::~AutoEntry() {

}

bool AutoEntry::IsComplete(){
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

void AutoEntry::Abort(){
	End();
}