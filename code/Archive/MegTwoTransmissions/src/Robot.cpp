#include "WPILib.h"
#include "MegTwoTransmissions.h"
#include "jankyDrivestick.h"

#define LRCHANNEL 5
#define RRCHANNEL 2
#define LFCHANNEL 6
#define RFCHANNEL 3
#define RPISTONCHANNEL 2
#define LPISTONCHANNEL 1
#define LPISTONMOD 19
#define RPISTONMOD 19
#define JOYSTICKCHANNEL 0

class Robot: public IterativeRobot
{
	CANTalon*lRMotor;
	CANTalon*rRMotor;
	CANTalon*lFMotor;
	CANTalon*rFMotor;
	jankyDrivestick*joystick;
	MegTwoTransmissions* tTrans;
	RobotDrive*drive;
	bool notPressed = true;
public:
	Robot(){
		lRMotor = NULL;
		rRMotor = NULL;
		lFMotor = NULL;
		rFMotor = NULL;
		joystick = NULL;
		tTrans = NULL;
		drive = NULL;
	}
	~Robot(){
		delete lRMotor;
		delete rRMotor;
		delete lFMotor;
		delete rFMotor;
		delete joystick;
		delete tTrans;
		delete drive;

	}
	void RobotInit()
	{
		lRMotor = new CANTalon(LRCHANNEL);
		rRMotor = new CANTalon(RRCHANNEL);
		lFMotor = new CANTalon(LFCHANNEL);
		rFMotor = new CANTalon(RFCHANNEL);
		joystick = new jankyDrivestick(JOYSTICKCHANNEL);
		tTrans = new MegTwoTransmissions(LPISTONCHANNEL, RPISTONCHANNEL, LPISTONMOD, RPISTONMOD);
		drive = new RobotDrive(lFMotor,lRMotor,rFMotor,rRMotor);
	}

	void TeleopPeriodic()
	{
		bool lPistonValue = tTrans->GetPistonL();
		bool rPistonValue = tTrans->GetPistonR();
		SmartDashboard::PutBoolean("Left Piston", lPistonValue);
		SmartDashboard::PutBoolean("Right Piston", rPistonValue);

		float yValue = joystick->GetY();
		float rotate = joystick->GetRawAxis(2);
		drive->ArcadeDrive(-yValue, -rotate);

		bool trigger = joystick->GetTrigger();
		if(trigger && notPressed){
			tTrans->SwitchGear();
			notPressed = false;
		}
		else if(!trigger){
			notPressed = true;
		}

	}
;};

START_ROBOT_CLASS(Robot)
