/*
 * BallManipulation.h
 *
 *  Created on: Feb 6, 2016
 *      Author: Elaine
 */

#ifndef INCLUDE_BALLMANIPULATION_H_
#include "WPILib.h"
#define INCLUDE_BALLMANIPULATION_H_

class BallManipulation {
public:
	BallManipulation(int ballMotorChannel, int pivotMotorChannel, int pivotEncoderChannelA,
			int pivotEncoderChannelB, int topLSChannel, int bottomLSChannel);
	virtual ~BallManipulation(void);

	void PivotUp();
	void DownForAuto();
	void PivotDown();
	void ResetPivotEncoder(void);
	void PivotBall(void);
	void PullIn(void);
	void PushOut(void);
	void ChangeSpeed(void);
	void DefenseUp(void);
	void DefenseDown(void);
	void StopPivotMotor(void);
	void StopBallMotor(void);
	int GetPivotEncoder(void);
	bool GetTopLS(void);
	bool GetBottomLS(void);

	CANTalon * pivotMotor;
	CANTalon * ballMotor;
	Encoder * pivotEncoder;
	DigitalInput * topLS; // limit switch
	DigitalInput * bottomLS;
	bool resetted;
};



#endif /* INCLUDE_BALLMANIPULATION_H_ */
