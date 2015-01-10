 /*
 * jankyRobotTemplate.cpp
 *
 *  Created on: Jan 21, 2013
 *      Author: Team1967
 */

#include "jankyRobotTemplate.h"
#include <string>

/*
 * Default constructor
 */

JankyRobotTemplate::JankyRobotTemplate()
{
	
}

/*
 * Default destructor
 */
JankyRobotTemplate::~JankyRobotTemplate()
{
	
}

void JankyRobotTemplate::RobotInit()
{
	printf("JankyRobotInit: Newing victors and RobotDrive\n");
	SmartDashboard::PutNumber("Left Front Victor Channel",LEFT_FRONT);
	SmartDashboard::PutNumber("Right Front Victor Channel",RIGHT_FRONT);
	SmartDashboard::PutNumber("Left Rear Victor Channel",LEFT_REAR);
	SmartDashboard::PutNumber("Right Rear Victor Channel",RIGHT_REAR);
	
	SmartDashboard::PutNumber("Left Victor Channel",LEFT);
	SmartDashboard::PutNumber("Right Victor Channel",RIGHT);
	
	SmartDashboard::PutNumber("Number of Motors",DEFAULT_NUMBER_MOTORS);
	SmartDashboard::PutNumber("Conflict Channel",DEFAULT_CHANNEL_CONFLICT);
	
	SmartDashboard::PutString("Motor Number Status","Default 4");
	SmartDashboard::PutString("Status of Channels","Default");
	
	SmartDashboard::PutString("Begin Changes","");
	
	/*
	pL = new Victor(LEFT);
	pR = new Victor(RIGHT);
	*/
	
	pLF = new Victor(LEFT_FRONT);
	pRF = new Victor(RIGHT_FRONT);
	pLR = new Victor(LEFT_REAR);
	pRR = new Victor(RIGHT_REAR);
	
	if(IsTest())
	{
		printf("JankyRobotTemplate::RobotInit - In Test mode. Adding LiveWindow items.\n");
		LiveWindow *lw = LiveWindow::GetInstance();
		//lw->AddActuator("Victors", "Left", pL);
		//lw->AddActuator("Victors", "Right", pR);
		lw->AddActuator("Victors", "Left Front", pLF);
		lw->AddActuator("Victors", "Right Front", pRF);
		lw->AddActuator("Victors", "Left Rear", pLR);
		lw->AddActuator("Victors", "Right Rear", pRR);
		lw->SetEnabled(true);
	}
	
	pRobot = new RobotDrive(pLF, pLR, pRF, pRR);

	//currentMotorNumber = 2;
	currentMotorNumber = 4;
	
	
}

void JankyRobotTemplate::AutonomousInit()
{
	printf("JankyRobotAutonomous: Setting up Watchdog\n");
// TODO: Figure out if watchdog removal has issues - and what does SetSafetyEnabled=false do?
//	Watchdog &dog = this->GetWatchdog();
//	dog.SetEnabled(false);
//	dog.SetExpiration(0.1);
	pRobot->SetSafetyEnabled(false);
}

void JankyRobotTemplate::OperatorControlInit()
{
	printf("JankyRobotTeleop: Setting up Watchdog\n");
//	Watchdog &dog = this->GetWatchdog();
//	dog.SetEnabled(false);
//	dog.SetExpiration(0.1);
	pRobot->SetSafetyEnabled(false);
}

void JankyRobotTemplate::ProgramIsAlive()
{
//	this->GetWatchdog().Feed();
	//pRobot->FeedSafetyHelper();
	
	static int counter = 0;
	counter++;
/*	
	if (counter > 500)
	{
		std::string sChange = SmartDashboard::GetString("Begin Changes");
		printf("Every second count = %s \n",sChange.c_str());
		std::transform(sChange.begin(), sChange.end(), sChange.begin(), ::tolower);
		if (sChange == "change")
		{
			int desiredNumberMotors = SmartDashboard::GetNumber("Number of Motors");
			printf("Change found\n");
			if (currentMotorNumber != desiredNumberMotors)
			{
				printf("Motor numbers different\n");
				this->SetNumberMotors(desiredNumberMotors);
			}
		}
		counter = 0;
	}
	*/
	
	Wait(0.002);
}

void JankyRobotTemplate::JankyRobotError(const char *pMessage)
{
	printf("JankyError:%s\n",pMessage);
}

