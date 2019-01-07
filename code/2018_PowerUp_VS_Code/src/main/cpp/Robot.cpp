/*----------------------------------------------------------------------------*/
/* Copyright (c) 2017-2018 FIRST. All Rights Reserved.                        */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include <iostream>
#include <string>
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "jankyXboxJoystick.h"
// #include <frc/IterativeRobot.h>
#include <math.h>
#include "DriveSegment.h"
#include "AutoPIDDrive.h"
#include <frc/livewindow/LiveWindow.h>
#include <frc/smartDashboard/SendableChooser.h>
#include <frc/smartDashboard/SmartDashboard.h>
#include "InAndOut.h"
#include <UpAndDown.h>
#include "jankyTask.h"
#include "jankyDrivestick.h"
#include "JankyAutoSequencer.h"
#include "JankyAutoSelector.h"

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
#define L_SAME_SCALE 10
#define R_SAME_SCALE 11
#define L_OPPOSITE_SCALE 12
#define R_OPPOSITE_SCALE 13

//chassis channels
#define FRONT_LEFT_MOTOR_CHANNEL 3
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 5
#define REAR_RIGHT_MOTOR_CHANNEL 4
#define ENCODER_A_CHANNEL 2
#define ENCODER_B_CHANNEL 3
#define ENCODER_UNITS_PER_ROTATION 4096
#define DIAMETER 6
#define CIRCUMFERENCE DIAMETER*M_PI
#define DRIVE_TIME 1.2
#define RAMPING_TIME 0.5

//joystick channels
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1
#define GC_XBOX_CHANNEL 2

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
//#define BOTTOM_LIM_SWITCH_CHANNEL 3
//#define TOP_LIM_SWITCH_CHANNEL 2
//#define GAME_MOTOR_ENCODER_CHANNEL_1 4
//#define GAME_MOTOR_ENCODER_CHANNEL_2 5

#define X_RES 320
#define Y_RES 240

class Robot : public frc::TimedRobot {
	JankyAutoSelector*selector;
	JankyAutoSequencer*sequencer;
	frc::Timer autonomousTimer;
	WPI_TalonSRX*flmotor;
	WPI_TalonSRX*rlmotor;
	WPI_TalonSRX*frmotor;
	WPI_TalonSRX*rrmotor;
	frc::RobotDrive*autoDrive;
	frc::SpeedControllerGroup*leftDrive;
	frc::SpeedControllerGroup*rightDrive;
	//frc::DifferentialDrive*drive;
	jankyDrivestick*left;
	jankyDrivestick*right;
	frc::ADXRS450_Gyro*gyro;
	jankyXboxJoystick*gameJoystick;
	InAndOut*inOut;
	UpAndDown*upDown;
	frc::LiveWindow*lw;
	//GC
	bool lbHasNotBeenPressed = true;
	bool rbHasNotBeenPressed = true;
	bool toggleDoor = true;
	bool _lastButton1 = false;
	bool togglePivot = true;
	double targetPositionRotations;
	std::string _sb;
	cs::UsbCamera*driveTeamCamera;
	//Auto
	int delayTime = 0;
	int automode = DEFAULT_MODE;
	char switchPos;
	char scalePos;
	//Driving
	double scaleFactor;
	int upDownEncoderDistance;
	int maxEncoderDistance;

public:
	Robot(){
		selector=NULL;
		sequencer=NULL;
		flmotor=NULL;
		rlmotor=NULL;
		frmotor=NULL;
		rrmotor=NULL;
		left=NULL;
		right=NULL;
		//drive=NULL;
		autoDrive=NULL;
		gyro=NULL;
		gameJoystick = NULL;
		inOut = NULL;
		upDown = NULL;
	}
	~Robot(){
		delete selector;
		//delete sequencer;
		delete flmotor;
		delete rlmotor;
		delete frmotor;
		delete rrmotor;
		delete left;
		delete right;
		//delete drive;
		delete autoDrive;
		delete gyro;
		delete gameJoystick;
		delete inOut;
		delete upDown;
	}

	virtual void RobotInit() override {
		selector = new JankyAutoSelector();
		flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
		rlmotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
		frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
		rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
		flmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
		frmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
		leftDrive = new frc::SpeedControllerGroup(*flmotor, *rlmotor);
		rightDrive = new frc::SpeedControllerGroup(*frmotor, *rrmotor);
		autoDrive = new frc::RobotDrive(flmotor, rlmotor, frmotor, rrmotor);
		//drive = new frc::DifferentialDrive(*leftDrive, *rightDrive);
		left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
		right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);
		gyro = new frc::ADXRS450_Gyro(frc::SPI::Port::kOnboardCS0);
		autoDrive->SetSafetyEnabled(false);
		//drive->SetSafetyEnabled(false);
		flmotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		frmotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		rlmotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		rrmotor->ConfigOpenloopRamp(RAMPING_TIME, 0);

