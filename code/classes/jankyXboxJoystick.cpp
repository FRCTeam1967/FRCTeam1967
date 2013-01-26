/******************************
 * JankyXboxJoystick.cpp      *
 *                            *
 *  Created on: Jan 21, 2013  *
 *  Author: Team1967          *
 ******************************/

#include "jankyXboxJoystick.h"
#include "WPILib.h"

/**********************************************************************************************
 * This is a demo program showing the use of the Joystick class. 
 * The Joystick class will automatically call the controls on the joystick in 
 * order to be applied onto the robot. 
 * JankyXboxJoystick class uses specifically the Xbox controller.
 **********************************************************************************************/ 
//JankyXboxJoystick : public Joystick
//{


/************************
* Default constructor  *
************************/
	
jankyXboxJoystick::jankyXboxJoystick(int portNumber) : Joystick (portNumber)
{
		buttonA = new JoystickButton (this, BUTTON_A),
        buttonB = new JoystickButton (this, BUTTON_B),
        buttonX = new JoystickButton (this, BUTTON_X),
        buttonY = new JoystickButton (this, BUTTON_Y),
        buttonLB = new JoystickButton (this, BUTTON_LB),
        buttonRB = new JoystickButton (this, BUTTON_RB),
        buttonBack = new JoystickButton (this, BUTTON_BACK),
        buttonStart = new JoystickButton (this, BUTTON_START),
        buttonThumbLeftButton = new JoystickButton (this, THUMBSTICK_LEFT_BUTTON),
        buttonThumbRightButton = new JoystickButton (this, THUMBSTICK_RIGHT_BUTTON);
}

jankyXboxJoystick::~jankyXboxJoystick()
{
	
}

bool jankyXboxJoystick::GetButtonA()
{
	bool buttonAbool = buttonA->Get();
}

bool jankyXboxJoystick::GetButtonB()
{
	bool buttonBbool = buttonB->Get();
}

bool jankyXboxJoystick::GetButtonX()
{
	bool buttonXbool = buttonX->Get();
}

bool jankyXboxJoystick::GetButtonY()
{
	bool buttonYbool = buttonY->Get();
}

bool jankyXboxJoystick::GetButtonLB()
{
	bool buttonLBbool = buttonLB->Get();
}

bool jankyXboxJoystick::GetButtonRB()
{
	bool buttonRBbool = buttonRB->Get();
}

bool jankyXboxJoystick::GetButtonBack()
{
	bool buttonStartBool = buttonBack->Get();
}

bool jankyXboxJoystick::GetButtonStart()
{
	bool buttonBackBool = buttonStart->Get();
}

float jankyXboxJoystick::GetTriggerValue() 
{
	float trigger = GetThrottle();
}

float jankyXboxJoystick::UseTankDriveLeft ()
{
	float x = 0 - GetY();
}

float jankyXboxJoystick::UseTankDriveRight ()
{
	float y = 0 - GetRawAxis(RIGHT_Y_AXIS_CHANNEL);
}


