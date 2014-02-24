///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyENCODER for a simple robot for 2013

#ifndef _JANKYENCODER_H
#include "jankyTask.h"
#include "WPILib.h"
#define _JANKYENCODER_H
#define RESET_INITMOTORSPEED 0.2
#define DEFAULT_MAX_TIME 9999

class JankyEncoder : public JankyTask	{
public:
	JankyEncoder(int encoderOneChannel, int encoderTwoChannel, int motorOneChannel);
	virtual ~JankyEncoder(void);
	
	//Member functions
	void setRevolution(float desiredRevolution);
	float getRevolution();
	void Reset(void);
	void startMotor();
	void stopMotor();
	void setSpeed(float desiredSpeed);
	bool isDone();
	bool Go();
	void Run();
	Talon * returnMotor(void);
	void SetMaxTime(float maxTime);
	
	//Member variables
	Talon * pMotor; 
	int targetcount;
	float currentRev;
	float motorSpeed;
	Encoder * pEncoder;
	Timer * maxTimer;
	bool bDone;
	bool bEncoding;
	float desiredMaxTime;

};

#endif
