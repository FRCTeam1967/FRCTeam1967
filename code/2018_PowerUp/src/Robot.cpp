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
#include <IterativeRobot.h>
#include <math.h>
#include "DriveSegment.h"
#include "AutoPIDDrive.h"
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "InAndOut.h"
#include <UpAndDown.h>
#include "jankyTask.h"
#include "jankyDrivestick.h"

//chassis channels
#define FRONT_LEFT_MOTOR_CHANNEL 3
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 5
#define REAR_RIGHT_MOTOR_CHANNEL 4
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

#define X_RES 1000
#define Y_RES 1000

class Robot : public frc::IterativeRobot {
	WPI_TalonSRX*flmotor;
	WPI_TalonSRX*rlmotor;
	WPI_TalonSRX*frmotor;
	WPI_TalonSRX*rrmotor;
	frc::RobotDrive*drive;
	//jankyXboxJoystick*xbox;
	jankyDrivestick*left;
	jankyDrivestick*right;
	frc::ADXRS450_Gyro*gyro;
	AutoPIDDrive*chassis;
	frc::Timer autonomousTimer;
	Encoder*encoder;
	jankyXboxJoystick*gameJoystick;
	InAndOut*inOut;
	UpAndDown*upDown;
	LiveWindow*lw;
	bool lbHasNotBeenPressed = true;
	bool rbHasNotBeenPressed = true;
	bool toggleDoor = true;
	bool _lastButton1 = false;
	bool togglePivot = true;
	double targetPositionRotations;
	std::string _sb;
	cs::UsbCamera*driveTeamCamera;


public:
	Robot(){
		flmotor=NULL;
		rlmotor=NULL;
		frmotor=NULL;
		rrmotor=NULL;
		drive=NULL;
		//xbox=NULL;
		left=NULL;
		right=NULL;
		gyro=NULL;
		chassis=NULL;
		encoder=NULL;
		gameJoystick = NULL;
		inOut = NULL;
		upDown = NULL;
	}
	~Robot(){
		delete flmotor;
		delete rlmotor;
		delete frmotor;
		delete rrmotor;
		delete drive;
		//delete xbox;
		delete left;
		delete right;
		delete gyro;
		delete chassis;
		delete encoder;
		delete gameJoystick;
		delete inOut;
		delete upDown;
	}

	void RobotInit() {
		flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
		rlmotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
		frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
		rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
		rlmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
		rrmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
		drive = new frc::RobotDrive(flmotor, rlmotor, frmotor, rrmotor); //change for all 4 motors
		//xbox = new jankyXboxJoystick(JOYSTICK_CHANNEL);
		left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
		right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);
		gyro = new ADXRS450_Gyro(SPI::Port::kOnboardCS0);
		drive->SetSafetyEnabled(false);
		//chassis = new AutoPIDDrive(drive);
		//PID = new PIDController(kP, kI, kD, gyro, chassis);
		flmotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		frmotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		rlmotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		rrmotor->ConfigOpenloopRamp(RAMPING_TIME, 0);

