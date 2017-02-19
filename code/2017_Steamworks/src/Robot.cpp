#include <iostream>
#include <memory>
#include <string>
#include "WPILib.h"
#include "CANTalon.h"
#include "jankyXboxJoystick.h"
#include "TwoTransmissions.h"
#include "PIDDrive.h"
#include "RopeClimbing.h"
#include "GearsFuel.h"
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <cmath>
//Channels for Jankybot
//Chassis
#define FRONT_LEFT_MOTOR_CHANNEL 3
#define REAR_LEFT_MOTOR_CHANNEL 4
#define FRONT_RIGHT_MOTOR_CHANNEL 1
#define REAR_RIGHT_MOTOR_CHANNEL 2
#define LPISTON_CHANNEL 0
#define RPISTON_CHANNEL 0
#define LPISTON_MOD 9
#define RPISTON_MOD 9

//Joystick Ports
#define DRIVESTICK_CHANNEL 0
#define GAME_COMPONENT_XBOX_CHANNEL 1

//#define RIGHT_ENCODER_CHANNEL_A 7 //not sure yet
//#define RIGHT_ENCODER_CHANNEL_B 6 //not sure yet
//#define LEFT_ENCODER_CHANNEL_A 4 //not sure yet
//#define LEFT_ENCODER_CHANNEL_B 5 //not sure yet

//Climbing
#define MOTOR_A_CHANNEL 5
#define MOTOR_B_CHANNEL 6
#define ENCODER_CHANNEL_A 1
#define ENCODER_CHANNEL_B 2
#define LIMIT_SWITCH_CHANNEL 0

//Gears and Fuel
#define BALL_MOTOR_SPEED 0.75
#define BALL_MOTOR_CHANNEL 4
#define DOOR_PISTON_CHANNEL 5
#define GEAR_BOX_CHANNEL 1 //the only piston on robot rn
#define PUSH_GEAR_CHANNEL 2

class Robot: public frc::IterativeRobot {
	CANTalon*flmotor;
	CANTalon*rlmotor;
	CANTalon*frmotor;
	CANTalon*rrmotor;
	jankyXboxJoystick*drivestick;
	TwoTransmissions*twotransmissions;
	RobotDrive*drive;
	//Encoder*right_encoder;
	//Encoder*left_encoder;
	RopeClimbing * climb;
	jankyXboxJoystick * gamecomponentxbox;
	ADXRS450_Gyro*gyro;
	//PIDDrive*myRobot;
    float kP;
    //PIDController * PID;
    GearsFuel * gefu;
	bool Xnotpressed = true;
	bool Bnotpressed = true;
	bool GuelXnotpressed = true;
	bool YnotPressed=true;
	float avg;
public:
	Robot(){
			flmotor=NULL;
			rlmotor=NULL;
			frmotor=NULL;
			rrmotor=NULL;
			drivestick=NULL;
			twotransmissions=NULL;
			drive=NULL;
			//right_encoder=NULL;
			//left_encoder=NULL;
			climb=NULL;
			gamecomponentxbox=NULL;
			gyro=NULL;
	        //myRobot = NULL;
	        kP = 0.03;
	        //PID=NULL;
	        gefu = NULL;

		}
	~Robot(){
			delete flmotor;
			delete rlmotor;
			delete frmotor;
			delete rrmotor;
			delete drivestick;
			delete twotransmissions;
			delete drive;
			//delete right_encoder;
			//delete left_encoder;
			delete climb;
			delete gamecomponentxbox;
			delete gyro;
	        //delete myRobot;
	        //delete PID;
	        delete gefu;

		}
	void RobotInit() {
			flmotor= new CANTalon(FRONT_LEFT_MOTOR_CHANNEL);
			rlmotor= new CANTalon(REAR_LEFT_MOTOR_CHANNEL);
			frmotor= new CANTalon(FRONT_RIGHT_MOTOR_CHANNEL);
			rrmotor= new CANTalon(REAR_RIGHT_MOTOR_CHANNEL);
			drivestick= new jankyXboxJoystick(DRIVESTICK_CHANNEL);
			twotransmissions= new TwoTransmissions(LPISTON_CHANNEL, RPISTON_CHANNEL, LPISTON_MOD, RPISTON_MOD);
			drive= new RobotDrive(flmotor, rlmotor, frmotor, rrmotor);
			//right_encoder = new Encoder(RIGHT_ENCODER_CHANNEL_A, RIGHT_ENCODER_CHANNEL_A);
			//left_encoder = new Encoder(LEFT_ENCODER_CHANNEL_A, LEFT_ENCODER_CHANNEL_A);
			climb = new RopeClimbing(MOTOR_A_CHANNEL, MOTOR_B_CHANNEL, ENCODER_CHANNEL_A, ENCODER_CHANNEL_B, LIMIT_SWITCH_CHANNEL);
			gamecomponentxbox = new jankyXboxJoystick(GAME_COMPONENT_XBOX_CHANNEL);
			gyro = new ADXRS450_Gyro(SPI::Port::kOnboardCS0);
	        //myRobot = new PIDDrive(flmotor, rlmotor, frmotor, rrmotor);
	        gefu = new GearsFuel (BALL_MOTOR_CHANNEL, DOOR_PISTON_CHANNEL, GEAR_BOX_CHANNEL, PUSH_GEAR_CHANNEL);
			//right_encoder->Reset();
			//left_encoder->Reset();
			gyro->Calibrate();
			drive->SetSafetyEnabled(false);
			printf("DOne with robot init");

	}

