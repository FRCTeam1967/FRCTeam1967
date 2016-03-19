/*
 * BallManipulation.cpp
 *
 *  Created on: Feb 6, 2016
 *      Author: Elaine
 */
#include "WPILib.h"
#include "BallManipulation.h"

#define PIVOT_BALL_SPEED 0.2
#define PIVOT_DEFENSE_SPEED 1.0
#define BALL_MOTOR_SPEED 1.0
#define CHANGED_BALL_MOTOR_SPEED 0.5
#define STOP_PLACE 20

BallManipulation::BallManipulation(int ballMotorChannel, int pivotMotorChannel, int pivotEncoderChannelA,
		int pivotEncoderChannelB, int topLSChannel, int bottomLSChannel) {
	ballMotor = new CANTalon(ballMotorChannel); // is actually a victor
	pivotMotor = new CANTalon(pivotMotorChannel);
	pivotEncoder = new Encoder(pivotEncoderChannelA, pivotEncoderChannelB);
	topLS = new DigitalInput(topLSChannel);
	bottomLS = new DigitalInput(bottomLSChannel);
}

BallManipulation::~BallManipulation(void) {
	delete ballMotor;
	delete pivotMotor;
	delete pivotEncoder;
	delete topLS;
	delete bottomLS;
}

void BallManipulation::DownForAuto(){
	pivotMotor->Set(-.3);
}
void BallManipulation::ChangeSpeed(void) {
	// changes speed of intake mechanism
	float currentSpeed = ballMotor->Get();
	if (currentSpeed == BALL_MOTOR_SPEED) {
		ballMotor->Set(CHANGED_BALL_MOTOR_SPEED);
	} else if (currentSpeed == (-BALL_MOTOR_SPEED)) {
		ballMotor->Set(-CHANGED_BALL_MOTOR_SPEED);
	}
}

void BallManipulation::PivotBall(void) {
	// pivots intake mechanism to a point, uses encoder
	bool resetted = false;
	bool donePivot = false;

	while(donePivot == false) {
		printf("\n encoder: %d", GetPivotEncoder());
		if (resetted == false && GetBottomLS() == true) {
			pivotMotor->Set(-PIVOT_BALL_SPEED);
		}
		else if (resetted == false && GetBottomLS() == false) {
			pivotEncoder->Reset();
			resetted = true;
			pivotMotor->Set(PIVOT_BALL_SPEED);
		}
		else if (GetPivotEncoder() >= STOP_PLACE && resetted == true) {
			pivotMotor->Set(0.0);
			donePivot = true;
		}
	}
}

void BallManipulation::PullIn(void) {
	// pull in ball
	ballMotor->Set(-BALL_MOTOR_SPEED);
}

void BallManipulation::PushOut(void) {
	// push out ball
	ballMotor->Set(BALL_MOTOR_SPEED);
}

void BallManipulation::PivotUp(void) {
	// pivot pivotMotor forward if top limit switch not pressed
	if (GetTopLS() == true) {
		pivotMotor->Set(CHANGED_BALL_MOTOR_SPEED);
	}
	else {
		pivotMotor->Set(0.0);
	}
}

void BallManipulation::PivotDown(void) {
	// pivot pivotMotor backwards if bottom limit switch not pressed
	if (GetBottomLS() == true) {
		pivotMotor->Set(-CHANGED_BALL_MOTOR_SPEED);
	}
	else {
		pivotMotor->Set(0.0);
	}
}
void BallManipulation::DefenseUp(void) {
	// pivot pivotMotor forward if top limit switch not pressed
	if (GetTopLS() == true) {
		pivotMotor->Set(PIVOT_DEFENSE_SPEED);
	}
	else {
		pivotMotor->Set(0.0);
	}
}

void BallManipulation::DefenseDown(void) {
	// pivot pivotMotor backwards if bottom limit switch not pressed
	if (GetBottomLS() == true) {
		pivotMotor->Set(-PIVOT_DEFENSE_SPEED);
	}
	else {
		pivotMotor->Set(0.0);
	}
}

void BallManipulation::ResetPivotEncoder(void) {
	// resets pivot encoder if bottom limit switch pressed
	if (GetBottomLS() == false) {
		pivotEncoder->Reset();
	}
}

bool BallManipulation::GetTopLS(void) {
	// return top limit switch value
	return topLS->Get();
}

bool BallManipulation::GetBottomLS(void) {
	// return bottom limit switch value
	return bottomLS->Get();
}

int BallManipulation::GetPivotEncoder(void) {
	// return pivot encoder value
	return pivotEncoder->Get();
}

void BallManipulation::StopPivotMotor(void) {
	// stop the pivot motor
	pivotMotor->Set(0.0);
}

void BallManipulation::StopBallMotor(void) {
	// stop the ball motor
	ballMotor->Set(0.0);
}


