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
#include "jankyDrivestick.h"
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

//chassis channels (for real robot)
#define FRONT_LEFT_MOTOR_CHANNEL 3
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 5
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
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1

//In and out channels
#define MOTOR_CLAW_CHANNEL 7
#define PISTON_DOOR_LEFT_CHANNEL 4
#define PISTON_DOOR_RIGHT_CHANNEL 4
#define MOTOR_ROLL_CHANNEL 8
//#define CLAW_ENCODER_CHANNEL_1 4
//#define CLAW_ENCODER_CHANNEL_2 5
//#define PISTON_IN_OUT_1_CHANNEL 2
//#define PISTON_IN_OUT_2_CHANNEL 3
//#define LIM_SWITCH_INSIDE_CHANNEL 0
//#define LIM_SWITCH_OUTSIDE_CHANNEL 1
#define AMNT_TO_MOVE_CLAW 4096/4


//Up and down channels
#define L_MOTOR_CHANNEL 6
#define R_MOTOR_CHANNEL 1

class Robot : public frc::IterativeRobot {
	JankyAutoSelector*selector;
	frc::Timer autonomousTimer;
	WPI_TalonSRX*flmotor;
	WPI_TalonSRX*rlmotor;
	WPI_TalonSRX*frmotor;
	WPI_TalonSRX*rrmotor;
	frc::RobotDrive*drive;
	jankyDrivestick*left;
	jankyDrivestick*right;
	jankyXboxJoystick*xbox;
	frc::ADXRS450_Gyro*gyro;
	//frc::Encoder*encoder;
	AutoPIDDrive*chassis;
	//frc::PIDController*PID;
	frc::Preferences*preferences;
	//WPI_TalonSRX*testmotor;
	JankyAutoSequencer*sequencer;
	LiveWindow*lw;
	//SensorCollection*leftEncoder;
	//SensorCollection*rightEncoder;
	InAndOut*inOut;
	UpAndDown*upDown;
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
		left=NULL;
		right=NULL;
		gyro=NULL;
		//encoder=NULL;
		chassis=NULL;
		//PID=NULL;
		//testmotor=NULL;
		sequencer=NULL;
		//leftEncoder=NULL;
		//rightEncoder=NULL;
		inOut = NULL;
		upDown = NULL;
	}
	~Robot(){
		delete selector;
		delete flmotor;
		delete rlmotor;
		delete frmotor;
		delete rrmotor;
		delete drive;
		delete xbox;
		delete left;
		delete right;
		delete gyro;
		//delete encoder;
		delete chassis;
		//delete PID;
		//delete testmotor;
		delete sequencer;
		//delete leftEncoder;
		//delete rightEncoder;
		delete inOut;
		delete upDown;
	}

	void RobotInit() {
		selector = new JankyAutoSelector();
		flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
		rlmotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
		frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
		rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
		drive = new frc::RobotDrive(flmotor, rlmotor, frmotor, rrmotor);
		left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
		right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);
		//xbox = new jankyXboxJoystick(JOYSTICK_CHANNEL);
		gyro = new ADXRS450_Gyro(SPI::Port::kOnboardCS0);
		//encoder = new Encoder(ENCODER_A_CHANNEL, ENCODER_B_CHANNEL);
		//chassis = new AutoPIDDrive(drive);
		preferences=Preferences::GetInstance();
		lw = LiveWindow::GetInstance();
		//testmotor=new WPI_TalonSRX(TEST_MOTOR_CHANNEL);
		drive->SetSafetyEnabled(false);
		//preferences->PutFloat("pValue", 0.04);
		//preferences->PutFloat("iValue", 0.0);
		//preferences->PutFloat("dValue", 0.04);
		//kP = preferences->GetFloat("pValue", 0.0);
		//kI = preferences->GetFloat("iValue", 0.0);
		//kD = preferences->GetFloat("dValue", 0.0);
		inOut = new InAndOut(PISTON_DOOR_LEFT_CHANNEL, PISTON_DOOR_RIGHT_CHANNEL, MOTOR_ROLL_CHANNEL, MOTOR_CLAW_CHANNEL);
		upDown = new UpAndDown(L_MOTOR_CHANNEL, R_MOTOR_CHANNEL);

		//PID = new PIDController(kP, kI, kD, gyro, chassis);
		//leftEncoder = &(rlmotor->GetSensorCollection());
		//rightEncoder = &(rrmotor->GetSensorCollection());
		sequencer = new JankyAutoSequencer(drive, gyro, &(rlmotor->GetSensorCollection()), &(rrmotor->GetSensorCollection()), rlmotor, rrmotor, inOut, upDown);
		selector->Init();

		//  Game Components
		inOut->StartUpInit();
		//inOut->PIDSetup();
		//		inOut -> Start();
		upDown->StartUpInit();
		//upDown->PIDSetup();
		upDown->Start();
		//make sure robot is left unmoved for ~10 seconds during calibration
		//encoder->SetDistancePerPulse(DISTANCE_PER_PULSE);
	}

	void AutonomousInit() override {
		//retrieving switch info from FMS
		std::string gameData;
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
		std::cout << gameData;
		//switchPos = gameData[0];
		if(gameData.empty()){
			//not connected to FMS
			//switchPos = 'E';  //for at competition
			switchPos = 'L'; //value for testing purposes
			printf("Overriding gameData because no valid FMS data \n");
		}
		else{
			switchPos = gameData[0];
		}

		autonomousTimer.Reset();
		autonomousTimer.Start();

		delayTime = selector->GetDelayTime();
		automode=selector->GetAutoMode(switchPos);
		selector->PrintValues();
		//gyro->Reset();
		//encoder->Reset();
		rlmotor->SetSelectedSensorPosition(0, 0, 10);
		rrmotor->SetSelectedSensorPosition(0, 0, 10);

		//leftEncoder->SetQuadraturePosition(0, 10);
		//rightEncoder->SetQuadraturePosition(0, 10);
		//PID->SetInputRange(-180.0, 180.0);
		//PID->SetOutputRange(-1.0, 1.0);
	}

	void AutonomousPeriodic() {
		//SmartDashboard::PutNumber("Encoder Count", encoder->Get());
		double autoTime=autonomousTimer.Get();
		if  (automode == DEFAULT_MODE) {
			//printf ("default\n");
		}
		else if(automode == L_CROSS_AUTOLINE && autoTime>delayTime){
			//printf("starting left and crossing auto line \n");
			sequencer->SetMode(L_CROSS_AUTOLINE);

		}
		else if(automode == L_SAME_SWITCH && autoTime>delayTime){
			//printf("starting left and loading cube onto switch on left side \n");
			sequencer->SetMode(L_SAME_SWITCH);
		}
		else if(automode == L_OPPOSITE_SWITCH && autoTime>delayTime){
			//printf("starting left and loading cube onto switch on right side \n");
			sequencer->SetMode(L_OPPOSITE_SWITCH);
		}
		else if(automode == M_LEFT_SWITCH && autoTime>delayTime){
			//printf("starting middle and loading cube onto left switch \n");
			sequencer->SetMode(M_LEFT_SWITCH);
		}
		else if(automode == M_RIGHT_SWITCH && autoTime>delayTime){
			//printf("starting middle and loading cube onto right switch \n");
			sequencer->SetMode(M_RIGHT_SWITCH);
		}
		else if(automode == R_CROSS_AUTOLINE && autoTime>delayTime){
			//printf("starting right and crossing auto line \n");
			sequencer->SetMode(R_CROSS_AUTOLINE);
		}
		else if(automode == R_SAME_SWITCH && autoTime>delayTime){
			//printf("starting right and loading cube onto switch on right side \n");
			sequencer->SetMode(R_SAME_SWITCH);
		}
		else if(automode == R_OPPOSITE_SWITCH && autoTime>delayTime){
			//printf("starting right and loading cube onto switch on left side \n");
			sequencer->SetMode(R_OPPOSITE_SWITCH);
		}
	}

	void TeleopInit() {
		sequencer->EndSequence();
		gyro->Reset();
		//UNCOMMENT FOR ACTUAL ROBOT
		/*rlmotor->SetSelectedSensorPosition(0, 0, 10);
		rlmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
		rrmotor->SetSelectedSensorPosition(0, 0, 10);
		rrmotor->GetSensorCollection().SetQuadraturePosition(0, 10);*/
	}

	void TeleopPeriodic() {
		//Driving
		drive->TankDrive(-left->GetY(), -right->GetY());
		//SmartDashboard::PutNumber("Encoder Count", encoder->Get());
		//SmartDashboard::PutNumber("Encoder Distance", (encoder->Get()*MEASURED_DIST_PER_PULSE));
		/*float encoderCount = testmotor->GetSensorCollection().GetQuadraturePosition();
		float encoderDistance = (encoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE; //double check that right type of encoder; also make sure this is a float
		SmartDashboard::PutNumber("Test Encoder Count", encoderCount);
		SmartDashboard::PutNumber("Test Encoder Distance", encoderDistance);*/
	}

	void TestPeriodic() {
		lw->Run();
	}

private:
};

START_ROBOT_CLASS(Robot)
