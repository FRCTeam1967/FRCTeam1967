/*
 * jankyRobotTemplate.cpp
 *
 *  Created on: Jan 21, 2013
 *      Author: Team1967
 */

#include "WPILib.h"
#include "jankyRobot.h"
#include "jankyRobotTemplate.h"

/*
 * Default constructor
 */

JankyRobotTemplate::JankyRobotTemplate()
{
	SmartDashboard::PutNumber("Left Front Victor Channel",LEFT_FRONT);
	SmartDashboard::PutNumber("Right Front Victor Channel",RIGHT_FRONT);
	SmartDashboard::PutNumber("Left Rear Victor Channel",LEFT_REAR);
	SmartDashboard::PutNumber("Right Rear Victor Channel",RIGHT_REAR);
	
	SmartDashboard::PutNumber("Left Victor Channel",LEFT);
	SmartDashboard::PutNumber("Right Victor Channel",RIGHT);
	
	SmartDashboard::PutNumber("Number of Motors",DEFAULT_NUMBER_MOTORS);
}

/*
 * Default destructor
 */
JankyRobotTemplate::~JankyRobotTemplate()
{
	
}

void JankyRobotTemplate::RobotInit()
{
	pLF = new Victor(LEFT_FRONT);
	pRF = new Victor(RIGHT_FRONT);
	pLR = new Victor(LEFT_REAR);
	pRR = new Victor(RIGHT_REAR);
	pL = new Victor(LEFT);
	pR = new Victor(RIGHT);
	//myRobot(pLF, pRF, pLR, pRR);
}

void JankyRobotTemplate::CheckMotor()
{
	int numberMotors = SmartDashboard::GetNumber("Number of Motors");
	
	if (numberMotors == 4)
	{
		delete pRobot;
		pRobot = new RobotDrive(pLF, pRF, pLR, pRR);	
	}
	else
	{
		delete pRobot;
		pRobot = new RobotDrive(pL, pR);	
	}
}

bool JankyRobotTemplate::CompareChannels(int channels[])
{
	for (int i=0; i<4; i++)
	{
		for (int j=0; j<4; j++)
		{
			if (i != j)
			{
				if (channels[i] == channels[j])
					return false;
			}
			else
				return true;
		}
	}
}

void JankyRobotTemplate::ResetVictorChannels()
{
	int victorChannels [4];
		
	victorChannels [0] = SmartDashboard::GetNumber("Left Front Victor Channel");
	victorChannels [1] = SmartDashboard::GetNumber("Right Front Victor Channel");
	victorChannels [2] = SmartDashboard::GetNumber("Left Rear Victor Channel");
	victorChannels [3] = SmartDashboard::GetNumber("Right Rear Victor Channel");
	
	if (JankyRobotTemplate::CompareChannels(victorChannels) == true)
	{
		delete pRobot;
		
		delete pLF;
		pLF = new Victor (victorChannels[0]);
		delete pRF;
		pRF = new Victor (victorChannels[1]);
		delete pLR;
		pLR = new Victor (victorChannels[2]);
		delete pRR;
		pRR = new Victor (victorChannels[3]);
		
		//pRobot = new RobotDrive(pL, pR);
		pRobot = new RobotDrive(pLF, pRF, pLR, pRR);
		
		SmartDashboard::PutNumber("Left Front Victor Channel",victorChannels[0]);
		SmartDashboard::PutNumber("Right Front Victor Channel",victorChannels[1]);
		SmartDashboard::PutNumber("Left Rear Victor Channel",victorChannels[2]);
		SmartDashboard::PutNumber("Right Rear Victor Channel",victorChannels[3]);
	}
	else
	{
		SmartDashboard::PutString("Status","Conflict");
	}
}


