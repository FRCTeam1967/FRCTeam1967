#include <iostream> 
#include <frc/smartdashboard/SmartDashboard.h>
#include "IntakeMech.h"
#include "ctre/Phoenix.h" 
#include <frc/Encoder.h>
#include <jankyXboxJoystick.h>

//motor speeds
#define MOTOR_ROLL_F_SPEED -1.0   //roller intake speed (the signs may have to be switched)
#define MOTOR_ROLL_R_SPEED 1.0   //roller outtake speed
#define MOTOR_STOP_SPEED 0.0  // stops motor

using namespace frc;
using namespace ctre;

IntakeMech::IntakeMech(int motorRollChannel,int leftPistonChannel,int rightPistonChannel){
  motorRoll = new WPI_VictorSPX(motorRollChannel);
  leftPiston = new Solenoid(10, leftPistonChannel); //10 is a questionable value 
  rightPiston = new Solenoid(10, rightPistonChannel);
}
IntakeMech::~IntakeMech(){
  delete motorRoll;
  delete leftPiston;
  delete rightPiston; 
}
void IntakeMech::RollersIn(){
  motorRoll -> Set(MOTOR_ROLL_F_SPEED);
}
void IntakeMech::RollersOut(){
  motorRoll -> Set(MOTOR_ROLL_R_SPEED);
}
void IntakeMech::RollersStop(){
  motorRoll -> Set(MOTOR_STOP_SPEED);
}
void IntakeMech::MechInRobot(){
  leftPiston -> Set(false);
  rightPiston -> Set(false);
}
void IntakeMech::MechOutRobot(){
  leftPiston -> Set(true);
  rightPiston -> Set(true);
}