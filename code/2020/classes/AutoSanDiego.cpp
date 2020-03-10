#include <iostream>
#include "AutoSanDiego.h"

#define FLYWHEEL_TIME 1
#define CONVEYOR_TIME 3
#define DISTANCE 4096
#define FORWARD 1
#define BACKWARD -1

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
    offsetFromVisionTarget = (SmartDashboard::GetNumber(VISION_OFFSET, NO_DATA_DEFAULT)); //positive is to the right

    // TIMERS
    shootingTimer = new Timer();
    shootingTimer -> Reset();
    flywheelTimer = new Timer();
    flywheelTimer -> Reset();

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

void AutoSanDiego::DriveStraight(int distancePulses, int forwardOrBack) {
    int avgEncoders = (GetRightEncoder() + GetLeftEncoder()) / 2;
    if (avgEncoders < distancePulses) {
        float speed = 0.5 * forwardOrBack;
        drive -> TankDrive(-speed, -speed);
    }
    else {
        drive -> TankDrive(0.0, 0.0);
        isFinishedDriving = true;
    }
}

void AutoSanDiego::CenterTurretAutomatically() {
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

void AutoSanDiego::RunAuto() {
    // TURRET
    CenterTurretAutomatically();

    // AUTO SEQUENCE
    switch(caseNum) {
        case 0:
            cout << "CASE 0" << endl;
            StartFlywheelTimer();
            caseNum = 1;
            break;
        case 1: 
            cout << "CASE 1" << endl;
            shootercontroller->Target();
            if(flywheelTimer->Get() > FLYWHEEL_TIME)
            {
                StartShootingTimer();
                EndFlywheelTimer();
                caseNum = 2;
            }
            break; 
        case 2: 
            cout << "CASE 2" << endl;
            shootercontroller->StartConveyorBelt();
            shootercontroller->BridgeForward(); 
            if(flywheelTimer->Get() > CONVEYOR_TIME)
            {
                shootercontroller->StopConveyorBelt();
                shootercontroller->StopBridge(); 
                EndShootingTimer();
                caseNum = 3;
            }
            break;
        case 3:
            cout << "CASE 3" << endl;
            DriveStraight(DISTANCE, FORWARD);
            if (isFinishedDriving == true) {
                caseNum = 4;
            }
            break;
        case 4: 
            cout << "IS FINISHED AUTO" << endl;
            break;
    }
}