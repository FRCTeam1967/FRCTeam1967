#include "CounterTask.h"
#include "jankyTask.h"
#include "WPILib.h"

//Constructor
CounterTask::CounterTask()
{
	counter = 0;
}

//Destructor
CounterTask::~CounterTask()
{
	
}

void CounterTask::Run()
{
	counter++;
}

int CounterTask::ReturnCounter()
{
	return counter;
}