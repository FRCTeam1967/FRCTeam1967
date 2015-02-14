#include "WPILib.h"
#include "jankyDrivestick.h"
#include "jankyAutonomousState.h"
#include "jankyFoxLiftState.h"
#include "jankyXboxJoystick.h"
#include "jankyTask.h"

class Robot: public IterativeRobot
{
private:
	LiveWindow *lw;
    // Channels for the wheels
    const static int frontLeftChannel	= 1;
    const static int rearLeftChannel	= 2;
    const static int frontRightChannel	= 3;
    const static int rearRightChannel	= 4;

    const static int driverJoystickChannel	= 0;
    const static int gameControllerJoystickChannel	= 1;

	RobotDrive* pRobotDrive;	// robot drive system
	jankyDrivestick* pDriverStick;
	jankyXboxJoystick* pGameController;
	jankyFoxLiftState* pFoxLift;

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		pRobotDrive = new RobotDrive(frontLeftChannel, rearLeftChannel,
					   frontRightChannel, rearRightChannel);

		pDriverStick = new jankyDrivestick(driverJoystickChannel);
		pGameController = new jankyXboxJoystick(gameControllerJoystickChannel);
		pFoxLift = new jankyFoxLiftState();

		pRobotDrive->SetExpiration(0.1);
		pRobotDrive->SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);	// invert the left side motors
		pRobotDrive->SetInvertedMotor(RobotDrive::kRearLeftMotor, true);	// you may need to change or remove this to match your robot

}

	void AutonomousInit()
	{
		pRobotDrive->SetSafetyEnabled(false);

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
		while (IsOperatorControl())
		{
			//MECANUM DRIVE
			float LeftYAxis = pDriverStick->GetY();
			pRobotDrive->MecanumDrive_Cartesian(LeftYAxis, pDriverStick->GetX(), pDriverStick->GetTwist(), 0.0);

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

			//REORIENTATION
			//When button is pushed, make ReorientPiston go down
			if (pGameController->GetButtonLB)
			{
				pFoxLift->Reorient();
			}
			else
			{
				pFoxLift->DoneReorienting();
			}

			//SINGULATION
			//When any of the buttons get pushed, make piston push out
			if (pGameController->Get3()
					|| pGameController->Get5()
					|| pGameController->Get2()
					|| pGameController->Get6()
					|| pGameController->Get4())
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
			}

		}

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
