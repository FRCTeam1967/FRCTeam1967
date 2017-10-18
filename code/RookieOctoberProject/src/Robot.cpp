#include <iostream>
#include <memory>
#include <string>
#include "WPILib.h"
#include "CANTalon.h"
#include "MegTwoTransmissions.h"
#include "jankyXboxJoystick.h"
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>

//Joystick Channel
#define DRIVESTICKCHANNEL 0

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

class Robot: public frc::IterativeRobot {
	CANTalon*lRMotor;
	CANTalon*rRMotor;
	CANTalon*lFMotor;
	CANTalon*rFMotor;
	jankyXboxJoystick*driveStick;
	MegTwoTransmissions* tTrans;
	RobotDrive*drive;
	Timer*autonomousTimer;

public:
	Robot(){
		lRMotor = NULL;
		rRMotor = NULL;
		lFMotor = NULL;
		rFMotor = NULL;
		driveStick = NULL;
		tTrans = NULL;
		drive = NULL;
		autonomousTimer = new Timer();
		autonomousTimer->Start();

	}

	~Robot(){
		delete lRMotor;
		delete rRMotor;
		delete lFMotor;
		delete rFMotor;
		delete driveStick;
		delete tTrans;
		delete drive;
	}

private:
	void RobotInit() {
		lRMotor = new CANTalon(LRCHANNEL);
		rRMotor = new CANTalon(RRCHANNEL);
		lFMotor = new CANTalon(LFCHANNEL);
		rFMotor = new CANTalon(RFCHANNEL);
		driveStick = new jankyXboxJoystick(DRIVESTICKCHANNEL);
		tTrans = new MegTwoTransmissions(L_TST_PISTON_CHANNEL, R_TST_PISTON_CHANNEL, L_TST_PISTON_MOD, R_TST_PISTON_MOD);
		drive = new RobotDrive(lFMotor,lRMotor,rFMotor,rRMotor);
	}

	void AutonomousInit() override {
		autonomousTimer->Reset();
        autonomousTimer->Start();
		drive->SetSafetyEnabled(false);
	}

	void AutonomousPeriodic() {
		//Example
		while(autonomousTimer->Get() <= 5){
			drive->TankDrive(1.0, 1.0);
		}
	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		float lValue = driveStick->GetLeftYAxis();
		float rValue = driveStick->GetRightYAxis();
		drive->TankDrive(lValue,-rValue ); //motors wired in different directions on both sides
		//-1.0 is value of both joysticks when pushed forward
	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
