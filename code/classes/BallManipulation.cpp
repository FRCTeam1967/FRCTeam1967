/*
 * BallManipulation.cpp
 *
 *  Created on: Feb 6, 2016
 *      Author: Sana
 */
#include "WPILib.h"
#include "BallManipulation.h"
#include "CANTalon.h"

#define PIVOT_BALL_SPEED 0.2
#define PIVOT_DEFENSE_SPEED 1.0
#define BALL_MOTOR_SPEED 1.0
#define CHANGED_BALL_MOTOR_SPEED 0.5
#define STOP_PLACE 20

BallManipulation::BallManipulation(int ballMotorChannel, int pivotMotorChannel, int pivotEncoderChannelA,
		int pivotEncoderChannelB, int topLSChannel,int middleLSChannel, int bottomLSChannel, int pistonModNumber, int pistonChannel) {
	ballMotor = new CANTalon(ballMotorChannel); // is actually a victor
	pivotMotor = new CANTalon(pivotMotorChannel);
	pivotEncoder = new Encoder(pivotEncoderChannelA, pivotEncoderChannelB);
	topLS = new DigitalInput(topLSChannel);
	middleLS = new DigitalInput(middleLSChannel);
	bottomLS = new DigitalInput(bottomLSChannel);
	shootPiston = new Solenoid(pistonModNumber, pistonChannel);
	timer = new Timer();
	int middleLSState = 0;
}

BallManipulation::~BallManipulation(void) {
	delete ballMotor;
	delete pivotMotor;
	delete pivotEncoder;
	delete topLS;
	delete middleLS;
	delete bottomLS;
	delete shootPiston;
}
void BallManipulation::SetPiston(bool on) {
	shootPiston->Set(on);
	SmartDashboard::PutBoolean("Bman piston", on);

}

void BallManipulation::BmanStart() {
	SetPiston(false);
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

void BallManipulation::DefenseUp(float buttonAxis) {
	// pivot pivotMotor forward if top limit switch not pressed
	if (GetTopLS() == true) {
		pivotMotor->Set(-buttonAxis);
	}
	else {
		pivotMotor->Set(0.0);
	}
}

void BallManipulation::DefenseDown(float buttonAxis) {
	// pivot pivotMotor backwards if bottom limit switch not pressed
	if (GetBottomLS() == true) {
		pivotMotor->Set(-buttonAxis);
	}
	else {
		pivotMotor->Set(0.0);
	}
}

/*void BallManipulation::DefenseUp(float buttonAxis) {
	// pivot pivotMotor forward if top limit switch not pressed
	if (GetTopLS() == true) {
		pivotMotor->Set(PIVOT_DEFENSE_SPEED);
	}
	else {
		pivotMotor->Set(0.0);
	}
}

void BallManipulation::DefenseDown(float buttonAxis) {
	// pivot pivotMotor backwards if bottom limit switch not pressed
	if (GetBottomLS() == true) {
		pivotMotor->Set(-PIVOT_DEFENSE_SPEED);
	}
	else {
		pivotMotor->Set(0.0);
	}
}*/

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

bool BallManipulation::GetMiddleLS(void) {
	return middleLS->Get();
}

bool BallManipulation::GetBottomLS(void) {
	// return bottom limit switch value
	return bottomLS->Get();
}

int BallManipulation::GetPivotEncoder(void) {
	// return pivot encoder value
	return pivotEncoder->Get();
}

bool BallManipulation::GetPiston(void) {
	return shootPiston->Get();
}



void BallManipulation::StopPivotMotor(void) {
	// stop the pivot motor
	pivotMotor->Set(0.0);
}

void BallManipulation::StopBallMotor(void) {
	// stop the ball motor
	ballMotor->Set(0.0);
}

void BallManipulation::ShootGoal(void) {
	// shoot a low goal

	PushOut();

	if (shootPiston->Get() == false) {
		shootPiston->Set(true);
		DefenseUp(-0.9);
	}
}

void BallManipulation::ToMiddleLS(void) {
	// goes to middle limit switch from top or bottom
	if(GetMiddleLS() == true) {
		if (middleLSState == 0) {
			middleLSState = 1;
			DefenseUp(-0.2);
		}
		else if (middleLSState == 1) {
			DefenseUp(-0.2);

		}
		else if(middleLSState == 2) {
			if(timer->HasPeriodPassed(0.4)) {
				timer->Stop();
				middleLSState = 3;
			}
			DefenseDown(0.1);
		}

	}
	else {
		if (middleLSState == 1 || middleLSState == 0) {
			middleLSState = 2;
			timer->Reset();
			timer->Start();
			DefenseDown(0.1);
		}
		else if(middleLSState == 2) {
			if(timer->HasPeriodPassed(0.4)) {
				timer->Stop();
				middleLSState = 3;
			}
			else {
				DefenseDown(0.1);
			}
		}


	}
}

void BallManipulation::MiddleLSIdle(void) {
	middleLSState = 0;
}


