#include "WPILib.h"
#include "jankyAccelerometer.h"

/*
 * Construct instance of an accelerator that runs in its own task.
 *
*/
JankyAccelerometer::JankyAccelerometer()
{
	pAccel = new BuiltInAccelerometer(Accelerometer::kRange_4G);
}

/*
 * Destructor
 */
JankyAccelerometer::~JankyAccelerometer()
{
	delete pAccel;
}

double JankyAccelerometer::returnX()
{
	double xVal = pAccel->GetX();
	return xVal;
}

double JankyAccelerometer::returnY()
{
	double yVal = pAccel->GetY();
	return yVal;
}


void JankyAccelerometer::Reset(void)
{

}

bool JankyAccelerometer::Go(void)
{

}

void JankyAccelerometer::Run(void)
{
	SmartDashboard::PutNumber ("X Coordinate", xVal);
	SmartDashboard::PutNumber ("Y Coordinate", yVal);
}

bool JankyAccelerometer::isDone(void)
{

}
