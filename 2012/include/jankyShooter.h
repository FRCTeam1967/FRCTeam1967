///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// Shooter for Rebound Rumble 2012

#ifndef _JANKYSHOOTER_H
#define _JANKYSHOOTER_H

#include "jankyRobot.h"

#define RPM_DEADBANDx4 100
#define MINSHOOT_WAIT 0.05



class JankyShooter {
public:
	JankyShooter(int JagPort, int EncoderAPort, int EncoderBPort);
	virtual ~JankyShooter(void);
	void setTargetRPM(int desiredrpm);
	void DoCalculations(void);
	int GetCurrentRPM(void);
	int GiveDesiredRPM(){
		return TargetRPMx4;
	};
	
	//Member variables
	int CurrentRPMx4;
	int TargetRPMx4;
	Victor ShooterMotor;
	Encoder ShooterEncoder;
	float MotorSpeed;
	PIDController PID;
	Timer EncoderTimer;
	int PreviousCount;
	double PreviousTime;
	int PreviousRPM;
	Timer ShooterTimer;
	
};

#endif
