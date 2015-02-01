#include "WPILib.h"
#include "jankyXboxJoystick.h"


class Robot: public IterativeRobot
{
	jankyXboxJoystick*stick;

public:
	Robot(){

}
	~Robot(){

	}
private:


	void RobotInit()
	{

	}
 /*
	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}*/

	void TeleopPeriodic()
	{

	}

	/*void TestPeriodic()
	{
		lw->Run();
	}*/
};

START_ROBOT_CLASS(Robot);
