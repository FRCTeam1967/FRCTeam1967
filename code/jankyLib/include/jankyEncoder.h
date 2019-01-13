///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// JankyENCODER for a simple robot for 2013

#ifndef _JANKYENCODER_H
#include "jankyTask.h"
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
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
	void Stop();
	void setSpeed(float desiredSpeed);
	bool isDone();
	bool Go();
	bool ReverseGo();
	void Run();
	WPI_TalonSRX * returnMotor(void);
	WPI_TalonSRX * returnMotor2(void);
	void SetMaxTime(float maxTime);
	
	//Member variables
	WPI_TalonSRX * pMotor;
	WPI_TalonSRX * pMotor2;
	int targetcount;
	float motorSpeed;
	float initialVal;
	frc::Encoder * pEncoder;
	frc::Timer * maxTimer;
	bool itStopped;
	bool bDone;
	bool bEncoding;
	float desiredMaxTime;
	bool motorStop;

};

#endif
