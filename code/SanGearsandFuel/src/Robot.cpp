

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "jankyXboxJoystick.h"
#include "WPILib.h"
#include "GearsFuel.h"
#define XBOX_CHANNEL 1
#define BALL_MOTOR_SPEED 0.75
#define BALL_MOTOR_CHANNEL 4
#define DOOR_PISTON_CHANNEL 5
#define GEAR_BOX_CHANNEL 1 //the only piston on robot rn
#define PUSH_GEAR_CHANNEL 2


class Robot: public frc::IterativeRobot {
	GearsFuel * gefu;
	jankyXboxJoystick * xbox;
	bool AnotPressed = true;
	bool BnotPressed = true;
	bool YnotPressed = true;
	Solenoid * solenoid1;
	Solenoid * solenoid2;
	Solenoid * solenoid3;


public:
	Robot() {
		gefu = NULL;
		xbox = NULL;
		solenoid1 = NULL;
		solenoid2 = NULL;
		solenoid3 = NULL;

	}
	~Robot() {
		delete gefu;
		delete xbox;

	}
public:
	void RobotInit() {
		gefu = new GearsFuel (BALL_MOTOR_CHANNEL, DOOR_PISTON_CHANNEL, GEAR_BOX_CHANNEL, PUSH_GEAR_CHANNEL);
		xbox = new jankyXboxJoystick(XBOX_CHANNEL);



	}
	void AutonomousInit() override {

		}


	void AutonomousPeriodic() {

		}



	void TeleopInit() {
		//gefu->FuelIntake();
		//gefu->BoxOut();
		//gefu->DoorDown(); //for testing
		//gefu->GearOut(); //for test purposes

	}

	void TeleopPeriodic() {
//	if(xbox->GetButtonX()) {
	//	gefu->StopBallMotor();
	//}
	//else{
	//	gefu->FuelIntake();
	//}
	if(xbox->GetButtonA() && AnotPressed) {
		gefu->DoorUp();
		AnotPressed=false;
	}

	else if (xbox->GetButtonA() && !(AnotPressed)) {
		gefu->DoorDown();
		AnotPressed=true;
	}

	if (xbox->GetButtonB()) {
		gefu->BoxOut();

	}
	else if (xbox->GetButtonX()) {
		gefu->BoxIn();
	}

	if (xbox->GetButtonY() && (YnotPressed)) {
		gefu->GearOut();
		YnotPressed=false;
	}
	else if (xbox->GetButtonY() && !(YnotPressed)) {
		gefu->GearIn();
		YnotPressed=true;
	}

	}






	void TestPeriodic() {

	}
private:
};


START_ROBOT_CLASS(Robot)

