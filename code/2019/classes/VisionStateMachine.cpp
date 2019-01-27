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
float vision_kP = 0.05;
float vision_kI = 0.0;
float vision_kD = 0.0;

float drive_kP = 0.04;
float drive_kI = 0.0;
float drive_kD = 0.0;

float turn_kP = 0.05;
float turn_kI = 0.0;
float turn_kD = 0.06;

AutoDrive*visionDrive;

VisionStateMachine::VisionStateMachine(frc::DifferentialDrive*drive, frc::ADXRS450_Gyro*gyro, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*leftmotor, WPI_TalonSRX*rightmotor){ //add cargo and hatch mechanisms
    visionDrive= new ::AutoDrive(drive, VISION_DRIVE_SPEED, vision_kP, vision_kI, vision_kD); // added :: to resolve "expected type specifier" error

    SetMachineName("VisionStateMachine");
    SetName(Idle, "Default Idle");
    SetName(AutoDrive, "Drive to tape w/ vision");
    SetName(DriveComplete, "Robot arrived to tape");
    SetName(ScoreCargoOnCargoShip, "Score cargo into cargo ship");
    SetName(ScoreCargoOnRocketShip, "Score cargo into rocket ship");
    SetName(ScoreHatchPanel, "Score hatch panel");
    SetName(CancelActions, "Cancel the current state");

    isIdle=true;
    isCancelled=false;
    testMode= false;
    regMode= false;

    Start();
}

VisionStateMachine::~VisionStateMachine(){
    Terminate();
    delete visionDrive;
}

void VisionStateMachine::StartSequence(){
    isIdle=false;
}

void VisionStateMachine::Cancel(){
    isCancelled=true;
}

void VisionStateMachine::StateEngine(int curState){
    switch (curState){
        case Idle:
            if(!isIdle){
                NewState(AutoDrive, "Button pressed to start vision");
            }
            else if(isCancelled){
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
            isIdle=true;
            regMode=false;
            testMode=false;
            NewState(Idle, "Sequence Complete");
            break;
        


        //THESE CASES WON'T BE NEEDED BC WE ARE GOING TO HAVE BUTTONS FOR OPERATOR TO PRESS TO RAISE THE ELEVATOR
        /*case ScoreCargoOnCargoShip:

            break;
        case ScoreCargoOnRocketShip:

            break;
        case ScoreHatchPanel:

            break;*/
    }
}