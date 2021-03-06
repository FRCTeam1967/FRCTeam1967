#ifndef _JANKYDRIVESTICK_H
#define _JANKYDRIVESTICK_H
//#include "frc/WPILib.h"
#include "frc/Joystick.h"

#define BUTTON_TRIG_PORT 1
#define BUTTON_3_PORT 3
#define BUTTON_5_PORT 5
#define BUTTON_2_PORT 2
#define BUTTON_6_PORT 6
#define BUTTON_4_PORT 4

class jankyDrivestick : public frc::Joystick {
public:
	jankyDrivestick(int portNumber);
	virtual ~jankyDrivestick(void);

	float GetX();
	float GetY();
	float GetTwist();
	float GetTrigger();
	bool Get3();
	bool Get5();
	bool Get2();
	bool Get6();
	bool Get4();
	bool Get7();
	bool Get8();
	bool Get9();
	bool Get10();
	float GetJoystickTwist();
	bool IsAnyTopButtonPressed();

};
#endif
