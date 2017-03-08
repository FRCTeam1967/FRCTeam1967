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
#include "PIDVision.h"
#include "GripPipeline.h"

//Channels for Jankybot
//Chassis
#define FRONT_LEFT_MOTOR_CHANNEL 1//3
#define REAR_LEFT_MOTOR_CHANNEL 2//4
#define FRONT_RIGHT_MOTOR_CHANNEL 3//1
#define REAR_RIGHT_MOTOR_CHANNEL 4//2
#define LPISTON_CHANNEL 0
#define RPISTON_CHANNEL 0
#define LPISTON_MOD 9
#define RPISTON_MOD 9
#define JOYSTICK_SENSITIVITY 0.4

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
#define GEAR_CHANNEL 1
#define INTAKE_CHANNEL 2
#define OUTTAKE_CHANNEL 3

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
	PIDVision*pv;
	//PIDDrive*myRobot;
    float kP;
    //PIDController * PID;
    GearsFuel * gefu;
	bool DriveXnotpressed = true;
	bool DriveBnotpressed = true;
	bool GuelXnotpressed = true;
	//
    bool AnotPressed = true;
    bool BnotPressed = true;
    bool YnotPressed = true;
    bool XnotPressed = true;
    bool LBnotPressed = true;
    bool RBnotPressed = true;
    bool isReadytoPushGear=false;
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
	        pv = NULL;
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
	        delete pv;
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
	        gefu = new GearsFuel (GEAR_CHANNEL, INTAKE_CHANNEL, OUTTAKE_CHANNEL);
			//right_encoder->Reset();
			//left_encoder->Reset();
			gyro->Calibrate();
			twotransmissions->LowGear();
			drive->SetSafetyEnabled(false);
			pv= new PIDVision(drive);
			printf("Done with robot init");


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
	/*	if(isReadytoPushGear) {
			GearOut();
		}*/
	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		printf("In teleop periodic");
		gyro->Reset();
		//Joystick Values
			float leftYaxis= drivestick->GetLeftYAxis();
			float rightYaxis= drivestick->GetRightYAxis();
			float joystick_sensitivity= JOYSTICK_SENSITIVITY; //
			float lAxisVal= (joystick_sensitivity*(pow(leftYaxis,3)))+((1-joystick_sensitivity)*leftYaxis);
			float rAxisVal= (joystick_sensitivity*(pow(rightYaxis,3)))+((1-joystick_sensitivity)*rightYaxis);

			//PIDVision

					if (drivestick->GetButtonY() && YnotPressed) {
						pv->DriveToPeg();
						YnotPressed=false;
					}
					else if (!drivestick->GetButtonY()) {
						YnotPressed=true;
					}
					else if (drivestick->GetButtonA() && AnotPressed) {
						pv->CancelDrivetoPeg();
						AnotPressed=false;
					}
					else if(!drivestick->GetButtonA()) {
						AnotPressed=true;
					}
					SmartDashboard::PutNumber("TapeDistance:", pv->GetDistanceToTape());
					SmartDashboard::PutNumber("Peg Offset from Center :" , pv->GetPegOffsetFromImageCenter());
			if(pv->CapturingVal()) {

			}
			else{
			//Tank Drive
				if(drivestick->GetButtonRB()){
					//Code to make robot drive straighter by making both sides equal each other when RB is pressed
						avg=(leftYaxis+rightYaxis)/2;
						rightYaxis=avg;
						leftYaxis=avg;
						drive->TankDrive(-lAxisVal,-rAxisVal);
									}
				//Code from squaring joystick value
				/*if (leftYaxis<0&&rightYaxis<0){ //forward driving
					drive->TankDrive((pow(leftYaxis,2)), (pow(rightYaxis,2)));
				}
				else{ //backward driving
					drive->TankDrive((pow(leftYaxis,2)*-1), (pow(rightYaxis,2)*-1));
				}*/

				else{//regular driving w/out pressing button
					drive->TankDrive(-lAxisVal,-rAxisVal);
					//Code from squaring joystick value
					/*if (leftYaxis<0&&rightYaxis<0){//forward driving with squared inputs for more precision
						drive->TankDrive((pow(leftYaxis,2)), (pow(rightYaxis,2)));
					}
					else if((leftYaxis<=0&&rightYaxis>0) or (leftYaxis<0&&rightYaxis>=0)){//(leftYaxis<=0&&rightYaxis>0) or (leftYaxis<0&&rightYaxis>=0)
						drive->TankDrive(pow(leftYaxis,2), (pow(rightYaxis,2)*-1));
					}
					else if((leftYaxis>=0&&rightYaxis<0) or (leftYaxis>0&&rightYaxis<=0)){
						drive->TankDrive((pow(leftYaxis,2)*-1), (pow(rightYaxis,2)));
					}
					else{
						drive->TankDrive((pow(leftYaxis,2)*-1), ((pow(rightYaxis,2))*-1));
					}*/
				}
			}
			//Manual Two Speed Transmissions
			bool ButtonX = drivestick->GetButtonX();
			bool ButtonB = drivestick->GetButtonB();

				//Button X for Low Gear
				if(ButtonX&&DriveXnotpressed)
				{
					twotransmissions->LowGear();
					DriveXnotpressed = false;
				}
				else if(!ButtonX)
				{
					DriveXnotpressed = true;
				}
				//Button B for High Gear
				if(ButtonB&&DriveBnotpressed)
				{
					twotransmissions->HighGear();
					DriveBnotpressed = false;
				}
				else if(!ButtonB)
				{
					DriveBnotpressed = true;
				}

			//Climbing Code
			// go through climb states when button A is pressed
				SmartDashboard::PutNumber("climbing encoder get: ", climb->GetEncoder());
				SmartDashboard::PutBoolean("limit switch: ", climb->GetLimitSwitch());
				SmartDashboard::PutNumber("climbing motor A current: ", climb->GetMotorACurrent());
				SmartDashboard::PutNumber("climb motor B current: ", climb->GetMotorBCurrent());
				SmartDashboard::PutNumber("Left Throttle", drivestick->GetLeftThrottle());
				SmartDashboard::PutNumber("Right Throttle", drivestick->GetRightThrottle());
				// Drivestick button A starts climbing motors
				// TODO: Move buttons to game component joystick
			//	if (drivestick->GetButtonA() && !drivestick->GetButtonY())
				//{
				//	climb->StartClimbing();
				//}

				// Drivestick button Y stops climbing motors
				//if (drivestick->GetButtonY())
				//{
					//climb->StopClimbing();
				//}

	//Gears and Fuel Code
		//Button X for Box pushed out

		        if (gamecomponentxbox->GetButtonB()) {
		            gefu->GearOut();
		            printf("gearout \n");
		        }
		            else  {
		             gefu->GearIn();
		        }
		     /*   if (gamecomponentxbox->GetButtonX() && XnotPressed) {
		            gefu->GearIn();
		            XnotPressed=false;
		            printf("gearin \n");
		        }
		        else if(!gamecomponentxbox->GetButtonX()) {
		            XnotPressed=true;
		        }*/
		        // for intake (piston 2)
		        if(gamecomponentxbox->GetButtonA() && AnotPressed) {
		                gefu->Horz();
		                AnotPressed=false;
		                printf("horz \n");
		            }
		        else if (!gamecomponentxbox->GetButtonA()) {
		            AnotPressed=true;
		                }
		            if (gamecomponentxbox->GetButtonY() && (YnotPressed)) {
		                gefu->Horz2();
		                YnotPressed=false;
		                printf("horz2 \n");
		            }
		            else if(!gamecomponentxbox->GetButtonY()) {
		                        YnotPressed=true;
		                    }
		        // for outtake (piston 3)
		        if (gamecomponentxbox->GetButtonLB() && (LBnotPressed)) {
		                gefu->Vert();
		                LBnotPressed=false;
		                printf("vert \n");
		            }
		        else if(!gamecomponentxbox->GetButtonLB()) {
		                LBnotPressed=true;
		            }
		            if (gamecomponentxbox->GetButtonRB() && (RBnotPressed)) {
		                gefu->Vert2();
		                RBnotPressed=false;
		                printf("vert2 \n");
		            }
		            else if(!gamecomponentxbox->GetButtonRB()) {
		                            RBnotPressed=true;
		                        }
	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
