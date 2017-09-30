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

#define TURN_SPEED .2
#define AUTO_TIME 4
#define GAUTO_TIME 3.75
#define STOP_TIME 1.5
#define BGAUTO_TIME 2

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
#define GEAR_CHANNEL 1 // 1 on robot
#define INTAKE_CHANNEL 2 //2 on robot
#define OUTTAKE_CHANNEL 3 //3 on robot
#define STARTCOLLECTING 1
#define CLOSE_DOOR 2
#define DUMP 3

#define ENCODERA_CHANNEL 5
#define ENCODERB_CHANNEL 6

#define STRAIGHT_DISTANCE 288 // inches - set to 288 to cross auto line from back, 206.3 is the distance from the center peg to the line
#define CIRCUMFERENCE 6 * 3.14 // inches
#define PULSES_PER_REVOLUTION 6
#define DISTANCE_PER_PULSE CIRCUMFERENCE / PULSES_PER_REVOLUTION // inches

// sendable chooser
#define DEFAULT_AUTO 1
#define BASELINE_AUTO 2
#define LIT_AUTO 3

class Robot: public frc::IterativeRobot {
	SendableChooser<int*>chooser;
	CANTalon*flmotor;
	CANTalon*rlmotor;
	CANTalon*frmotor;
	CANTalon*rrmotor;
	jankyXboxJoystick*drivestick;
	TwoTransmissions*twoTransmissions;
	RobotDrive*drive;
	Timer autonomousTimer;
	RopeClimbing * climb;
	jankyXboxJoystick*gameComponentXbox;
//	ADXRS450_Gyro*gyro;
	//PIDDrive*myRobot;
    float kP;
    //PIDController * PID;
    GearsFuel * gefu;
	//JankyFuelDoor*fuel_door;
	bool DriveXnotpressed = true;
	bool DriveLBnotpressed = true;
	bool GuelXnotpressed = true;
	// gears/fuel booleans
    bool AnotPressed = true;
    bool XnotPressed = true;

    bool RBnotPressed = true;
    bool LBnotPressed = true;

	float avg;

    Counter * encoderA;
    Counter * encoderB;

    // chooser variables
    int defaultAuto=DEFAULT_AUTO;
    int baseLine= BASELINE_AUTO;
    int autoMode = DEFAULT_AUTO;
    int litAuto = LIT_AUTO;

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
//			gyro=NULL;
	        //myRobot = NULL;
	        kP = 0.03;
	        //PID=NULL;
	        gefu = NULL;
            encoderA = NULL;
	        encoderB = NULL;
	        //fuel_door=NULL;
	        autonomousTimer.Start();
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
//			delete gyro;
	        //delete myRobot;
	        //delete PID;
	        delete gefu;
	        delete encoderA;
	        delete encoderB;
	        //delete fuel_door;
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
//			gyro = new ADXRS450_Gyro(SPI::Port::kOnboardCS0);
	        //myRobot = new PIDDrive(flmotor, rlmotor, frmotor, rrmotor);
	        gefu = new GearsFuel (GEAR_CHANNEL);
	        //fuel_door = new JankyFuelDoor(OUTTAKE_CHANNEL, INTAKE_CHANNEL);
			//right_encoder->Reset();
			//left_encoder->Reset();
//			gyro->Calibrate();
			twoTransmissions->LowGear();

			drive->SetSafetyEnabled(false);
			//fuel_door->SetToQuiet();

			// camera for drive practice
	        cs::UsbCamera cam0 = CameraServer::GetInstance()->StartAutomaticCapture(0);
	        cs::UsbCamera cam1 = CameraServer::GetInstance()->StartAutomaticCapture(1);

			 encoderA = new Counter(ENCODERA_CHANNEL);
			 encoderB = new Counter(ENCODERB_CHANNEL);
			 encoderA->Reset();
			 encoderB->Reset();


