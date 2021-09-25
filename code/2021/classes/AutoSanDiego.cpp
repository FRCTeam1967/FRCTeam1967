#include <iostream>
#include "AutoSanDiego.h"

#define FLYWHEEL_TIME 1
#define CONVEYOR_TIME 3
#define DRIVING_TIME 2
#define DISTANCE 50
#define FORWARD -1
#define BACKWARD 1

using namespace frc;
using namespace ctre;
using namespace std;

AutoSanDiego::AutoSanDiego(WPI_VictorSPX*flmotor, WPI_TalonSRX*frmotor, WPI_TalonSRX*rlmotor, WPI_VictorSPX*rrmotor, ShooterControllerInfiniteRecharge * shootercontroller) {
    this->shootercontroller = shootercontroller;
    this->flmotor = flmotor;
    this->frmotor = frmotor;
    this->rlmotor = rlmotor;
    this->rrmotor = rrmotor;

    Initialize();
}

AutoSanDiego::~AutoSanDiego() {
    delete leftDrive;
    delete rightDrive;
    delete drive;
    delete flywheelTimer;
    delete shootingTimer;
}

void AutoSanDiego::Initialize() {
    // SET UP DRIVE TRAIN
    leftDrive = new SpeedControllerGroup(*flmotor, *rlmotor);
    rightDrive = new SpeedControllerGroup(*frmotor, *rrmotor);
    drive = new DifferentialDrive(*leftDrive, *rightDrive);

    // CHASSIS ENCODERS
    rlmotor->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
    rlmotor->SetSelectedSensorPosition(0, 0, 10);
    rlmotor->GetSensorCollection().SetQuadraturePosition(0,10);
    frmotor->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
    frmotor->SetSelectedSensorPosition(0, 0, 10);
    frmotor->GetSensorCollection().SetQuadraturePosition(0,10);

    // VISION DATA
    SmartDashboard::PutNumber(VISION_OFFSET, NO_DATA_DEFAULT);
    offsetFromVisionTarget = (SmartDashboard::GetNumber(VISION_OFFSET, NO_DATA_DEFAULT)); //positive is to the right

    // TIMERS
    shootingTimer = new Timer();
    shootingTimer -> Reset();
    flywheelTimer = new Timer();
    flywheelTimer -> Reset();
    drivingTimer = new Timer();
    drivingTimer -> Reset();
    delayTimer = new Timer();
    delayTimer -> Reset();

    // VARIABLES
    isFinishedDriving = false;
    caseNum = 0;
}

void AutoSanDiego::ResetEncoders() {
    rlmotor->GetSensorCollection().SetQuadraturePosition(0,10);
    frmotor->GetSensorCollection().SetQuadraturePosition(0,10);
} 

int AutoSanDiego::GetRightEncoder() {
    return frmotor->GetSensorCollection().SetQuadraturePosition(0,10);
}

int AutoSanDiego::GetLeftEncoder() {
    return rlmotor->GetSensorCollection().SetQuadraturePosition(0,10);
}

void AutoSanDiego::StartShootingTimer() {
    shootingTimer->Start();
}

void AutoSanDiego::EndShootingTimer() {
    shootingTimer->Stop();
    shootingTimer->Reset();
}

void AutoSanDiego::StartFlywheelTimer() {
    flywheelTimer->Start();
}

void AutoSanDiego::EndFlywheelTimer() {
    flywheelTimer->Stop();
    flywheelTimer->Reset();
}

void AutoSanDiego::StartDelayTimer() {
    delayTimer->Start();
}

void AutoSanDiego::EndDelayTimer() {
    delayTimer->Stop();
    delayTimer->Reset();
}



void AutoSanDiego::DriveStraight(int distancePulses, int forwardOrBack) {
    int avgEncoders = (GetRightEncoder() + GetLeftEncoder()) / 2;
    cout << "DISTANCE: " << avgEncoders << endl;
    if (drivingTimer->Get() < DRIVING_TIME) {
        float speed = 0.5 * forwardOrBack;
        drive -> TankDrive(-speed, -speed);
    }
    else {
        drive -> TankDrive(0.0, 0.0);
        isFinishedDriving = true;
    }
    cout << avgEncoders << endl;
}

