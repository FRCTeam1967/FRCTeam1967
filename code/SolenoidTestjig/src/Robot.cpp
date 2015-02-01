#include "WPILib.h"
#include "solenoidTestjig.h"
#include "jankyTask.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw;

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		solenoidTestjig * solenoid = new solenoidTestjig(0);
		solenoid->Go();
	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
