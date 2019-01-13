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

#define MOTOR_A_CHANNEL 7 // 5 on jankybot, 7 on programming board
#define MOTOR_B_CHANNEL 3 // 6 on jankybot
#define ENCODER_CHANNEL_A 3 // 1 on jankybot, 3 on programming board
#define ENCODER_CHANNEL_B 4 // 2 on jankybot, 4 on programming board
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
		SmartDashboard::PutNumber("climbing encoder get: ", climb->GetEncoder());
		SmartDashboard::PutBoolean("limit switch: ", climb->GetLimitSwitch());
		SmartDashboard::PutBoolean("climbing state: ", climb->GetState());

		climb->StartClimbing();

		if (joystick->GetButtonB())
		{
			//climb->SwitchStates();
			climb->StopClimbing();
		}
		if (joystick->GetButtonA())
		{
			climb->SetArmed();
			//climb->encoder->Reset();
			climb->StartClimbing();
		}
		/*if (joystick->GetButtonX())
		{
			//climb->encoder->Reset();
			//climb->StartClimb();
			while (climb->encoder->Get() < 5005)
			{
				climb->StartClimb();
			}
			climb->StopClimb();
		}*/

	}

	void TestPeriodic() {
	}

private:

};

START_ROBOT_CLASS(Robot)
