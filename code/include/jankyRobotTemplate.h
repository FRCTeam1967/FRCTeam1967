///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyRobotTemplate for a simple robot for 2013

#ifndef _JANKYROBOTTEMPLATE_H
#define _JANKYROBOTTEMPLATE_H

#define LEFT_FRONT 1
#define RIGHT_FRONT 2
#define LEFT_REAR 3
#define RIGHT_REAR 4
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
	int numberMotors;
	int fourVictorChannels[4];
	int twoVictorChannels[2];
	
	void RobotInit();
	void CheckMotor();
	void CompareChannels();
	void ResetVictorChannels();
	
};

#endif
