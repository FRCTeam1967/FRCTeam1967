#include <iostream>
#include <memory>
#include <string>
#include "WPILib.h"
#include "ctre/Phoenix.h"
#include <ADIS16448_IMU.h>
#include "jankyXboxJoystick.h"
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include <cmath>


#define JOYSTICK_CHANNEL 0

#define LRCHANNEL 2
#define RRCHANNEL 4
#define LFCHANNEL 1
#define RFCHANNEL 3



class Robot: public frc::IterativeRobot {
	WPI_TalonSRX*lRMotor;
	WPI_TalonSRX*rRMotor;
	WPI_TalonSRX*lFMotor;
	WPI_TalonSRX*rFMotor;
	jankyXboxJoystick*driveStick;
	RobotDrive*drive;
	Timer*autonomousTimer;
	ADIS16448_IMU*accelerometer;
	//	Accelerometer*accelerometer;
	double tipAccelerometerValueX;
	double tipAccelerometerValueY;
	double tipAccelerometerValueZ;

public:
	Robot(){
		lRMotor = NULL;
		rRMotor = NULL;
		lFMotor = NULL;
		rFMotor = NULL;
		driveStick = NULL;
		drive = NULL;
	}

	~Robot(){
		delete lRMotor;
		delete rRMotor;
		delete lFMotor;
		delete rFMotor;
		delete driveStick;
		delete drive;
	}

	void RobotInit() {
		lRMotor = new WPI_TalonSRX(LRCHANNEL);
		rRMotor = new WPI_TalonSRX(RRCHANNEL);
		lFMotor = new WPI_TalonSRX(LFCHANNEL);
		rFMotor = new WPI_TalonSRX(RFCHANNEL);
		driveStick = new jankyXboxJoystick(JOYSTICK_CHANNEL);
		drive = new RobotDrive(lFMotor,lRMotor,rFMotor,rRMotor);
		accelerometer = new ADIS16448_IMU();
		//		accelerometer = new BuiltInAccelerometer(Accelerometer::kRange_4G);
	}

	void AutonomousInit() override {
	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {


	}

	void TeleopPeriodic() {
		float lValue = driveStick->GetLeftYAxis();
		float rValue = driveStick->GetRightYAxis();
		drive->TankDrive(-lValue,-rValue ); //motors wired in different directions on both sides
		//-1.0 is value of both joysticks when pushed forward

		//ADIS16448_IMU accelerometer
		tipAccelerometerValueX = accelerometer -> GetAccelX();
		tipAccelerometerValueY = accelerometer -> GetAccelY();
		tipAccelerometerValueZ = accelerometer -> GetAccelZ();

		SmartDashboard::PutNumber("Accelerometer X",tipAccelerometerValueX);
		SmartDashboard::PutNumber("Accelerometer Y", tipAccelerometerValueY);
		SmartDashboard::PutNumber("Accelerometer Z", tipAccelerometerValueZ);

		//BuiltIn accelerometer code

		//		tipAccelerometerValueX = accelerometer->GetX();
		//		tipAccelerometerValueY = accelerometer->GetY();
		//		tipAccelerometerValueZ = accelerometer->GetZ();


	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
