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

#define BUTTON_A_PORT 1
#define BUTTON_B_PORT 2
#define BUTTON_X_PORT 3
#define BUTTON_Y_PORT 4
#define BUTTON_LB_PORT 5
#define BUTTON_RB_PORT 6
#define BUTTON_BACK_PORT 7
#define BUTTON_START_PORT 8
#define LEFT_X_AXIS_CHANNEL 1
#define LEFT_Y_AXIS_CHANNEL 2
#define RIGHT_X_AXIS_CHANNEL 4
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
	float jankyXboxJoystick::GetLeftThrottle();
	float jankyXboxJoystick::GetRightThrottle();
	float jankyXboxJoystick::GetLeftXAxis();
	float jankyXboxJoystick::GetLeftYAxis();
	float jankyXboxJoystick::GetRightXAxis();
	float jankyXboxJoystick::GetRightYAxis();
	
};
#endif



