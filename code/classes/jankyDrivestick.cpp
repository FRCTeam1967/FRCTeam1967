#include "WPILib.h"
#include <math.h>
#include "jankyDrivestick.h"
#define DEADBAND_SIZE 0.05

jankyDrivestick::jankyDrivestick(int portNumber) : Joystick (portNumber)
{

}

jankyDrivestick::~jankyDrivestick()
{

}

float jankyDrivestick::GetX()
{
	float LeftXAxis = Joystick::GetX()*(-1.0);
	//return LeftXAxis;
	/*if (abs(LeftXAxis) < DEADBAND_SIZE)
	{
		LeftXAxis = 0.0;
	}*/
	if (LeftXAxis < DEADBAND_SIZE && LeftXAxis > DEADBAND_SIZE*(-1.0))
	{
		LeftXAxis = 0.0;
	}
	return LeftXAxis;
	//return pow(LeftXAxis*(-1), 3);
}

float jankyDrivestick::GetY()
{
	float LeftYAxis = Joystick::GetY();
	if (LeftYAxis < DEADBAND_SIZE && LeftYAxis > DEADBAND_SIZE*(-1.0))
		{
			LeftYAxis = 0.0;
		}
		return LeftYAxis;
	/*return LeftYAxis;
	if(abs(LeftYAxis) < DEADBAND_SIZE)
	{
		LeftYAxis = 0.0;
	}
*/
	//return pow(LeftYAxis, 3);
}

float jankyDrivestick::GetJoystickTwist()
{
	float LeftTwist = Joystick::GetThrottle();
	if (LeftTwist < DEADBAND_SIZE && LeftTwist > DEADBAND_SIZE*(-1.0))
		{
			LeftTwist = 0.0;
		}
		return LeftTwist;
	/*return LeftTwist;
	if(abs(LeftTwist) < DEADBAND_SIZE)
	{
		LeftTwist = 0.0;
	}
*/
	//return pow(LeftTwist, 3);
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
