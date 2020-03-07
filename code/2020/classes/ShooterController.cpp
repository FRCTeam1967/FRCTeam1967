#include <iostream>
#include "ShooterController.h"


ShooterControllerInfiniteRecharge::ShooterControllerInfiniteRecharge(int conveyorBeltMotorChannel, int motorRollChannel,int leftPistonChannel,int rightPistonChannel, int flywheelChannelNumber)
{
    conveyorBeltMotor = new WPI_TalonSRX(conveyorBeltMotorChannel);
    conveyorBeltMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
    currentEncoderCount = conveyorBeltMotor->GetSensorCollection().SetQuadraturePosition(0,10);
    flywheelmech = new FlywheelMechInfiniteRecharge(flywheelChannelNumber);
    intakemech = new IntakeMech(motorRollChannel, leftPistonChannel, rightPistonChannel);

    ResetEncoderCount();
    SetDesiredCount(0);
    Start();
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

int ShooterControllerInfiniteRecharge::GetEncoderCount()
{ 
    currentEncoderCount = conveyorBeltMotor->GetSensorCollection().GetQuadraturePosition();
    frc::SmartDashboard::PutNumber("Current Encoder Count", currentEncoderCount);
    return currentEncoderCount;
}

void ShooterControllerInfiniteRecharge::ResetEncoderCount()
{
    currentEncoderCount = conveyorBeltMotor->GetSensorCollection().SetQuadraturePosition(0,10);
    desiredEncoderCount = 0;
    run = false;
}
    
void ShooterControllerInfiniteRecharge::SetDesiredCount(int count)
{
    desiredEncoderCount = count;
    run = true;
}

void ShooterControllerInfiniteRecharge::GetOneBall()
{
    if(GetEncoderCount() < desiredEncoderCount)
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
    if(GetEncoderCount() > desiredEncoderCount)
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

void ShooterControllerInfiniteRecharge::Run() 
{
    if(GetEncoderCount() > desiredEncoderCount && run)
    { 
        intakemech -> RollersIn();
        conveyorBeltMotor->Set(1.0);
    }
    else if(GetEncoderCount() < desiredEncoderCount && run)
    { 
        intakemech -> RollersOut();
        conveyorBeltMotor->Set(-1.0);
    }
    else 
    {
        intakemech -> RollersStop();
        conveyorBeltMotor->Set(0.0);
        ResetEncoderCount();
        std::cout << "STOPPPPPPPPPP" << std::endl;
    } 

    std::cout << "STORAGE ENCODER: " << currentEncoderCount << std::endl;
    std::cout << "STORAGE ENCODER: " << desiredEncoderCount << std::endl;
}