#include <iostream>
#include "ShooterController.h"


ShooterControllerInfiniteRecharge::ShooterControllerInfiniteRecharge(int conveyorBeltMotorChannel, int motorRollChannel,int leftPistonChannel,int rightPistonChannel, int flywheelChannelNumber)
{
    conveyorBeltMotor = new WPI_TalonSRX(conveyorBeltMotorChannel);
    conveyorBeltMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
    currentEncoderCount = conveyorBeltMotor->GetSensorCollection().SetQuadraturePosition(0,10);
    flywheelmech = new FlywheelMechInfiniteRecharge(flywheelChannelNumber);
    intakemech = new IntakeMech(motorRollChannel, leftPistonChannel, rightPistonChannel);
}

ShooterControllerInfiniteRecharge::~ShooterControllerInfiniteRecharge()
{
    delete conveyorBeltMotor;
    delete flywheelmech;
    delete intakemech;
}

void ShooterControllerInfiniteRecharge::Target()
{
    flywheelmech ->SetRPM();
}

void ShooterControllerInfiniteRecharge::GetEncoderCount()
{ 
    currentEncoderCount = conveyorBeltMotor->GetSensorCollection().GetQuadraturePosition();
    frc::SmartDashboard::PutNumber("Current Encoder Count", currentEncoderCount);
}

void ShooterControllerInfiniteRecharge::ResetEncoderCount()
{
    currentEncoderCount = conveyorBeltMotor->GetSensorCollection().SetQuadraturePosition(0,10);
}
    
void ShooterControllerInfiniteRecharge::SetDesiredCount(double desiredCount)
{
    desiredEncoderCount = 5120;
}

void ShooterControllerInfiniteRecharge::GetOneBall()
{
    if(currentEncoderCount < desiredEncoderCount)
    { 
        intakemech -> RollersIn();
        conveyorBeltMotor->Set(1.0);
    }
    else
    {
        conveyorBeltMotor->Set(0.0);
        intakemech -> RollersStop();
        ResetEncoderCount();
    }
}

void ShooterControllerInfiniteRecharge::OuttakeOneBall()
{
    if(currentEncoderCount > desiredEncoderCount)
    { 
        intakemech -> RollersOut();
        conveyorBeltMotor->Set(-1.0);
    }
    else
    {
        intakemech -> RollersStop();
        conveyorBeltMotor->Set(0.0);
        ResetEncoderCount();
    }
}

void ShooterControllerInfiniteRecharge::StopConveyorBelt() 
{
    conveyorBeltMotor->Set(0.0);
}

double ShooterControllerInfiniteRecharge::GetRunningRPM()
{
    return flywheelmech->GetRunningRPM();
}
double ShooterControllerInfiniteRecharge::GetDesiredRPM()
{
    return flywheelmech->ReturnDesiredRPM();
}