#include "WPILib.h"
#include "AniTwoTransmissions.h"
#include "jankyDrivestick.h"

#define STICKCHANNEL 0	//The channels need to be changed to be different numbers
#define LRCHANNEL 5
#define LFCHANNEL 6
#define RRCHANNEL 2
#define RFCHANNEL 3
#define LPISTCHANNEL 1
#define RPISTCHANNEL 2
#define LPISTMOD 19
#define RPISTMOD 19

class Robot: public IterativeRobot
{
	jankyDrivestick*joystick;	//Keep all the objects in same order
	CANTalon*lrmotor;
	CANTalon*lfmotor;
	CANTalon*rrmotor;
	CANTalon*rfmotor;
	AniTwoTransmissions*twotransmission;
	RobotDrive*drive;
	bool allowTrigger = true;

public:
	Robot()
	{
		joystick=NULL;
		lrmotor=NULL;
		lfmotor=NULL;
		rrmotor=NULL;
		rfmotor=NULL;
		twotransmission=NULL;
		drive=NULL;
	}

	~Robot()
	{
		delete joystick;
		delete lrmotor;
		delete lfmotor;
		delete rrmotor;
		delete rfmotor;
		delete twotransmission;
		delete drive;
	}

	void RobotInit()
	{
		joystick = new jankyDrivestick(STICKCHANNEL);
		lrmotor = new CANTalon(LRCHANNEL);
		lfmotor = new CANTalon(LFCHANNEL);
		rrmotor = new CANTalon(RRCHANNEL);
		rfmotor = new CANTalon(RFCHANNEL);
		twotransmission = new AniTwoTransmissions(LPISTCHANNEL, RPISTCHANNEL, LPISTMOD, RPISTMOD);
		drive = new RobotDrive(lfmotor, lrmotor, rfmotor, rrmotor);
	}

	void TeleopPeriodic()
	{
		float yval=joystick->GetY();
		float xval=joystick->GetX();
		drive->ArcadeDrive(yval, xval);
		bool trigger = joystick->GetTrigger();
		if (allowTrigger&&trigger)
		{
			twotransmission->SwitchGear();
			allowTrigger=false;
		}
		else if (!trigger)
		{
			allowTrigger=true;
		}
		bool lPistonValue = twotransmission->GetPistonL();
		SmartDashboard::PutBoolean("Left Piston Value", lPistonValue);
		bool rPistonValue = twotransmission->GetPistonR();
		SmartDashboard::PutBoolean("Right Piston Value", rPistonValue);
	}
};

START_ROBOT_CLASS(Robot)
