/*       CargoManip.cpp
*   created on: Jan 19, 2019
*    by: Sandhya, Isha, GC2    */

#include "Settings.h"
#include "frc/WPILib.h" 
#include <iostream> 
#include <frc/smartdashboard/SmartDashboard.h>
#include "CargoManip.h"
#include "ctre/Phoenix.h" 
#include "hal/Constants.h"
#include <math.h>
#include <frc/Encoder.h>
#include <jankyXboxJoystick.h>

#define MOTOR_ROLL_F_SPEED -1.0   //roller intake speed
#define MOTOR_ROLL_R_SPEED 1.0   //roller outtake speed
#define MOTOR_PIVOT_F_SPEED -1.0  //mech out of bot speed
#define MOTOR_PIVOT_R_SPEED 1.0 //mech in bot speed
#define MOTOR_STOP_SPEED 0.0  // stops motor
#define ENCODER_COUNTS_PER_REVOLUTION (((10*(18/16))*4096))
#define HP_ANGLE_PULSES -4500
#define GROUND_PULSES -7000
#define ROBOT_PULSES -1000

CargoManip::CargoManip(int motorRollChannel, int motorPivotChannel){
  motorRoll = new WPI_VictorSPX(motorRollChannel);
  pivotMotor = new WPI_TalonSRX(motorPivotChannel);
  ResetPivotEncoder();

  //pid initialization for pivot motor
  kTimeoutMs = 50;
  kPIDLoopIdx = 0;
  pivotEncoderCount = 0.0;
	encoderAngle = 0.0;
  desiredAngle = 0;
  desiredAnglePulses = ROBOT_PULSES;

  pivotMotor -> ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, kPIDLoopIdx, kTimeoutMs);
	pivotMotor -> SetSensorPhase(false);
  pivotMotor -> ConfigNominalOutputForward(0, kTimeoutMs);
	pivotMotor -> ConfigNominalOutputReverse(0, kTimeoutMs);
	pivotMotor -> ConfigPeakOutputForward(1.0, kTimeoutMs); //going in? test
	pivotMotor -> ConfigPeakOutputReverse(-1.0, kTimeoutMs);
 
	pivotMotor->Config_kF(kPIDLoopIdx, 0.0, kTimeoutMs); //not using feedforward
	pivotMotor->Config_kP(kPIDLoopIdx, 1.6, kTimeoutMs); //p val: 0.01 (tune)
	pivotMotor->Config_kI(kPIDLoopIdx, 0.0, kTimeoutMs); //i val: 0
	pivotMotor->Config_kD(kPIDLoopIdx, 0.0, kTimeoutMs); //d val: 0 (use if needed while tuning p)
  pivotMotor -> SelectProfileSlot(0.0, kPIDLoopIdx); //kpidloopidx = pidloopidx?

  pivotMotor -> Set(ControlMode::Position, desiredAnglePulses);
}

CargoManip::~CargoManip(){
  delete motorRoll;
  delete pivotMotor;
}

void CargoManip::ResetPivotEncoder(){
  pivotMotor -> ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	pivotMotor -> SetSelectedSensorPosition(0, 0, 10);
}

void CargoManip::RollersIn(){
  motorRoll -> Set(MOTOR_ROLL_F_SPEED);
}

void CargoManip::RollersOut(){
  motorRoll -> Set(MOTOR_ROLL_R_SPEED);
}

void CargoManip::RollersStop(){
  motorRoll -> Set(MOTOR_STOP_SPEED);
}

void CargoManip::FindEncoderCount(){
  //pivotMotor -> SetSelectedSensorPosition(0, 0, 10);
  pivotEncoderCount = pivotMotor -> GetSensorCollection().GetQuadraturePosition();
  frc::SmartDashboard::PutNumber("Current Pivot Angle Pulses:", pivotEncoderCount);
}


void CargoManip::FindEncoderAngle(){
  FindEncoderCount();
  encoderAngle = ((pivotEncoderCount / ENCODER_COUNTS_PER_REVOLUTION)/360);
  frc::SmartDashboard::PutNumber("Current Pivot Angle:", encoderAngle);
}

void CargoManip::SetPIDAngle(float desiredAnglePulses){
  //desiredAnglePulses = -1*(((desiredAngle / 360) * ENCODER_COUNTS_PER_REVOLUTION));
  pivotMotor -> Set(ControlMode::Position, desiredAnglePulses);
  frc::SmartDashboard::PutNumber("Desired Pivot Angle:", desiredAngle);
  frc::SmartDashboard::PutNumber("Desired Pivot Angle Pulses:", desiredAnglePulses);
}

/*void CargoManip::CargoMechIn(){
  FindEncoderAngle();
  if (desiredAngle == 20){ //current angle should be 0 from view, 90 zeroed 
      desiredAngle = 0;
      SetPIDAngle(0);
  }
  else if (desiredAngle   == 90){ //current angle should be 70, 20 zeroed
      desiredAngle = 20;
      SetPIDAngle(20);
  }
}

void CargoManip::CargoMechOut(){
  FindEncoderAngle();
    if (desiredAngle == 0){ //currently angle should be 90 from view, 0 zeroes
      desiredAngle = 20;
      SetPIDAngle(20);
    }
    else if (desiredAngle == 20){ //current angle should be 70
      desiredAngle = 90;
      SetPIDAngle(90);
  }
}*/

void CargoManip::CargoInRobot(){
    desiredAnglePulses = ROBOT_PULSES;
    SetPIDAngle(ROBOT_PULSES);
}

void CargoManip::CargoHPAngle(){
    desiredAnglePulses = HP_ANGLE_PULSES;
    SetPIDAngle(HP_ANGLE_PULSES);
}

void CargoManip::CargoGroundAngle(){
    desiredAnglePulses = GROUND_PULSES;
    SetPIDAngle(GROUND_PULSES);
}

float CargoManip::GetHatchPanelDistance(){
  return (pivotMotor -> GetSensorCollection().GetAnalogIn());
}