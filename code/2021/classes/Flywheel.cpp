#include <iostream>
#include "Flywheel.h"
#include "Settings.h"

FlywheelMechInfiniteRecharge::FlywheelMechInfiniteRecharge(int flywheelChannelNumber)
{ 
    kSlotIdx = 0;
    kPIDLoopIdx = 0; 
    kTimeoutMs = 30; 
    flywheelMotor = new TalonFX(flywheelChannelNumber); //insert channel number accordingly 
    flywheelMotor->ConfigFactoryDefault();
    flywheelMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, kTimeoutMs);
    flywheelMotor->SetSensorPhase(true);
    flywheelMotor->ConfigNominalOutputForward(0, kTimeoutMs);
    flywheelMotor->ConfigNominalOutputReverse(0, kTimeoutMs);
    flywheelMotor->ConfigPeakOutputForward(1, kTimeoutMs);
    flywheelMotor->ConfigPeakOutputReverse(-1, kTimeoutMs);
    flywheelMotor->Config_kF(kPIDLoopIdx, 0.045, kTimeoutMs);
    flywheelMotor->Config_kP(kPIDLoopIdx, 0.0012, kTimeoutMs);
    flywheelMotor->Config_kI(kPIDLoopIdx, 0.0, kTimeoutMs);
    flywheelMotor->Config_kD(kPIDLoopIdx, 0.0001, kTimeoutMs);

    frc::SmartDashboard::PutNumber(VISION_DISTANCE, NO_DATA_DEFAULT);
}

FlywheelMechInfiniteRecharge::~FlywheelMechInfiniteRecharge()
{    
    delete flywheelMotor; 
}

void FlywheelMechInfiniteRecharge::CalculateDistanceToVisionTarget() 
{
    //distanceToVisionTarget = frc::SmartDashboard::GetNumber(VISION_DISTANCE, NO_DATA_DEFAULT);
}

double FlywheelMechInfiniteRecharge::GetDistanceToVisionTarget()
{
    return distanceToVisionTarget;
}

void FlywheelMechInfiniteRecharge::SetRPM()
{
    // offset was 5500 originally then 2750 was tried
    desiredRPM = (5.4545 * distanceToVisionTarget) + 4000; //4951.4266; used to be 5500; m=3.7272
    targetVelocity_UnitsPer100ms = desiredRPM * 2048 / 600;
    frc::SmartDashboard::PutNumber("Desired RPM: ", desiredRPM);
    flywheelMotor->Set(ControlMode::Velocity, targetVelocity_UnitsPer100ms);
}

void FlywheelMechInfiniteRecharge::SetRPMAuto() 
{
    desiredRPM = 6000;
    targetVelocity_UnitsPer100ms = desiredRPM * 2048 / 600;
    flywheelMotor->Set(ControlMode::Velocity, targetVelocity_UnitsPer100ms);
}

double FlywheelMechInfiniteRecharge::ReturnDesiredRPM()
{
    return desiredRPM; 
}

double FlywheelMechInfiniteRecharge::GetRunningRPM()
{ 
    runningRPM = (flywheelMotor->GetSelectedSensorVelocity(kPIDLoopIdx)) / 2048 * 600;
    frc::SmartDashboard::PutNumber("RPM flywheel is running at", runningRPM);
    return runningRPM;
}

void FlywheelMechInfiniteRecharge::StopFlywheel() {
    flywheelMotor->Set(ControlMode::Velocity, 0);
}
 void FlywheelMechInfiniteRecharge::SetSelectorVisionDistance(int selectorZone){
  if (selectorZone == SHOOTING_ZONE_1){
      distanceToVisionTarget = 275; //in inches 
  }   
  else if (selectorZone == SHOOTING_ZONE_2){
      distanceToVisionTarget = 100;
  }
  else if (selectorZone == SHOOTING_ZONE_3){
      distanceToVisionTarget = 150;
  }
  else if (selectorZone == SHOOTING_ZONE_4){
      distanceToVisionTarget = 250;
  }
  else if (selectorZone == SHOOTING_DISTANCE_50){
      distanceToVisionTarget = 50;
  }
  else if (selectorZone == SHOOTING_DISTANCE_75){
      distanceToVisionTarget = 75;
  }
  else if (selectorZone == SHOOTING_DISTANCE_100){
      distanceToVisionTarget = 100;
  }
  else if (selectorZone == SHOOTING_DISTANCE_125){
      distanceToVisionTarget = 125;
  }
  else if (selectorZone == SHOOTING_DISTANCE_150){
      distanceToVisionTarget = 150;
  }
  else if (selectorZone == SHOOTING_DISTANCE_175){
      distanceToVisionTarget = 175;
  }
  else if (selectorZone == SHOOTING_DISTANCE_200){
      distanceToVisionTarget = 200;
  }
  else if (selectorZone == SHOOTING_DISTANCE_225){
      distanceToVisionTarget = 225;
  }
  else if (selectorZone == SHOOTING_DISTANCE_250){
      distanceToVisionTarget = 250;
  }
  else if (selectorZone == SHOOTING_DISTANCE_275){
      distanceToVisionTarget = 275;
  }
  else if (selectorZone == SHOOTING_DISTANCE_300){
      distanceToVisionTarget = 10;
  }
  // take out because was for testing: distanceToVisionTarget /= 10;

  frc::SmartDashboard::PutNumber("Manual Distance to Vision Target: ", distanceToVisionTarget);
 }