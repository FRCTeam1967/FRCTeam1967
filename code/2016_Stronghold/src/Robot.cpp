#include "WPILib.h"
#include "MegTwoTransmissions.h"
#include "jankyDrivestick.h"
#include "jankyXboxJoystick.h"
#include "BallManipulation.h"

//Chassis Channels
#define LRCHANNEL 5
#define RRCHANNEL 2
#define LFCHANNEL 6
#define RFCHANNEL 3
#define R_TST_PISTON_CHANNEL 2
#define L_TST_PISTON_CHANNEL 3
#define L_TST_PISTON_MOD 19
#define R_TST_PISTON_MOD 19

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

class Robot: public IterativeRobot
{
	CANTalon*lRMotor;
	CANTalon*rRMotor;
	CANTalon*lFMotor;
	CANTalon*rFMotor;
	jankyDrivestick*driveStick;
	MegTwoTransmissions* tTrans;
	RobotDrive*drive;
	bool notPressed = true;
	BallManipulation * bman;
	jankyXboxJoystick * xbox;
public:
	Robot(){
		lRMotor = NULL;
		rRMotor = NULL;
		lFMotor = NULL;
		rFMotor = NULL;
		driveStick = NULL;
		tTrans = NULL;
		drive = NULL;
		bman = NULL;
		xbox = NULL;
	}
	~Robot(){
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
	void RobotInit()
	{
		lRMotor = new CANTalon(LRCHANNEL);
		rRMotor = new CANTalon(RRCHANNEL);
		lFMotor = new CANTalon(LFCHANNEL);
		rFMotor = new CANTalon(RFCHANNEL);
		driveStick = new jankyDrivestick(DRIVESTICKCHANNEL);
		tTrans = new MegTwoTransmissions(L_TST_PISTON_CHANNEL, R_TST_PISTON_CHANNEL, L_TST_PISTON_MOD, R_TST_PISTON_MOD);
		drive = new RobotDrive(lFMotor,lRMotor,rFMotor,rRMotor);
		bman = new BallManipulation(BM_ROLLER_MOTOR , BM_PIVOT_MOTOR, BM_ENCODER_A,
						BM_ENCODER_B, LS_TOP, LS_BOTTOM);
		xbox = new jankyXboxJoystick(XBOXCHANNEL);
	}

	void TeleopPeriodic()
	{
		bool lPistonValue = tTrans->GetPistonL();
		bool rPistonValue = tTrans->GetPistonR();
		SmartDashboard::PutBoolean("Left Piston", lPistonValue);
		SmartDashboard::PutBoolean("Right Piston", rPistonValue);

		float yValue = driveStick->GetY();
		float rotate = driveStick->GetRawAxis(2);
		drive->ArcadeDrive(-yValue, -rotate);

		bool trigger = driveStick->GetTrigger();
		if(trigger && notPressed){
			tTrans->SwitchGear();
			notPressed = false;
		}
		else if(!trigger){
			notPressed = true;
		}

		SmartDashboard::PutNumber("Pivot Encoder Value: ", bman->GetPivotEncoder());
		SmartDashboard::PutNumber("Right X Axis: ", xbox->GetRightXAxis());
		SmartDashboard::PutNumber("Right Y Axis: ", xbox->GetRightYAxis());
		SmartDashboard::PutNumber("Left X Axis: ", xbox->GetLeftXAxis());
		SmartDashboard::PutNumber("Left Y Axis: ", xbox->GetLeftYAxis());
		SmartDashboard::PutNumber("Right Twist: ", xbox->GetRightTwist());
		SmartDashboard::PutBoolean("Top: ", bman->GetTopLS());
		SmartDashboard::PutBoolean("Bottom: ", bman->GetBottomLS());

		if (xbox->GetButtonRB() == true) {
			bman->ChangeSpeed();
		}
		if (xbox->GetButtonB() == true) {
			bman->PivotBall();
		}

		float leftYValue = xbox->GetLeftYAxis();
		float rightYValue = xbox->GetRightYAxis();

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
;};

START_ROBOT_CLASS(Robot)
