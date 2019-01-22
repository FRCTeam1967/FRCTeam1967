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

//create autodrive object

VisionStateMachine::VisionStateMachine(frc::DifferentialDrive*drive, frc::ADXRS450_Gyro*gyro, SensorCollection*leftEncoder, SensorCollection*rightEncoder, WPI_TalonSRX*leftmotor, WPI_TalonSRX*rightmotor){ //add cargo and hatch mechanisms

    SetMachineName("VisionStateMachine");
    SetName(Idle, "Default Idle");
    SetName(AutoDrive, "Drive to retroreflective tape with vision");
    SetName(DriveComplete, "Robot arrived to retroreflective tape");
    SetName(ScoreCargoOnCargoShip, "Score cargo into cargo ship");
    SetName(ScoreCargoOnRocketShip, "Score cargo into rocket ship");
    SetName(ScoreHatchPanel, "Score hatch panel on either cargo ship or rocket ship");
    SetName(CancelActions, "Cancel the current state");
    isIdle=true;

    Start();
}

VisionStateMachine::~VisionStateMachine(){

}

void VisionStateMachine::StartSequence(){
    isIdle=false;
}

void VisionStateMachine::StateEngine(int curState){
    switch (curState){
        case Idle:
            if(!isIdle){
                NewState(AutoDrive, "Button pressed to start vision");
            }
            break;
        case AutoDrive:
            // if autodrive object is complete, then switch state to drive complete
            break;
        case DriveComplete:
            //go back to idle
            break;
        //MIGHT NOT BE NEEDED BC WE ARE GOING TO HAVE BUTTONS FOR OPERATOR TO PRESS TO RAISE THE ELEVATOR
        /*case ScoreCargoOnCargoShip:

            break;
        case ScoreCargoOnRocketShip:

            break;
        case ScoreHatchPanel:

            break;*/
        case CancelActions:

            break;
    }
}