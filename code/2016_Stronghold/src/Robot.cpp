#include "WPILib.h"
#include "MegTwoTransmissions.h"
#include "jankyDrivestick.h"
#include "jankyXboxJoystick.h"
#include "BallManipulation.h"
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
#define L_TST_PISTON_CHANNEL 3
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
#define LS_BOTTOM 5
#define BM_ROLLER_MOTOR 4
#define MOVE_AXIS 0.9

//Scaling Channel
#define SC_ENCODER_A 2
#define SC_ENCODER_B 3
#define SC_PISTON_CHANNEL 1
#define SC_PISTON_MOD 19
#define SC_MOTOR_ONE 7
#define SC MOTOR_TWO 8

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
						BM_ENCODER_B, LS_TOP, LS_BOTTOM);
		xbox = new jankyXboxJoystick(XBOXCHANNEL);
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
			if((lValue > .05 && rValue < -.05)||(lValue < -.05 && rValue > .05)){
				drive->TankDrive(-lValue, rValue);
			}
			else{
				drive->TankDrive(lValue,-rValue );
			}
		}
		if (driveStick->GetButtonRB()){
			reverse = true;
		}
		else if(driveStick->GetButtonLB()){
			reverse = false;
		}

		bool ButtonX = driveStick->GetButtonX();
		bool ButtonB = driveStick->GetButtonB();
		bool Xnotpressed = true;
		bool Bnotpressed = true;
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
		SmartDashboard::PutBoolean("Bottom: ", bman->GetBottomLS());

		if (xbox->GetButtonRB() == true) {
			if (rightYValue < -MOVE_AXIS) {
				bman->PivotUp();
			}
			else if (rightYValue > MOVE_AXIS) {
				bman->PivotDown();
			}
			else {
				bman->StopPivotMotor();
			}
		}
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
				bman->DefenseUp();
			}
			else if (rightYValue > MOVE_AXIS) {
				bman->DefenseDown();
			}
			else {
				bman->StopPivotMotor();
			}
		}

	}
;};

START_ROBOT_CLASS(Robot)