		lw = frc::LiveWindow::GetInstance();
		gameJoystick = new jankyXboxJoystick(GC_XBOX_CHANNEL);
		inOut = new InAndOut(PISTON_DOOR_LEFT_CHANNEL, PISTON_DOOR_RIGHT_CHANNEL, MOTOR_ROLL_CHANNEL, MOTOR_CLAW_CHANNEL);
		upDown = new UpAndDown(L_MOTOR_CHANNEL, R_MOTOR_CHANNEL);
		driveTeamCamera = new cs::UsbCamera;
		frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
		driveTeamCamera->SetFPS(15);
		driveTeamCamera->SetResolution(X_RES, Y_RES);
		//		CameraServer::GetInstance()->GetVideo();
		frc::CameraServer::GetInstance()->PutVideo("DriveTeamCam", 640, 480);
		scaleFactor=1.0;
		//Prepare for auto
		//sequencer = new JankyAutoSequencer(autoDrive, gyro, &(rlmotor->GetSensorCollection()), &(rrmotor->GetSensorCollection()), rlmotor, rrmotor, inOut, upDown);
		sequencer = new JankyAutoSequencer(autoDrive, gyro, &(flmotor->GetSensorCollection()), &(frmotor->GetSensorCollection()), rlmotor, rrmotor, inOut, upDown);
		selector->Init();

