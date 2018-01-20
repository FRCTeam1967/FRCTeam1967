#include <iostream>
#include <string>

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
//We added these includes:
#include <jankyXboxJoystick.h>
#include "WPILib.h"
#include "ctre/Phoenix.h"
#include <InAndOut.h>

//Add channel numbers later (THESE ARE NOT ACCURATE)
#define MOTOR_CLAW_CHANNEL 2
//#define PISTON_PUSH_CHANNEL 2
#define PISTON_DOOR_LEFT_CHANNEL 2
#define PISTON_DOOR_RIGHT_CHANNEL 3
#define LIM_SWITCH_UP_CHANNEL 6
#define LIM_SWITCH_DOWN_CHANNEL 9
#define GC_XBOX_CHANNEL 0
#define MOTOR_ROLL_CHANNEL 2

class Robot : public frc::IterativeRobot {
	InAndOut*inOut;
	jankyXboxJoystick*gameJoystick;
	bool lbHasNotBeenPressed = true;
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
		inOut = new InAndOut(MOTOR_CLAW_CHANNEL, PISTON_DOOR_RIGHT_CHANNEL, PISTON_DOOR_LEFT_CHANNEL, LIM_SWITCH_UP_CHANNEL, LIM_SWITCH_DOWN_CHANNEL, MOTOR_ROLL_CHANNEL);
		gameJoystick = new jankyXboxJoystick(GC_XBOX_CHANNEL);
	}

	void AutonomousInit() override {

	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {

		//		Define all of the buttons on the joystick
		//		bool buttonA = gameJoystick->GetButtonA();
//		bool buttonB = gameJoystick->GetButtonB();
//		bool buttonX = gameJoystick -> GetButtonX();
		bool buttonRB = gameJoystick -> GetButtonRB();
		bool buttonLB = gameJoystick -> GetButtonLB();


		//		Push out the power cube:
		//		if (buttonA){
		//			inOut->PistonPushOpen(); 	//if button A is pressed, the piston will go out
		//		}
		//		else if (!buttonA) {
		//			inOut->PistonPushClose(); 	//When you take your finger off of "A" the piston will go back in.
		//		}

		//		Open/Close the claw's "doors" with pistons
		if (buttonLB && lbHasNotBeenPressed == true){
			inOut->PistonDoorOpen(); 	//if button B is pressed & B has not been pressed before, the piston will go out
			lbHasNotBeenPressed = false; //Makes it so that b has now been pressed
		}
		else if (buttonLB && lbHasNotBeenPressed == false) {
			inOut->PistonDoorClose(); 	//if button B is pressed & B has been pressed before, the piston will go back in
			lbHasNotBeenPressed = true; //Makes it so that b has not been pressed
		}

		//		Put claw mechanism up/down based on what limit switches are pressed
		//if the claw is in the robot & you press X, the arm will go down until the claw is fully out of the robot
		if (inOut->GetLimSwitchUp() == true && buttonRB) {
			while (inOut ->GetLimSwitchDown() == false){
				inOut ->MotorClawForward(); //Have motor go @ speed of 1.0
			}
			inOut -> MotorClawStop();
		}
		//if the claw out of the robot & you press X, the arm will go up until the claw is fully inside the robot
		else if (inOut ->GetLimSwitchDown() == true &&buttonRB) {
			while (inOut ->GetLimSwitchUp() == false){
				inOut ->MotorClawReverse(); //Have motor go @ speed of -1.0
			}
			inOut -> MotorClawStop();
		}

		//Make the rollers go forward and backward:
		float leftValue = gameJoystick -> GetLeftYAxis();

		if (leftValue > 0.2) {
			inOut -> MotorRollForward();
		}
		else if (leftValue <-0.2) {
			inOut -> MotorRollReverse();
		}
		else {
			inOut -> MotorRollStop();
		}

	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
