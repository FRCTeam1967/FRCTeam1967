#include <iostream>
#include <memory>
#include <string>
#include <cstdlib>

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "WPILib.h"
#include "jankyXboxJoystick.h"
#include "CANTalon.h"
#include "JankyLights.h"

class Robot: public frc::IterativeRobot {
	JankyLights * light;
	jankyXboxJoystick * stick;


public:
	void RobotInit() {
		light = new JankyLights(0);
		stick = new jankyXboxJoystick(0);
	}


	void AutonomousInit() override {

	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {
		SmartDashboard::PutNumber("button RT", stick->GetRightThrottle());

		//TODO change if using game component stick
		if (stick->GetButtonY()) // turn lights on with button y
		{
			light->LightsOn();
		}
		else if (stick->GetRightThrottle() > .2) // make lights blink faster as right throttle pressed
		{
			light->Blink(1-stick->GetRightThrottle());
		}
		else // lights off otherwise
		{
			light->LightsOff();
		}

		// make lights blink if right axis in top right and bottom left quarters
		if ((stick->GetRightYAxis() > 0 && stick->GetRightXAxis() > 0)
				|| (stick->GetRightYAxis() < 0 && stick->GetRightXAxis() < 0))
		{
			light->LightsOn();
		}
		else // turn lights off otherwise
		{
			light->LightsOff();
		}

		double y = -stick->GetLeftYAxis();
		double x = stick->GetLeftXAxis();

		SmartDashboard::PutNumber("left y", y);
		SmartDashboard::PutNumber("left x", x);

		// a work in progress to make it on and off in each eighth of the left axis
/*		if (y > 0 && ( (y > x && x > 0) || (y < x && x < 0) ) )
		{
			light->LightsOn();
		}
		else if (y < 0 && ((x < 0 && abs(y) > x) || (abs(y) < x && x > 0)) )
		{
			light->LightsOn();
		}
		else
		{
			light->LightsOff();
		}*/
	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
