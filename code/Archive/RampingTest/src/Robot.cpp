#include <iostream>
#include <memory>
#include <string>
#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "jankyXboxJoystick.h"
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <cmath>


#define JOYSTICK_CHANNEL 0

#define LRCHANNEL 2
#define RRCHANNEL 4
#define LFCHANNEL 1
#define RFCHANNEL 3

#define RAMPING_TIME 0.5


class Robot: public frc::IterativeRobot {
	WPI_TalonSRX*lRMotor;
	WPI_TalonSRX*rRMotor;
	WPI_TalonSRX*lFMotor;
	WPI_TalonSRX*rFMotor;
	jankyXboxJoystick*driveStick;
	RobotDrive*drive;
	Timer*autonomousTimer;

public:
	Robot(){
		lRMotor = NULL;
		rRMotor = NULL;
		lFMotor = NULL;
		rFMotor = NULL;
		driveStick = NULL;
		drive = NULL;
	}

	~Robot(){
		delete lRMotor;
		delete rRMotor;
		delete lFMotor;
		delete rFMotor;
		delete driveStick;
		delete drive;
	}

	void RobotInit() {
		lRMotor = new WPI_TalonSRX(LRCHANNEL);
		rRMotor = new WPI_TalonSRX(RRCHANNEL);
		lFMotor = new WPI_TalonSRX(LFCHANNEL);
		rFMotor = new WPI_TalonSRX(RFCHANNEL);
		driveStick = new jankyXboxJoystick(JOYSTICK_CHANNEL);
		drive = new RobotDrive(lFMotor,lRMotor,rFMotor,rRMotor);

	}

	void AutonomousInit() override {
	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		float lValue = driveStick->GetLeftYAxis();
		float rValue = driveStick->GetRightYAxis();
		drive->TankDrive(-lValue,-rValue ); //motors wired in different directions on both sides

		lRMotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		rRMotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		lFMotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		rFMotor->ConfigOpenloopRamp(RAMPING_TIME, 0);


	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
