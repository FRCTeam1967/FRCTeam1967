#include <iostream>
#include "Flywheel.h"

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
    distanceToVisionTarget = frc::SmartDashboard::GetNumber(VISION_DISTANCE, NO_DATA_DEFAULT);
}

double FlywheelMechInfiniteRecharge::GetDistanceToVisionTarget()
{
    return distanceToVisionTarget;
}

void FlywheelMechInfiniteRecharge::SetRPM()
{
    desiredRPM = (3.7272 * distanceToVisionTarget) + 5500; //4951.4266; 
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