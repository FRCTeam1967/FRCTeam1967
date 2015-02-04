#include "WPILib.h"

#define ANALOG_SONARSENSOR_PORT 0

class Robot: public IterativeRobot
{
	AnalogOutput * sonarSensor;
public:
	Robot(void)
	{
		sonarSensor = NULL;
	}

	~Robot(void)
	{
		delete sonarSensor;
	}

	void RobotInit()
	{
		sonarSensor = new AnalogOutput(ANALOG_SONARSENSOR_PORT);
		printf("End of RobotInit\n");
	}

	void AutonomousInit()
	{
		AutonomousInit();
	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		SmartDashboard::PutNumber("Sonar Sensor Voltage", sonarSensor->GetVoltage());
	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Robot);
