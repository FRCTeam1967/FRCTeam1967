///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyRobotTemplate for a simple robot for 2013

#ifndef _JANKYROBOTTEMPLATE_H
#define _JANKYROBOTTEMPLATE_H

#define LEFT_FRONT 9 //6
#define RIGHT_FRONT 5
#define LEFT_REAR 4
#define RIGHT_REAR 1
#define LEFT 8 //supposed to be 6?
#define RIGHT 5
#define DEFAULT_NUMBER_MOTORS 2
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
	int currentMotorNumber; //Keeping track of how many motors are currently being used
	virtual void RobotInit();
	void AutonomousInit();
	void OperatorControlInit();
	void ProgramIsAlive();
	void JankyRobotError(const char *pMessage);
	void SetNumberMotors(int desiredNumberMotors);
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


/*	int fourVictorChannels[4];
	int twoVictorChannels[2];
	void RobotInit();
	void CheckMotor();
	void CompareChannels();
	void ResetVictorChannels();
*/
	
};

#endif
