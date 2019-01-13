#include "WPILib.h"

class Robot: public IterativeRobot
{
	DigitalInput sonar;

public:
	Robot():sonar(3)
		{}
private:
	//LiveWindow *lw;

	/*void RobotInit()
	{
		//lw = LiveWindow::GetInstance();
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
	*/
	void TeleopPeriodic()
	{
		SmartDashboard::PutNumber("Pulses", sonar.Get());
	}

	/*void TestPeriodic()
	{
		//lw->Run();
	}*/
};

START_ROBOT_CLASS(Robot);
