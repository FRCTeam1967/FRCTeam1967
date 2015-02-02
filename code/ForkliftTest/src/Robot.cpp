#include "WPILib.h"
#include "ForkliftTest.h"
#include "jankyXboxJoystick.h"

#define FORKLIFT_PORT 1
#define ROLLER_LEFT 5
#define ROLLER_RIGHT 6

class Robot: public IterativeRobot
{
	jankyXboxJoystick*stick;
	JankyForklift*forky;

public:
	Robot(){
		stick = NULL;
		forky = NULL;
}
	~Robot(){
		delete stick;
		delete forky;
	}
private:


	void RobotInit()
	{
		stick = new jankyXboxJoystick(FORKLIFT_PORT);
		forky = new JankyForklift(ROLLER_LEFT, ROLLER_RIGHT);
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
		if (stick->GetButtonA()== true){
			forky->pullIn();
		}
		if (stick->GetButtonB() == true){
			forky->pushOut();
		}
	}

	/*void TestPeriodic()
	{
		lw->Run();
	}*/
};

START_ROBOT_CLASS(Robot);
