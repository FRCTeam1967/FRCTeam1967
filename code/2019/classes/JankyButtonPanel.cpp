/*     JankyButtonPanel.cpp
*    Created on: Feb 15, 2019
*    Author: Sandhya Ganesan
*/

#include <frc/WPILib.h>
#include <frc/Joystick.h>
#include "JankyButtonPanel.h"

//include ports here

jankyButtonPanel::jankyButtonPanel(int joystickPortA, int joystickPortB, int elevatorBottomLS, int elevatorTopLS, int cargoBottomLS, int cargoTopLS){
    joystick1 = new frc::Joystick(joystickPortA);
    joystick2 = new frc::Joystick(joystickPortB);
    elevatorBottomLimSwitch = new frc::DigitalInput(elevatorBottomLS);
    elevatorTopLimSwitch = new frc::DigitalInput(elevatorTopLS);
    cargoBottomLimSwitch = new frc::DigitalInput(cargoBottomLS);
    cargoTopLimSwitch = new frc::DigitalInput(cargoBottomLS);
    //cargoLeftLimSwitch = new frc::DigitalInput(cargoLeftLS);
    //cargoRightLimSwitch = new frc::DigitalInput(cargoRightLS);
    x=0;
    y=0;
}

jankyButtonPanel::~jankyButtonPanel(){
    delete joystick1;
    delete joystick2;
    delete elevatorBottomLimSwitch;
    delete elevatorTopLimSwitch;
    delete cargoBottomLimSwitch;
    delete cargoTopLimSwitch;
    //delete cargoLeftLimSwitch;
    //delete cargoRightLimSwitch;
}

bool jankyButtonPanel::GetRocketCargoMed(){
    return joystick1 -> GetRawButton(ROCKET_MED_CARGO_PORT);
}

bool jankyButtonPanel::GetRocketCargoLow(){
    return joystick1 -> GetRawButton(ROCKET_LOW_CARGO_PORT);
}

bool jankyButtonPanel::GetRocketCargoHigh(){
    return joystick1 -> GetRawButton(ROCKET_HIGH_CARGO_PORT);
}

bool jankyButtonPanel::GetRocketHatchLow(){ //human player station, cargo ship hatch
    return joystick2 -> GetRawButton(ROCKET_LOW_HATCH_PORT);
}

bool jankyButtonPanel::GetRocketHatchMed(){
    return joystick2 -> GetRawButton(ROCKET_MED_HATCH_PORT);
}

bool jankyButtonPanel::GetRocketHatchHigh(){
    return joystick2 -> GetRawButton(ROCKET_HIGH_HATCH_PORT);
}

bool jankyButtonPanel::GetShipCargo(){
    return joystick1 -> GetRawButton(SHIP_CARGO_PORT);
}

bool jankyButtonPanel::GetGroundHeight(){
    return joystick2 -> GetRawButton(GROUND_PORT);
}

float jankyButtonPanel::GetElevatorYAxis(){
    if (elevatorBottomLimSwitch -> Get()){
        x = -(elevatorBottomLimSwitch -> Get());
    }
    else if (elevatorTopLimSwitch -> Get()){
        x = elevatorTopLimSwitch -> Get();
    }
    else {
        x = 0;
    }
    return x;
}

bool jankyButtonPanel::GetCargoIn(){
    return joystick1 -> GetRawButton(CARGO_IN_PORT);
}

bool jankyButtonPanel::GetCargoOut(){
    return joystick1 -> GetRawButton(CARGO_OUT_PORT);
}

float jankyButtonPanel::GetRollersYAxis(){
    if (cargoTopLimSwitch -> Get()){
        y = -(cargoTopLimSwitch -> Get());
    }
    else if (cargoTopLimSwitch -> Get()){
        y = cargoTopLimSwitch -> Get();
    }
    else {
        y = 0;
    }
    return y;
}

bool jankyButtonPanel::GetBottomPistons(){
    return joystick2 -> GetRawButton(BOTTOM_PISTONS_PORT);
}

bool jankyButtonPanel::GetTopPistons(){
    return joystick2 -> GetRawButton(TOP_PISTONS_PORT);
}