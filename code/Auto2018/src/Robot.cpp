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
#include "AutoPIDDrive.h"
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
#define ENCODER_A_CHANNEL 2
#define ENCODER_B_CHANNEL 3
#define PULSES_PER_REVOLUTION 7
#define GEAR_RATIO 2
#define CIRCUMFERENCE 50
#define DISTANCE_PER_PULSE CIRCUMFERENCE/PULSES_PER_REVOLUTION
#define DRIVE_TIME 1.2

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
	frc::ADXRS450_Gyro*gyro;
	frc::Encoder*encoder;
	AutoPIDDrive*chassis;
	PIDController*PID;

	int delayTime = 0;
	int automode = DEFAULT_MODE;
	char switchPos;
	float kP = 0.04;
	float kI = 0;
	float kD = 0;

public:
	Robot(){
		selector=NULL;
		flmotor=NULL;
		rlmotor=NULL;
		frmotor=NULL;
		rrmotor=NULL;
		drive=NULL;
		xbox=NULL;
		gyro=NULL;
		encoder=NULL;
		chassis=NULL;
		PID=NULL;
	}
	~Robot(){
		delete selector;
		delete flmotor;
		delete rlmotor;
		delete frmotor;
		delete rrmotor;
		delete drive;
		delete xbox;
		delete gyro;
		delete encoder;
		delete chassis;
		delete PID;
	}

	void RobotInit() {
		selector = new JankyAutoSelector();
		flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
		rlmotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
		frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
		rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
		drive = new frc::RobotDrive(flmotor, rlmotor, frmotor, rrmotor);
		xbox = new jankyXboxJoystick(JOYSTICK_CHANNEL);
		gyro = new ADXRS450_Gyro(SPI::Port::kOnboardCS0);
		encoder = new Encoder(ENCODER_A_CHANNEL, ENCODER_B_CHANNEL);
		chassis = new AutoPIDDrive(drive);
		PID = new PIDController(kP, kI, kD, gyro, chassis);

		selector->Init();
		gyro->Calibrate(); //make sure robot is left unmoved for ~10 seconds during calibration
		encoder->SetDistancePerPulse(DISTANCE_PER_PULSE);
	}

	void AutonomousInit() override {
		//retrieving switch info from FMS
		std::string gameData;
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
		std::cout << gameData;
		switchPos = gameData[0];

		autonomousTimer.Reset();
		autonomousTimer.Start();

		delayTime = selector->GetDelayTime();
		automode=selector->GetAutoMode(switchPos);
		selector->PrintValues();
		gyro->Reset();
		encoder->Reset();
		PID->SetInputRange(-180.0, 180.0);
		PID->SetOutputRange(-1.0, 1.0);
		PID->SetSetpoint(0.0);
		PID->Enable();
	}

	void AutonomousPeriodic() {
		if  (automode == DEFAULT_MODE) {
			printf ("default\n");
		}
		else if(automode == L_CROSS_AUTOLINE && autonomousTimer.Get()>delayTime){
			printf("starting left and crossing auto line \n");
			float angle = gyro->GetAngle();
			printf("Angle: %f \n", angle);
			if(autonomousTimer.Get()>DRIVE_TIME){
				PID->SetSetpoint(90.0);
			}
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

	void TeleopInit() {
		gyro->Reset();
		encoder->Reset();
		PID->Disable();
	}

	void TeleopPeriodic() {
		drive->TankDrive(-xbox->GetLeftYAxis(), -xbox->GetRightYAxis());
		SmartDashboard::PutNumber("Encoder Count", encoder->Get());
		SmartDashboard::PutNumber("Encoder Distance", encoder->GetDistance());
	}

	void TestPeriodic() {}

private:
};

START_ROBOT_CLASS(Robot)
