#include "WPILib.h"
#include "BallManipulation.h"
#include "jankyXboxJoystick.h"

#define BALL_MOTOR_CHANNEL 4
#define PIVOT_MOTOR_CHANNEL 1
#define PIVOT_ENCODER_CHANNEL_A 0
#define PIVOT_ENCODER_CHANNEL_B 1
#define JOYSTICK_PORT 0
#define LIMIT_SWITCH_TOP 4
#define LIMIT_SWITCH_BOTTOM 5

#define MOVE_AXIS 0.9

class Robot: public IterativeRobot
{
	BallManipulation * bman;
	jankyXboxJoystick * joystick;

public:
	Robot() {
		bman = NULL;
		joystick = NULL;
	}
	~Robot() {
		delete bman;
		delete joystick;
	}
private:

	void RobotInit()
	{
		bman = new BallManipulation(BALL_MOTOR_CHANNEL, PIVOT_MOTOR_CHANNEL, PIVOT_ENCODER_CHANNEL_A,
				PIVOT_ENCODER_CHANNEL_B, LIMIT_SWITCH_TOP, LIMIT_SWITCH_BOTTOM);
		joystick = new jankyXboxJoystick(JOYSTICK_PORT);
	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{
		SmartDashboard::PutNumber("Pivot Encoder Value: ", bman->GetPivotEncoder());
		SmartDashboard::PutNumber("Right X Axis: ", joystick->GetRightXAxis());
		SmartDashboard::PutNumber("Right Y Axis: ", joystick->GetRightYAxis());
		SmartDashboard::PutNumber("Left X Axis: ", joystick->GetLeftXAxis());
		SmartDashboard::PutNumber("Left Y Axis: ", joystick->GetLeftYAxis());
		SmartDashboard::PutNumber("Right Twist: ", joystick->GetRightTwist());
		SmartDashboard::PutBoolean("Top: ", bman->GetTopLS());
		SmartDashboard::PutBoolean("Bottom: ", bman->GetBottomLS());

		if (joystick->GetButtonRB() == true) {
			bman->ChangeSpeed();
		}
		if (joystick->GetButtonB() == true) {
			bman->PivotBall();
		}

		float leftYValue = joystick->GetLeftYAxis();
		float rightYValue = joystick->GetRightYAxis();

		// pull in/push out ball using left joystick on xbox
		if (leftYValue < -MOVE_AXIS) {
			bman->PushOut();
		}
		else if (leftYValue > MOVE_AXIS) {
			//printf("%d\n", bman->GetPivotEncoder());
			bman->PullIn();
		}
		else {
			bman->StopBallMotor();
		}


		// move defenses up/down using right joystick on xbox
		if (rightYValue < -MOVE_AXIS) {
			bman->DefenseUp();
		}
		else if (rightYValue > MOVE_AXIS) {
			bman->DefenseDown();
		}
		else {
			bman->StopPivotMotor();
		}
	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Robot)
