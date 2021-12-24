#include <iostream>
#include "ShooterController.h"

using namespace frc;
using namespace ctre;

ShooterControllerInfiniteRecharge::ShooterControllerInfiniteRecharge(int conveyorBeltMotorChannel, int motorRollChannel,int leftPistonChannel,int rightPistonChannel, int flywheelChannelNumber, int bridgeChannel, int turretChannel)
{
    conveyorBeltMotor = new WPI_TalonSRX(conveyorBeltMotorChannel);
    conveyorBeltMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
    currentEncoderCount = conveyorBeltMotor->GetSensorCollection().SetQuadraturePosition(0,10);
    flywheelmech = new FlywheelMechInfiniteRecharge(flywheelChannelNumber);
    intakemech = new IntakeMech(motorRollChannel, leftPistonChannel, rightPistonChannel);

    bridgeMotor = new WPI_VictorSPX(bridgeChannel);
    turretMotor = new WPI_TalonSRX(turretChannel); 

    ResetEncoderCount();
    SetDesiredCount(0);
    Start();
}

ShooterControllerInfiniteRecharge::~ShooterControllerInfiniteRecharge()
{
    delete conveyorBeltMotor;
    delete flywheelmech;
    delete intakemech;
    delete bridgeMotor;
    delete turretMotor;
}

void ShooterControllerInfiniteRecharge::Target()
{
    flywheelmech ->SetRPM();
}

void ShooterControllerInfiniteRecharge::SetRPMAuto()
{
    flywheelmech ->SetRPMAuto();
}


void ShooterControllerInfiniteRecharge::FlywheelOut()
{
    flywheelmech -> FlywheelOut();
}


void ShooterControllerInfiniteRecharge::SetSelectorVisionDistance(int selectorZone){ 
    flywheelmech -> SetSelectorVisionDistance(selectorZone);
}

int ShooterControllerInfiniteRecharge::GetEncoderCount()
{ 
    currentEncoderCount = conveyorBeltMotor->GetSensorCollection().GetQuadraturePosition();
    frc::SmartDashboard::PutNumber("Current Conveyor Encoder Count", currentEncoderCount);
    return -1 *  currentEncoderCount;
}

void ShooterControllerInfiniteRecharge::ResetEncoderCount()
{
    currentEncoderCount = conveyorBeltMotor->GetSensorCollection().SetQuadraturePosition(0,10);
    desiredEncoderCount = 0;
    run = false;
    manualSConveyor = true;
}
    
void ShooterControllerInfiniteRecharge::SetDesiredCount(int count)
{
    desiredEncoderCount = count;
    run = true;
    manualSConveyor = false;
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
    manualSConveyor = true;
}

double ShooterControllerInfiniteRecharge::GetRunningRPM()
{
    return flywheelmech->GetRunningRPM();
}
double ShooterControllerInfiniteRecharge::GetDesiredRPM()
{
    return flywheelmech->ReturnDesiredRPM();
}

void ShooterControllerInfiniteRecharge::IntakeIn() {
    intakemech -> RollersIn();
}

void ShooterControllerInfiniteRecharge::IntakeOut() {
    intakemech -> RollersOut();
}

void ShooterControllerInfiniteRecharge::IntakeStop() {
    intakemech -> RollersStop();
}

void ShooterControllerInfiniteRecharge::StopTarget() {
    flywheelmech->StopFlywheel();
}

void ShooterControllerInfiniteRecharge::StartConveyorBelt() {
    manualSConveyor = true;
    conveyorBeltMotor->Set(-0.6); //was 1.0, then 0.7; not sure why - makes it forward
}

void ShooterControllerInfiniteRecharge::IntakePistonsDown() {
    intakemech->MechOutRobot();
}

void ShooterControllerInfiniteRecharge::IntakePistonsUp() {
    intakemech->MechInRobot();
}

void ShooterControllerInfiniteRecharge::BridgeForward() {
    bridgeMotor->Set(1.0);
}

void ShooterControllerInfiniteRecharge::StopBridge() {
    bridgeMotor->Set(0.0);
}

void ShooterControllerInfiniteRecharge::BridgeBackward(){
    bridgeMotor->Set(-1.0);
}


void ShooterControllerInfiniteRecharge::TurretLeft(float turretSpeed) {
    turretMotor->Set(-turretSpeed);
}

void ShooterControllerInfiniteRecharge::TurretRight(float turretSpeed) {
    turretMotor->Set(turretSpeed);
}

void ShooterControllerInfiniteRecharge::StopTurret() {
    turretMotor->Set(0.0);
}

/*
void ShooterControllerInfiniteRecharge::ConveyorBeltOut(){//not pulsing
    manualSConveyor = true;
    conveyorBeltMotor->Set(-0.7);
}
*/

void ShooterControllerInfiniteRecharge::Run() 
{
    if(!manualSConveyor && (GetEncoderCount() > desiredEncoderCount) && (run) && (desiredEncoderCount < 0))
    { 
        conveyorBeltMotor->Set(0.7);
    }
    else if(!manualSConveyor && (GetEncoderCount() < desiredEncoderCount) && (run) && (desiredEncoderCount > 0))
    { 
        conveyorBeltMotor->Set(-0.7);
    }
    else if (!manualSConveyor)
    {
        conveyorBeltMotor->Set(0.0);
        ResetEncoderCount();
    } 
    frc::SmartDashboard::PutNumber("Conveyor Encoder Count", GetEncoderCount());
    frc::SmartDashboard::PutNumber("Desired Conveyor Encoder", desiredEncoderCount);
    //std::cout << "CONVEYOR ENCODER: " << GetEncoderCount() << std::endl;
}