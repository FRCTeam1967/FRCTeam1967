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
	AutoDrive(frc::DifferentialDrive*drive, double speed, double p, double i, double d);

	float horizontalOffset;
	float distance;
	int badDataCounter;
	int noDataCounter;
	bool abortBadData;
	double _speed;
	frc::DifferentialDrive*chassis;
	frc::PIDController*pid;
	frc::Timer*visionTimer;
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