			 // sendable chooser
			 		chooser.AddDefault("does nothing (default)", &defaultAuto);
			 		chooser.AddObject("base line", &baseLine);
			 		chooser.AddObject("gear!!", &litAuto);
			 		SmartDashboard::PutData("Autonomous modes", &chooser);
			 		printf("Done with robot init \n");

	}


	void AutonomousInit() override {
		autonomousTimer.Reset();
		autonomousTimer.Start();
		drive->SetSafetyEnabled(false);

//		gyro->Reset();
   //     PID = new PIDController(kP, 0.005, 0.009, gyro, myRobot);
    //    PID->Enable();
		 encoderA->Reset();
		 encoderB->Reset();

//		 	 	autonomous = new Autonomous(drive);
		 		drive->SetSafetyEnabled(false);
		 	//	gyro->Reset();
		 		//     PID = new PIDController(kP, 0.005, 0.009, gyro, myRobot);
		 		//    PID->Enable();
		 		if(&defaultAuto == chooser.GetSelected())
		 		{
		 			printf("default auto yashna is a bleating goat ~anisha \n");
		 			autoMode = DEFAULT_AUTO;
		 		}
		 		else if(&baseLine == chooser.GetSelected())
		 		{
		 			printf("base line also Yashna is a garbage queen ~sana \n");
		 			autoMode = BASELINE_AUTO;
		 		}
		 		else if (&litAuto == chooser.GetSelected()) {
		 			printf("gear auto \n");
		 			autoMode = LIT_AUTO;
		 		}
	}

	void AutonomousPeriodic() {
		//PID->SetInputRange(-180,180);
	    //PID->SetOutputRange(-1,1);
		//PID->SetSetpoint(30.0);

		if (autoMode == DEFAULT_AUTO)
			printf ("default\n");
		else if (autoMode == BASELINE_AUTO)
			{
//			if (encoderA->Get() * DISTANCE_PER_PULSE < STRAIGHT_DISTANCE || encoderB->Get()  * DISTANCE_PER_PULSE < STRAIGHT_DISTANCE)
//				{
//					drive->TankDrive(.6, .6); // gets more inaccurate as speed is increased
//				}
			if(autonomousTimer.Get()<AUTO_TIME) {
				drive->TankDrive(0.5,0.5);
			}
			else if (autonomousTimer.Get()>AUTO_TIME) {
				drive->TankDrive(0.0,0.0);
			}
//			else
//				{
//					drive->TankDrive(0.0, 0.0);
//				}
			}
		else if(autoMode==LIT_AUTO) {
			if(autonomousTimer.Get()<GAUTO_TIME) {
				drive->TankDrive(0.5,0.5);
			}
			else if(autonomousTimer.Get()>GAUTO_TIME && autonomousTimer.Get()<STOP_TIME) {
				drive->TankDrive(0.0,0.0);
				gefu->GearOut();
			}
			else if(autonomousTimer.Get()>STOP_TIME && autonomousTimer.Get()<BGAUTO_TIME) {
				drive->TankDrive(-0.5, -0.5);
			}
			else{
				drive->TankDrive(0.0,0.0);
			}


		}

		SmartDashboard::PutNumber("encoder A get", encoderA->Get());
		SmartDashboard::PutNumber("encoder B get", encoderB->Get());
		SmartDashboard::PutNumber("encoder A distance", encoderA->Get() * DISTANCE_PER_PULSE);
		SmartDashboard::PutNumber("encoder B distance", encoderB->Get() * DISTANCE_PER_PULSE);

	}

	void TeleopInit() {
	    //fuel_door->SetToQuiet();
	}

	void TeleopPeriodic() {
		printf("In teleop periodic");
//		gyro->Reset();

		// TODO: finish turning with LB and RB
		if (drivestick->GetButtonLB()&&LBnotPressed)
		{
			drive->TankDrive(-.4,.4);
			LBnotPressed=false;
		}
		else if(!drivestick->GetButtonLB()){
			LBnotPressed=true;
		}

		if (drivestick->GetButtonRB())
		{
			drive->TankDrive(.4, -.4);
		}

		//Joystick Values
			float leftYaxis= drivestick->GetLeftYAxis();
			float rightYaxis= drivestick->GetRightYAxis();
			float joystick_sensitivity= JOYSTICK_SENSITIVITY; //
			float lAxisVal= (joystick_sensitivity*(pow(leftYaxis,3)))+((1-joystick_sensitivity)*leftYaxis);
			float rAxisVal= (joystick_sensitivity*(pow(rightYaxis,3)))+((1-joystick_sensitivity)*rightYaxis);

		//Tank Drive
			// RT makes it go straight
			// LT makes it high gear
			// RB makes it turn right
			// LB makes it turn left
			if(drivestick->GetRightThrottle() >= .9){//Code to make robot drive straighter by making both sides equal each other when RB is pressed
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
			bool ButtonLT = drivestick->GetLeftThrottle() >= .9;

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
				if (ButtonLT)
				{
					twoTransmissions->HighGear();
				}
				else
				{
					twoTransmissions->LowGear();
				}

			//Climbing Code
			// go through climb states when button A is pressed
//				SmartDashboard::PutNumber("climbing encoder get: ", climb->GetEncoder());
//				SmartDashboard::PutBoolean("limit switch: ", climb->GetLimitSwitch());
//				SmartDashboard::PutNumber("climbing motor A current: ", climb->GetMotorACurrent());
//				SmartDashboard::PutNumber("climb motor B current: ", climb->GetMotorBCurrent());
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
				}
                else {
                    gefu->GearIn();
                }

				//Fuel Door
				// A for collecting fuel (fuel door opens, block pushed down, fuel door closes)
				bool ButtonA=gameComponentXbox->GetButtonA();
				bool ButtonRB=gameComponentXbox->GetButtonRB();
				bool gameComponentButtonLB=gameComponentXbox->GetButtonLB();
				/*SmartDashboard::PutBoolean("Button A", ButtonA);
				SmartDashboard::PutBoolean("A not Pressed", AnotPressed);
				SmartDashboard::PutBoolean("Button RB", ButtonRB);
				SmartDashboard::PutBoolean("Button LB", gameComponentButtonLB);
				if(ButtonA&&AnotPressed){
					fuel_door->Command(STARTCOLLECTING);
//					printf("Button A Pressed, Fuel Collect Command\n");
					AnotPressed=false;
				}
				else if (!ButtonA){
					AnotPressed=true;
//					printf("Button A not Pressed\n");
				}
				// RB for closing fuel door
				if(ButtonRB&&RBnotPressed){
					fuel_door->Command(CLOSE_DOOR);
//					printf("Button RB Pressed, Fuel Collect Command\n");
					RBnotPressed=false;
				}
				else if (!ButtonRB){
					RBnotPressed=true;
//					printf("Button RB not Pressed\n");
				}
				// LB (on game component xbox controller) for dumping fuel (opening fuel door)
				if(gameComponentButtonLB&&LBnotPressed){
					fuel_door->Command(DUMP);
//					printf("Button LB Pressed, Fuel Collect Command\n");
					LBnotPressed=false;
				}
				else if(!gameComponentButtonLB){
					LBnotPressed=true;
//					printf("Button LB not Pressed\n");
				}*/

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
