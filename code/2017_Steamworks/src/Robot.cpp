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
#include "JankyFuelDoor.h"
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <cmath>
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
#define FUEL_COLLECT 1
#define FUEL_CLOSE_DOOR 2
#define FUEL_DUMP 3

class Robot: public frc::IterativeRobot {
	CANTalon*flmotor;
	CANTalon*rlmotor;
	CANTalon*frmotor;
	CANTalon*rrmotor;
	jankyXboxJoystick*drivestick;
	TwoTransmissions*twoTransmissions;
	RobotDrive*drive;
	//Encoder*right_encoder;
	//Encoder*left_encoder;
	RopeClimbing * climb;
	jankyXboxJoystick * gameComponentXbox;
	ADXRS450_Gyro*gyro;
	//PIDDrive*myRobot;
    float kP;
    //PIDController * PID;
    GearsFuel * gefu;
	JankyFuelDoor*fuel_door;
	bool DriveXnotpressed = true;
	bool DriveLBnotpressed = true;
	bool GuelXnotpressed = true;
	// gears/fuel booleans
    bool AnotPressed = true;
    bool XnotPressed = true;
    bool RBnotPressed = true;
    bool LBnotPressed = true;

	float avg;
public:
	Robot(){
			flmotor=NULL;
			rlmotor=NULL;
			frmotor=NULL;
			rrmotor=NULL;
			drivestick=NULL;
			twoTransmissions=NULL;
			drive=NULL;
			//right_encoder=NULL;
			//left_encoder=NULL;
			climb=NULL;
			gameComponentXbox=NULL;
			gyro=NULL;
	        //myRobot = NULL;
	        kP = 0.03;
	        //PID=NULL;
	        gefu = NULL;
	        fuel_door=NULL;
		}
	~Robot(){
			delete flmotor;
			delete rlmotor;
			delete frmotor;
			delete rrmotor;
			delete drivestick;
			delete twoTransmissions;
			delete drive;
			//delete right_encoder;
			//delete left_encoder;
			delete climb;
			delete gameComponentXbox;
			delete gyro;
	        //delete myRobot;
	        //delete PID;
	        delete gefu;
	        delete fuel_door;
		}
	void RobotInit() {
			flmotor= new CANTalon(FRONT_LEFT_MOTOR_CHANNEL);
			rlmotor= new CANTalon(REAR_LEFT_MOTOR_CHANNEL);
			frmotor= new CANTalon(FRONT_RIGHT_MOTOR_CHANNEL);
			rrmotor= new CANTalon(REAR_RIGHT_MOTOR_CHANNEL);
			drivestick= new jankyXboxJoystick(DRIVESTICK_CHANNEL);
			twoTransmissions= new TwoTransmissions(LPISTON_CHANNEL, RPISTON_CHANNEL, LPISTON_MOD, RPISTON_MOD);
			drive= new RobotDrive(flmotor, rlmotor, frmotor, rrmotor);
			//right_encoder = new Encoder(RIGHT_ENCODER_CHANNEL_A, RIGHT_ENCODER_CHANNEL_A);
			//left_encoder = new Encoder(LEFT_ENCODER_CHANNEL_A, LEFT_ENCODER_CHANNEL_A);
			climb = new RopeClimbing(MOTOR_A_CHANNEL, MOTOR_B_CHANNEL, ENCODER_CHANNEL_A, ENCODER_CHANNEL_B, LIMIT_SWITCH_CHANNEL);
			gameComponentXbox = new jankyXboxJoystick(GAME_COMPONENT_XBOX_CHANNEL);
			gyro = new ADXRS450_Gyro(SPI::Port::kOnboardCS0);
	        //myRobot = new PIDDrive(flmotor, rlmotor, frmotor, rrmotor);
	        gefu = new GearsFuel (GEAR_CHANNEL);
	        fuel_door = new JankyFuelDoor(OUTTAKE_CHANNEL, INTAKE_CHANNEL);
			//right_encoder->Reset();
			//left_encoder->Reset();
			gyro->Calibrate();
			twoTransmissions->LowGear();
			drive->SetSafetyEnabled(false);
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

		//Tank Drive
			if(drivestick->GetButtonRB()){//Code to make robot drive straighter by making both sides equal each other when RB is pressed
				avg=(leftYaxis+rightYaxis)/2;
				rightYaxis=avg;
				leftYaxis=avg;
				drive->TankDrive(-lAxisVal,-rAxisVal);
				//Code from squaring joystick value
				/*if (leftYaxis<0&&rightYaxis<0){ //forward driving
					drive->TankDrive((pow(leftYaxis,2)), (pow(rightYaxis,2)));
				}
				else{ //backward driving
					drive->TankDrive((pow(leftYaxis,2)*-1), (pow(rightYaxis,2)*-1));
				}*/
			}
			else{ //regular driving w/out pressing button
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

			//Manual Two Speed Transmissions
			//bool ButtonX = drivestick->GetButtonX();
			bool ButtonLB = drivestick->GetButtonLB();

				//Button X for Low Gear
				/*if(ButtonX&&DriveXnotpressed)
				{
					twoTransmissions->LowGear();
					DriveXnotpressed = false;
				}
				else if(!ButtonX)
				{
					DriveXnotpressed = true;
				}
				//Button LB for High Gear
				if(ButtonLB&&DriveLBnotpressed)
				{
					twoTransmissions->HighGear();
					DriveLBnotpressed = false;
				}
				else if(!ButtonLB)
				{
					DriveLBnotpressed = true;
				}*/

				// hold down LB for high gear
				if (ButtonLB)
				{
					twoTransmissions->LowGear();
				}
				else
				{
					twoTransmissions->HighGear();
				}

			//Climbing Code
			// go through climb states when button A is pressed
				SmartDashboard::PutNumber("climbing encoder get: ", climb->GetEncoder());
				SmartDashboard::PutBoolean("limit switch: ", climb->GetLimitSwitch());
				SmartDashboard::PutNumber("climbing motor A current: ", climb->GetMotorACurrent());
				SmartDashboard::PutNumber("climb motor B current: ", climb->GetMotorBCurrent());
				SmartDashboard::PutNumber("Left Throttle", drivestick->GetLeftThrottle());
				SmartDashboard::PutNumber("Right Throttle", drivestick->GetRightThrottle());
				// Drivestick button Y starts climbing motors
				// Climbing only goes as long as Y is held down
				if (gameComponentXbox->GetButtonY() && !gameComponentXbox->GetButtonB())
				{
					climb->StartClimbing();
				}
				else
				{
					climb->StopClimbing();
				}

				// Drivestick button B stops climbing motors (very useless)
				if (gameComponentXbox->GetButtonB())
				{
					climb->StopClimbing();
				}

				//Gears and Fuel Code
				//Button X for Box pushed out

                //Gear Piston (1): Make piston go out & in with button X
				if (gameComponentXbox->GetButtonX()) {
					gefu->GearOut();
				    printf("gearout \n");
				}
                else {
                    gefu->GearIn();
                }

				//Fuel Door
				// A for collecting fuel (fuel door opens, block pushed down, fuel door closes)
				if(gameComponentXbox->GetButtonA()&&AnotPressed){
					fuel_door->Command(FUEL_COLLECT);
					AnotPressed=false;
				}
				else if (!gameComponentXbox->GetButtonA()){
					AnotPressed=true;
				}
				// RB for closing fuel door
				if(gameComponentXbox->GetButtonRB()&&RBnotPressed){
					fuel_door->Command(FUEL_CLOSE_DOOR);
					RBnotPressed=false;
				}
				else if (!gameComponentXbox->GetButtonRB()){
					RBnotPressed=true;
				}
				// LB (on game component xbox controller) for dumping fuel (opening fuel door)
				if(gameComponentXbox->GetButtonLB()&&LBnotPressed){
					fuel_door->Command(FUEL_DUMP);
					LBnotPressed=false;
				}
				else if(!gameComponentXbox->GetButtonLB()){
					LBnotPressed=true;
				}

				// TODO: figure out if drive team needs this section
/*                //Outtake Piston (2): Make piston go out & in
                //Fuel Outtake
                if(gameComponentXbox->GetButtonA() && AnotPressed) {
                    gefu->Horz();
                    AnotPressed=false;
                }
                else if (gameComponentXbox->GetButtonA() && !AnotPressed) {
                    gefu->Horz2();
                    AnotPressed=true;
                }*/
	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
