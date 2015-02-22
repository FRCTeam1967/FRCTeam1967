#include "WPILib.h"
#include "jankyXboxJoystick.h"
#include "jankyFoxLiftState.h"

#define DRIVE_JOYSTICK_PORT 0
#define GC_JOYSTICK_PORT 1

class Robot: public IterativeRobot
{
	const static int fLeft=2;
	const static int rLeft=4;
	const static int fRight=1;
	const static int rRight=3;
	RobotDrive meghaRobot;
	jankyXboxJoystick* gameComponent;
	Joystick*driver;
	JankyFoxliftState*foxlift;

public:
	Robot(): meghaRobot(fLeft, rLeft, fRight, rRight)
	{
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
		SmartDashboard::PutNumber("Twist", driver->GetZ());
		SmartDashboard::PutNumber("XAxis", driver->GetX());
		SmartDashboard::PutBoolean("limit Switch Down closed", foxlift->IsLSwitchDownClosed());
		SmartDashboard::PutNumber("Box Lift Speed", foxlift->motorLift->Get());
		SmartDashboard::PutNumber("Intake Roller 1 Speed", foxlift->motorRoller1->Get());
		SmartDashboard::PutNumber("Intake Roller 2 Speed", foxlift->motorRoller2->Get());
		SmartDashboard::PutBoolean("Reorientation", foxlift->reorientation->Get());
		SmartDashboard::PutBoolean("Singulation IN/OUT", foxlift->singulationOne->Get());
		SmartDashboard::PutBoolean("Singulation UP/DOWN", foxlift->singulationTwo->Get());
		SmartDashboard::PutBoolean("Intake pistons", foxlift->rollerPistons->Get());
		SmartDashboard::PutNumber("Get Throttle", driver->GetThrottle());
		//MECANUM DRIVE
		float yValue = driver->GetY();
		float xValue = driver->GetX()*(-1);
		//this is apparently changing the twist
		float rotation = driver->GetThrottle();
		// GetZ() apparently is changed by the lever at the bottom.
		//driver->GetZ() = 0;
		//driver->SetAxisChannel(Joystick::kTwistAxis, 2);
		meghaRobot.MecanumDrive_Cartesian(xValue, yValue, rotation, 0.0);
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