		inOut->StartUpInit();
		//inOut->PIDSetup();
		//inOut -> Start();
		upDown->StartUpInit();
		//upDown->PIDSetup();
		upDown->Start();
		gyro->Calibrate();
		printf("done w/ robotinit \n");
		/*rlmotor->SetSelectedSensorPosition(0, 0, 10);
		rlmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
		rrmotor->SetSelectedSensorPosition(0, 0, 10);
		rrmotor->GetSensorCollection().SetQuadraturePosition(0, 10);*/
	}

	virtual void AutonomousInit() override {
		//retrieving switch info from FMS
		std::string gameData;
		gameData = frc::DriverStation::GetInstance().GetGameSpecificMessage();
		std::cout << gameData;
		if(gameData.empty()){
			//not connected to FMS
			//switchPos = 'E';  //for at competition
			//scalePos = 'E';  //for at competition
			switchPos = 'L'; //value for testing purposes
			scalePos = 'L'; //value for testing purposes
			printf("Overriding gameData because no valid FMS data \n");
		}
		else{
			switchPos = gameData[0];
			scalePos = gameData[1];
			printf("Switch Position: %d \n", switchPos);
			printf("Scale Position %d \n", scalePos);
		}

		/*if(sequencer){
			delete sequencer;
			sequencer = NULL;
		}
		sequencer = new JankyAutoSequencer(autoDrive, gyro, &(flmotor->GetSensorCollection()), &(frmotor->GetSensorCollection()), rlmotor, rrmotor, inOut, upDown);*/
		//upDown->StartUpInit();

		delayTime = selector->GetDelayTime();
		automode=selector->GetAutoMode(switchPos, scalePos);

		selector->PrintValues();

		autonomousTimer.Reset();
		autonomousTimer.Start();
		gyro->Reset();
		flmotor->SetSelectedSensorPosition(0, 0, 10);
		flmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
		frmotor->SetSelectedSensorPosition(0, 0, 10);
		frmotor->GetSensorCollection().SetQuadraturePosition(0, 10);


	}

	virtual void AutonomousPeriodic() override {
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
		else if(automode == L_SAME_SCALE && autoTime>delayTime){
			sequencer->SetMode(L_SAME_SCALE);
		}
		else if(automode == R_SAME_SCALE && autoTime>delayTime){
			sequencer->SetMode(R_SAME_SCALE);
		}
		else if(automode == L_OPPOSITE_SCALE && autoTime>delayTime){
			sequencer->SetMode(L_OPPOSITE_SCALE);
		}
		else if(automode == R_OPPOSITE_SCALE && autoTime>delayTime){
			sequencer->SetMode(R_OPPOSITE_SCALE);
		}
	}

	virtual void TeleopInit() override {
		/*if(sequencer){
			sequencer->EndSequence();
			delete sequencer;
			sequencer=NULL;
		}*/

		gyro->Reset();
		flmotor->SetSelectedSensorPosition(0, 0, 10);
		flmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
		frmotor->SetSelectedSensorPosition(0, 0, 10);
		frmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
	}

	virtual void TeleopPeriodic() override {
		//Driving
		//NEED TO TEST
		upDownEncoderDistance=-upDown->GetGameMotorEncoderDistance();
		frc::SmartDashboard::PutNumber("Up&Down Encoder Distance", upDownEncoderDistance);

		if(upDownEncoderDistance<18){
			scaleFactor = 1.0;
		}
		else if(upDownEncoderDistance<35){
			scaleFactor = 1.0;
		}
		else if(upDownEncoderDistance<50){
			scaleFactor = 0.85;
		}
		else if(upDownEncoderDistance<75){
			scaleFactor = 0.7;
		}
		else{
			scaleFactor = 1.0;
		}

		frc::SmartDashboard::PutNumber("Scale Factor", scaleFactor);
		autoDrive->TankDrive(-left->GetY()*scaleFactor, -right->GetY()*scaleFactor);

		//drive->TankDrive(-xbox->GetLeftYAxis(), -xbox->GetRightYAxis());
		double leftEncoderCount= -(flmotor->GetSensorCollection().GetQuadraturePosition());
		double leftEncoderDistance = (leftEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE;
		double rightEncoderCount= frmotor->GetSensorCollection().GetQuadraturePosition();
		double rightEncoderDistance = (rightEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE;
		frc::SmartDashboard::PutNumber("Left Encoder Count", leftEncoderCount);
		frc::SmartDashboard::PutNumber("Left Encoder Distance", leftEncoderDistance);
		frc::SmartDashboard::PutNumber("Right Encoder Count", rightEncoderCount);
		frc::SmartDashboard::PutNumber("Right Encoder Distance", rightEncoderDistance);
		frc::SmartDashboard::PutNumber("Gyro val", gyro->GetAngle());
		//printf("leftEncoderCount %f", leftEncoderCount);
		//printf("rightEncoderCount %f", rightEncoderCount);
		//Game Components
		// Define all of the buttons/throttles on the game controller
		bool buttonRB = gameJoystick -> GetButtonRB();
		bool buttonLB = gameJoystick -> GetButtonLB();
		float leftValue = gameJoystick -> GetLeftYAxis();
		float rightValue = gameJoystick -> GetRightYAxis();
		bool buttonX = gameJoystick -> GetButtonX();
		bool buttonY = gameJoystick -> GetButtonY();
		bool buttonA = gameJoystick -> GetButtonA();
		bool buttonB = gameJoystick -> GetButtonB();
		bool buttonRT = gameJoystick -> GetRightThrottle();
		bool buttonLT = gameJoystick -> GetLeftThrottle();
		bool buttonLeft = gameJoystick->GetButtonBack();
		bool buttonRight = gameJoystick->GetButtonStart();

		// Open/Close the claw's "doors" with pistons
		if (buttonLB ){
			inOut->PistonDoorOpen();
		}
		if (buttonLT){
			inOut->PistonDoorClose();
		}


		//Make the rollers go forward and backward:
		if (leftValue > 0.2) {
			inOut -> MotorRollReverse();
		}
		else if (leftValue < -0.2) {
			inOut -> MotorRollForward();
		}
		else {
			inOut -> MotorRollStop();
		}

		//Have mechanism go up to different heights based on what button is pressed
		if (buttonX) {
			upDown -> SwitchHeight();
		}
		else if (buttonY) {
			upDown -> ScaleLowHeight();
		}
		else if (buttonB) {
			upDown -> ScaleMedHeight();
		}
		else if (buttonA) {
			upDown -> ScaleHight();
		}
		else if (buttonRT) {
			upDown -> RegularHeight();
		}
		else if (buttonRB) {
			upDown ->InBetweenSwitchAndScale();
		}

			if (buttonLeft) {
				upDown->RLMotorReverse();
				upDown->isMechanismRunning = false;
			}
			else if (buttonRight) {
				upDown->RLMotorForward();
				upDown->isMechanismRunning = false;
			}
			else {
				upDown->RLMotorStop();
			}

		//Move motor claw manually
		if (rightValue > 0.2) {
			inOut -> MotorClawOutOfRobot();
		}
		else if (rightValue < -0.2) {
			inOut -> MotorClawIntoRobot();
		}
		else {
			inOut -> MotorClawStop();
		}
		//UNUSED
		/*
		//Put claw mechanism up/down based on what limit switches are pressed
		if (buttonRB && rbHasNotBeenPressed == true) {
			inOut->GetDesiredDistance();
			rbHasNotBeenPressed = false;
		}
		else if (!buttonRB && !rbHasNotBeenPressed){
			rbHasNotBeenPressed = true;
		}

				if (buttonRB && rbHasNotBeenPressed == true){
					if (togglePivot) {
						inOut -> MotorRollStop();
					}
					else {
						inOut -> MotorRollReverse();
					}
					togglePivot = !togglePivot;
					rbHasNotBeenPressed = false;
				}
				else if (!buttonRB && !rbHasNotBeenPressed) {
		//			inOut -> MotorRollStop();
					rbHasNotBeenPressed = true;
				}
		 */
	}

	virtual void TestPeriodic() override {
		lw->Run();
	}

private:
};
int main() 
{ 
	return frc::StartRobot<Robot>(); 
}
