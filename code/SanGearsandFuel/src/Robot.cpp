

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "jankyXboxJoystick.h"
#include "WPILib.h"
#include "GearsFuel.h"
#define XBOX_CHANNEL 1
#define GEAR_CHANNEL 1
#define INTAKE_CHANNEL 2
#define OUTTAKE_CHANNEL 3


class Robot: public frc::IterativeRobot {
	GearsFuel * gefu;
	jankyXboxJoystick * xbox;
	bool AnotPressed = true;
	bool BnotPressed = true;
	bool YnotPressed = true;
	bool XnotPressed = true;
	bool LBnotPressed = true;
	bool RBnotPressed = true;
	Solenoid * gearPiston;
	Solenoid * horzPiston;
	Solenoid * vertPiston;


public:
	Robot() {
		gefu = NULL;
		xbox = NULL;
		gearPiston = NULL;
		horzPiston = NULL;
		vertPiston = NULL;

	}
	~Robot() {
		delete gefu;
		delete xbox;

	}
public:
	void RobotInit() {
		gefu = new GearsFuel ( GEAR_CHANNEL, INTAKE_CHANNEL, OUTTAKE_CHANNEL);
		xbox = new jankyXboxJoystick(XBOX_CHANNEL);



	}
	void AutonomousInit() override {

		}


	void AutonomousPeriodic() {

		}



	void TeleopInit() {


	}

	void TeleopPeriodic() {
		//for gear piston
		printf("in teleop per \n");

		if (xbox->GetButtonB() && BnotPressed) {

			gefu->GearOut();
			BnotPressed=false;
			printf("gearout \n");
		}
			else if (!xbox->GetButtonB()) {
				BnotPressed=true;

		}
		if (xbox->GetButtonX() && XnotPressed) {
			gefu->GearIn();
			XnotPressed=false;
			printf("gearin \n");
		}
		else if(!xbox->GetButtonX()) {
			XnotPressed=true;
		}
		// for intake (piston 2)
		if(xbox->GetButtonA() && AnotPressed) {
				gefu->Horz();
				AnotPressed=false;
				printf("horz \n");
			}
		else if (!xbox->GetButtonA()) {
			AnotPressed=true;
				}

			if (xbox->GetButtonY() && (YnotPressed)) {
				gefu->Horz2();
				YnotPressed=false;
				printf("horz2 \n");
			}
			else if(!xbox->GetButtonY()) {
						YnotPressed=true;
					}
		// for outtake (piston 3)
		if (xbox->GetButtonLB() && (LBnotPressed)) {
				gefu->Vert();
				LBnotPressed=false;
				printf("vert \n");
			}
		else if(!xbox->GetButtonLB()) {
				LBnotPressed=true;
			}
			if (xbox->GetButtonRB() && (RBnotPressed)) {
				gefu->Vert2();
				RBnotPressed=false;
				printf("vert2 \n");
			}
			else if(!xbox->GetButtonRB()) {
							RBnotPressed=true;
						}

			}

	void TestPeriodic() {

	}
private:
};


START_ROBOT_CLASS(Robot)

