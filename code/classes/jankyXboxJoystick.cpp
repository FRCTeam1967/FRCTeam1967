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
 * order to be applied to the robot. 
 * JankyXboxJoystick class uses specifically the Xbox controller.
 **********************************************************************************************/ 

/************************
* Default constructor  *
************************/
	
jankyXboxJoystick::jankyXboxJoystick(int portNumber) : Joystick (portNumber)
{
		
}

jankyXboxJoystick::~jankyXboxJoystick()
{
	
}

/**
 * Get button value (true or false) of button A (bottom button). 
 */
bool jankyXboxJoystick::GetButtonA()
{
	return Joystick::GetRawButton(BUTTON_A_PORT);
}

/**
 * Get button value (true or false) of button B (right most button). 
 */
bool jankyXboxJoystick::GetButtonB()
{
	return Joystick::GetRawButton(BUTTON_B_PORT);
}

/**
 * Get button value (true or false) of button X (left most button). 
 */
bool jankyXboxJoystick::GetButtonX()
{
	return Joystick::GetRawButton(BUTTON_X_PORT);
}

/**
 * Get button value (true or false) of button Y (top button). 
 */
bool jankyXboxJoystick::GetButtonY()
{
	if (Joystick::GetRawButton(BUTTON_Y_PORT))
	{	
		printf("Button Y Pressed\n");
		return true;
	}
	else 
		return false;
	
	//return Joystick::GetRawButton(BUTTON_Y_PORT);	
}

/**
 * Get button value (true or false) of button A (bottom button). 
 */
bool jankyXboxJoystick::GetButtonLB()
{
	return Joystick::GetRawButton(BUTTON_LB_PORT);
}

/**
 * Get button value (true or false) of button A (bottom button). 
 */
bool jankyXboxJoystick::GetButtonRB()
{
	return Joystick::GetRawButton(BUTTON_RB_PORT);
}

/**
 * Get button value (true or false) of button A (bottom button). 
 */
bool jankyXboxJoystick::GetButtonBack()
{
	return Joystick::GetRawButton(BUTTON_BACK_PORT);
}

/**
 * Get button value (true or false) of button A (bottom button). 
 */
bool jankyXboxJoystick::GetButtonStart()
{
	return Joystick::GetRawButton(BUTTON_START_PORT);
}

/**
 * Returns float from left trigger
 */
float jankyXboxJoystick::GetLeftThrottle() 
{
	float leftThrottle = GetThrottle();
	if (leftThrottle > 0) {
		return leftThrottle;
	}
	else return 0;
}

/**
 * Returns float from right trigger
 */
float jankyXboxJoystick::GetRightThrottle() 
{
	float rightThrottle = GetThrottle();
	if (rightThrottle < 0) {
		rightThrottle = rightThrottle * -1;
		return rightThrottle;
	}
	else return 0;
}

/**
 * Get the state of the left thumbstick axis. Returns a float value.
 */
float jankyXboxJoystick::GetLeftXAxis ()
{
	float x = GetRawAxis(LEFT_X_AXIS_CHANNEL);
	return x;
}

/**
 * Get the state of the right thumbstick axis. Returns a float value.
 */
float jankyXboxJoystick::GetLeftYAxis()
{
	float y = (GetRawAxis(LEFT_Y_AXIS_CHANNEL));
	return y;
}

float jankyXboxJoystick::GetRightXAxis ()
{
	float x = GetRawAxis(RIGHT_X_AXIS_CHANNEL);
	return x;
}

float jankyXboxJoystick::GetRightYAxis ()
{
	float y = (GetRawAxis(RIGHT_Y_AXIS_CHANNEL));
	return y;
}
float jankyXboxJoystick::GetLeftTwist()
{
	float leftTwist = GetTwist();
	return leftTwist;	
}
float jankyXboxJoystick::GetRightTwist()
{
	float rightTwist = GetTwist();
	return rightTwist;	
}
