#include "WPILib.h"
//#include "jankyDrivestick.h"
#include "jankyAutonomousState.h"
//#include "jankyFoxliftState.h"
#include "JankyFoxLiftStateDemo.h"
#include "jankyXboxJoystick.h"
#include "jankyTask.h"
//#include "JankyCore.h"

class Robot: public IterativeRobot
{
private:
	//LiveWindow *lw;
    // Channels for the wheels
    const static int frontLeftChannel	= 1;
    const static int rearLeftChannel	= 2;
    const static int frontRightChannel	= 3;
    const static int rearRightChannel	= 4;

    const static int driverJoystickChannel	= 0;
    const static int gameControllerJoystickChannel	= 1;

	RobotDrive* pRobotDrive;	// robot drive system
	Joystick* pDriverStick;
	jankyXboxJoystick* pGameController;
	JankyFoxLiftStateDemo* pFoxLift;
	AxisCamera * camera;

	void RobotInit()
	{
		printf("RobotInit()");
		//lw = LiveWindow::GetInstance();
		pRobotDrive = new RobotDrive(frontLeftChannel, rearLeftChannel,
					   frontRightChannel, rearRightChannel);
		printf("pRobotDrive");
		pDriverStick = new Joystick(driverJoystickChannel);
		printf("pDriverStick");
		pGameController = new jankyXboxJoystick(gameControllerJoystickChannel);
		printf("pGameController");
		pFoxLift = new JankyFoxLiftStateDemo();
		printf("pFoxLift");
		pRobotDrive->SetExpiration(0.1);
		printf("pRobotDrive");
		pRobotDrive->SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);	// invert the left side motors
		pRobotDrive->SetInvertedMotor(RobotDrive::kRearLeftMotor, true);	// you may need to change or remove this to match your robot
		CameraInit();
		printf("end of RobotInit()");
}

	void AutonomousInit()
	{
		printf("AutonomousInit()");
		pRobotDrive->SetSafetyEnabled(false);
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
	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{
		pRobotDrive->SetSafetyEnabled(false);

	}

	void TeleopPeriodic()
	{

			//MECANUM DRIVE
			/*float LeftYAxis = pDriverStick->GetY();
			if(abs(LeftYAxis) < 0.01)
			{
				LeftYAxis = 0.0;
			}
			float LeftXAxis = pDriverStick->GetX();
			if (abs(LeftXAxis) < 0.01)
			{
				LeftXAxis = 0.0;
			}
			float LeftTwist = pDriverStick->GetTwist();
			if(abs(LeftTwist) < 0.01)
			{
				LeftTwist = 0.0;
			}*/

			pRobotDrive->MecanumDrive_Cartesian(pDriverStick->GetX(), pDriverStick->GetY(), pDriverStick->GetTwist(), 0.0);
			SmartDashboard::PutNumber("X-Value", pDriverStick->GetX());
			SmartDashboard::PutNumber("Y-Value", pDriverStick->GetY());
			//FOXLIFT
			//When button is pushed, lower the boxlift
			if (pGameController->GetButtonA())
			{
				pFoxLift->GoDown();
			}
			//When button is pushed, raise the boxlift
			if (pGameController->GetButtonY())
			{
				pFoxLift->GoUp();
			}
			if (pGameController->GetY() <0.25) {
				pFoxLift->SpinIn();
			}
			if(pGameController->GetY()>0.25) {
				pFoxLift->SpinOut();
			}


			//REORIENTATION
			//When button is pushed, make ReorientPiston go down
			/*if (pGameController->GetButtonLB())
			{
				pFoxLift->Reorient();
			}
			else
			{
				pFoxLift->DoneReorienting();
			}*/

			//SINGULATION
			//When any of the buttons get pushed, make piston push out
			/*if (pGameController->GetRawButton(3)
					|| pGameController->GetRawButton(5)
					|| pGameController->GetRawButton(2)
					|| pGameController->GetRawButton(6)
					|| pGameController->GetRawButton(4))
			{
				pFoxLift->SingulateOne();
			}
			else
			{
				pFoxLift->DoneSingulating();
			}
			//When button is pushed, make piston go down
			if (pGameController->GetTrigger())
			{
				pFoxLift->SingulateTwo();
			}*/

		}


	/*void TestPeriodic()
	{
		//lw->Run();
	}*/
};

START_ROBOT_CLASS(Robot);
