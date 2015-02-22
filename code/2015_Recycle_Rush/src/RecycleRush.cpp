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
	RobotDrive* robot;	// robot drive system
	jankyDrivestick* joystick;
	jankyXboxJoystick* gameComponent;
	JankyFoxliftState* foxlift;
	AxisCamera * camera;

public:
	Robot(){
		printf("in robot constructor \n");
		robot = NULL;
		gameComponent = NULL;
		joystick = NULL;
		foxlift = NULL;
		camera = NULL;
	}
	~Robot(){
		delete robot;
		delete gameComponent;
		delete joystick;
		delete foxlift;
		delete camera;
	}

private:

	void RobotInit()
	{
		printf("RobotInit()");
		robot = new RobotDrive(FRONT_LEFT_CHANNEL, REAR_LEFT_CHANNEL,
					   FRONT_RIGHT_CHANNEL, REAR_RIGHT_CHANNEL);
		printf("robot");
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

	void AutonomousPeriodic()
	{

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
		if (gameComponent->GetButtonY() == true){
			foxlift->GoUp();
		}
		 if(gameComponent->GetButtonA() == true){
			foxlift->GoDown();
		}
		//REORIENTATION
		if (gameComponent->GetButtonLB() == true){
			foxlift->Reorient();
		}
		if (gameComponent->GetButtonLB() == false){
			foxlift->DoneReorienting();
		}
		//SINGULATION
		if (joystick->IsAnyTopButtonPressed() == true && joystick->GetTrigger() == true){
			foxlift->SingulateTwo();
		}
		else if (joystick->IsAnyTopButtonPressed() == true && joystick->GetTrigger() == false){
			foxlift->SingulateOne();
		}
		if (joystick->IsAnyTopButtonPressed() == false){
			foxlift->DoneSingulating();
		}
	}


void TeleopInit()
	{
		robot->SetSafetyEnabled(false);

	}

		/*void TestPeriodic()
	{
		//lw->Run();
	}*/
};

START_ROBOT_CLASS(Robot);
