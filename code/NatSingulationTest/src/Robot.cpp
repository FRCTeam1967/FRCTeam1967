#include "WPILib.h"
#include "Joystick.h"
#include "NatSolenoidTestJig.h"

#define JOYSTICK 0
#define SOLENOID_PORT 0
#define SOLENOID2_PORT 1

class Robot: public IterativeRobot
{
	Joystick * stick;
	NatSolenoidTestJig * singOne;
	NatSolenoidTestJig * singTwo;


public:
	Robot(){
		stick = NULL;
		singOne = NULL;
		singTwo = NULL;
		//printf("I done something\n");
	}
	~Robot() {
		delete stick;
		delete singOne;
		delete singTwo;
	}
private:

	void RobotInit()
	{
		//printf("init\n");
		stick = new Joystick(JOYSTICK);
		//printf("joystick\n");
		//printf("solenoidtest 1\n");
		singOne = new NatSolenoidTestJig(SOLENOID_PORT);
		//printf("solenoid 1\n");
		singTwo = new NatSolenoidTestJig(SOLENOID2_PORT);
		//printf("solenoid 2\n");

		// piston starts out
	}

	//Get if any of the top buttons on the joystick is pressed
	bool GetTopButtons()
	{
		return stick->GetRawButton(2) || stick->GetRawButton(3) || stick->GetRawButton(4) ||
			stick->GetRawButton(5) || stick->GetRawButton(6);
	}

	void TeleopPeriodic()
	{
		//printf("I done it\n");
		//put the values of both the solenoids on SmartDashboard
		SmartDashboard::PutBoolean("Solenoid One", singOne->GetState());
		SmartDashboard::PutBoolean("Solenoid Two", singTwo->GetState());

		// Piston One goes out when any of the top buttons are pressed
        if (GetTopButtons() == true)
        {
        	singOne->OverrideEnable();
        	printf("Piston One Out\n");
        }
        // if none of the top buttons are pressed then Piston One comes in
        else if(GetTopButtons() == false)
        {
        	singOne->OverrideDisable();
        }
        // Piston Two goes out if Piston One is out and the trigger is pulled
        if (stick->GetTrigger() == true && GetTopButtons()==true)
            {
            		 singTwo->OverrideEnable();
            		 printf("Piston Two Out\n");
             }
        //If the trigger is not pulled or none of the top buttons are pushed Piston Two comes in
        else if (stick->GetTrigger() == false || GetTopButtons()== false)
        {
        	singTwo->OverrideDisable();
        }

	}

};

START_ROBOT_CLASS(Robot);
