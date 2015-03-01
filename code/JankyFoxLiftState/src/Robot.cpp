#include "WPILib.h"
#include "jankyXboxJoystick.h"
#include "jankyFoxLiftState.h"

#define DRIVE_JOYSTICK_PORT 0
#define GC_JOYSTICK_PORT 1
#define DEADBAND_SIZE .3

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
	LiveWindow *lw;

public:
	Robot(): meghaRobot(fLeft, rLeft, fRight, rRight)
	{
		printf("in robot constructor \n");
		gameComponent = NULL;
		driver = NULL;
		foxlift = NULL;
		lw = NULL;
	}
	~Robot(){
		delete gameComponent;
		delete driver;
		delete foxlift;
		// RMW: Does not get deleted because it was not 'new' allocated -- delete lw;
	}
private:


	void RobotInit()
	{
		printf("in robot init \n");
		lw = LiveWindow::GetInstance();

		driver = new Joystick(DRIVE_JOYSTICK_PORT);
		gameComponent = new jankyXboxJoystick(GC_JOYSTICK_PORT);
		foxlift = new JankyFoxliftState();

		foxlift->SetFoxlift();// put in autonomous and teleop init


		/*Compressor *compressor = new Compressor(0);
		SmartDashboard::PutBoolean("Compressor Status is Okay?", !compressor->StatusIsFatal());
		//SmartDashboard::PutBoolean("Compressor is Closed Loop Control", compressor->GetClosedLoopControl());
		SmartDashboard::PutBoolean("Compressor Shorted Fault?", compressor->GetCompressorShortedFault());
		SmartDashboard::PutBoolean("Compressor Shorted Sticky Fault?", compressor->GetCompressorShortedStickyFault());
		delete compressor;*/
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
		SmartDashboard::PutNumber("Get Timer for rollerIn", foxlift->rollerInTimer->Get());
		SmartDashboard::PutNumber("Get Timer for waitForUndo", foxlift->preRollerTimer->Get());
		SmartDashboard::PutNumber("Get Timer for rollerOut", foxlift->rollerOutTimer->Get());
		SmartDashboard::PutNumber("Ultrasonic", foxlift->toteIn->GetRangeInches());

		SmartDashboard::PutNumber("Tote In", foxlift->ToteIn());
		//MECANUM DRIVE
		float yValue = driver->GetY();
		float xValue = driver->GetX()*(-1);
		//this is apparently changing the twist
		float rotation = driver->GetThrottle();

		meghaRobot.MecanumDrive_Cartesian(xValue, yValue, rotation, 0.0);
		//Override
		if(gameComponent->GetButtonRB() == true){
			foxlift->ManualOverrideOn();
		}
		if(gameComponent->GetButtonRB() == false){
			foxlift->ManualOverrideOff();
		}
		//ForkLift
		if(abs(gameComponent->GetLeftYAxis()) <= DEADBAND_SIZE){
			foxlift->StopRollers();
		}
		if(gameComponent->GetLeftYAxis() > DEADBAND_SIZE){
			foxlift->PushOutTote();
		}
		if(gameComponent->GetLeftYAxis() < (-1)*(DEADBAND_SIZE)){
			foxlift->SuckInTote();
		}
		//Boxlift
		if (gameComponent->GetButtonY() == true){
			foxlift->GoUp();
		}
        if (gameComponent->GetButtonB() == true){
			printf("ButtonB is pressed\n");
				foxlift->StopLift();
		}
		 if(gameComponent->GetButtonA() == true){
			foxlift->GoDown();
		}
		//Reorientation
		if (gameComponent->GetButtonLB() == true){
			foxlift->Reorient();
		}
		//Singulation
		if (driver->GetRawButton(2) == true && driver->GetTrigger() == true){
			foxlift->SingulateTwo();
		}
		else if (driver->GetRawButton(2) == true && driver->GetTrigger() == false){
			foxlift->SingulateOne();
		}
		//Done Singulating/Reorienting
		if (driver->GetRawButton(2) == false && gameComponent->GetButtonLB() == false){
			foxlift->DoneSingReor();
		}

	}

	void TestInit()
	{
		foxlift->Terminate();
	}
	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
