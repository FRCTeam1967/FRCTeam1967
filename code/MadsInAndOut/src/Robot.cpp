#include <iostream>
#include <string>

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>

#include <jankyXboxJoystick.h>
#include "WPILib.h"
#include "ctre/Phoenix.h"
#include <InAndOut.h>

//Add channel numbers later (THESE ARE NOT ACCURATE)
#define MOTOR_CLAW_CHANNEL 2
#define PISTON_DOOR_LEFT_CHANNEL 2
#define PISTON_DOOR_RIGHT_CHANNEL 3
#define LIM_SWITCH_INSIDE_CHANNEL 6
#define LIM_SWITCH_OUTSIDE_CHANNEL 9
#define GC_XBOX_CHANNEL 0
#define MOTOR_ROLL_CHANNEL 2
#define CLAW_ENCODER_CHANNEL_1 4
#define CLAW_ENCODER_CHANNEL_2 5

class Robot : public frc::IterativeRobot {
	InAndOut*inOut;
	jankyXboxJoystick*gameJoystick;
	bool lbHasNotBeenPressed = true;
	bool rbHasNotBeenPressed = true;
	bool clawGoingForward = false;
	bool clawGoingBackward = false;
public:
	Robot() {
		inOut = NULL;
		gameJoystick = NULL;
	}
	~Robot(){
		delete inOut;
		delete gameJoystick;
	}
	void RobotInit() {
		inOut = new InAndOut(MOTOR_CLAW_CHANNEL, PISTON_DOOR_RIGHT_CHANNEL, PISTON_DOOR_LEFT_CHANNEL, LIM_SWITCH_INSIDE_CHANNEL, LIM_SWITCH_OUTSIDE_CHANNEL, MOTOR_ROLL_CHANNEL, CLAW_ENCODER_CHANNEL_1, CLAW_ENCODER_CHANNEL_2);
		gameJoystick = new jankyXboxJoystick(GC_XBOX_CHANNEL);
	}

	void AutonomousInit() override {

	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {

		// Define all of the buttons/throttle on the joystick
		bool buttonRB = gameJoystick -> GetButtonRB();
		bool buttonLB = gameJoystick -> GetButtonLB();
		bool buttonBack = gameJoystick ->GetButtonBack();
		bool buttonStart = gameJoystick -> GetButtonStart();
		float leftValue = gameJoystick -> GetLeftYAxis();

		SmartDashboard::PutBoolean("Limit switch outside value", inOut -> GetLimSwitchOutside());
		SmartDashboard::PutBoolean("Limit switch inside value", inOut -> GetLimSwitchInside());

		//Move claw mechanism with encoders
		if (buttonBack) {
			inOut -> OutsideDistance();
		}
		else if (buttonStart) {
			inOut -> InsideDistance();
		}

		inOut -> MoveClawMechanism();


		//Put claw mechanism up/down based on what limit switches are pressed
		if (buttonRB) {
			if (inOut -> GetLimSwitchInside()) {
				inOut -> MotorClawOutOfRobot();
				clawGoingForward = true;
			}
			else {
				inOut -> MotorClawIntoRobot();
				clawGoingBackward = true;
			}
		}
		if (inOut -> GetLimSwitchOutside() && clawGoingForward) {
			inOut -> MotorClawStop();
			clawGoingForward = false;
		}
		else if (inOut -> GetLimSwitchInside() && clawGoingBackward) {
			inOut -> MotorClawStop();
			clawGoingBackward = false;
		}

		// Open/Close the claw's "doors" with pistons
		if (buttonLB && lbHasNotBeenPressed == true){
			inOut->PistonDoorOpen(); 	//if button B is pressed & B has not been pressed before, the piston will go out
			lbHasNotBeenPressed = false; //Makes it so that b has now been pressed
		}
		else if (buttonLB && lbHasNotBeenPressed == false) {
			inOut->PistonDoorClose(); 	//if button B is pressed & B has been pressed before, the piston will go back in
			lbHasNotBeenPressed = true; //Makes it so that b has not been pressed
		}

		//Make the rollers go forward and backward:
		if (leftValue > 0.2) {
			inOut -> MotorRollForward(); //If you push the left throttle go forward, rollers will push the cube out of the mechanism
		}
		else if (leftValue < -0.2) {
			inOut -> MotorRollReverse(); //If you push the left throttle go forward, rollers will bring the cube into the mechanism
		}
		else {
			inOut -> MotorRollStop(); //If you do not touch the throttle, the rollers will stay still
		}

	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
