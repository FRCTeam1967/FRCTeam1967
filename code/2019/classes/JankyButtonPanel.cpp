/*     JankyButtonPanel.cpp
*    Created on: Feb 15, 2019
*    Author: Sandhya Ganesan
*/
#include "Settings.h"
#include <frc/WPILib.h>
#include <frc/Joystick.h>
#include "JankyButtonPanel.h"

#define ELEVATOR_X_AXIS_CHANNEL 1 // before 1
#define ELEVATOR_Y_AXIS_CHANNEL 0 // before 2
#define ROLLER_X_AXIS_CHANNEL 1
#define ROLLER_Y_AXIS_CHANNEL 0

//include ports here

jankyButtonPanel::jankyButtonPanel(int joystickPortA, int joystickPortB){
    joystick1 = new frc::Joystick(joystickPortA); // cargo side joystick
    joystick2 = new frc::Joystick(joystickPortB); // hatch side joystick
    x=0;
    y=0;
}

jankyButtonPanel::~jankyButtonPanel(){
    delete joystick1;
    delete joystick2;
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
    float x = joystick2->GetRawAxis(ELEVATOR_Y_AXIS_CHANNEL);
	return x;
}

bool jankyButtonPanel::GetCargoIn(){
    return joystick1 -> GetRawButton(CARGO_IN_PORT);
}

bool jankyButtonPanel::GetCargoOut(){
    return joystick1 -> GetRawButton(CARGO_OUT_PORT);
}

float jankyButtonPanel::GetRollersYAxis(){
    float y = -joystick1->GetRawAxis(ROLLER_Y_AXIS_CHANNEL);
	return y;    
}

bool jankyButtonPanel::GetBottomPistons(){
    return joystick2 -> GetRawButton(BOTTOM_PISTONS_PORT);
}

bool jankyButtonPanel::GetTopPistons(){
    return joystick2 -> GetRawButton(TOP_PISTONS_PORT);
}