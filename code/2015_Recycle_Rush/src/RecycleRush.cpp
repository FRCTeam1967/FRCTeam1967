#include "WPILib.h"
#include "jankyAutonomousState.h"
#include "jankyfoxliftState.h"
#include "jankyXboxJoystick.h"
#include "jankyTask.h"
#include "jankyDriveStick.h"

#define DRIVE_JOYSTICK_PORT 0
#define GC_JOYSTICK_PORT 1
#define FRONT_LEFT_CHANNEL 2
#define REAR_LEFT_CHANNEL 4
#define FRONT_RIGHT_CHANNEL 1
#define REAR_RIGHT_CHANNEL 3

class Robot: public IterativeRobot
{
	LiveWindow *lw;
	RobotDrive *robot;	// robot drive system
	jankyDrivestick *joystick;
	jankyXboxJoystick *gameComponent;
	JankyFoxliftState *foxlift;
	AxisCamera *camera;
	Talon *pLF;
	Talon *pRF;
	Talon *pLR;
	Talon *pRR;

public:
	Robot()
	{
		printf("in robot constructor \n");
		robot = NULL;
		gameComponent = NULL;
		joystick = NULL;
		foxlift = NULL;
		camera = NULL;
		pLF = NULL;
		pRF = NULL;
		pLR = NULL;
		pRR = NULL;
	}
	~Robot()
	{
		delete robot;
		delete gameComponent;
		delete joystick;
		delete foxlift;
		delete camera;
		delete pLF;
		delete pRF;
		delete pLR;
		delete pRR;
	}

private:

	void RobotInit()
	{
		printf("RobotInit()");
		pLF = new Talon(FRONT_LEFT_CHANNEL);
		pRF = new Talon(FRONT_RIGHT_CHANNEL);
		pLR = new Talon(REAR_LEFT_CHANNEL);
		pRR = new Talon(REAR_RIGHT_CHANNEL);
		robot = new RobotDrive(pLF, pLR, pRF, pRR);
		char group [] = "DriveTrain";
		lw = LiveWindow::GetInstance();
		lw->AddActuator(group, "Front Left", pLF);
		lw->AddActuator(group, "Front Right", pRF);
		lw->AddActuator(group, "Rear Left", pLR);
		lw->AddActuator(group, "Rear Right", pRR);
		printf("in robot init \n");
	    joystick = new jankyDrivestick(DRIVE_JOYSTICK_PORT);
		gameComponent = new jankyXboxJoystick(GC_JOYSTICK_PORT);
		foxlift = new JankyFoxliftState();
        foxlift->SetFoxlift();
		CameraInit();
		printf("end of RobotInit()");
	}

	void AutonomousInit()
	{
		printf("AutonomousInit()");
		robot->SetSafetyEnabled(false);
		printf("end of AutonomousInit()");
	}

	void AutonomousPeriodic()
	{

	}

	void CameraInit()
	{
		camera = new AxisCamera("10.19.67.11");
		camera->WriteResolution (AxisCamera::kResolution_320x240);
		camera->WriteCompression(30);
		camera->WriteRotation(AxisCamera::kRotation_0);
		camera->WriteMaxFPS(15);
		camera->WriteColorLevel(25);
		camera->WriteBrightness(50);
		camera->WriteWhiteBalance(AxisCamera::kWhiteBalance_Automatic);
		camera->WriteExposureControl(AxisCamera::kExposureControl_Automatic);
		camera->WriteRotation(AxisCamera::kRotation_180);
	}

	void TeleopInit()
	{
		robot->SetSafetyEnabled(false);
	}

	void TeleopPeriodic()
	{
		SmartDashboard::PutBoolean("limit Switch Top closed", foxlift->IsLSwitchTopClosed());
		SmartDashboard::PutNumber("Twist", joystick->GetZ());
		SmartDashboard::PutNumber("XAxis", joystick->GetX());
		SmartDashboard::PutBoolean("limit Switch Down closed", foxlift->IsLSwitchDownClosed());
		SmartDashboard::PutNumber("Box Lift Speed", foxlift->motorLift->Get());
		SmartDashboard::PutNumber("Intake Roller 1 Speed", foxlift->motorRoller1->Get());
		SmartDashboard::PutNumber("Intake Roller 2 Speed", foxlift->motorRoller2->Get());
		SmartDashboard::PutBoolean("Reorientation", foxlift->reorientation->Get());
		SmartDashboard::PutBoolean("Singulation IN/OUT", foxlift->singulationOne->Get());
		SmartDashboard::PutBoolean("Singulation UP/DOWN", foxlift->singulationTwo->Get());
		SmartDashboard::PutBoolean("Intake pistons", foxlift->rollerPistons->Get());
		SmartDashboard::PutNumber("Get Throttle", joystick->GetThrottle());

		//MECANUM DRIVE
		float yValue = joystick->GetY();
		float xValue = joystick->GetX();
		//this is apparently changing the twist
		float rotation = joystick->GetJoystickTwist();
		// GetZ() apparently is changed by the lever at the bottom.
		robot->MecanumDrive_Cartesian(xValue, yValue, rotation, 0.0);
		SmartDashboard::PutNumber("X-Value", xValue);
		SmartDashboard::PutNumber("Y-Value", yValue);
		SmartDashboard::PutNumber("Rotation Value", rotation);

		//BOXLIFT
		// When button is pressed, raise the boclift
		if (gameComponent->GetButtonY() == true){
			foxlift->GoUp();
		}
		// When button is pressed, lower the boclift
		if(gameComponent->GetButtonA() == true){
			foxlift->GoDown();
		}
		//REORIENTATION
		//When button is pressed and held, extend reorientation
		if (gameComponent->GetButtonLB() == true){
			foxlift->Reorient();
		}
		//When button is released, retract reorientation
		if (gameComponent->GetButtonLB() == false){
			foxlift->DoneReorienting();
		}
		//SINGULATION
		//When any joystick top button is pressed and the trigger is pressed, extend piston one and low piston 2
		if (joystick->IsAnyTopButtonPressed() == true && joystick->GetTrigger() == true){
			foxlift->SingulateTwo();
		}
		//When any joystick top button is pressed, extend piston 1;
		else if (joystick->IsAnyTopButtonPressed() == true && joystick->GetTrigger() == false){
			foxlift->SingulateOne();
		}
		//When none of the top buttons is pressed, you are done singulating
		if (joystick->IsAnyTopButtonPressed() == false){
			foxlift->DoneSingulating();
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
