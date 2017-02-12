#include <iostream>
#include <memory>
#include <string>

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "WPILib.h"
#include "CANTalon.h"
#include "jankyXboxJoystick.h"
#include "RopeClimbing.h"

#define MOTOR_A_CHANNEL 5
#define MOTOR_B_CHANNEL 6
#define ENCODER_CHANNEL_A 1
#define ENCODER_CHANNEL_B 2
#define JOYSTICK_CHANNEL 0
#define LIMIT_SWITCH_CHANNEL 0

class Robot: public frc::IterativeRobot {
	RopeClimbing * climb;
	jankyXboxJoystick * joystick;

public:
	void RobotInit() {
		climb = new RopeClimbing(MOTOR_A_CHANNEL, MOTOR_B_CHANNEL, ENCODER_CHANNEL_A, ENCODER_CHANNEL_B, LIMIT_SWITCH_CHANNEL);
		joystick = new jankyXboxJoystick(JOYSTICK_CHANNEL);
	}


	void AutonomousInit() override {

	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		// go through climb states when button A is pressed
		SmartDashboard::PutNumber("climbing encoder: ", climb->GetEncoder());
		SmartDashboard::PutBoolean("limit switch: ", climb->GetLimitSwitch());
		/*if (climb->GetLimitSwitch())
		{
			climb->StopClimb();
		}*/

		if (joystick->GetButtonA())
		{
			climb->SwitchStates();
		}
		if (joystick->GetButtonB())
		{
			climb->StopClimb();
		}
	}

	void TestPeriodic() {
	}

private:

};

START_ROBOT_CLASS(Robot)
