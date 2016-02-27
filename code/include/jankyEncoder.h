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
	JankyEncoder(int encoderOneChannel, int encoderTwoChannel, int motorOneChannel, int motorTwoChannel);
	virtual ~JankyEncoder(void);
	
	//Member functions
	void setRevolution(float desiredRevolution);
	float getRevolution();
	void Reset(void);
	void startMotor();
	void stopMotor();
	void motorGo();
	void stopCheck();
	bool typeWind();
	void setWind(bool wind);
	void reverseWindCheck();
	void Stop();
	void setSpeed(float desiredSpeed);
	bool isStopButtonPressed();
	bool isDone();
	bool Go();
	bool ReverseGo();
	void Run();
	CANTalon * returnMotor(void);
	CANTalon * returnMotor2(void);
	void SetMaxTime(float maxTime);
	
	//Member variables
	CANTalon * pMotor;
	CANTalon * pMotor2;
	int targetcount;
	//float currentRev;
	float motorSpeed;
	Encoder * pEncoder;
	Timer * maxTimer;
	bool bDone;
	bool bEncoding;
	bool directionCheck;
	bool windCheck;
	float desiredMaxTime;
	bool motorStop;
	float pEncoderStartVal;
	bool hasStopBeenPressed;

};

#endif
