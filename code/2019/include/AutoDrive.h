/*
 * AutoDrive.h
 *
 *  Created on: Feb 24, 2018
 *      Author: AnishaKabir
 */
#include "Settings.h"
#include "frc/WPILib.h"
#include "JankyAutoEntry.h"
#include "ctre/Phoenix.h"

#ifndef AUTODRIVE_H_
#define AUTODRIVE_H_

class AutoDrive : public JankyAutoEntry, public frc::PIDOutput, public frc::PIDSource {
public:
	#ifdef JANKY_BOT_2019
	AutoDrive(frc::DifferentialDrive*drive, double speed, double p, double i, double d, WPI_TalonSRX*flmotor, WPI_TalonSRX*frmotor, WPI_VictorSPX*rlmotor, WPI_VictorSPX*rrmotor);
	#else
	AutoDrive(frc::DifferentialDrive*drive, double speed, double p, double i, double d, WPI_TalonSRX*flmotor, WPI_TalonSRX*frmotor, WPI_TalonSRX*rlmotor, WPI_TalonSRX*rrmotor);
	#endif
	float horizontalOffset;
	float distance;
	int badDataCounter;
	int noDataCounter;
	bool abortBadData;
	double _speed;
	frc::DifferentialDrive*chassis;
	frc::PIDController*pid;
	frc::Timer*visionTimer;
	WPI_TalonSRX* _flmotor;
	WPI_TalonSRX* _frmotor;
	#ifdef JANKY_BOT_2019
	WPI_VictorSPX* _rlmotor;
	WPI_VictorSPX* _rrmotor;
	#else
	WPI_TalonSRX* _rlmotor;
	WPI_TalonSRX* _rrmotor;
	#endif
	double kP;
	double kI;
	double kD;
	virtual ~AutoDrive();
	void Start();
	void RunAction();
	bool JobDone();
	void End();
	double PIDGet();
	void PIDWrite(double output);
};

#endif /* AUTODRIVE_H_ */
