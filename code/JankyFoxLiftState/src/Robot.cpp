#include "WPILib.h"
#include "jankyXboxJoystick.h"
#include "jankyFoxLiftState.h"

#define DRIVE_JOYSTICK_PORT 0
#define GC_JOYSTICK_PORT 1

class Robot: public IterativeRobot
{
	jankyXboxJoystick* gameComponent;
	Joystick*driver;
	JankyFoxliftState*foxlift;

public:
	Robot(){
		printf("in robot constructor \n");
		gameComponent = NULL;
		driver = NULL;
		foxlift = NULL;
	}
	~Robot(){
		delete gameComponent;
		delete driver;
		delete foxlift;
	}
private:


	void RobotInit()
	{
		printf("in robot init \n");
		driver = new Joystick(DRIVE_JOYSTICK_PORT);
		gameComponent = new jankyXboxJoystick(GC_JOYSTICK_PORT);
		foxlift = new JankyFoxliftState();

		foxlift->SetFoxlift();
	}

	void TeleopPeriodic()
	{
		SmartDashboard::PutBoolean("limit Switch Top closed", foxlift->IsLSwitchTopClosed());
		SmartDashboard::PutBoolean("limit Switch Down closed", foxlift->IsLSwitchDownClosed());
		SmartDashboard::PutNumber("Box Lift Speed", foxlift->motorLift->Get());
		SmartDashboard::PutNumber("Intake Roller 1 Speed", foxlift->motorRoller1->Get());
		SmartDashboard::PutNumber("Intake Roller 2 Speed", foxlift->motorRoller2->Get());
		SmartDashboard::PutBoolean("Reorientation", foxlift->reorientation->Get());
		SmartDashboard::PutBoolean("Singulation IN/OUT", foxlift->singulationOne->Get());
		SmartDashboard::PutBoolean("Singulation UP/DOWN", foxlift->singulationTwo->Get());
		SmartDashboard::PutBoolean("Intake pistons", foxlift->rollerPistons->Get());

		//Boxlift
		if (gameComponent->GetButtonY() == true){
			foxlift->GoUp();
		}
		else if(gameComponent->GetButtonA() == true){
			foxlift->GoDown();
		}
		//Reorientation
		else if (gameComponent->GetButtonLB() == true){
			foxlift->Reorient();
		}
		else if (gameComponent->GetButtonLB() == false){
			foxlift->DoneReorienting();
		}
		//Singulation
		else if (driver->GetRawButton(2) == true && driver->GetTrigger() == true){
			foxlift->SingulateTwo();
		}
		else if (driver->GetRawButton(2) == true && driver->GetTrigger() == false){
			foxlift->SingulateOne();
		}
		else if (driver->GetRawButton(2) == false){
			foxlift->DoneSingulating();
		}
	}

	/*void TestPeriodic()
	{
		lw->Run();
	}*/
};

START_ROBOT_CLASS(Robot);
