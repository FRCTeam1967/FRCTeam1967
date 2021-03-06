///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// Shooter for Rebound Rumble 2012

#ifndef _JANKYSHOOTER_H
#define _JANKYSHOOTER_H

#include "jankyRobot.h"
#define SHOOTER_P 0.0
#define SHOOTER_I 0.0
#define SHOOTER_D 0.0

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
	frc::Victor ShooterMotor;
	frc::Encoder ShooterEncoder;
	float MotorSpeed;
	frc::PIDController PID;
	frc::Timer EncoderTimer;
	int PreviousCount;
	double PreviousTime;
	int PreviousRPM;
	frc::Timer ShooterTimer;
	double RPMerrorOld;
};

class JankyShooter2: public frc::Victor,frc::PIDSource {
public:
	JankyShooter2(int JagPort, int EncoderAPort, int EncoderBPort);
	virtual ~JankyShooter2(void);
	void setTargetRPM(int desiredrpm);
	int GetCurrentRPM(void); 
	int GiveDesiredRPM(){
		return TargetRPMx4;
	};
	virtual double PIDGet(){
		//static int value = 0;
		//SmartDashboard::GetInstance()->PutInt("PIDGet",value++);
		frc::SmartDashboard::PutNumber("PID RPM",GetCurrentRPM());
		frc::SmartDashboard::PutNumber("PID Error",PID.GetError());
		return (double)GetCurrentRPM();
	};
	virtual void PIDWrite(double Err){
		static int value2 = 0;
		frc::SmartDashboard::PutNumber("PIDWrite",value2++);

		MotorSpeed=MotorSpeed + Err;
		
		if (MotorSpeed<0.0)
			MotorSpeed=0.0;
		if (MotorSpeed>1.0)
			MotorSpeed=1.0;
		frc::SmartDashboard::PutNumber("Speed of Motor",MotorSpeed);
		//SmartDashboard::PutNumber("VictorGet",Victor::Get());
		//Victor::Set(MotorSpeed);
		
	};
	virtual void Set(double Motor);
	void InteractivePIDSetup(void);
		
	//Member variables
	
	frc::Encoder ShooterEncoder;
	int CurrentRPMx4;
	int TargetRPMx4;
	float MotorSpeed;
	frc::PIDController PID;
	frc::Timer EncoderTimer;
	int PreviousCount;
	double PreviousTime;
	int PreviousRPM;
	
};
#endif
