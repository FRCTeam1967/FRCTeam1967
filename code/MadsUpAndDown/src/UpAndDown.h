#include "WPILib.h"
#include "jankyTask.h"
#include "ctre/Phoenix.h"

#ifndef UPANDDOWN_H_
#define UPANDDOWN_H_

class UpAndDown : public JankyTask {
public:
	UpAndDown(int lMotorChannel, int rMotorChannel, int bottomLimSwitchChannel, int topLimSwitchChannel, int gameMotorEncoderChannel1, int gameMotorEncoderChannel2);
	virtual ~UpAndDown();

	virtual void Run();

	void SwitchHeight();
	void ScaleLowHeight();
	void ScaleMedHeight();
	void ScaleHight();
	void RegularHeight();

	void ResetEncoder();

	void SmartDashboardComments();

protected:
	void RLMotorForward();
	void RLMotorReverse();
	void RLMotorStop();

	bool GetBottomLimSwitch();
	bool GetTopLimSwitch();

	double GetEncoderDistance();
	double GetEncoderDistancePerPulse();

private:
	void EmergencyStopMechanism();

	void PutMechanismDown();

	bool isMechanismRunning = false;
	double desiredHeight = 0.0;
	double amountToMove = 0.0;

	bool reachedMaxHeight = false;
	bool reachedMinHeight = true;
	bool needsToPutDownMechanism = true;
	bool bottomLimSwitchHasNotBeenPressed = true;
	bool topLimSwitchHasNotBeenPressed = true;

	WPI_TalonSRX*lMotor;
	WPI_TalonSRX*rMotor;

	DigitalInput*bottomLimSwitch;
	DigitalInput*topLimSwitch;

	Encoder*gameMotorEncoder;
};



#endif /* UPANDDOWN_H_ */
