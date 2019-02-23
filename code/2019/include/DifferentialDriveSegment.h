/*
 * DriveSegment.h
 *
 *  Created on: Feb 8, 2018
 *      Author: Anisha Kabir
 */

#ifndef DIFFERENTIALDRIVESEGMENT_H_
#define DIFFERENTIALDRIVESEGMENT_H_
#include "Settings.h"
#include "JankyAutoEntry.h"
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"

class DifferentialDriveSegment : public JankyAutoEntry, public frc::PIDOutput{
public:
	DifferentialDriveSegment(frc::ADXRS450_Gyro*gyro, frc::DifferentialDrive*drive, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*leftmotor, WPI_TalonSRX*rightmotor, int inchDistance, double speed, double p, double i, double d);
	//DriveSegment(RobotDrive*drive, Encoder*testEncoder, int inchDistance, double speed);
	virtual ~DifferentialDriveSegment();

protected:
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
	 * @brief One time action for preparing or starting
	 * sets the encoders
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	void Start();

	/**
	 * @brief disable the chassis
	 * inherited from JankyAutoEntry
	 * called by the JankyAutoEntry
	 */
	void End();

private:
	double kP;
	double kI;
	double kD;
	double maxTime;
	bool encoderReset;
	double leftDist;
	double rightDist;
	frc::Timer*encoderTimer;
	frc::Timer*armTimer;
	frc::ADXRS450_Gyro*_gyro;
	frc::PIDController*pid;
	frc::DifferentialDrive*chassis;
	SensorCollection*_leftEncoder;
	SensorCollection*_rightEncoder;
	WPI_TalonSRX*_leftmotor;
	WPI_TalonSRX*_rightmotor;
	frc::Encoder*_encoder;
	int distance;
	double _speed;
	void PIDWrite(double output);
};

#endif /* DRIVESEGMENT_H_ */
