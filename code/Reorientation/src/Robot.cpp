#include "WPILib.h"
#include "jankyXboxJoystick.h"
#include "NatSolenoidTestJig.h"
#include "Reorientation.h"

#define JOYSTICK 0
#define SOLENOID 0

class Robot: public IterativeRobot
{
	jankyXboxJoystick * stick;
	NatSolenoidTestJig * sold;

public:
	Robot(){
		stick = NULL;
		sold = NULL;
	}
	~Robot() {
		delete stick;
		delete sold;
	}
private:

	void RobotInit()
	{
		stick = new jankyXboxJoystick(JOYSTICK);
		sold = new NatSolenoidTestJig(SOLENOID);

	}

	void TeleopPeriodic()
	{
		/*SmartDashboard::PutNumber("kXAxis", stick->GetAxis(Joystick::kXAxis));
		SmartDashboard::PutNumber("kYAxis", stick->GetAxis(Joystick::kYAxis));
		SmartDashboard::PutNumber("kZAxis", stick->GetAxis(Joystick::kZAxis));
		SmartDashboard::PutNumber("kXTwistAxis", stick->GetAxis(Joystick::kTwistAxis));
		SmartDashboard::PutNumber("kXThrottle", stick->GetAxis(Joystick::kThrottleAxis));
		SmartDashboard::PutNumber("kNumAxisTypes", stick->GetAxis(Joystick::kNumAxisTypes));
		SmartDashboard::PutNumber("GetTwist", stick->GetTwist());*/
		// piston starts in
		sold->OverrideDisable();

		// piston extends when button pressed, GetTwist is left trigger
        if (stick->GetButtonLB() == true || stick->GetTwist() > 0.5) {
        	sold->OverrideEnable();
        	printf("Left bumper or left trigger pressed\n");
        }

        printf("after getbutton\n");
	}
};

START_ROBOT_CLASS(Robot);
