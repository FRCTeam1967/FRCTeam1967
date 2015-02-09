#include "WPILib.h"
#include "jankyEncoder.h"

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
	Joystick* pDriverStick;
	Joystick* pGameController;

	JankyEncoder *pJenc;

	void RobotInit()
	{
		lw = LiveWindow::GetInstance();
		pRobotDrive = new RobotDrive(frontLeftChannel, rearLeftChannel,
					   frontRightChannel, rearRightChannel);

		pDriverStick = new Joystick(driverJoystickChannel);
		pGameController = new Joystick(gameControllerJoystickChannel);

		pRobotDrive->SetExpiration(0.1);
		pRobotDrive->SetInvertedMotor(RobotDrive::kFrontLeftMotor, true);	// invert the left side motors
		pRobotDrive->SetInvertedMotor(RobotDrive::kRearLeftMotor, true);	// you may need to change or remove this to match your robot

		pJenc = new JankyEncoder(1,2,3);
		pJenc->Reset();
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
    	// Use the joystick X axis for lateral movement, Y axis for forward movement, and Z axis for rotation.
    	// This sample does not use field-oriented drive, so the gyro input is set to zero.
		pRobotDrive->MecanumDrive_Cartesian(pDriverStick->GetX(), pDriverStick->GetY(), pDriverStick->GetTwist());

	}

	void TestPeriodic()
	{
		lw->Run();
	}
};

START_ROBOT_CLASS(Robot);
