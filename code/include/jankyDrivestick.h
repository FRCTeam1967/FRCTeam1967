#ifndef _JANKYDRIVESTICK_H
#define _JANKYDRIVESTICK_H
#include "WPILib.h"

#define DEADBAND_SIZE 0.01 //need to adjust this size for competition

class jankyDrivestick : public Joystick {
public:
	jankyDrivestick(int portNumber);
	virtual ~jankyDrivestick(void);

	float GetX();
	float GetY();
	float GetJoystickTwist();
	bool IsAnyTopButtonPressed();
};
#endif
