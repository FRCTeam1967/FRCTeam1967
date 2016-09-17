#include "WPILib.h"
#include "MegTwoTransmissions.h"
#include "jankyDrivestick.h"
#include "jankyXboxJoystick.h"
#include "BallManipulation.h"
#include "JankyScaling.h"
#include "jankyEncoder.h"
#include <math.h>

//Autonomous Pound Defines
#define TIME_CROSS_LOWBAR 2
#define TIME_ARMS .8
#define TIME_CROSS 2
#define TIME_ROCK 1.5
#define TIME_REACH .3
#define TIME_BACK_LOWBAR .7

//Chassis Channels
#define LRCHANNEL 5
#define RRCHANNEL 2
#define LFCHANNEL 6
#define RFCHANNEL 3
#define R_TST_PISTON_CHANNEL 2
#define L_TST_PISTON_CHANNEL 3 // three is not really there
#define L_TST_PISTON_MOD 19
#define R_TST_PISTON_MOD 19
#define DEADBAND_VALUE .1

//Joystick Channels
#define DRIVESTICKCHANNEL 0
#define XBOXCHANNEL 1

//B-Man Channels/Speeds
#define BM_ENCODER_A 0
#define BM_ENCODER_B 1
#define BM_PIVOT_MOTOR 1
#define LS_TOP 4
#define LS_MIDDLE 6
#define LS_BOTTOM 5
#define BM_PISTON 4
#define BM_PISTON_MOD 19
#define BM_ROLLER_MOTOR 4
#define MOVE_AXIS 0.2

//Scaling Channel
#define SC_ENCODER_A 2
#define SC_ENCODER_B 3
#define SC_PISTON_CHANNEL 1
#define SC_PISTON_MOD 19
#define SC_MOTOR_ONE 7
#define SC_MOTOR_TWO 8

// on janky bot- 8 is used for chassis

class Robot: public IterativeRobot
{
	SendableChooser*chooserTwo;
	CANTalon*lRMotor;
	CANTalon*rRMotor;
	CANTalon*lFMotor;
	CANTalon*rFMotor;
	jankyXboxJoystick*driveStick;
	MegTwoTransmissions* tTrans;
	RobotDrive*drive;
	bool notPressed = true;
	BallManipulation * bman;
	jankyXboxJoystick * xbox;
	bool reverse = true;
	Timer*autonomousTimer;
	jankyScaling*scaling;				//creating pointer for jankyScaling to be called scaling
	bool toggle = true;
	bool pivotPressed = false;
	bool scalingStopCheck = false;
	bool Xnotpressed = true;
	bool Bnotpressed = true;

public:
	Robot(){
		chooserTwo = NULL;
		lRMotor = NULL;
		rRMotor = NULL;
		lFMotor = NULL;
		rFMotor = NULL;
		driveStick = NULL;
		tTrans = NULL;
		drive = NULL;
		bman = NULL;
		xbox = NULL;
		scaling = NULL;
		autonomousTimer = new Timer();
		autonomousTimer->Start();
	}
	~Robot(){
		delete chooserTwo;
		delete lRMotor;
		delete rRMotor;
		delete lFMotor;
		delete rFMotor;
		delete driveStick;
		delete tTrans;
		delete drive;
		delete bman;
		delete xbox;
		delete scaling;

	}
private:
	//autonomous variables
	int defaultAuto;
	int reachDefenseAuto;
	int crossRockDefenseAuto;
	int crossLowBarAuto;
	int lowBarBackAuto;
	int crossLowBarBoulderAuto;
	int lowBarBackBoulderAuto;

