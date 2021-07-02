// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "Robot.h"

#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "jankyActuator.h"
#include "jankyXboxJoystick.h"

using namespace std;

JankyActuator * singleActuatorSample = NULL;
JankyActuator * doubleActuatorSample = NULL;
jankyXboxJoystick * _1PistonJoy = NULL; 
jankyXboxJoystick * _2PistonJoy = NULL; 

void Robot::RobotInit() {
  singleActuatorSample = new JankyActuator(2); //TODO: verify channel number (current 2)
  singleActuatorSample->SetFullCycleTime(6.0);
  singleActuatorSample->SetActuationTime(2.0);

  doubleActuatorSample = new JankyActuator(0, 1); //TODO: verify channel numbers (this assumes 0 and 1 are the channels numbers) 
  doubleActuatorSample->SetFullCycleTime(6.0);
  doubleActuatorSample->SetActuationTime(2.0);

  _1PistonJoy = new jankyXboxJoystick(1);//TODO: verify joystick channel (currently 1)
  _2PistonJoy = new jankyXboxJoystick(0); //TODO: verify joystick channel (currently 0)
}

void Robot::RobotPeriodic() {}


void Robot::AutonomousInit() {

}

void Robot::AutonomousPeriodic() {
}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {

  //joystick for 1 piston
  static bool joy1LBpressed = false;
  static bool joy1RBpressed = false;
  static bool joy1LeftTwistPressed = false;
  static bool joy1RightTwistPressed = false;
  static bool joy1ButtonAPressed = false;
  static bool joy1ButtonBPressed = false;
  static bool joy1ButtonXPressed = false;
  static bool joy1ButtonYPressed = false;

  if ((_1PistonJoy->GetButtonLB()) && (joy1LBpressed == false)){
    printf("Go\n");
    singleActuatorSample->Go();
    joy1LBpressed = true;
  }
  else if ((!_1PistonJoy->GetButtonLB()) && (joy1LBpressed == true)){
    joy1LBpressed = false;
  }

  else if ((_1PistonJoy->GetButtonRB())&&(joy1RBpressed == false)){
    printf("Terminate\n");
    singleActuatorSample->Terminate();
    joy1RBpressed = true;
  }
  else if ((!_1PistonJoy->GetButtonRB())&& (joy1RBpressed == true)){
    joy1RBpressed = false;
  }

  else if ((_1PistonJoy->GetLeftTwist()) && (joy1LeftTwistPressed == false)){
    printf("Set cycle time to 4 and actuation time to 1\n");
    singleActuatorSample->SetFullCycleTime(4.0);
    singleActuatorSample->SetActuationTime(1.0);
    joy1LeftTwistPressed = true;
  }
  else if ((!_1PistonJoy->GetLeftTwist()) && (joy1LeftTwistPressed == true)){
    joy1LeftTwistPressed = false;
  }

  else if ((_1PistonJoy->GetRightTwist()) && (joy1RightTwistPressed == false)){
    printf("Set cycle time to 6 and actuation time to 2\n");
    singleActuatorSample->SetFullCycleTime(6.0);
    doubleActuatorSample->SetActuationTime(2.0);
    joy1RightTwistPressed = true;
  }
  else if ((!_1PistonJoy->GetRightTwist()) && (joy1RightTwistPressed == true)){
    joy1RightTwistPressed = false;
  }

  else if ((_1PistonJoy->GetButtonA()) && (joy1ButtonAPressed == false)){
    printf("Start\n");
    singleActuatorSample->Start();
    joy1ButtonAPressed = true;
  }
  else if ((!_1PistonJoy->GetButtonA()) && (joy1ButtonAPressed == true)){
    joy1ButtonAPressed = false;
  }

  else if ((_1PistonJoy->GetButtonB()) && (joy1ButtonBPressed == false)){
    printf("Pause\n");
    singleActuatorSample->Pause();
    joy1ButtonBPressed = true;
  }
  else if ((!_1PistonJoy->GetButtonB()) && (joy1ButtonBPressed == true)){
    joy1ButtonBPressed = false;
  }

  else if ((_1PistonJoy->GetButtonX()) && (joy1ButtonXPressed == false)){
    printf("OverrideEnable\n");
    singleActuatorSample->OverrideEnable();
    joy1ButtonXPressed = true;
  }
  else if ((!_1PistonJoy->GetButtonX()) && (joy1ButtonXPressed == true)){
    joy1ButtonXPressed = false;
  }
  
  else if ((_1PistonJoy->GetButtonY()) && (joy1ButtonYPressed == false)){
    printf("OverrideEnable\n");
    singleActuatorSample->OverrideDisable();
    joy1ButtonYPressed = true;
  }
  else if ((!_1PistonJoy->GetButtonY()) && (joy1ButtonYPressed == true)){
    joy1ButtonYPressed = false;
  }

    //joystick for 2 pistons
  static bool joy2LBpressed = false;
  static bool joy2RBpressed = false;
  static bool joy2LeftTwistPressed = false;
  static bool joy2RightTwistPressed = false;
  static bool joy2ButtonAPressed = false;
  static bool joy2ButtonBPressed = false;
  static bool joy2ButtonXPressed = false;
  static bool joy2ButtonYPressed = false;
  static bool joy2ButtonBackPressed = false;
  static bool joy2ButtonStartPressed = false;

  if ((_2PistonJoy->GetButtonLB()) && (joy2LBpressed == false)){
    printf("Go\n");
    doubleActuatorSample->Go();
    joy2LBpressed = true;
  }
  else if ((!_2PistonJoy->GetButtonLB()) && (joy2LBpressed == true)){
    joy2LBpressed = false;
  }

  else if ((_2PistonJoy->GetButtonRB())&&(joy2RBpressed == false)){
    printf("Terminate\n");
    doubleActuatorSample->Terminate();
    joy2RBpressed = true;
  }
  else if ((!_2PistonJoy->GetButtonRB())&& (joy2RBpressed == true)){
    joy2RBpressed = false;
  }

  else if ((_2PistonJoy->GetLeftTwist()) && (joy2LeftTwistPressed == false)){
    printf("Set cycle time to 4 and actuation time to 1\n");
    doubleActuatorSample->SetFullCycleTime(4.0);
    doubleActuatorSample->SetActuationTime(1.0);
    joy2LeftTwistPressed = true;
  }
  else if ((!_2PistonJoy->GetLeftTwist()) && (joy2LeftTwistPressed == true)){
    joy2LeftTwistPressed = false;
  }

  else if ((_2PistonJoy->GetRightTwist()) && (joy2RightTwistPressed == false)){
    printf("Set cycle time to 6 and actuation time to 2\n");
    doubleActuatorSample->SetFullCycleTime(6.0);
    doubleActuatorSample->SetActuationTime(2.0);
    joy2RightTwistPressed = true;
  }
  else if ((!_2PistonJoy->GetRightTwist()) && (joy2RightTwistPressed == true)){
    joy2RightTwistPressed = false;
  }

  else if ((_2PistonJoy->GetButtonA()) && (joy2ButtonAPressed == false)){
    printf("Start\n");
    doubleActuatorSample->Start();
    joy2ButtonAPressed = true;
  }
  else if ((!_2PistonJoy->GetButtonA()) && (joy2ButtonAPressed == true)){
    joy2ButtonAPressed = false;
  }

  else if ((_2PistonJoy->GetButtonB()) && (joy2ButtonBPressed == false)){
    printf("Pause\n");
    doubleActuatorSample->Pause();
    joy2ButtonBPressed = true;
  }
  else if ((!_2PistonJoy->GetButtonB()) && (joy2ButtonBPressed == true)){
    joy2ButtonBPressed = false;
  }

  else if ((_2PistonJoy->GetButtonX()) && (joy2ButtonXPressed == false)){
    printf("OverrideEnable\n");
    doubleActuatorSample->OverrideEnable();
    joy2ButtonXPressed = true;
  }
  else if ((!_2PistonJoy->GetButtonX()) && (joy2ButtonXPressed == true)){
    joy2ButtonXPressed = false;
  }
  
  else if ((_2PistonJoy->GetButtonY()) && (joy2ButtonYPressed == false)){
    printf("OverrideEnable\n");
    doubleActuatorSample->OverrideDisable();
    joy2ButtonYPressed = true;
  }
  else if ((!_2PistonJoy->GetButtonY()) && (joy2ButtonYPressed == true)){
    joy2ButtonYPressed = false;
  }

  else if ((_2PistonJoy->GetButtonBack()) && (joy2ButtonBackPressed == false)){
    printf("SetDualPistonActuationSync true\n");
    doubleActuatorSample->SetDualPistonActuationSync(true);
    joy2ButtonBackPressed = true;
  }
  else if ((!_2PistonJoy->GetButtonBack()) && (joy2ButtonBackPressed == true)){
    joy2ButtonBackPressed = false;
  }

  else if ((_2PistonJoy->GetButtonStart()) && (joy2ButtonStartPressed == false)){
    printf("SetDualPistonActuationSync false\n");
    doubleActuatorSample->SetDualPistonActuationSync(false);
    joy2ButtonStartPressed = true;
  }
  else if ((!_2PistonJoy->GetButtonStart()) && (joy2ButtonStartPressed == true)){
    joy2ButtonStartPressed = false;
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
