#include "Robot.h"
#include <iostream>
#include <SmartDashboard/SmartDashboard.h>
#include "Giraffe.h" // # + include + " nameOfHeaderFile "
#include "WPILib.h"

class Robot : public frc::IterativeRobot {
	Solenoid piston1; //declare object

	//pointer
	Solenoid * piston2; //className + * + object name + ;
public:
	//constructor
	Robot()
	{
		piston2 = NULL; // objectName + = + NULL + ;
	}

	//deconstructor
	~Robot()
	{
		delete piston2; // delete + objectName + ;
	}

	void Robot::RobotInit()
	{
		piston1(5);
		piston2 = new Solenoid(7); // objectName + = + new + className + ( parameter ) + ;
	}

	void Robot::AutonomousInit()
	{

	}

	void Robot::AutonomousPeriodic()
	{

	}

	void Robot::TeleopInit()
	{

	}

	void Robot::TeleopPeriodic()
	{
		piston1.Set(true); // object + . + method + ( parameter )
		piston2->Set(true); //when using pointers, use an arrow
	}

	void Robot::TestPeriodic()
	{

	}
private:

};

START_ROBOT_CLASS(Robot)
