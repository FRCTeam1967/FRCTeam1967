#include "WPILib.h"
#include "jankySonarSensorTest.h"
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
		jankySonarSensorTest * sensor = new jankySonarSensorTest(0);
		jankySonarSensorTest->Go();

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