	void AutonomousInit() override {
		drive->SetSafetyEnabled(false);
		gyro->Reset();
   //     PID = new PIDController(kP, 0.005, 0.009, gyro, myRobot);
    //    PID->Enable();
	}

	void AutonomousPeriodic() {
		//PID->SetInputRange(-180,180);
	    //PID->SetOutputRange(-1,1);
		//PID->SetSetpoint(30.0);
	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		printf("In teleop periodic");
		gyro->Reset();
		//Joystick Values
			float leftYaxis= drivestick->GetLeftYAxis();
			float rightYaxis= drivestick->GetRightYAxis();

		//Tank Drive
			if(drivestick->GetButtonRB()){//Code to make robot drive straighter by making both sides equal each other when RB is pressed
				avg=(leftYaxis+rightYaxis)/2;
				rightYaxis=avg;
				leftYaxis=avg;
				if (leftYaxis<0&&rightYaxis<0){ //forward driving
					drive->TankDrive((pow(leftYaxis,2)), (pow(rightYaxis,2)));
				}
				else{ //backward driving
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
			bool ButtonX = drivestick->GetButtonX();
			bool ButtonB = drivestick->GetButtonB();

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

			//Climbing Code
			// go through climb states when button A is pressed
				SmartDashboard::PutNumber("climbing encoder get: ", climb->GetEncoder());
				SmartDashboard::PutBoolean("limit switch: ", climb->GetLimitSwitch());
				//SmartDashboard::PutBoolean("climbing state: ", climb->GetState());

				if (gamecomponentxbox->GetButtonB())
				{
					//climb->SwitchStates();
					climb->StopClimbingMotors();
				}
				if (gamecomponentxbox->GetButtonA())
				{
					//climb->encoder->Reset();
					climb->StartClimbingMotors();
				}
	//Gears and Fuel Code
		//Button X for Box pushed out
		if (gamecomponentxbox->GetButtonX() && GuelXnotpressed) {
			gefu->BoxOut();
			GuelXnotpressed=false;
		}
		else if (!gamecomponentxbox->GetButtonX()) {
			GuelXnotpressed=true;
		}
		//Button Y for Box in
		if (gamecomponentxbox->GetButtonY() && YnotPressed) {
			gefu->BoxIn();
			YnotPressed=false;
		}
		else if(!gamecomponentxbox->GetButtonX()) {
			YnotPressed=true;
		}
	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
