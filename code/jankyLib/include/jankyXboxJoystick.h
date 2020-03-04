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

// Do NOT include this global header file. Include only what you need. --Mentor-Bob: #include "frc/WPILib.h"
#include "frc/Joystick.h"

#define BUTTON_A_PORT 1
#define BUTTON_B_PORT 2
#define BUTTON_X_PORT 3
#define BUTTON_Y_PORT 4
#define BUTTON_LB_PORT 5
#define BUTTON_RB_PORT 6
#define BUTTON_BACK_PORT 7
#define BUTTON_START_PORT 8
#define LEFT_STICK_BUTTON_PORT 9
#define RIGHT_STICK_BUTTON_PORT 10
#define LEFT_X_AXIS_CHANNEL 0 // before 1
#define LEFT_Y_AXIS_CHANNEL 1 // before 2
#define RIGHT_X_AXIS_CHANNEL 4
#define RIGHT_Y_AXIS_CHANNEL 5

class jankyXboxJoystick : public frc::Joystick {
public:
	jankyXboxJoystick(int portNumber);
	virtual ~jankyXboxJoystick(void);
	
	bool GetButtonA();
	bool GetButtonB();
	bool GetButtonX();
	bool GetButtonY();
	bool GetButtonLB();
	bool GetButtonRB();
	bool GetButtonBack();
	bool GetButtonStart();
	bool GetLeftStickButton();
	bool GetRightStickButton();
	float GetLeftThrottle();
	float GetRightThrottle();
	float GetLeftXAxis();
	float GetLeftYAxis();
	float GetRightXAxis();
	float GetRightYAxis();
	float GetLeftTwist();
	float GetRightTwist();
};
#endif



