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

//chassis channels
#define FRONT_LEFT_MOTOR_CHANNEL 1
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 3
#define REAR_RIGHT_MOTOR_CHANNEL 4
#define ENCODER_UNITS_PER_ROTATION 4096
#define DIAMETER 6
#define CIRCUMFERENCE DIAMETER*M_PI
#define DRIVE_TIME 1.2
#define RAMPING_TIME 0.5

//joystick channels
#define JOYSTICK_CHANNEL 0
#define GC_XBOX_CHANNEL 1

//Add channel numbers later (THESE ARE NOT ACCURATE)
#define MOTOR_CLAW_CHANNEL 6
#define PISTON_DOOR_LEFT_CHANNEL 0
#define PISTON_DOOR_RIGHT_CHANNEL 1
#define LIM_SWITCH_INSIDE_CHANNEL 1
#define LIM_SWITCH_OUTSIDE_CHANNEL 2
#define MOTOR_ROLL_CHANNEL 6
#define CLAW_ENCODER_CHANNEL_1 4
#define CLAW_ENCODER_CHANNEL_2 5
//#define PISTON_IN_OUT_1_CHANNEL 2
//#define PISTON_IN_OUT_2_CHANNEL 3

//These are NOT ACCURATE for the competition robot
#define L_MOTOR_CHANNEL 6
#define R_MOTOR_CHANNEL 3
#define BOTTOM_LIM_SWITCH_CHANNEL 1
#define TOP_LIM_SWITCH_CHANNEL 2
#define GAME_MOTOR_ENCODER_CHANNEL_1 4
#define GAME_MOTOR_ENCODER_CHANNEL_2 5

class Robot : public frc::IterativeRobot {
	//WPI_TalonSRX*flmotor;
	WPI_TalonSRX*rlmotor;
	//WPI_TalonSRX*frmotor;
	WPI_TalonSRX*rrmotor;
	frc::RobotDrive*drive;
	jankyXboxJoystick*xbox;
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


public:
	Robot(){
		//flmotor=NULL;
		rlmotor=NULL;
		//frmotor=NULL;
		rrmotor=NULL;
		drive=NULL;
		xbox=NULL;
		gyro=NULL;
		chassis=NULL;
		encoder=NULL;
		gameJoystick = NULL;
		inOut = NULL;
		upDown = NULL;
	}
	~Robot(){
		//delete flmotor;
		delete rlmotor;
		//delete frmotor;
		delete rrmotor;
		delete drive;
		delete xbox;
		delete gyro;
		delete chassis;
		delete encoder;
		delete gameJoystick;
		delete inOut;
		delete upDown;
	}

	void RobotInit() {
		//flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
		rlmotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
		//frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
		rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
		rlmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
		rrmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
		drive = new frc::RobotDrive(rlmotor, rrmotor); //change for all 4 motors
		xbox = new jankyXboxJoystick(JOYSTICK_CHANNEL);
		gyro = new ADXRS450_Gyro(SPI::Port::kOnboardCS0);
		drive->SetSafetyEnabled(false);
		//chassis = new AutoPIDDrive(drive);
		//PID = new PIDController(kP, kI, kD, gyro, chassis);
		//flmotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		//frmotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		rlmotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		rrmotor->ConfigOpenloopRamp(RAMPING_TIME, 0);
		//CONFIGURE RAMPING FOR TWO OTHER MOTORS THAT ELEC ADDS
		lw = LiveWindow::GetInstance();
		gameJoystick = new jankyXboxJoystick(GC_XBOX_CHANNEL);
		inOut = new InAndOut(PISTON_DOOR_LEFT_CHANNEL, PISTON_DOOR_RIGHT_CHANNEL, MOTOR_ROLL_CHANNEL, MOTOR_CLAW_CHANNEL);
		upDown = new UpAndDown(L_MOTOR_CHANNEL, R_MOTOR_CHANNEL, GAME_MOTOR_ENCODER_CHANNEL_1, GAME_MOTOR_ENCODER_CHANNEL_2);
		gyro->Calibrate(); //make sure robot is left unmoved for ~10 seconds during calibration
	}

