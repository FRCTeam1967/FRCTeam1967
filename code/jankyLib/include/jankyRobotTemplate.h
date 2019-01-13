///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyRobotTemplate for a simple robot for 2013
///
/// 2015 - WPILib changed SimpleRobot to SampleRobot to discourage the use of this base class.

#include "frc/WPILib.h"

#ifndef _JANKYROBOTTEMPLATE_H
#define _JANKYROBOTTEMPLATE_H


//These four values below HAVE to be checked
#define LEFT_FRONT 4 //5
#define RIGHT_FRONT 3
#define LEFT_REAR 2
#define RIGHT_REAR 1
#define LEFT 15
#define RIGHT 16
#define DEFAULT_NUMBER_MOTORS 2
#define DEFAULT_CHANNEL_CONFLICT 0

class JankyRobotTemplate : public frc::SampleRobot	{
public:
	JankyRobotTemplate(void);
	virtual ~JankyRobotTemplate(void);
	
	//Member variables and pointers
	frc::RobotDrive * pRobot;
	frc::Victor * pLF;
	frc::Victor * pRF;
	frc::Victor * pLR;
	frc::Victor * pRR;
	frc::Victor * pL;
	frc::Victor * pR;
	int currentMotorNumber; //Keeping track of how many motors are currently being used
	virtual void RobotInit();
	void AutonomousInit();
	void OperatorControlInit();
	void ProgramIsAlive();
	void JankyRobotError(const char *pMessage);
	void SetNumberMotors(int desiredNumberMotors);
	void TankDrive(frc::GenericHID *leftStick, frc::GenericHID *rightStick, bool squaredInputs = true);
	void TankDrive(frc::GenericHID &leftStick, frc::GenericHID &rightStick, bool squaredInputs = true);
	void TankDrive(frc::GenericHID *leftStick, uint32_t leftAxis, frc::GenericHID *rightStick, uint32_t rightAxis, bool squaredInputs = true);
	void TankDrive(frc::GenericHID &leftStick, uint32_t leftAxis, frc::GenericHID &rightStick, uint32_t rightAxis, bool squaredInputs = true);
	void TankDrive(float leftValue, float rightValue, bool squaredInputs = true);
	void ArcadeDrive(frc::GenericHID *stick, bool squaredInputs = true);
	void ArcadeDrive(frc::GenericHID &stick, bool squaredInputs = true);
	void ArcadeDrive(frc::GenericHID *moveStick, uint32_t moveChannel, frc::GenericHID *rotateStick, uint32_t rotateChannel, bool squaredInputs = true);
	void ArcadeDrive(frc::GenericHID &moveStick, uint32_t moveChannel, frc::GenericHID &rotateStick, uint32_t rotateChannel, bool squaredInputs = true);
	void ArcadeDrive(float moveValue, float rotateValue, bool squaredInputs = true);


/*	int fourVictorChannels[4];
	int twoVictorChannels[2];
	void RobotInit();
	void CheckMotor();
	void CompareChannels();
	void ResetVictorChannels();
*/
	
};

#endif
