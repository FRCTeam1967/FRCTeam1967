#include <iostream>
#include <string>

#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "WPILib.h"

class Robot : public frc::IterativeRobot {
	cs::UsbCamera*driveTeamCamera;
public:
	void RobotInit() {
		driveTeamCamera = new cs::UsbCamera;
		CameraServer::GetInstance()->StartAutomaticCapture(0);
		driveTeamCamera->SetFPS(15);
		CameraServer::GetInstance()->GetVideo();
		CameraServer::GetInstance()->PutVideo("DriveTeamCam", 640, 480);
	}

	void AutonomousInit() override {

	}

	void AutonomousPeriodic() {

	}

	void TeleopInit() {

	}

	void TeleopPeriodic() {

	}

	void TestPeriodic() {

	}

private:

};

START_ROBOT_CLASS(Robot)
