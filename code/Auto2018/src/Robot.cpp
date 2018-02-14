/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include "WPILib.h"
#include <Preferences.h>
#include "ctre/Phoenix.h"
#include "jankyXboxJoystick.h"
#include "JankyAutoSelector.h"
#include "AutoPIDDrive.h"
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "DriveSegment.h"
#include "JankyAutoSequencer.h"
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
#define TEST_MOTOR_CHANNEL 6
#define ENCODER_A_CHANNEL 2
#define ENCODER_B_CHANNEL 3
#define MEASURED_DIST_PER_PULSE 0.0912 //inches per encoder pulse
#define ENCODER_UNITS_PER_ROTATION 4096
#define DIAMETER 6
#define CIRCUMFERENCE DIAMETER*M_PI
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
	//frc::PIDController*PID;
	frc::Preferences*preferences;
	//WPI_TalonSRX*testmotor;
	JankyAutoSequencer*sequencer;
	//SensorCollection*leftEncoder;
	///SensorCollection*rightEncoder;
	int delayTime = 0;
	int automode = DEFAULT_MODE;
	char switchPos;
	/*float kP;
	float kI;
	float kD;*/

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
		//PID=NULL;
		//testmotor=NULL;
		sequencer=NULL;
		//leftEncoder=NULL;
		//rightEncoder=NULL;
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
		//delete PID;
		//delete testmotor;
		delete sequencer;
		//delete leftEncoder;
		//delete rightEncoder;
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
		//chassis = new AutoPIDDrive(drive);
		preferences=Preferences::GetInstance();
		//testmotor=new WPI_TalonSRX(TEST_MOTOR_CHANNEL);
		drive->SetSafetyEnabled(false);
		//preferences->PutFloat("pValue", 0.04);
		//preferences->PutFloat("iValue", 0.0);
		//preferences->PutFloat("dValue", 0.04);
		//kP = preferences->GetFloat("pValue", 0.0);
		//kI = preferences->GetFloat("iValue", 0.0);
		//kD = preferences->GetFloat("dValue", 0.0);

		//PID = new PIDController(kP, kI, kD, gyro, chassis);
		//leftEncoder = &(rlmotor->GetSensorCollection());
		//rightEncoder = &(rrmotor->GetSensorCollection());
		sequencer = new JankyAutoSequencer(drive, gyro, encoder);
		selector->Init();
		//make sure robot is left unmoved for ~10 seconds during calibration
		//encoder->SetDistancePerPulse(DISTANCE_PER_PULSE);
	}

	void AutonomousInit() override {
		//retrieving switch info from FMS
		std::string gameData;
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
		std::cout << gameData;
		//switchPos = gameData[0];
		switchPos = 'L'; //CHANGE IN FINAL CODE
		autonomousTimer.Reset();
		autonomousTimer.Start();

		delayTime = selector->GetDelayTime();
		automode=selector->GetAutoMode(switchPos);
		selector->PrintValues();
		//gyro->Reset();
		//encoder->Reset();
		//rlmotor->SetSelectedSensorPosition(0, 0, 10);
		//rrmotor->SetSelectedSensorPosition(0, 0, 10);
		//leftEncoder->SetQuadraturePosition(0, 10);
		//rightEncoder->SetQuadraturePosition(0, 10);
		//PID->SetInputRange(-180.0, 180.0);
		//PID->SetOutputRange(-1.0, 1.0);
	}

	void AutonomousPeriodic() {
		SmartDashboard::PutNumber("Encoder Count", encoder->Get());
		double autoTime=autonomousTimer.Get();
		if  (automode == DEFAULT_MODE) {
			printf ("default\n");
		}
		else if(automode == L_CROSS_AUTOLINE && autoTime>delayTime){
			printf("starting left and crossing auto line \n");
			sequencer->SetMode(L_CROSS_AUTOLINE);

		}
		else if(automode == L_SAME_SWITCH && autoTime>delayTime){
			printf("starting left and loading cube onto switch on left side \n");
			sequencer->SetMode(L_SAME_SWITCH);
		}
		else if(automode == L_OPPOSITE_SWITCH && autoTime>delayTime){
			printf("starting left and loading cube onto switch on right side \n");
			sequencer->SetMode(L_OPPOSITE_SWITCH);
		}
		else if(automode == M_LEFT_SWITCH && autoTime>delayTime){
			printf("starting middle and loading cube onto left switch \n");
			sequencer->SetMode(M_LEFT_SWITCH);
		}
		else if(automode == M_RIGHT_SWITCH && autoTime>delayTime){
			printf("starting middle and loading cube onto right switch \n");
			sequencer->SetMode(M_RIGHT_SWITCH);
		}
		else if(automode == R_CROSS_AUTOLINE && autoTime>delayTime){
			printf("starting right and crossing auto line \n");
			sequencer->SetMode(R_CROSS_AUTOLINE);
		}
		else if(automode == R_SAME_SWITCH && autoTime>delayTime){
			printf("starting right and loading cube onto switch on right side \n");
			sequencer->SetMode(R_SAME_SWITCH);
		}
		else if(automode == R_OPPOSITE_SWITCH && autoTime>delayTime){
			printf("starting right and loading cube onto switch on left side \n");
			sequencer->SetMode(R_OPPOSITE_SWITCH);
		}
	}

	void TeleopInit() {
		gyro->Reset();
		//UNCOMMENT FOR ACTUAL ROBOT
		/*rlmotor->SetSelectedSensorPosition(0, 0, 10);
		rlmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
		rrmotor->SetSelectedSensorPosition(0, 0, 10);
		rrmotor->GetSensorCollection().SetQuadraturePosition(0, 10);*/
	}

	void TeleopPeriodic() {
		//testmotor->Set(0.35);
		drive->TankDrive(-xbox->GetLeftYAxis(), -xbox->GetRightYAxis());
		//SmartDashboard::PutNumber("Encoder Count", encoder->Get());
		//SmartDashboard::PutNumber("Encoder Distance", (encoder->Get()*MEASURED_DIST_PER_PULSE));
		/*float encoderCount = testmotor->GetSensorCollection().GetQuadraturePosition();
		float encoderDistance = (encoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE; //double check that right type of encoder; also make sure this is a float
		SmartDashboard::PutNumber("Test Encoder Count", encoderCount);
		SmartDashboard::PutNumber("Test Encoder Distance", encoderDistance);*/
	}

	void TestPeriodic() {}

private:
};

START_ROBOT_CLASS(Robot)
