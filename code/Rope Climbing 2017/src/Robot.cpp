#include <iostream>
#include <memory>
#include <string>

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "WPILib.h"
#include "jankyXboxJoystick.h"
#include "RopeClimbing.h"
#include "CANTalon.h"

#define MOTOR_A_CHANNEL 0
#define MOTOR_B_CHANNEL 5
#define ENCODER_CHANNEL_A 3
#define ENCODER_CHANNEL_B 4
#define JOYSTICK_CHANNEL 0
#define LIMIT_SWITCH_CHANNEL 6
#define TALON_CHANNEL 0

class Robot: public frc::IterativeRobot {
	RopeClimbing * climb;
	jankyXboxJoystick * joystick;
	CANTalon*talon;
	Encoder*encoder;

public:
	void RobotInit() {
		climb = new RopeClimbing(MOTOR_A_CHANNEL, MOTOR_B_CHANNEL, ENCODER_CHANNEL_A, ENCODER_CHANNEL_B, LIMIT_SWITCH_CHANNEL);
		joystick = new jankyXboxJoystick(JOYSTICK_CHANNEL);
		talon = new CANTalon(TALON_CHANNEL);
		encoder = new Encoder(ENCODER_CHANNEL_A, ENCODER_CHANNEL_B);
	}


	void AutonomousInit() override {

	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		// go through climb states when button A is pressed
		if (joystick->GetButtonA()){
			climb->SwitchStates();
		}

		if (joystick -> GetButtonB()){
			climb -> StopClimb();
		}

		SmartDashboard::PutNumber("Encoder Value", encoder -> Get());
		SmartDashboard::PutNumber("Talon Value", talon -> Get());
	}

	void TestPeriodic() {
	}

private:

};

START_ROBOT_CLASS(Robot)