	void RobotInit()
	{
		printf("hi");
		chooserTwo = new SendableChooser();
		lRMotor = new CANTalon(LRCHANNEL);
		rRMotor = new CANTalon(RRCHANNEL);
		lFMotor = new CANTalon(LFCHANNEL);
		rFMotor = new CANTalon(RFCHANNEL);
		driveStick = new jankyXboxJoystick(DRIVESTICKCHANNEL);
		tTrans = new MegTwoTransmissions(L_TST_PISTON_CHANNEL, R_TST_PISTON_CHANNEL, L_TST_PISTON_MOD, R_TST_PISTON_MOD);
		drive = new RobotDrive(lFMotor,lRMotor,rFMotor,rRMotor);
		bman = new BallManipulation(BM_ROLLER_MOTOR , BM_PIVOT_MOTOR, BM_ENCODER_A,
						BM_ENCODER_B, LS_TOP, LS_MIDDLE, LS_BOTTOM, BM_PISTON_MOD, BM_PISTON);
		xbox = new jankyXboxJoystick(XBOXCHANNEL);
		scaling = new jankyScaling(SC_ENCODER_A, SC_ENCODER_B,SC_MOTOR_ONE, SC_MOTOR_TWO, SC_PISTON_CHANNEL );
		drive -> SetSafetyEnabled(false);
		chooserTwo->AddDefault("Does nothing(Default)",&defaultAuto);
		chooserTwo->AddObject("Reaches Defense", &reachDefenseAuto);
		chooserTwo->AddObject("Crosses Rock Wall+ Terrain",&crossRockDefenseAuto);
		chooserTwo->AddObject("Crosses Low Bar(no ball)",&crossLowBarAuto);
		chooserTwo->AddObject("Crosses Low Bar and Comes Back(no ball)",&lowBarBackAuto);
		chooserTwo->AddObject("Crosses Low Bar(w/ ball)",&crossLowBarBoulderAuto);
		chooserTwo->AddObject("Crosses Low Bar and Comes Back(w/ ball)",&lowBarBackBoulderAuto);
		SmartDashboard::PutData("Autonomous modes", chooserTwo);
	}
	void AutonomousInit()
		{
			autonomousTimer->Reset();
			drive->SetSafetyEnabled(false);
			if(&defaultAuto == chooserTwo->GetSelected()){
				printf("default auto/n");
			}
			else if(&reachDefenseAuto == chooserTwo->GetSelected()){
				while (autonomousTimer->Get() <TIME_REACH){
					drive->ArcadeDrive(0.0, 1.0);
				}
				drive->ArcadeDrive(0.0,0.0);
			}
			else if(&crossRockDefenseAuto == chooserTwo->GetSelected()){
				while(autonomousTimer->Get() < TIME_ROCK){
					drive->ArcadeDrive(0.0, 1.0);
				}
				drive->ArcadeDrive(0.0,0.0);
			}
			else if(&crossLowBarAuto == chooserTwo->GetSelected()){
				while(autonomousTimer->Get() < TIME_ARMS){
					bman->DownForAuto();
				}
				bman->StopPivotMotor();
				while (autonomousTimer ->Get() > TIME_ARMS && autonomousTimer->Get() < TIME_CROSS_LOWBAR){
					drive->ArcadeDrive(0.0, 1.0);
				}
				drive->ArcadeDrive(0.0,0.0);
			}
			else if(&lowBarBackAuto == chooserTwo->GetSelected()){
				while(autonomousTimer->Get() < TIME_CROSS_LOWBAR){
					drive->ArcadeDrive(0.0, 1.0);
				}
				while(autonomousTimer->Get() >= TIME_CROSS_LOWBAR && autonomousTimer->Get() < TIME_BACK_LOWBAR){
					drive->ArcadeDrive(-1.0, 0.0);
				}
				drive->ArcadeDrive(0.0,0.0);
			}
			else if(&crossLowBarBoulderAuto== chooserTwo->GetSelected()){
				while(autonomousTimer->Get() < TIME_CROSS_LOWBAR){
					drive->ArcadeDrive(1.0, 0.0);
				}
				bman->PushOut();
				drive->ArcadeDrive(0.0,0.0);
			}
			else if(&lowBarBackBoulderAuto== chooserTwo->GetSelected()){
				while(autonomousTimer->Get() < TIME_CROSS_LOWBAR){
					drive->ArcadeDrive(1.0, 0.0);
				}
				while(autonomousTimer->Get() >= TIME_CROSS_LOWBAR && autonomousTimer->Get() < TIME_BACK_LOWBAR){
					drive->ArcadeDrive(-1.0, 0.0);
				}
				bman->PushOut();
				drive->ArcadeDrive(0.0,0.0);
			}

		}

	void TeleopInit(){
			scaling->ScalingStart();
			bman->BmanStart();
		}

