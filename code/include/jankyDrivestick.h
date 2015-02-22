#ifndef _JANKYDRIVESTICK_H
#define _JANKYDRIVESTICK_H
#include "WPILib.h"

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
