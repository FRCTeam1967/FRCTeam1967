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

void JankyAutoEntry::Start()
{
	active = true;
	RunAction();
}

void JankyAutoEntry::End()
{
	active = false;
}

bool JankyAutoEntry::GetActive()
{
	return active;
}
