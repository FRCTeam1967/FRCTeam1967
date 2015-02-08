#include "WPIlib.h"
#include "ForkliftTest.h"
#include "NatSolenoidTestJig.h"

JankyForklift::JankyForklift(int talonOneChannel, int talonTwoChannel, int pistonOneChannel)

{
	left = new Talon(talonOneChannel);
	right = new Talon(talonTwoChannel);
	sold = new NatSolenoidTestJig(pistonOneChannel);
}

JankyForklift::~JankyForklift(){
	delete left;
	delete right;
	delete sold;
}


void JankyForklift::pullIn(){
	left->Set(-1.0);
	right->Set(1.0);
	sold->OverrideDisable();
}

void JankyForklift::pushOut(){
	left ->Set(1.0);
	right->Set(-1.0);
	sold ->OverrideEnable();
}

