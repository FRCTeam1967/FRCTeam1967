/*
 * VisionSegment.h
 *
 *  Created on: Feb 24, 2018
 *      Author: AnishaKabir
 */
#include "frc/WPILib.h"
#include "JankyAutoEntry.h"

#ifndef VISIONSEGMENT_H_
#define VISIONSEGMENT_H_

class VisionSegment : public JankyAutoEntry, public frc::PIDOutput, public frc::PIDSource {
public:
	VisionSegment(frc::RobotDrive*drive, double speed, double p, double i, double d);
	float horizontalOffset;
	float distance;
	int badDataCounter;
	bool abortBadData;
	double _speed;
	frc::RobotDrive*chassis;
	frc::PIDController*pid;
	frc::Timer*visionTimer;
	double kP;
	double kI;
	double kD;
	virtual ~VisionSegment();
	void Start();
	void RunAction();
	bool JobDone();
	void End();
	double PIDGet();
	void PIDWrite(double output);
};

#endif /* VISIONSEGMENT_H_ */
