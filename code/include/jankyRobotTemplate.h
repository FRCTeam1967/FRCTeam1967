///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyRobotTemplate for a simple robot for 2013

#ifndef _JANKYROBOTTEMPLATE_H
#define _JANKYROBOTTEMPLATE_H

#define LEFT_FRONT 3
#define RIGHT_FRONT 2
#define LEFT_REAR 4
#define RIGHT_REAR 1
#define LEFT 5
#define RIGHT 6
#define DEFAULT_NUMBER_MOTORS 4
#define DEFAULT_CHANNEL_CONFLICT 0

class JankyRobotTemplate : public SimpleRobot	{
public:
	JankyRobotTemplate(void);
	virtual ~JankyRobotTemplate(void);
	
	
	//Member variables and pointers
	RobotDrive * pRobot;
	Victor * pLF;
	Victor * pRF;
	Victor * pLR;
	Victor * pRR;
	Victor * pL;
	Victor * pR;
	virtual void RobotInit();
	void AutonomousInit();
	void OperatorControlInit();
	void ProgramIsAlive();
	void JankyRobotError(const char *pMessage);
	void SetNumberMotors();
	void TankDrive(GenericHID *leftStick, GenericHID *rightStick, bool squaredInputs = true);
	void TankDrive(GenericHID &leftStick, GenericHID &rightStick, bool squaredInputs = true);
	void TankDrive(GenericHID *leftStick, UINT32 leftAxis, GenericHID *rightStick, UINT32 rightAxis, bool squaredInputs = true);
	void TankDrive(GenericHID &leftStick, UINT32 leftAxis, GenericHID &rightStick, UINT32 rightAxis, bool squaredInputs = true);
	void TankDrive(float leftValue, float rightValue, bool squaredInputs = true);
	void ArcadeDrive(GenericHID *stick, bool squaredInputs = true);
	void ArcadeDrive(GenericHID &stick, bool squaredInputs = true);
	void ArcadeDrive(GenericHID *moveStick, UINT32 moveChannel, GenericHID *rotateStick, UINT32 rotateChannel, bool squaredInputs = true);
	void ArcadeDrive(GenericHID &moveStick, UINT32 moveChannel, GenericHID &rotateStick, UINT32 rotateChannel, bool squaredInputs = true);
	void ArcadeDrive(float moveValue, float rotateValue, bool squaredInputs = true);

/*	int numberMotors;
	int fourVictorChannels[4];
	int twoVictorChannels[2];
	void RobotInit();
	void CheckMotor();
	void CompareChannels();
	void ResetVictorChannels();
*/
};

#endif
