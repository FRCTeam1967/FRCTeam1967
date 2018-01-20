#include "WPILib.h"
#include "ctre/Phoenix.h"

#ifndef UPANDDOWN_H_
#define UPANDDOWN_H_

class UpAndDown {
public:
	double newDistance;
	double amountToMove;

	UpAndDown(int lMotorChannel, int rMotorChannel, int limSwitchOneChannel, int limSwitchTwoChannel, int gameMotorEncoderChannel1, int gameMotorEncoderChannel2);
	virtual ~UpAndDown();

	void RLMotorForward();
	void RLMotorReverse();
	void RLMotorStop();

	bool GetLimSwitchOne();
	bool GetLimSwitchTwo();

	void SwitchHeight();
	void ScaleLowHeight();
	void ScaleMedHeight();
	void ScaleHight();
	void RegularHeight();

	double GetEncoderDistance();
	void ResetEncoder();

	void EmergencyStopMechanism();
	void MoveToNewHeight();
private:

	WPI_TalonSRX*lMotor;
	WPI_TalonSRX*rMotor;

	DigitalInput*limSwitchOne;
	DigitalInput*limSwitchTwo;

	Encoder*gameMotorEncoder;
};



#endif /* UPANDDOWN_H_ */
