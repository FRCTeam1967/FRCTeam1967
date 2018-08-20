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
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>

#define FRONT_LEFT_MOTOR_CHANNEL 3
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 5
#define REAR_RIGHT_MOTOR_CHANNEL 4
#define ENCODER_A_CHANNEL 2
#define ENCODER_B_CHANNEL 3
#define JOYSTICK_CHANNEL 0

//removed scale factor to see accuracy
//#define SCALE_FACTOR 1.25
#define ENCODER_UNITS_PER_ROTATION 4096
#define DIAMETER 6.25
#define CIRCUMFERENCE_INCHES DIAMETER*M_PI//*SCALE_FACTOR
#define DRIVE_PID_TIMER 1

class Robot : public frc::IterativeRobot {
	WPI_TalonSRX*flmotor;
	WPI_TalonSRX*rlmotor;
	WPI_TalonSRX*frmotor;
	WPI_TalonSRX*rrmotor;
	jankyXboxJoystick*driveStick;
	//frc::ADXRS450_Gyro*gyro;
	frc::Timer autonomousTimer;
	frc::RobotDrive*drive;

	int inchDistance;
	int target;
	bool distReached = false;
	bool timerDone = false;
	double lEncoderCount;
	double rEncoderCount;

public:
	Robot(){
		flmotor=NULL;
		rlmotor=NULL;
		frmotor=NULL;
		rrmotor=NULL;
		driveStick=NULL;
		//gyro=NULL;
		drive=NULL;
	}

	~Robot(){
		delete flmotor;
		delete rlmotor;
		delete frmotor;
		delete rrmotor;
		delete driveStick;
		//delete gyro;
	}

	void RobotInit() {
		flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
		rlmotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
		frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
		rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
		driveStick = new jankyXboxJoystick(JOYSTICK_CHANNEL);

		frmotor->SetInverted(true);
		rrmotor->SetInverted(true);

		frmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 10);

		frmotor->SetSelectedSensorPosition(0, 0, 10);

		frmotor->ConfigPeakOutputForward(0.4, 10); //configure back to max before teleop
		frmotor->ConfigPeakOutputReverse(-0.4, 10);
		frmotor->ConfigNominalOutputForward(0, 10);
		frmotor->ConfigNominalOutputReverse(0, 10);

		frmotor->Config_kP(0, 0.06, 10);
		frmotor->Config_kI(0, 0.0, 10);
		frmotor->Config_kD(0, 0.0, 10);
		frmotor->Config_kF(0, 0.0, 10);
	}

	void AutonomousInit() override {
		//deleting robotdrive so it doesn't conflict with talons
		if(drive){
			delete drive;
			drive=NULL;
		}

		//startRightEncoderCount=frmotor->GetSensorCollection().GetQuadraturePosition();

		inchDistance=80;
		target = (inchDistance/CIRCUMFERENCE_INCHES)*ENCODER_UNITS_PER_ROTATION; //converting inches to encoder counts
		//printf("Left Encoder distance %f Right Encoder distance %f \n", (lEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES, (rEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES);

		flmotor->Set(ControlMode::Follower, 5); //5 for id of motor to follow
		rrmotor->Set(ControlMode::Follower, 5);
		rlmotor->Set(ControlMode::Follower, 5);
		frmotor->Set(ControlMode::Position, target);
	}

	void AutonomousPeriodic() {
		lEncoderCount = -flmotor->GetSensorCollection().GetQuadraturePosition();
		rEncoderCount = frmotor->GetSensorCollection().GetQuadraturePosition();

		//printf("FL Motor Output %f \n", flmotor->Get());
		//printf("FR Motor Output %f \n", frmotor->Get());
		//printf("RL Motor Output %f \n", rlmotor->Get());
		//printf("RR Motor Output %f \n", rrmotor->Get());


		if(!distReached){
			//printf("Left Encoder distance %f Right Encoder distance %f \n", (lEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES, (rEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE_INCHES);
		}
		if(rEncoderCount>=target && !distReached){
			autonomousTimer.Start(); //timer for pid to stabilize once it reaches target
			distReached=true;
			printf("dist reached \n");
		}
		if(autonomousTimer.Get()>DRIVE_PID_TIMER && !timerDone){
			timerDone=true;
			frmotor->Set(ControlMode::PercentOutput, 0);
			flmotor->Set(ControlMode::PercentOutput, 0);
			rrmotor->Set(ControlMode::PercentOutput, 0);
			rlmotor->Set(ControlMode::PercentOutput, 0);
		}
	}

	void TeleopInit() {
		if(drive){
			delete drive;
			drive=NULL;
		}
		drive= new frc::RobotDrive(flmotor, rlmotor, frmotor, rrmotor);

		frmotor->Set(ControlMode::PercentOutput, 0);
		flmotor->Set(ControlMode::PercentOutput, 0);
		rrmotor->Set(ControlMode::PercentOutput, 0);
		rlmotor->Set(ControlMode::PercentOutput, 0);

		frmotor->ConfigPeakOutputForward(1.0, 10);
		frmotor->ConfigPeakOutputReverse(-1.0, 10);
		frmotor->ConfigNominalOutputForward(0, 10);
		frmotor->ConfigNominalOutputReverse(0, 10);

		drive->SetSafetyEnabled(false);
	}

	void TeleopPeriodic() {
		drive->TankDrive(-driveStick->GetLeftYAxis(), driveStick->GetRightYAxis());
	}

	void TestPeriodic() {}
};

START_ROBOT_CLASS(Robot)
