/*
 * SanaPIDDrive.cpp
 *
 *  Created on: Feb 12, 2017
 *      Author: Sana
 */
#include "WPIlib.h"
#include "PIDDrive.h"
#include "SpeedController.h"
#define MOTOR_1_CHANNEL 1
#define MOTOR_2_CHANNEL 2
#define MOTOR_3_CHANNEL 3
#define MOTOR_4_CHANNEL 4



	PIDDrive::PIDDrive(SpeedController * motor1, SpeedController * motor2,SpeedController * motor3,SpeedController * motor4){


		driveRobot = new RobotDrive(motor1, motor2, motor3, motor4);


	}
	PIDDrive::~PIDDrive(void) {

		delete driveRobot;

	}


void PIDDrive::PIDWrite(double val) {
    driveRobot->Drive(-0.5, val);
}

