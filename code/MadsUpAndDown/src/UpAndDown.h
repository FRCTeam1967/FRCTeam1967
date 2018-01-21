#include "WPILib.h"
#include "ctre/Phoenix.h"

#ifndef UPANDDOWN_H_
#define UPANDDOWN_H_

class UpAndDown {
public:
	double desiredHeight = 0.0;
	double amountToMove;
	bool reachedMaxHeight = false;
	bool reachedMinHeight = true;
	bool isMechanismRunning = false;

	UpAndDown(int lMotorChannel, int rMotorChannel, int bottomLimSwitchChannel, int topLimSwitchChannel, int gameMotorEncoderChannel1, int gameMotorEncoderChannel2);
	virtual ~UpAndDown();

	void RLMotorForward();
	void RLMotorReverse();
	void RLMotorStop();

	bool GetBottomLimSwitch();
	bool GetTopLimSwitch();

	void SwitchHeight();
	void ScaleLowHeight();
	void ScaleMedHeight();
	void ScaleHight();
	void RegularHeight();

	double GetEncoderDistance();
	void ResetEncoder();
	double GetEncoderDistancePerPulse();

	void EmergencyStopMechanism();
	void MoveToNewHeight();
private:

	WPI_TalonSRX*lMotor;
	WPI_TalonSRX*rMotor;

	DigitalInput*bottomLimSwitch;
	DigitalInput*topLimSwitch;

	Encoder*gameMotorEncoder;
};



#endif /* UPANDDOWN_H_ */
