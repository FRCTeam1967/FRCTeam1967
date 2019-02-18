/*     JankyButtonPanel.cpp
*    Created on: Feb 15, 2019
*    Author: Sandhya Ganesan
*/

#include <frc/WPILib.h>
#include <frc/Joystick.h>
#include "JankyButtonPanel.h"

//include ports here

jankyButtonPanel::jankyButtonPanel(int portNumber) : Joystick (portNumber){}

jankyButtonPanel::~jankyButtonPanel(){}

bool jankyButtonPanel::GetRocketCargoMed(){
    return Joystick::GetRawButton(ROCKET_MED_CARGO_PORT);
}

bool jankyButtonPanel::GetRocketCargoLow(){
    return Joystick::GetRawButton(ROCKET_LOW_CARGO_PORT);
}

bool jankyButtonPanel::GetRocketCargoHigh(){
    return Joystick::GetRawButton(ROCKET_HIGH_CARGO_PORT);
}

bool jankyButtonPanel::GetRocketHatchLow(){ //human player station, cargo ship hatch
    return Joystick::GetRawButton(ROCKET_LOW_HATCH_PORT);
}

bool jankyButtonPanel::GetRocketHatchMed(){
    return Joystick::GetRawButton(ROCKET_MED_HATCH_PORT);
}

bool jankyButtonPanel::GetRocketHatchHigh(){
    return Joystick::GetRawButton(ROCKET_HIGH_HATCH_PORT);
}

bool jankyButtonPanel::GetShipCargo(){
    return Joystick::GetRawButton(SHIP_CARGO_PORT);
}

bool jankyButtonPanel::GetGroundHeight(){
    return Joystick::GetRawButton(GROUND_PORT);
}

float jankyButtonPanel::GetElevatorYAxis(){
	float x = (GetRawAxis(ELEVATOR_Y_AXIS_CHANNEL));
	return x;
}

bool jankyButtonPanel::GetCargoIn(){
    return Joystick::GetRawButton(CARGO_IN_PORT);
}

bool jankyButtonPanel::GetCargoOut(){
    return Joystick::GetRawButton(CARGO_OUT_PORT);
}

float jankyButtonPanel::GetRollersYAxis(){
    float y = (GetRawAxis(CARGO_ROLLERS_Y_AXIS_CHANNEL));
	return y;
}

bool jankyButtonPanel::GetBottomPistons(){
    return Joystick::GetRawButton(BOTTOM_PISTONS_PORT);
}

bool jankyButtonPanel::GetTopPistons(){
    return Joystick::GetRawButton(TOP_PISTONS_PORT);
}