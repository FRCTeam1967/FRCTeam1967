/*
 * VisionStateMachine.cpp
 *
 *  Created on: Jan 20, 2019
 *      Author: AnishaKabir
 */
#include "frc/WPILib.h"
#include <VisionStateMachine.h>
#include "jankyStateMachine.h"
#include "JankyAutoEntry.h"
#include "ctre/Phoenix.h"
#include "AutoDrive.h"
#include "DifferentialDriveSegment.h"
#include "DifferentialTurnSegment.h"

#define VISION_DRIVE_SPEED 0.4 //arbitrary speed for now
#define DRIVE_SPEED 0.5
#define TURN_SPEED 0.3

//P, I, and D values for the AutoDrive in which the robot turns to get horizontal offset to zero
// THESE VALUES NEED TO BE TUNED
float vision_kP = 0.0;
float vision_kI = 0.0;
float vision_kD = 0.0;

float drive_kP = 0.04;
float drive_kI = 0.0;
float drive_kD = 0.0;

float turn_kP = 0.05;
float turn_kI = 0.0;
float turn_kD = 0.06;

AutoDrive*visionDrive;
DifferentialDriveSegment*driveSegment;
DifferentialTurnSegment*turn;

VisionStateMachine::VisionStateMachine(frc::DifferentialDrive*drive, frc::ADXRS450_Gyro*gyro, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*flmotor, WPI_TalonSRX*frmotor, WPI_TalonSRX*rlmotor, WPI_TalonSRX*rrmotor){ //add cargo and hatch mechanisms
    visionDrive= new ::AutoDrive(drive, VISION_DRIVE_SPEED, vision_kP, vision_kI, vision_kD, flmotor, frmotor, rlmotor, rrmotor); // added :: to resolve "expected type specifier" error
    driveSegment= new DifferentialDriveSegment(gyro, drive, leftEncoder, rightEncoder, flmotor, frmotor, 78, DRIVE_SPEED, drive_kP, drive_kI, drive_kD);
    turn= new DifferentialTurnSegment(gyro, drive, -90, TURN_SPEED, turn_kP, turn_kI, turn_kD);

    SetMachineName("VisionStateMachine");
    SetName(Idle, "Default Idle");
    SetName(AutoDrive, "Drive to tape w/ vision");
    SetName(DriveComplete, "Robot arrived to tape");
    SetName(ScoreCargoOnCargoShip, "Score cargo into cargo ship");
    SetName(ScoreCargoOnRocketShip, "Score cargo into rocket ship");
    SetName(ScoreHatchPanel, "Score hatch panel");
    SetName(CancelActions, "Cancel the current state");
    SetName(DriveSegment, "Drive");
    SetName(TurnSegment, "Turn");

    isCancelled=false;

    Start();
}

VisionStateMachine::~VisionStateMachine(){
    Terminate();
    delete visionDrive;
    delete driveSegment;
    delete turn;
}

void VisionStateMachine::StartSequence(){
    if(GetCurrentState()==Idle){
        NewState(AutoDrive, "Button pressed to start vision");
    }
}

void VisionStateMachine::StartSequenceTest(){
    if(GetCurrentState()==Idle){
        NewState(DriveSegment, "testing");
    }
}

bool VisionStateMachine::IsIdle(){
    return (GetCurrentState()==Idle);
}
void VisionStateMachine::Cancel(){
    isCancelled=true;
}

void VisionStateMachine::StateEngine(int curState){
    switch (curState){
        case Idle:
            if(isCancelled){
                isCancelled=false;
            }
            break;
        case AutoDrive:
            if(visionDrive->IsComplete()){
                NewState(DriveComplete, "AutoDrive complete");
            }
            if(isCancelled){
                visionDrive->Abort();
                NewState(Idle, "AutoDrive cancelled");
            }
            break;
        case DriveComplete:
            NewState(Idle, "Sequence Complete");
            break;
        case DriveSegment:
            if(driveSegment->IsComplete()){
                NewState(TurnSegment, "Drive complete");
            }
            if(isCancelled){
                driveSegment->Abort();
                NewState(Idle, "AutoDrive cancelled");
            }
            break;
        case TurnSegment:
            if(turn->IsComplete()){
                NewState(DriveComplete, "Turn complete");
            }
            if(isCancelled){
                turn->Abort();
                NewState(Idle, "AutoDrive cancelled");
            }
            break;

        /*case ScoreCargoOnCargoShip:

            break;
        case ScoreCargoOnRocketShip:

            break;
        case ScoreHatchPanel:

            break;*/
    }
}