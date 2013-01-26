/*
 * jankyRobotTemplate.cpp
 *
 *  Created on: Jan 21, 2013
 *      Author: Team1967
 */

#include "WPILib.h"
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
	SmartDashboard::PutNumber("Conflict Channel",DEFAULT_CHANNEL_CONFLICT);
	
	SmartDashboard::PutString("Motor Number Status","Default");
	SmartDashboard::PutString("Status of Channels","Default");
	
	numberMotors = DEFAULT_NUMBER_MOTORS;
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
	fourVictorChannels[0] = LEFT_FRONT;
	pRF = new Victor(RIGHT_FRONT);
	fourVictorChannels[0] = RIGHT_FRONT;
	pLR = new Victor(LEFT_REAR);
	fourVictorChannels[0] = LEFT_REAR;
	pRR = new Victor(RIGHT_REAR);
	fourVictorChannels[0] = RIGHT_REAR;
	
	pL = new Victor(LEFT);
	twoVictorChannels[0] = LEFT;
	pR = new Victor(RIGHT);
	twoVictorChannels[0] = RIGHT;
	
	pRobot = new RobotDrive(pLF, pRF, pLR, pRR);
}

void JankyRobotTemplate::CheckMotor()
{
	numberMotors = SmartDashboard::GetNumber("Number of Motors");
	
	if (numberMotors == 4)
	{
		delete pRobot;
		pRobot = new RobotDrive(pLF, pRF, pLR, pRR);	
	}
	else if (numberMotors == 2)
	{
		delete pRobot;
		pRobot = new RobotDrive(pL, pR);	
	}
	else
	{
		SmartDashboard::PutString("Motor Number Status", "Not a valid number of motors");
	}
}

void JankyRobotTemplate::CompareChannels()
{
	int newChannels[numberMotors];
	bool conflict = true;
	
//Comparing new channels to old channels from victor array
	if (numberMotors == 4)
	{
		newChannels[0] = SmartDashboard::GetNumber("Left Front Victor Channel");
		newChannels[1] = SmartDashboard::GetNumber("Right Front Victor Channel");
		newChannels[2] = SmartDashboard::GetNumber("Left Rear Victor Channel");
		newChannels[3] = SmartDashboard::GetNumber("Right Rear Victor Channel");
		
		for (int i=0; i<numberMotors; i++)
		{
			if (newChannels[i] == fourVictorChannels[i])
			{
				SmartDashboard::PutString("Status of Channels","Same channels as last time");
				return;
			}
		}
	}
	else
	{
		newChannels[0] = SmartDashboard::GetNumber("Left Victor Channel");
		newChannels[1] = SmartDashboard::GetNumber("Right Victor Channel");
		
		for (int i=0; i<numberMotors; i++)
		{
			if (newChannels[i] = twoVictorChannels[i])
			{
				SmartDashboard::PutString("Status of Channels","Same channels as last time");
				conflict = true;
				return;
			}
			else
				conflict = false;
		}
	}
	
//Comparing the new channels to themselves - ensure that there aren't any conflicts
	for (int i=0; i<numberMotors; i++)
	{
		for (int j=0; j<numberMotors; j++)
		{
			if (i != j)	//don't want to compare 'i' to itself - should be on the same channel theoretically
			{
				if (newChannels[i] == newChannels[j])
				{
					SmartDashboard::PutString("Status of Channels","Conflict: same channel value");
					conflict = true;
					return;
				}
			}
			else
				conflict = false;
		}
	}

	for (int k=0; k<numberMotors; k++)
	{
		if (numberMotors == 4 && conflict == false)
			newChannels[k] = fourVictorChannels[k];
		if (numberMotors == 2 && conflict == false)
			newChannels[k] = twoVictorChannels[k];
	}
}

void JankyRobotTemplate::ResetVictorChannels()
{
	if (numberMotors == 4)
	{	
		delete pRobot;
		
		delete pLF;
		pLF = new Victor (fourVictorChannels[0]);
		delete pRF;
		pRF = new Victor (fourVictorChannels[1]);
		delete pLR;
		pLR = new Victor (fourVictorChannels[2]);
		delete pRR;
		pRR = new Victor (fourVictorChannels[3]);
		
		pRobot = new RobotDrive(pLF, pRF, pLR, pRR);
		
		SmartDashboard::PutNumber("Left Front Victor Channel",fourVictorChannels[0]);
		SmartDashboard::PutNumber("Right Front Victor Channel",fourVictorChannels[1]);
		SmartDashboard::PutNumber("Left Rear Victor Channel",fourVictorChannels[2]);
		SmartDashboard::PutNumber("Right Rear Victor Channel",fourVictorChannels[3]);
	}
	else if (numberMotors ==2)
	{
		delete pRobot;
		
		delete pL;
		pL = new Victor (twoVictorChannels[0]);
		delete pR;
		pR = new Victor (twoVictorChannels[1]);
		
		pRobot = new RobotDrive(pL,pR);
		
		SmartDashboard::PutNumber("Left Victor Channel",twoVictorChannels[0]);
		SmartDashboard::PutNumber("Right Victor Channel",twoVictorChannels[1]);
	}
	else
	{
		SmartDashboard::PutString("Status of Channels","Conflict");
	}
}


