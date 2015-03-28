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
#define GC_DEADBAND_SIZE 0.5
#define GCSTOP_DEADBAND_SIZE 0.75
#define AUTOZONE_TIMER 1.2
#define AUTOZONE_BUMP_TIMER 1.3
#define AUTONOMOUS_TIME 10

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
	Timer*autonomousTimer;
	SendableChooser *chooser;
	JankyAutonomousState *jankyAuto;
	Timer* autoZoneTimer;
	Timer* autoZoneBumpTimer;

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
		autoZoneTimer = NULL;
		autoZoneBumpTimer = NULL;
		autonomousTimer = new Timer();
		autonomousTimer->Start();
	}
	~Robot()
	{
		delete robot;
		delete autonomousTimer;
		delete gameComponent;
		delete joystick;
		delete foxlift;
		delete camera;
		delete pLF;
		delete pRF;
		delete pLR;
		delete pRR;
		delete jankyAuto;
		delete autoZoneTimer;
		delete autoZoneBumpTimer;
	}

private:
	int defaultAuto;
	int autoZoneAndBin;
	int autoZone;
	int autoZoneBump;
	int autoZoneAndBinWithHug;
	int autoZoneAndBingulate;

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
		chooser->AddDefault("Autonomous Nothing (Default)", &defaultAuto);
		chooser->AddObject("Autonomous Tote using Boxlift", &autoZoneAndBin);
		chooser->AddObject("Autonomous Forward w/ no bump", &autoZone);
		chooser->AddObject("Autonomous Forward w/ bump", &autoZoneBump);
		chooser->AddObject("Autonomous Tote with Hugging", &autoZoneAndBinWithHug);
		chooser->AddObject("Autonomous Bingulate", &autoZoneAndBingulate);
		//put the different options on SmartDashboard
		SmartDashboard::PutData("Autonomous modes", chooser);

		foxlift = new JankyFoxliftState();
        foxlift->SetFoxlift();
		//CameraInit();
		printf("end of RobotInit()");
	}

	void AutonomousInit()
	{
		autonomousTimer->Reset();
		foxlift->SetFoxlift();
		printf("AutonomousInit() called\n");
		robot->SetSafetyEnabled(false);

		if (&defaultAuto == chooser->GetSelected()) //THE BOXLIFT HAS TO START ABOVE THE TOTE HEIGHT
		{
			printf("defaultAuto running\n");
		}
		else if (&autoZoneAndBin == chooser->GetSelected())
		{
			printf("autoZoneAndBin running\n");
			jankyAuto = new JankyAutonomousState(robot, foxlift);
		}
		else if (&autoZone == chooser->GetSelected())
		{
			printf("autoZone running\n");
			autoZoneTimer = new Timer();
			autoZoneTimer->Start();
		}
		else if (&autoZoneBump == chooser->GetSelected())
		{
			printf("autoZone running\n");
			autoZoneBumpTimer = new Timer();
			autoZoneBumpTimer->Start();
		}
		else if (&autoZoneAndBinWithHug == chooser->GetSelected())
		{
			printf("autoZoneAndBinWithHug running\n");
			jankyAuto = new JankyAutonomousState(robot, foxlift);
		}
		else if (&autoZoneAndBingulate == chooser->GetSelected())
		{
		printf("autoZoneAndBingulate\n");
		jankyAuto = new JankyAutonomousState(robot, foxlift);
		jankyAuto->StartBinAuto();
		}
		else
		{
			printf("something's wrong\n");
		}
		printf("end of AutonomousInit()");
	}

	void AutonomousPeriodic()
	{
		if (&autoZoneAndBin == chooser->GetSelected())
		{
			jankyAuto->GoForBox();
		}
		else if (&autoZoneAndBinWithHug == chooser->GetSelected())
		{
			jankyAuto->GoForHug();
		}
		else if (&autoZone == chooser->GetSelected())
		{
			if(autoZoneTimer->Get() >= AUTOZONE_TIMER )
			{
				robot->MecanumDrive_Cartesian(0.0, 0.0, 0.0, 0.0);
			}
			else
			{
				robot->MecanumDrive_Cartesian(0.0, -1.0, 0.0, 0.0);
			}
		}
		else if (&autoZoneBump == chooser->GetSelected())
		{
			if(autoZoneBumpTimer->Get() >= AUTOZONE_BUMP_TIMER )
			{
				robot->MecanumDrive_Cartesian(0.0, 0.0, 0.0, 0.0);
			}
			else
			{
				robot->MecanumDrive_Cartesian(0.0, -1.0, 0.0, 0.0);
			}

		}
		/*if(autonomousTimer->Get()>= AUTONOMOUS_TIME && jankyAuto){
			printf("going to delete autonomous \n");
			delete jankyAuto;
			jankyAuto = NULL;
			printf("deleted autonomous \n");
		}*/
	}

	void CameraInit()
	{
		printf("before opening camera\n");
		camera = new AxisCamera("camera1967"); //image on SmartDash must have the IP address: camera1967.local
		printf("after opening camera\n");
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
		foxlift->SetFoxlift();
		//CameraInit();
	}

	void TeleopPeriodic()
	{
		SmartDashboard::PutBoolean("limit Switch Mid closed", foxlift->IsLSwitchMidClosed());
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
		//if(gameComponent->GetLeftYAxis() > GC_DEADBAND_SIZE){
		if(gameComponent->GetY() < (-1)*(GC_DEADBAND_SIZE)){
			foxlift->PushOutTote();
			//foxlift->StopRollers();
		}
		//else if(gameComponent->GetLeftYAxis() < (-1)*(GC_DEADBAND_SIZE)){
		else if(gameComponent->GetY() > GC_DEADBAND_SIZE){
			foxlift->SuckInTote();
		}
		if(abs(gameComponent->GetX()) > GCSTOP_DEADBAND_SIZE)
		{
			foxlift->StopRollers();
		}
		//else if(abs(gameComponent->GetLeftYAxis()) <= GC_DEADBAND_SIZE){
		/*else if(abs(gameComponent->GetY()*-1) <= GC_DEADBAND_SIZE){
				foxlift->StopRollers();
		}*/
		//Arm pistons
		if(gameComponent->GetRightYAxis() < (-1)*(GC_DEADBAND_SIZE)){
			foxlift->ExtendArmsManual();
		}
		else if(gameComponent->GetRightYAxis() > GC_DEADBAND_SIZE){
				foxlift->RetractArmsManual();
		}
		//BOXLIFT
		// When button is pressed, raise the boxlift
		if (gameComponent->GetButtonY() && !gameComponent->GetButtonA()&& !gameComponent->GetButtonX())
		{
			foxlift->GoUp();
		}
		// When button is pressed, lower the boxlift
		if(gameComponent->GetButtonA() && !gameComponent->GetButtonY()&& !gameComponent->GetButtonX())
		{
			foxlift->GoDown();
		}
		if(gameComponent->GetButtonX() && !gameComponent->GetButtonY()&& !gameComponent->GetButtonA())
		{
			foxlift->GoMid();
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
