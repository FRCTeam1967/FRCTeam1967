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
	return pow(Joystick::GetX()*(-1), 3);
}

float jankyDrivestick::GetY()
{
	return pow(Joystick::GetY(), 3);
}

float jankyDrivestick::GetJoystickTwist()
{
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

/*float LeftYAxis = pDriverStick->GetY();
if(abs(LeftYAxis) < 0.01)
{
	LeftYAxis = 0.0;
}
float LeftXAxis = pDriverStick->GetX();
if (abs(LeftXAxis) < 0.01)
{
	LeftXAxis = 0.0;
}
float LeftTwist = pDriverStick->GetTwist();
if(abs(LeftTwist) < 0.01)
{
	LeftTwist = 0.0;
}*/


