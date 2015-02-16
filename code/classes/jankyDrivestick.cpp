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
	return pow(Joystick::GetX(), 3);
}

float jankyDrivestick::GetY()
{
	return pow(Joystick::GetY(), 3);
}

float jankyDrivestick::GetTwist()
{
	return pow(Joystick::GetTwist(), 3);
}

/*bool jankyDrivestick::Get3()
{
	return Joystick::GetRawButton();
}

bool jankyDrivestick::Get5()
{
	return Joystick::GetRawButton(BUTTON_5_PORT);
}

bool jankyDrivestick::Get2()
{
	return Joystick::GetRawButton(BUTTON_2_PORT);
}

bool jankyDrivestick::Get6()
{
	return Joystick::GetRawButton(BUTTON_6_PORT);
}

/*bool jankyDrivestick::Get3()
{
	return Joystick::GetRawButton(BUTTON_4_PORT);
}*/

float jankyDrivestick::GetTrigger()
{
	return Joystick::GetRawButton(BUTTON_TRIG_PORT);
}

