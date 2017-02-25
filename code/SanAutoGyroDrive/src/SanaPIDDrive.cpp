/*
 * SanaPIDDrive.cpp
 *
 *  Created on: Feb 12, 2017
 *      Author: Sana
 */
#include "WPIlib.h"
#include "PIDDrive.h"
#include "SpeedController.h"
#include "RobotDrive.h"
//#define MOTOR_1_CHANNEL 1
//#define MOTOR_2_CHANNEL 2
//#define MOTOR_3_CHANNEL 3
//#define MOTOR_4_CHANNEL 4
#define MOTOR_CHANNEL_FL 1
#define MOTOR_CHANNEL_RL 2
#define MOTOR_CHANNEL_FR 6
#define MOTOR_CHANNEL_RR 5
#define JOYSTICK_CHANNEL 0



	PIDDrive::PIDDrive(RobotDrive*drive) {
			//SpeedController * motor1, SpeedController * motor2,SpeedController * motor3,SpeedController * motor4){


		driveRobot =drive;



	}
	PIDDrive::~PIDDrive(void) {

		delete driveRobot;

	}


void PIDDrive::PIDWrite(double val) {
	driveRobot->Drive(-0.5, -val);
    SmartDashboard::PutNumber("value", val);
    printf("val %5.2f \n", val );//
}

