#include "WPILib.h"
//#include "Commands/Command.h"
//#include "Commands/ExampleCommand.h"
//#include "CommandBase.h"
//#include "ForkliftTest.h"
//#include "NatSolenoidTestJig.h"
//#define SINGULATION_ONE 3
//#define SINGULATION_TWO 4

class Robot: public IterativeRobot
{
private:
	int auto1;
	int auto2;
	//LiveWindow *lw;
	SendableChooser*chooser;


	void RobotInit()
	{
		//CommandBase::init();
		//lw = LiveWindow::GetInstance();

		chooser = new SendableChooser();
		//adding the location of the ints auto1 and auto2
		// need to addDefault otherwise it won't work
		chooser->AddDefault("Autonomous One", &auto1);
		chooser->AddObject("Autonomous Two", &auto2);

		SmartDashboard::PutString("My Test", "Natasha was here");
		//put the different options on SmartDashboard
		SmartDashboard::PutData("Autonomous modes", chooser);

	}

	void AutonomousInit()
	{
		if (&auto1 == chooser->GetSelected())
		{
			printf("auto1 running\n");
		}
		else if(&auto2 == chooser->GetSelected())
		{
			printf("auto2 running\n");
		}
		else
		{
			printf("something's wrong\n");
		}
		//SmartDashboard::PutData("Selected", autonomousCommand);
	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{

	}

	void TestPeriodic()
	{
		//lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
