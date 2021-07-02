// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "jankyActuator.h"
#include "jankyXboxJoystick.h"

using namespace std;

JankyActuator * actuatorSample = NULL;
jankyXboxJoystick * _joy = NULL; 

void Robot::RobotInit() {
  actuatorSample = new JankyActuator(0, 1); //TODO: verify channel numbers (this assumes 0 and 1 are the channels numbers) 
  actuatorSample->SetFullCycleTime(6.0);
  actuatorSample->SetActuationTime(2.0);

  _joy = new jankyXboxJoystick(0); //TODO: verify joystick channel (currently 0)
}

void Robot::RobotPeriodic() {}


void Robot::AutonomousInit() {

}

void Robot::AutonomousPeriodic() {
}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {

  static bool lBpressed = false;
  static bool rBpressed = false;
  static bool leftTwistPressed = false;
  static bool rightTwistPressed = false;
  static bool buttonAPressed = false;
  static bool buttonBPressed = false;
  static bool buttonXPressed = false;
  static bool buttonYPressed = false;

  if ((_joy->GetButtonLB()) && (lBpressed == false)){
    printf("Go\n");
    actuatorSample->Go();
    lBpressed = true;
  }
  else if ((!_joy->GetButtonLB()) && (lBpressed == true)){
    lBpressed = false;
  }

  else if ((_joy->GetButtonRB())&&(rBpressed == false)){
    printf("Terminate\n");
    actuatorSample->Terminate();
    rBpressed = true;
  }
  else if ((!_joy->GetButtonRB())&& (rBpressed == true)){
    rBpressed = false;
  }

  else if ((_joy->GetLeftTwist()) && (leftTwistPressed == false)){
    printf("Set cycle time to 4 and actuation time to 1\n");
    actuatorSample->SetFullCycleTime(4.0);
    actuatorSample->SetActuationTime(1.0);
    leftTwistPressed = true;
  }
  else if ((!_joy->GetLeftTwist()) && (leftTwistPressed == true)){
    leftTwistPressed = false;
  }

  else if ((_joy->GetRightTwist()) && (rightTwistPressed == false)){
    printf("Set cycle time to 6 and actuation time to 2\n");
    actuatorSample->SetFullCycleTime(6.0);
    actuatorSample->SetActuationTime(2.0);
    rightTwistPressed = true;
  }
  else if ((!_joy->GetRightTwist()) && (rightTwistPressed == true)){
    rightTwistPressed = false;
  }

  else if ((_joy->GetButtonA()) && (buttonAPressed == false)){
    printf("Start\n");
    actuatorSample->Start();
    buttonAPressed = true;
  }
  else if ((!_joy->GetButtonA()) && (buttonAPressed == true)){
    buttonAPressed = false;
  }

  else if ((_joy->GetButtonB()) && (buttonBPressed == false)){
    printf("Pause\n");
    actuatorSample->Pause();
    buttonBPressed = true;
  }
  else if ((!_joy->GetButtonB()) && (buttonBPressed == true)){
    buttonBPressed = false;
  }

  else if ((_joy->GetButtonX()) && (buttonXPressed == false)){
    printf("OverrideEnable\n");
    actuatorSample->OverrideEnable();
    buttonXPressed = true;
  }
  else if ((!_joy->GetButtonX()) && (buttonXPressed == true)){
    buttonXPressed = false;
  }
  
  else if ((_joy->GetButtonY()) && (buttonYPressed == false)){
    printf("OverrideEnable\n");
    actuatorSample->OverrideDisable();
    buttonYPressed = true;
  }
  else if ((!_joy->GetButtonY()) && (buttonYPressed == true)){
    buttonYPressed = false;
  }

}

void Robot::DisabledInit() {}

void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