	void AutonomousInit() override {
		gyro->Reset();
		rlmotor->SetSelectedSensorPosition(0, 0, 10);
		rlmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
		rrmotor->SetSelectedSensorPosition(0, 0, 10);
		rrmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
	}

	void AutonomousPeriodic() {
		drive->TankDrive(0.4, 0.4);
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
		upDown->Start();
	}

	void TeleopPeriodic() {
		drive->TankDrive(-xbox->GetLeftYAxis(), -xbox->GetRightYAxis());
		double leftEncoderCount= -(rlmotor->GetSensorCollection().GetQuadraturePosition());
		double leftEncoderDistance = (leftEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE;
		double rightEncoderCount= rrmotor->GetSensorCollection().GetQuadraturePosition();
		double rightEncoderDistance = (rightEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE;
		SmartDashboard::PutNumber("Left Encoder Count", leftEncoderCount);
		SmartDashboard::PutNumber("Left Encoder Distance", leftEncoderDistance);
		SmartDashboard::PutNumber("Right Encoder Count", rightEncoderCount);
		SmartDashboard::PutNumber("Right Encoder Distance", rightEncoderDistance);

		// Define all of the buttons/throttles on the joystick
		bool buttonRB = gameJoystick -> GetButtonRB();
		bool buttonLB = gameJoystick -> GetButtonLB();
		float leftValue = gameJoystick -> GetLeftYAxis();
		float rightValue = gameJoystick -> GetRightYAxis();
		//bool buttonBack = gameJoystick ->GetButtonBack();
		//bool buttonStart = gameJoystick -> GetButtonStart();

		//Put claw mechanism up/down based on what limit switches are pressed
		if (buttonRB) {
			if (inOut -> GetLimSwitchInside()) {
				inOut -> MotorClawOutOfRobot();
			}
			else if (inOut -> GetLimSwitchInside() == false){
				inOut -> MotorClawIntoRobot();
			}
		}
		inOut -> MotorClawStopWithLimSwitches();

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
		if (leftValue > 0.2) {
			inOut -> MotorRollForward();
		}
		else if (leftValue < -0.2) {
			inOut -> MotorRollReverse();
		}
		else {
			inOut -> MotorRollStop();
		}

		//Make the claw mechanism go forward or backward manually --> use this for small movements
		if (rightValue > 0.2) {
			inOut -> MotorClawIntoRobot();
		}
		else if (rightValue <-0.2) {
			inOut -> MotorClawOutOfRobot();
		}
		else {
			inOut -> MotorClawStop();
		}

		// Unused
		/*
		//Move claw mechanism with encoders
		if (buttonBack) {
			inOut -> OutsideDistance();
		}
		else if (buttonStart) {
			inOut -> InsideDistance();
		}

		inOut -> MoveClawMechanism();
		 */

		//assign button variables to the corresponding buttons on the joystick
		bool buttonX = gameJoystick -> GetButtonX();
		bool buttonY = gameJoystick -> GetButtonY();
		bool buttonA = gameJoystick -> GetButtonA();
		bool buttonB = gameJoystick -> GetButtonB();
		bool buttonRT = gameJoystick -> GetRightThrottle();

		//  For testing mechanism
		//	float leftValue = gameJoystick -> GetLeftYAxis();

		//have mechanism go up to different heights based on what button is pressed
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

		//For testing up/down mechanism
		/*
		if (leftValue > 0.2) {
			upDown ->RLMotorForward();
		}
		else if (leftValue < -0.2) {
			upDown ->RLMotorReverse();
		}
		else {
			upDown->RLMotorStop();
		}
		 */
	}

	void TestPeriodic() {
		lw->Run();
	}

private:
};

START_ROBOT_CLASS(Robot)
