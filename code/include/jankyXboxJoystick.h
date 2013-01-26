/*
 * JankyXboxJoystick.h
 *
 *  Created on: Jan 21, 2013
 *      Author: Team1967 - The Janksters
 */

/*
 * Description:
 */

#ifndef _JANKYXBOXJOYSTICK_H
#define _JANKYXBOXJOYSTICK_H

#include "WPILib.h"

#define BUTTON_A 1
#define BUTTON_B 2
#define BUTTON_X 3
#define BUTTON_Y 4
#define BUTTON_LB 5
#define BUTTON_RB 6
#define BUTTON_BACK 7
#define BUTTON_START 8
#define THUMBSTICK_LEFT_BUTTON 9
#define THUMBSTICK_RIGHT_BUTTON 10
#define RIGHT_Y_AXIS_CHANNEL 5

class jankyXboxJoystick : public Joystick {
public:
	jankyXboxJoystick(int portNumber);
	virtual ~jankyXboxJoystick(void);
	
	bool jankyXboxJoystick::GetButtonA();
	bool jankyXboxJoystick::GetButtonB();
	bool jankyXboxJoystick::GetButtonX();
	bool jankyXboxJoystick::GetButtonY();
	bool jankyXboxJoystick::GetButtonLB();
	bool jankyXboxJoystick::GetButtonRB();
	bool jankyXboxJoystick::GetButtonBack();
	bool jankyXboxJoystick::GetButtonStart();
	float jankyXboxJoystick::GetTriggerValue();
	float jankyXboxJoystick::UseTankDriveLeft();
	float jankyXboxJoystick::UseTankDriveRight();
	
	JoystickButton * buttonA;
	JoystickButton * buttonB;
	JoystickButton * buttonX;
	JoystickButton * buttonY;
	JoystickButton * buttonLB;
	JoystickButton * buttonRB;
	JoystickButton * buttonBack;
	JoystickButton * buttonStart;
	JoystickButton * buttonThumbLeftButton;
	JoystickButton * buttonThumbRightButton;
	JoystickButton * buttonThumbLeftAxis;
	JoystickButton * buttonThumbRightAxis;
	static const int leftthumbstickNumber;
	static const int rightthumbstickNumber;
	
};
#endif



