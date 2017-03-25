#include "WPILib.h"

#define ULTRASONIC_PING 0
#define ULTRASONIC_ECHO 9

class Robot: public IterativeRobot
{
private:
	Ultrasonic * ultra;

	void RobotInit()
	{
		ultra = new Ultrasonic(ULTRASONIC_PING, ULTRASONIC_ECHO);
		// ultra.SetAutomaticMode(true);
		// double range = ultra.GetRangeInches();
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
		SmartDashboard::PutNumber("Ultrasonic", ultra->GetRangeInches());
	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Robot)