void AutoSanDiego::CenterTurretAutomatically() {
    offsetFromVisionTarget = (SmartDashboard::GetNumber(VISION_OFFSET, NO_DATA_DEFAULT));
    if (offsetFromVisionTarget == BAD_DATA)
    {
      shootercontroller->StopTurret();
    }
    else if (offsetFromVisionTarget < LOWER_BOUND) 
    {
      shootercontroller-> TurretRight();
    }
    else if (offsetFromVisionTarget > UPPER_BOUND)
    {
      shootercontroller-> TurretLeft();
    }
    else {
      shootercontroller->StopTurret();
    }
}

void AutoSanDiego::setInitialDelayTime(AutoSDDelaySelector::Options delay){
    //is there a better way to get a number from enum?
    if (delay == AutoSDDelaySelector::Options::zeroSec){
        initialDelay = 0;
    }
    else if (delay == AutoSDDelaySelector::Options::oneSec){
        initialDelay = 1;
    }
    else if (delay == AutoSDDelaySelector::Options::twoSec){
        initialDelay = 2;
    }
    else if (delay == AutoSDDelaySelector::Options::threeSec){
        initialDelay = 3;
    }
    else if (delay == AutoSDDelaySelector::Options::fourSec){
        initialDelay = 4;
    }
    else if (delay == AutoSDDelaySelector::Options::fiveSec){
        initialDelay = 5;
    }
    else if (delay == AutoSDDelaySelector::Options::sixSec){
        initialDelay = 6;
    }
    else{
        initialDelay = 0; //in case there's some error
    }
    SmartDashboard::PutNumber("initial auto delay: ", initialDelay);
}


void AutoSanDiego::RunAuto() {
    CenterTurretAutomatically();
    // AUTO SEQUENCE
    switch(caseNum) {
        case 0:
            cout << "CASE 0" << endl;
            StartDelayTimer();
            drive -> TankDrive(0.0, 0.0);
            caseNum = 1;
            break;
        case 1:
            cout << "CASE 1" << endl;
            CenterTurretAutomatically(); //center during delay
            drive -> TankDrive(0.0, 0.0);
            if (delayTimer -> Get() >= initialDelay){
                EndDelayTimer();
                caseNum = 2;
            }
            break;
        case 2:
            cout << "CASE 2" << endl; 
            CenterTurretAutomatically(); //centered during delay --> make sure it works (even if delay = 0)
            if (offsetFromVisionTarget == BAD_DATA){
                CenterTurretAutomatically();
                SmartDashboard::PutBoolean("Turret Centering", false);
            }
            else{
                StartFlywheelTimer(); 
                SmartDashboard::PutBoolean("Turret Centering", true);
                caseNum = 3;
            }
            drive -> TankDrive(0.0, 0.0);
            break;
        case 3: 
            cout << "CASE 3" << endl;
            CenterTurretAutomatically();
            shootercontroller->SetRPMAuto();
            if(flywheelTimer->Get() > FLYWHEEL_TIME)
            {
                StartShootingTimer();
                EndFlywheelTimer();
                caseNum = 4;
            }
            drive -> TankDrive(0.0, 0.0);
            break; 
        case 4: 
            cout << "CASE 4" << endl;
            CenterTurretAutomatically();
            shootercontroller->StartConveyorBelt();
            shootercontroller->BridgeForward(); 
            if(shootingTimer->Get() > CONVEYOR_TIME)
            {
                shootercontroller->StopConveyorBelt();
                shootercontroller->StopBridge(); 
                shootercontroller->StopTarget();
                drivingTimer->Start();
                EndShootingTimer();
                caseNum = 5;
            }
            drive -> TankDrive(0.0, 0.0);
            break;
        case 5:
            cout << "CASE 5" << endl;
            CenterTurretAutomatically();
            DriveStraight(DISTANCE, BACKWARD);
            if (isFinishedDriving == true) {
                caseNum = 6;
            }
            break;
        case 6: 
            CenterTurretAutomatically();
            drive -> TankDrive(0.0, 0.0);
            cout << "IS FINISHED AUTO" << endl;
            break;
    }
}

//selector for delay:
AutoSDDelaySelector::AutoSDDelaySelector(){
    Initialize(optionsList, selectorName);
}
AutoSDDelaySelector::~AutoSDDelaySelector(){ }

AutoSDDelaySelector::Options AutoSDDelaySelector::GetSelection(){
    Options selectedDelay = (Options) GetSelectedValue();
    return selectedDelay;
}
