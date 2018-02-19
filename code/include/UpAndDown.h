#include "WPILib.h"
#include "jankyTask.h"
#include "ctre/Phoenix.h"

#ifndef UPANDDOWN_H_
#define UPANDDOWN_H_

class UpAndDown : public JankyTask {
public:
	UpAndDown(int lMotorChannel, int rMotorChannel);
	virtual ~UpAndDown();

	virtual void Run();

	void SwitchHeight();
	void ScaleLowHeight();
	void ScaleMedHeight();
	void ScaleHight();
	void RegularHeight();

	void ResetEncoder();

	void SmartDashboardComments();

	bool GetIfMechIsRunning();

	void StartUpInit();

	void RLMotorForward();
	void RLMotorReverse();
	void RLMotorStop();

protected:

	int GetBottomLimSwitch();
	int GetTopLimSwitch();

	double GetGameMotorEncoderDistance();

	//  Unused
	//	double GetEncoderDistance();
	//	double GetEncoderDistancePerPulse();

private:
	void EmergencyStopMechanism();

	void PutMechanismDown();

	bool isMechanismRunning;
	double desiredHeight;
	double amountToMove;

	bool reachedMaxHeight;
	bool reachedMinHeight;
	bool needsToPutDownMechanism;
	bool bottomLimSwitchHasNotBeenPressed;
	bool topLimSwitchHasNotBeenPressed;

	double lmotorEncoderCount;
	double lmotorEncoderDistance;

	WPI_TalonSRX*lMotor;
	WPI_TalonSRX*rMotor;

	//  Unused
	//	WPI_TalonSRX*bottomLimSwitch;
	//	WPI_TalonSRX*topLimSwitch;
	//	Encoder*gameMotorEncoder;
};

#endif /* UPANDDOWN_H_ */
