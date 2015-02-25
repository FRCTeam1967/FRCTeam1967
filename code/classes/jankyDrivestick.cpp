#include "WPILib.h"
#include <math.h>
#include "jankyDrivestick.h"

jankyDrivestick::jankyDrivestick(int portNumber) : Joystick (portNumber)
{

}

jankyDrivestick::~jankyDrivestick()
{

}

float jankyDrivestick::GetX()
{
	float LeftXAxis = Joystick::GetX();
	if (abs(LeftXAxis) < DEADBAND_SIZE)
	{
		LeftXAxis = 0.0;
	}
	return pow(LeftXAxis*(-1), 3);
}

float jankyDrivestick::GetY()
{
	float LeftYAxis = Joystick::GetY();
	if(abs(LeftYAxis) < DEADBAND_SIZE)
	{
		LeftYAxis = 0.0;
	}
	return pow(Joystick::GetY(), 3);
}

float jankyDrivestick::GetJoystickTwist()
{
	float LeftTwist = Joystick::GetThrottle();
	if(abs(LeftTwist) < DEADBAND_SIZE)
	{
		LeftTwist = 0.0;
	}
	return pow(Joystick::GetThrottle(), 3);
}

bool jankyDrivestick::IsAnyTopButtonPressed()
{
	if (Joystick::GetRawButton(5) == true ||
	   Joystick::GetRawButton(3) == true ||
	   Joystick::GetRawButton(2) == true ||
	   Joystick::GetRawButton(6) == true ||
	   Joystick::GetRawButton(4) == true)
    {
		return true;
    }
	else
	{
		return false;
	}
}
