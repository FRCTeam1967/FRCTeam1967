#include <iostream>
#include <memory>
#include <string>
#include "WPILib.h"
#include "CANTalon.h"
#include "jankyXboxJoystick.h"
#include "TwoTransmissions.h"
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <cmath>
//#define FRONT_LEFT_MOTOR_CHANNEL 2
//#define REAR_LEFT_MOTOR_CHANNEL 1
//#define FRONT_RIGHT_MOTOR_CHANNEL 5
//#define REAR_RIGHT_MOTOR_CHANNEL 6
#define LPISTON_CHANNEL 0 //not sure yet
#define RPISTON_CHANNEL 0 //not sure yet
#define LPISTON_MOD 0 //not sure yet
#define RPISTON_MOD 0 //not sure yet
#define FRONT_LEFT_MOTOR_CHANNEL 3
#define REAR_LEFT_MOTOR_CHANNEL 4
#define FRONT_RIGHT_MOTOR_CHANNEL 1
#define REAR_RIGHT_MOTOR_CHANNEL 2
#define JOYSTICK_CHANNEL 0
#define RIGHT_ENCODER_CHANNEL_1 5 //not sure yet
#define RIGHT_ENCODER_CHANNEL_2 6 //not sure yet
#define LEFT_ENCODER_CHANNEL_1 1 //not sure yet
#define LEFT_ENCODER_CHANNEL_2 2 //not sure yet


class Robot: public frc::IterativeRobot {
	CANTalon*flmotor;
	CANTalon*rlmotor;
	CANTalon*frmotor;
	CANTalon*rrmotor;
	jankyXboxJoystick*joystick;
	TwoTransmissions*twotransmissions;
	RobotDrive*drive;
	BuiltInAccelerometer*Baccel;
	Encoder*right_encoder;
	Encoder*left_encoder;
	double joystickdif;
	bool Xnotpressed = true;
	bool Bnotpressed = true;
	float avg;
public:
	Robot(){
		flmotor=NULL;
		rlmotor=NULL;
		frmotor=NULL;
		rrmotor=NULL;
		joystick=NULL;
		twotransmissions=NULL;
		drive=NULL;
		Baccel = NULL;
		right_encoder=NULL;
		left_encoder=NULL;
	}
	~Robot(){
		delete flmotor;
		delete rlmotor;
		delete frmotor;
		delete rrmotor;
		delete joystick;
		delete twotransmissions;
		delete drive;
		delete Baccel;
		delete right_encoder;
		delete left_encoder;
	}

	void RobotInit() {
		flmotor= new CANTalon(FRONT_LEFT_MOTOR_CHANNEL);
		rlmotor= new CANTalon(REAR_LEFT_MOTOR_CHANNEL);
		frmotor= new CANTalon(FRONT_RIGHT_MOTOR_CHANNEL);
		rrmotor= new CANTalon(REAR_RIGHT_MOTOR_CHANNEL);
		joystick= new jankyXboxJoystick(JOYSTICK_CHANNEL);
		twotransmissions= new TwoTransmissions(LPISTON_CHANNEL, RPISTON_CHANNEL, LPISTON_MOD, RPISTON_MOD);
		drive= new RobotDrive(flmotor, rlmotor, frmotor, rrmotor);
		Baccel = new BuiltInAccelerometer();
		right_encoder = new Encoder(RIGHT_ENCODER_CHANNEL_1, RIGHT_ENCODER_CHANNEL_2);
		left_encoder = new Encoder(LEFT_ENCODER_CHANNEL_1, LEFT_ENCODER_CHANNEL_2);
		right_encoder->Reset();
		left_encoder->Reset();
	}

	void AutonomousInit() override {

	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {
		right_encoder->Reset();
		left_encoder->Reset();

	}

	void TeleopPeriodic() {
		//Tank Drive
		float leftYaxis= joystick->GetLeftYAxis();
		float rightYaxis= joystick->GetRightYAxis();
		SmartDashboard::PutNumber("Left Y Axis", leftYaxis);
		SmartDashboard::PutNumber("Right Y Axis", rightYaxis);

		//Code to make robot drive straighter by making both sides be the absolute value of each other when RB is pressed
		if(joystick->GetButtonRB()){
			avg=(leftYaxis+rightYaxis)/2;
			rightYaxis=avg;
			leftYaxis=avg;
			if (leftYaxis<0&&rightYaxis<0){//forward driving
				drive->TankDrive((pow(leftYaxis,2)), (pow(rightYaxis,2)));
			}
			else{//backward driving
				drive->TankDrive((pow(leftYaxis,2)*-1), (pow(rightYaxis,2)*-1));
			}
		}
		else{ //regular driving w/out pressing button
			if (leftYaxis<0&&rightYaxis<0){//forward driving
				drive->TankDrive((pow(leftYaxis,2)), (pow(rightYaxis,2)));
			}
			else if(leftYaxis<0&&rightYaxis>0){
				drive->TankDrive(pow(leftYaxis,2), (pow(rightYaxis,2)*-1));
			}
			else if(leftYaxis>0&&rightYaxis<0){
				drive->TankDrive((pow(leftYaxis,2)*-1), (pow(rightYaxis,2)));
			}
			else{
				drive->TankDrive((pow(leftYaxis,2)*-1), ((pow(rightYaxis,2))*-1));
			}
		}
		//Manual Two Speed Transmissions
		bool ButtonX = joystick->GetButtonX();
		bool ButtonB = joystick->GetButtonB();

			//Button X for Low Gear
			if(ButtonX&&Xnotpressed)
			{
				twotransmissions->LowGear();
				Xnotpressed = false;
			}
			else if(!ButtonX)
			{
				Xnotpressed = true;
			}
			//Button B for High Gear
			if(ButtonB&&Bnotpressed)
			{
				twotransmissions->HighGear();
				Bnotpressed = false;
			}
			else if(!ButtonB)
			{
				Bnotpressed = true;
			}

		//Accelerometer
        Baccel->GetX();
        Baccel->GetY();
        Baccel->GetZ();
        SmartDashboard::PutNumber("Accelerometer-GetX", Baccel->GetX());
        SmartDashboard::PutNumber("Accelerometer-GetY", Baccel->GetY());
        SmartDashboard::PutNumber("Accelerometer-GetZ", Baccel->GetZ());
        //Encoder Distance
        SmartDashboard::PutNumber("Right Encoder Distance", right_encoder->GetDistance());
        SmartDashboard::PutNumber("Left Encoder Distance", left_encoder->GetDistance());
        //Encoder Count
        SmartDashboard::PutNumber("Right Encoder Count", right_encoder->Get());
        SmartDashboard::PutNumber("Left Encoder Count", left_encoder->Get());
        // Encoder Rate
        SmartDashboard::PutNumber("Right Encoder Rate", right_encoder->GetRate());
        SmartDashboard::PutNumber("Left Encoder Rate", left_encoder->GetRate());

	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
