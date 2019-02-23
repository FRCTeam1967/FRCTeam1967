/*
 * TurnSegment.h
 *
 *  Created on: Feb 10, 2018
 *      Author: Maxine, Bob W
 */

#ifndef SRC_DIFFERENTIALTURNSEGMENT_H_
#define SRC_DIFFERENTIALTURNSEGMENT_H_
#include "Settings.h"
#include "frc/WPILib.h"
#include "JankyAutoEntry.h"

class DifferentialTurnSegment : public JankyAutoEntry, public frc::PIDOutput{
public:

	DifferentialTurnSegment(frc::ADXRS450_Gyro*gyro, frc::DifferentialDrive*chassis, double degreeOfTurn, double speed, double p, double i, double d);
	virtual ~DifferentialTurnSegment();


protected:
	/**
	 * @brief One time action for preparing or starting
	 * Resets the Gyro and sets the PID
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	void Start();

	/**
	 * @brief Returns whether the turn is complete or not
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	bool JobDone();

	/**
	 * @brief is called repeatedly by jankyAutoEntry by IsComplete()
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	void RunAction();



	/**
	 * @brief disables the PID
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	void End();

private:
	frc::Timer*turnTimer;
	frc::PIDController*pid;
	frc::ADXRS450_Gyro*_gyro;
	frc::DifferentialDrive*drive;
	int c;//c is counter
	double targetAngle;
	double _speed;
	double kP;
	double kI;
	double kD;
	/**
	 * @brief makes the chassis a PIDOutput
	 */
	void PIDWrite(double output);
};

#endif /* SRC_TURNSEGMENT_H_ */
