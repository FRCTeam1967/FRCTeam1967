#include "WPILib.h"
#include "solenoidTestJig.h"
#include "jankyXboxJoystick.h"
//#include "jankyTask.h"

#define SOLENOIDTESTJIG_PORT 0
#define SOLTESTJIG_PORT 0

class Robot: public IterativeRobot
{
	jankyXboxJoystick * stick;
		SolenoidTestJig * sold;
public:
		Robot() {
				stick = NULL;
				sold = NULL;
			}
			~Robot() {
				delete stick;
				delete sold;
			}

private:
	//LiveWindow *lw;

	void RobotInit()
	{
		//lw = LiveWindow::GetInstance();
		stick = new jankyXboxJoystick(SOLENOIDTESTJIG_PORT);
		sold = new SolenoidTestJig(SOLTESTJIG_PORT);
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
		if (stick->GetButtonA() == true) {
							sold->OverrideEnable();
							printf("It got here\n");
		}
		printf("after getbutton\n");

	}

	void TestPeriodic()
	{
		//lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
