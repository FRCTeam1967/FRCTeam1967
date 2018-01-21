/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include "WPILib.h"
#include "ctre/Phoenix.h"
#include "jankyXboxJoystick.h"
#include "JankyAutoSelector.h"
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
using namespace std;

//auto modes
#define DEFAULT_MODE 1
#define L_CROSS_AUTOLINE 2
#define L_SAME_SWITCH 3
#define L_OPPOSITE_SWITCH 4
#define M_LEFT_SWITCH 5
#define M_RIGHT_SWITCH 6
#define R_CROSS_AUTOLINE 7
#define R_SAME_SWITCH 8
#define R_OPPOSITE_SWITCH 9

//chassis channels
#define FRONT_LEFT_MOTOR_CHANNEL 1
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 3
#define REAR_RIGHT_MOTOR_CHANNEL 4

//joystick channels
#define JOYSTICK_CHANNEL 0

class Robot : public frc::IterativeRobot {
	JankyAutoSelector*selector;
	frc::Timer autonomousTimer;
	WPI_TalonSRX*flmotor;
	WPI_TalonSRX*rlmotor;
	WPI_TalonSRX*frmotor;
	WPI_TalonSRX*rrmotor;
	frc::RobotDrive*drive;
	jankyXboxJoystick*xbox;


	int delayTime = 0;
	int automode = DEFAULT_MODE;
	char switchPos;

public:
	Robot(){
		selector=NULL;
		flmotor=NULL;
		rlmotor=NULL;
		frmotor=NULL;
		rrmotor=NULL;
		drive=NULL;
		xbox=NULL;
	}
	~Robot(){
		delete selector;
		delete flmotor;
		delete rlmotor;
		delete frmotor;
		delete rrmotor;
		delete drive;
		delete xbox;
	}

	void RobotInit() {
		selector = new JankyAutoSelector();
		flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
		rlmotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
		frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
		rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
		drive = new frc::RobotDrive(flmotor, rlmotor, frmotor, rrmotor);
		xbox = new jankyXboxJoystick(JOYSTICK_CHANNEL);

		selector->Init();
	}

	void AutonomousInit() override {
		//retrieving switch info from FMS
		std::string gameData;
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
		std::cout << gameData;
		switchPos = 'L';//gameData[0];

		autonomousTimer.Reset();
		autonomousTimer.Start();

		delayTime = selector->GetDelayTime();
		automode=selector->GetAutoMode(switchPos);
		selector->PrintValues();
	}

	void AutonomousPeriodic() {
		//printf("selected action: %lax \n", (selector->GetAction()));
		if  (automode == DEFAULT_MODE) {
			printf ("default\n");
		}
		else if(automode == L_CROSS_AUTOLINE && autonomousTimer.Get()>delayTime){
			printf("starting left and crossing auto line \n");
			//code
		}
		else if(automode == L_SAME_SWITCH && autonomousTimer.Get()>delayTime){
			printf("starting left and loading cube onto switch on left side \n");
			//code
		}
		else if(automode == L_OPPOSITE_SWITCH && autonomousTimer.Get()>delayTime){
			printf("starting left and loading cube onto switch on right side \n");
			//code
		}
		else if(automode == M_LEFT_SWITCH && autonomousTimer.Get()>delayTime){
			printf("starting middle and loading cube onto left switch \n");
			//code
		}
		else if(automode == M_RIGHT_SWITCH && autonomousTimer.Get()>delayTime){
			printf("starting middle and loading cube onto right switch \n");
			//code
		}
		else if(automode == R_CROSS_AUTOLINE && autonomousTimer.Get()>delayTime){
			printf("starting right and crossing auto line \n");
			//code
		}
		else if(automode == R_SAME_SWITCH && autonomousTimer.Get()>delayTime){
			printf("starting right and loading cube onto switch on right side \n");
			//code
		}
		else if(automode == R_OPPOSITE_SWITCH && autonomousTimer.Get()>delayTime){
			printf("starting right and loading cube onto switch on left side \n");
			//code
		}
	}

	void TeleopInit() {}

	void TeleopPeriodic() {
		drive->TankDrive(-xbox->GetLeftYAxis(), -xbox->GetRightYAxis());
	}

	void TestPeriodic() {}

private:
};

START_ROBOT_CLASS(Robot)
