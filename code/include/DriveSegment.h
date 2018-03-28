/*
 * DriveSegment.h

 *
 *  Created on: Feb 8, 2018
 *      Author: Anisha Kabir
 */

#ifndef DRIVESEGMENT_H_
#define DRIVESEGMENT_H_
#include "JankyAutoEntry.h"
#include "ctre/Phoenix.h"

class DriveSegment : public JankyAutoEntry, public frc::PIDOutput{
public:
	DriveSegment(frc::ADXRS450_Gyro*gyro, RobotDrive*drive, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*leftmotor, WPI_TalonSRX*rightmotor, int inchDistance, double speed, double p, double i, double d);
	//DriveSegment(RobotDrive*drive, Encoder*testEncoder, int inchDistance, double speed);
	virtual ~DriveSegment();

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
	Timer*encoderTimer;
	frc::ADXRS450_Gyro*_gyro;
	PIDController*pid;
	RobotDrive*chassis;
	SensorCollection*_leftEncoder;
	SensorCollection*_rightEncoder;
	WPI_TalonSRX*_leftmotor;
	WPI_TalonSRX*_rightmotor;
	Encoder*_encoder;
	int distance;
	double _speed;
	void PIDWrite(double output);
};

#endif /* DRIVESEGMENT_H_ */
