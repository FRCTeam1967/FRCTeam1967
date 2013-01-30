#include "CounterTaskTestjig.h"
#include "jankyTask.h"
#include "WPILib.h"

//Constructor
CounterTaskTestjig::CounterTaskTestjig()
{
	counter = 0;
}

//Destructor
CounterTaskTestjig::~CounterTaskTestjig()
{
	
}

void CounterTaskTestjig::Run()
{
	counter++;
}

int CounterTaskTestjig::ReturnCounter()
{
	return counter;
}