void JankyRobotTemplate::SetNumberMotors(int desiredNumberMotors)
{
	if (desiredNumberMotors == 4)
	{
		delete pRobot;
		delete pLF;
		delete pRF;
		delete pLR;
		delete pRR;
		
		pL = new Victor(LEFT);
		pR = new Victor(RIGHT);
		
		pRobot = new RobotDrive(pL, pR);
		
		currentMotorNumber = 2;
	}
	else if (desiredNumberMotors == 2)
	{
		delete pRobot;
		delete pL;
		delete pR;
		
		pLF = new Victor(LEFT_FRONT);
		pRF = new Victor(RIGHT_FRONT);
		pLR = new Victor(LEFT_REAR);
		pRR = new Victor(RIGHT_REAR);
		
		pRobot = new RobotDrive(pLF, pLR, pRF, pRR);
		
		currentMotorNumber = 4;
	}
	else
	{
		JankyRobotError("Motors: Not a valid number of motors");
	}
}

void JankyRobotTemplate::TankDrive(GenericHID *leftStick, GenericHID *rightStick, bool squaredInputs)
{
	if (pRobot)
	{
		pRobot->TankDrive(leftStick, rightStick, squaredInputs);
	}
	else
		JankyRobotError("TankDrive: no pRobot");
}

void JankyRobotTemplate::TankDrive(GenericHID &leftStick, GenericHID &rightStick, bool squaredInputs)
{
	if (pRobot)
	{
		pRobot->TankDrive(leftStick, rightStick, squaredInputs);
	}
	else
		JankyRobotError("TankDrive: no pRobot");
}

void JankyRobotTemplate::TankDrive(GenericHID *leftStick, uint32_t leftAxis, GenericHID *rightStick, uint32_t rightAxis, bool squaredInputs)
{
	if (pRobot)
	{
		pRobot->TankDrive(leftStick, leftAxis, rightStick, rightAxis, squaredInputs);
	}
	else
		JankyRobotError("TankDrive: no pRobot");}

void JankyRobotTemplate::TankDrive(GenericHID &leftStick, uint32_t leftAxis, GenericHID &rightStick, uint32_t rightAxis, bool squaredInputs)
{
	if (pRobot)
	{
		pRobot->TankDrive(leftStick, leftAxis, rightStick, rightAxis, squaredInputs);
	}
	else
		JankyRobotError("TankDrive: no pRobot");
}

void JankyRobotTemplate::TankDrive(float leftValue, float rightValue, bool squaredInputs)
{
	if (pRobot)
	{
		pRobot->TankDrive(leftValue, rightValue, squaredInputs);
	}
	else
		JankyRobotError("TankDrive: no pRobot");
}

void JankyRobotTemplate::ArcadeDrive(GenericHID *stick, bool squaredInputs)
{
	if (pRobot)
	{
		pRobot->ArcadeDrive(stick, squaredInputs);
	}
	else
		JankyRobotError("TankDrive: no pRobot");
}

void JankyRobotTemplate::ArcadeDrive(GenericHID &stick, bool squaredInputs)
{
	if (pRobot)
	{
		pRobot->ArcadeDrive(stick, squaredInputs);
	}
	else
		JankyRobotError("TankDrive: no pRobot");
}

void JankyRobotTemplate::ArcadeDrive(GenericHID *moveStick, uint32_t moveChannel, GenericHID *rotateStick, uint32_t rotateChannel, bool squaredInputs)
{
	if (pRobot)
	{
		pRobot->ArcadeDrive(moveStick, moveChannel, rotateStick, rotateChannel, squaredInputs);
	}
	else
		JankyRobotError("TankDrive: no pRobot");
}

void JankyRobotTemplate::ArcadeDrive(GenericHID &moveStick, uint32_t moveChannel, GenericHID &rotateStick, uint32_t rotateChannel, bool squaredInputs)
{
	if (pRobot)
	{
		pRobot->ArcadeDrive(moveStick, moveChannel, rotateStick, rotateChannel, squaredInputs);
	}
	else
		JankyRobotError("TankDrive: no pRobot");
}

void JankyRobotTemplate::ArcadeDrive(float moveValue, float rotateValue, bool squaredInputs)
{
	if (pRobot)
	{
		pRobot->ArcadeDrive(moveValue, rotateValue, squaredInputs);
	}
	else
		JankyRobotError("TankDrive: no pRobot");
}


/*
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
*/
