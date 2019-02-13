/*       CargoManip.cpp
*   created on: Jan 19, 2019
*    by: Sandhya, Isha, GC2    */

#include "frc/WPILib.h" 
#include <iostream> 
#include <frc/smartdashboard/SmartDashboard.h>
#include "CargoManip.h"
#include "ctre/Phoenix.h" 
#include <math.h>
#include <frc/Encoder.h>
#include <jankyXboxJoystick.h>


#define MOTOR_ROLL_F_SPEED -1.0   //roller intake speed
#define MOTOR_ROLL_R_SPEED 1.0   //roller outtake speed
#define MOTOR_PIVOT_F_SPEED -0.4  //mech out of bot speed
#define MOTOR_PIVOT_R_SPEED 0.8 //mech in bot speed
#define MOTOR_STOP_SPEED 0.0  // stops motor
#define ENCODER_COUNTS_PER_REVOLUTION 4096

CargoManip::CargoManip(int motorRollChannel, int motorPivotChannel, int limSwitchInsideChannel, int limSwitchOutsideChannel){
  motorRoll = new WPI_VictorSPX(motorRollChannel);
  pivotMotor = new WPI_TalonSRX(motorPivotChannel);
  limSwitchInside = new frc::DigitalInput(limSwitchInsideChannel);
  limSwitchOutside = new frc::DigitalInput(limSwitchOutsideChannel);
  //encoderRoll = new frc::Encoder();
  //encoderPivot = new frc::Encoder();
}

CargoManip::~CargoManip(){
  delete motorRoll;
  delete pivotMotor;
  delete limSwitchInside;
  delete limSwitchOutside;
  //delete encoderRoll;
  //delete encoderPivot;
}


/*void CargoManip::ButtonVals(){

}*/

void CargoManip::RollersIn(){
  motorRoll -> Set(MOTOR_ROLL_F_SPEED);

}

void CargoManip::RollersOut(){
  motorRoll -> Set(MOTOR_ROLL_R_SPEED);
}

void CargoManip::RollersStop(){
  motorRoll -> Set(MOTOR_STOP_SPEED);
}

int CargoManip::GetLimSwitchInside(){
  return limSwitchInside -> Get();
}

int CargoManip::GetLimSwitchOutside(){
  return limSwitchOutside -> Get();
}

void CargoManip::CargoMechOutRobot(){
  pivotMotor -> Set(MOTOR_PIVOT_F_SPEED);
  cargoMechExtended = true;
  cargoMechGoingForward = true;
  cargoMechGoingBackward = false;
}

void CargoManip::CargoMechInRobot(){
  pivotMotor -> Set(MOTOR_PIVOT_R_SPEED);
  cargoMechExtended = false;
  cargoMechGoingForward = false;
  cargoMechGoingBackward = true;
}

void CargoManip::CargoMechStop(){
  pivotMotor -> Set(MOTOR_STOP_SPEED);
  cargoMechExtended = false;
  cargoMechGoingForward = false;
  cargoMechGoingBackward = false;
}

void CargoManip::CargoMechStopWithLimSwitch(){
	if ((GetLimSwitchOutside()==1) && cargoMechGoingForward) {
		CargoMechStop();
	}
	else if ((GetLimSwitchInside()==1) && cargoMechGoingBackward) {
		CargoMechStop();
	}
}

bool CargoManip::getCargoMechPosition(){
  return cargoMechExtended;
}

double CargoManip::getEncoderCount(){
  pivotMotor -> ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
  pivotMotor -> SetSelectedSensorPosition(0, 0, 10);
  double encoderCount = pivotMotor -> GetSensorCollection().SetQuadraturePosition(0, 10);
}

float CargoManip::GetEncoderAngle(){
  float encoderAngle = ((encoderCount / ENCODER_COUNTS_PER_REVOLUTION) * 360);
  frc::SmartDashboard::PutNumber("Pivot Encoder Angle:", encoderAngle);
}

void CargoManip::StartInit(){
  pivotMotor -> GetSensorCollection().SetQuadraturePosition(0,10);
	encoderCount = 0.0;
	encoderAngle = 0.0;
}