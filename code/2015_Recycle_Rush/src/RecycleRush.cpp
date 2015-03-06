#include "WPILib.h"
#include "jankyAutonomousState.h"
#include "jankyFoxLiftState.h"
#include "jankyXboxJoystick.h"
#include "jankyTask.h"
#include "jankyDrivestick.h"

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
	SendableChooser *chooser;
	JankyAutonomousState *jankyAuto;

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
		jankyAuto = NULL;
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
		delete jankyAuto;
	}

private:
	int autoZone;
	int autoZoneAndBin;

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

		chooser = new SendableChooser();
		// need to addDefault otherwise it won't work
		chooser->AddDefault("Autonomous Mode 1", &autoZone);
		chooser->AddObject("Autonomous Mode 2", &autoZoneAndBin);
		//put the different options on SmartDashboard
		SmartDashboard::PutData("Autonomous modes", chooser);

		foxlift = new JankyFoxliftState();
        foxlift->SetFoxlift();
		CameraInit();
		printf("end of RobotInit()");
	}

	void AutonomousInit()
	{
		printf("AutonomousInit() called\n");
		robot->SetSafetyEnabled(false);

		/*if (&autoZone == chooser->GetSelected())
		{
			printf("autoZone running\n");
		}
		*/
		if (&autoZoneAndBin == chooser->GetSelected())
		{
			printf("autoZoneAndBin running\n");
			jankyAuto = new JankyAutonomousState(robot, foxlift);
			jankyAuto->StartAuto();
		}
		else
		{
			printf("something's wrong\n");
		}
		printf("end of AutonomousInit()");
	}

	void AutonomousPeriodic()
	{
		printf("AutonomousPeriodic() called");
		jankyAuto->Go();
	}

	void CameraInit()
	{/*
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
	*/}

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

		//Override
		if(gameComponent->GetButtonRB() == true){
			foxlift->ManualOverrideOn();
		}
		if(gameComponent->GetButtonRB() == false){
			foxlift->ManualOverrideOff();
		}

		//ForkLift
		if(gameComponent->GetRightYAxis() > DEADBAND_SIZE){
			printf("PushOutTote\n");
			foxlift->PushOutTote();
		}
		else if(gameComponent->GetRightYAxis() < (-1)*(DEADBAND_SIZE)){
			foxlift->SuckInTote();
		}
		else if(abs(gameComponent->GetRightYAxis()) <= DEADBAND_SIZE){
				foxlift->StopRollers();
		}

		//BOXLIFT
		// When button is pressed, raise the boxlift
		if (gameComponent->GetButtonY() && !gameComponent->GetButtonA())
		{
			foxlift->GoUp();
		}
		// When button is pressed, lower the boxlift
		if(gameComponent->GetButtonA() && !gameComponent->GetButtonY())
		{
			foxlift->GoDown();
		}
		//emergency stop
		if (gameComponent->GetButtonB() == true)
		{
			printf("ButtonB is pressed\n");
			foxlift->StopLift();
		}

		//REORIENTATION
		//When button is pressed and held, extend reorientation
		if (gameComponent->GetButtonLB())
		{
			foxlift->Reorient();
		}

		//ARMS
		if (gameComponent->GetButtonBack()){
			foxlift->RetractArms();
		}
		if(gameComponent->GetButtonStart()){
			foxlift->ExtendArms();
		}

		//SINGULATION
		//When any joystick top button is pressed and the trigger is pressed, extend piston one and low piston 2
		if (joystick->IsAnyTopButtonPressed() && joystick->GetTrigger())
		{
			foxlift->SingulateTwo();
		}
		//When any joystick top button is pressed, extend piston 1;
		else if (joystick->IsAnyTopButtonPressed() && !joystick->GetTrigger())
		{
			foxlift->SingulateOne();
		}

		//Done Singulating and Reorienting
		if (!joystick->IsAnyTopButtonPressed() && !gameComponent->GetButtonLB())
		{
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
