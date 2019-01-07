/*
 * BallManipulation.h
 *
 *  Created on: Feb 6, 2016
 *      Author: Elaine
 */

#ifndef INCLUDE_BALLMANIPULATION_H_
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#define INCLUDE_BALLMANIPULATION_H_

class BallManipulation {
public:
	BallManipulation(int ballMotorChannel, int pivotMotorChannel, int pivotEncoderChannelA,
			int pivotEncoderChannelB, int topLSChannel, int middleLSChannel, int bottomLSChannel,
			int pistonModNumber, int pistonChannel);
	virtual ~BallManipulation(void);

	void BmanStart();
	void PivotUp();
	void DownForAuto();
	void PivotDown();
	void ResetPivotEncoder(void);
	void PivotBall(void);
	void PullIn(void);
	void PushOut(void);
	void ChangeSpeed(void);
	void DefenseUp(float buttonAxis);
	void DefenseDown(float buttonAxis);
	void StopPivotMotor(void);
	void StopBallMotor(void);
	void ShootGoal(void);
	int GetPivotEncoder(void);
	bool GetTopLS(void);
	bool GetMiddleLS(void);
	bool GetBottomLS(void);
	bool GetPiston(void);
	void SetPiston(bool on);
	void ToMiddleLS(void);
	void MiddleLSIdle(void);

	WPI_TalonSRX * pivotMotor;
	WPI_TalonSRX * ballMotor;
	frc::Encoder * pivotEncoder;
	frc::DigitalInput * topLS; // limit switch
	frc::DigitalInput * middleLS;
	frc::DigitalInput * bottomLS;
	frc::Solenoid * shootPiston;
	frc::Timer * timer;
	bool resetted;
	int middleLSState;
};



#endif /* INCLUDE_BALLMANIPULATION_H_ */
