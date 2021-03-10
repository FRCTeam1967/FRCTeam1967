// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "SelectorSample.h"

SelectorSample* pathSelector = NULL;

void Robot::RobotInit() {
  pathSelector = new SelectorSample();
  pathSelector->PrintValues();
}

void Robot::RobotPeriodic() {

}

void Robot::AutonomousInit() {
  SelectorSample::Options optionReceived = pathSelector->GetSelection();
  std::string pathName = "doNothing.wpilib.json";
  if (optionReceived == SelectorSample::Options::Path1){
    pathName = "path1.wpilib.json";
  }
  else if (optionReceived == SelectorSample::Options::Path2){
    pathName = "path2.wpilib.json";
  }
  else if (optionReceived == SelectorSample::Options::Path3){
    pathName = "path3.wpilib.json";
  }
  //the rest of the trajectory stuff would be the same as normal
}

void Robot::AutonomousPeriodic() {
}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {}

void Robot::DisabledInit() {}

void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
