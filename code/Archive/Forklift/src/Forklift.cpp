#include "WPIlib.h"
#include "Forklift.h"

JankyForklift::JankyForklift(int talonOneChannel, int talonTwoChannel)
{
	left = new Talon(talonOneChannel);
	right = new Talon(talonTwoChannel);
}
JankyForklift::~JankyForklift(){
	delete left;
	delete right;
}

void JankyForklift::pullIn()
{
	//this->left;
	left->Set(-1.0);
	right->Set(-1.0);
}

void JankyForklift::pushOut()
{
	left ->Set(1.0);
	right->Set(1.0);
}