	void TeleopPeriodic()
	{
		bool lPistonValue = tTrans->GetPistonL();
		bool rPistonValue = tTrans->GetPistonR();
		float leftYValue = xbox->GetLeftYAxis();
		float rightYValue = xbox->GetRightYAxis();
		SmartDashboard::PutBoolean("High Gear Left Piston", lPistonValue);
		SmartDashboard::PutBoolean("High Gear Right Piston", rPistonValue);
		SmartDashboard::PutBoolean("Is Chassis Reversed", reverse);
		float lValue = driveStick->GetLeftYAxis();
		float rValue = driveStick->GetRightYAxis();
		if (reverse == false){
					drive->TankDrive(-lValue, rValue);
		}
		else{
			/*if((lValue > .05 && rValue < -.05)||(lValue < -.05 && rValue > .05)){
				drive->TankDrive(-lValue, rValue);
			}
			else{
				drive->TankDrive(lValue,-rValue );
			}*/
			drive->TankDrive(lValue,-rValue );
		}
		if (driveStick->GetButtonRB()){
			reverse = true;
		}
		else if(driveStick->GetButtonLB()){
			reverse = false;
		}
		SmartDashboard::PutBoolean("Has Scaling Stopped?", scaling->motorEncoder->itStopped);
		SmartDashboard::PutNumber("Encoder Number", scaling->motorEncoder->pEncoder->Get());
		SmartDashboard::PutNumber("Encoder Revolution", (scaling->motorEncoder->pEncoder->Get()/360));
		SmartDashboard::PutBoolean("Get button x", xbox->GetButtonX());
		SmartDashboard::PutBoolean("Get button y", xbox->GetButtonY());
		SmartDashboard::PutBoolean("Get button a", xbox->GetButtonA());
		SmartDashboard::PutBoolean("Get button b", xbox->GetButtonB());

		//scaling
				if (toggle && xbox->GetButtonX() == true){			//scaling Release; when button A on the Xbox controller is pressed, Release will be enabled
					toggle = false;
					printf("Going to Release now\n");
					scaling->Release();
				}
				else if (toggle && xbox->GetButtonY() == true){			//scaling WindUp; when button B on the Xbox controller is pressed, WindUp will be enabled
					toggle = false;
					printf("Going to Wind Up now\n");
					scaling->ManualLiftUp();
				}
				else if (toggle && xbox->GetButtonA() == true){
					toggle = false;
					printf("Going to Wind Down now\n");
					scaling->ManualDropDown();
				}
				else if (toggle && xbox->GetButtonB() == true){
					toggle = false;
					printf("Stopping WindUp\n");
					scaling->StopWU();
				}
				else if (!xbox->GetButtonX() && !xbox->GetButtonY() && !xbox->GetButtonA() && !xbox->GetButtonB()) {
					toggle = true;
				}
/*
				if (xbox->GetButtonLB()==true)
				{
					scaling->motorEncoder->motorStop=false;
					if (xbox->GetButtonY() == true)
					{
						scaling->ManualLiftUp();
					}
					if (xbox->GetButtonB() == true)
					{
						scaling->ManualDropDown();
					}
					scalingStopCheck=true;

				}*/
				else if(scalingStopCheck==true&&xbox->GetButtonLB()==false)
					{
						scaling->StopWU();
						scalingStopCheck = false;
					}



		bool ButtonX = driveStick->GetButtonX();
		bool ButtonB = driveStick->GetButtonB();

		//Button X for Low Gear
		if(ButtonX&&Xnotpressed)
		{
			tTrans->LowGear();
			Xnotpressed = false;
		}
		else if(!ButtonX)
		{
			Xnotpressed = true;
		}
		//Button B for High Gear
		if(ButtonB&&Bnotpressed)
		{
			tTrans->HighGear();
			Bnotpressed = false;
		}
		else if(!ButtonB)
		{
			Bnotpressed = true;
		}
		//bool trigger = driveStick->GetButtonY();
		//	if(trigger && notPressed){
		//		tTrans->SwitchGear();
		//		notPressed = false;
		//	}
		//	else if(!trigger){
		//		notPressed = true;
		//	}

		//if(abs(rotate) < DEADBAND_VALUE ){
			//drive->ArcadeDrive(0.0,0.0);
		//}
		//else{
			/*if(driveStick->Get2() == true){
				drive->ArcadeDrive(yValue, rotate);
			}
			else{
				drive->ArcadeDrive(-yValue, -rotate);
			}
		//}



		bool trigger = driveStick->GetTrigger();
		if(trigger && notPressed){
			tTrans->SwitchGear();
			notPressed = false;
		}
		else if(!trigger){
			notPressed = true;
		}
		COMMENTED OUT CODE BC THE DRIVE TEAM NOW WANTS TO USE AN XBOX JOYSTICK TO DRIVE
*/
		SmartDashboard::PutNumber("Pivot Encoder Value: ", bman->GetPivotEncoder());
		SmartDashboard::PutNumber("Right X Axis: ", xbox->GetRightXAxis());
		SmartDashboard::PutNumber("Right Y Axis: ", xbox->GetRightYAxis());
		SmartDashboard::PutNumber("Left X Axis: ", xbox->GetLeftXAxis());
		SmartDashboard::PutNumber("Left Y Axis: ", xbox->GetLeftYAxis());
		SmartDashboard::PutNumber("Right Twist: ", xbox->GetRightTwist());
		SmartDashboard::PutBoolean("Top: ", bman->GetTopLS());
		SmartDashboard::PutBoolean("Middle: ", bman->GetMiddleLS());
		SmartDashboard::PutBoolean("Bottom: ", bman->GetBottomLS());
		SmartDashboard::PutNumber("Left Throttle: ", xbox->GetLeftThrottle());
		SmartDashboard::PutNumber("Left Twist: ", xbox->GetLeftTwist());
		SmartDashboard::PutBoolean("Piston: ", bman->GetPiston());
		SmartDashboard::PutBoolean("RB: ", xbox->GetButtonRB());

		/*if (xbox->GetButtonRB() == true) {
			if (rightYValue < -MOVE_AXIS) {
				bman->PivotUp();
			}
			else if (rightYValue > MOVE_AXIS) {
				bman->PivotDown();
			}
			else {
				bman->StopPivotMotor();
			}
		}*/

		//if (xbox->GetButtonB() == true) {
			//bman->PivotBall();
		//}


		// pull in/push out ball using left xbox on xbox
		if (leftYValue < -MOVE_AXIS) {
			bman->PushOut();
		}
		else if (leftYValue > MOVE_AXIS) {
			//printf("%d\n", bman->GetPivotEncoder());
			bman->PullIn();
		}
		else {
			bman->StopBallMotor();
		}


		// move defenses up/down using right xbox on xbox
		if (xbox->GetButtonRB() == false){
			if (rightYValue < -MOVE_AXIS) {
				bman->DefenseUp(-0.4);
			}
			else if (rightYValue > MOVE_AXIS) {
				bman->DefenseDown(0.4);
			}
			else {
				bman->StopPivotMotor();
			}
		}


/*		if (xbox->GetButtonRB() == false){
			if (rightYValue < -MOVE_AXIS) {
				bman->DefenseUp(rightYValue);
			}
			else if (rightYValue > MOVE_AXIS) {
				bman->DefenseDown(rightYValue);
*/

		// move to middle limit switch from top if lb pressed
		if (xbox->GetButtonLB() == true) {
			bman->ToMiddleLS();
		}
		else {
			bman->MiddleLSIdle();
		}

		// shoot goal if rb pressed
		if (xbox->GetButtonRB() == true) {
				SmartDashboard::PutNumber("Piston Path", 1);
				//bman->SetPiston(true);
			bman->ShootGoal();
		}
		else {
			SmartDashboard::PutNumber("Piston Path", 3);
			bman->SetPiston(false);
		}

/*
		// move defenses up/down using right xbox on xbox
		if (xbox->GetButtonRB() == false){
			if (rightYValue < -MOVE_AXIS) {
			   bman->DefenseUp();
			 }
			else if (rightYValue > MOVE_AXIS) {
				   bman->DefenseDown();
			}
			else {
			  bman->StopPivotMotor();
			}
		}
*/
	}


};

START_ROBOT_CLASS(Robot)