		//CONFIGURE RAMPING FOR TWO OTHER MOTORS THAT ELEC ADDS
		lw = LiveWindow::GetInstance();
		gameJoystick = new jankyXboxJoystick(GC_XBOX_CHANNEL);
		inOut = new InAndOut(PISTON_DOOR_LEFT_CHANNEL, PISTON_DOOR_RIGHT_CHANNEL, MOTOR_ROLL_CHANNEL, MOTOR_CLAW_CHANNEL);
		upDown = new UpAndDown(L_MOTOR_CHANNEL, R_MOTOR_CHANNEL);
		gyro->Calibrate(); //make sure robot is left unmoved for ~10 seconds during calibration
		driveTeamCamera = new cs::UsbCamera;
		CameraServer::GetInstance()->StartAutomaticCapture(0);
		driveTeamCamera->SetFPS(15);
		driveTeamCamera->SetResolution(X_RES, Y_RES);
//		CameraServer::GetInstance()->GetVideo();
		CameraServer::GetInstance()->PutVideo("DriveTeamCam", 640, 480);
	}

	void AutonomousInit() override {
		gyro->Reset();
		rlmotor->SetSelectedSensorPosition(0, 0, 10);
		rlmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
		rrmotor->SetSelectedSensorPosition(0, 0, 10);
		rrmotor->GetSensorCollection().SetQuadraturePosition(0, 10);

		//Game components
		//inOut->StartUpInit();
		//inOut->PIDSetup();
		//inOut -> Start();
		//upDown->StartUpInit();
		//upDown->PIDSetup();
		//upDown->Start();
	}

	void AutonomousPeriodic() {
		double leftEncoderCount= -(rlmotor->GetSensorCollection().GetQuadraturePosition());
		double leftEncoderDistance = (leftEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE;
		double rightEncoderCount= rrmotor->GetSensorCollection().GetQuadraturePosition();
		double rightEncoderDistance = (rightEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE;
		SmartDashboard::PutNumber("Left Encoder Count", leftEncoderCount);
		SmartDashboard::PutNumber("Left Encoder Distance", leftEncoderDistance);
		SmartDashboard::PutNumber("Right Encoder Count", rightEncoderCount);
		SmartDashboard::PutNumber("Right Encoder Distance", rightEncoderDistance);

	}

	void TeleopInit() {
		gyro->Reset();
		rlmotor->SetSelectedSensorPosition(0, 0, 10);
		rlmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
		rrmotor->SetSelectedSensorPosition(0, 0, 10);
		rrmotor->GetSensorCollection().SetQuadraturePosition(0, 10);

		//  Game Components
		inOut->StartUpInit();
		//inOut->PIDSetup();
		//		inOut -> Start();
		upDown->StartUpInit();
		//upDown->PIDSetup();
		upDown->Start();
	}

	void TeleopPeriodic() {
		//Driving
		drive->TankDrive(-left->GetY(), -right->GetY());
		//drive->TankDrive(-xbox->GetLeftYAxis(), -xbox->GetRightYAxis());
		double leftEncoderCount= -(rlmotor->GetSensorCollection().GetQuadraturePosition());
		double leftEncoderDistance = (leftEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE;
		double rightEncoderCount= rrmotor->GetSensorCollection().GetQuadraturePosition();
		double rightEncoderDistance = (rightEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE;
		SmartDashboard::PutNumber("Left Encoder Count", leftEncoderCount);
		SmartDashboard::PutNumber("Left Encoder Distance", leftEncoderDistance);
		SmartDashboard::PutNumber("Right Encoder Count", rightEncoderCount);
		SmartDashboard::PutNumber("Right Encoder Distance", rightEncoderDistance);
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

		//Put claw mechanism up/down based on what limit switches are pressed
		//if (buttonRB && rbHasNotBeenPressed == true) {
		//	inOut->GetDesiredDistance();
		//	rbHasNotBeenPressed = false;
		//}
		//else if (!buttonRB && !rbHasNotBeenPressed){
		//	rbHasNotBeenPressed = true;
		//}

		// Open/Close the claw's "doors" with pistons
		if (buttonLB && lbHasNotBeenPressed == true){
			if (toggleDoor) {
				inOut->PistonDoorOpen();
			}
			else {
				inOut->PistonDoorClose();
			}
			toggleDoor = !toggleDoor;
			lbHasNotBeenPressed = false;
		}
		else if (!buttonLB && !lbHasNotBeenPressed) {
			lbHasNotBeenPressed = true;
		}

		//Make the rollers go forward and backward:
		//		if (leftValue > 0.2) {
		//			inOut -> MotorRollReverse();
		//		}
		//		else if (leftValue < -0.2) {
		//			inOut -> MotorRollForward();
		//		}
		//		else {
		//			inOut -> MotorRollStop();
		//		}

		if (buttonRB && rbHasNotBeenPressed == true){
			if (togglePivot) {
				inOut -> MotorRollReverse();
			}
			else {
				inOut -> MotorRollForward();
			}
			togglePivot = !togglePivot;
			rbHasNotBeenPressed = false;
		}
		else if (!buttonRB && !rbHasNotBeenPressed) {
			inOut -> MotorRollStop();
			rbHasNotBeenPressed = true;
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



		//Move motor claw manually
		if (rightValue > 0.2) {
			inOut -> MotorClawIntoRobot();
		}
		else if (rightValue < -0.2) {
			inOut -> MotorClawOutOfRobot();
		}
		else {
			inOut -> MotorClawStop();
		}
	}

	void TestPeriodic() {
		lw->Run();
	}

private:
};

START_ROBOT_CLASS(Robot)
