/*      JankyButtonPanel.h
*    Created on: Feb 15, 2019
*    Author: Sandhya Ganesan
*/


#include <frc/WPILib.h>
#include <frc/Joystick.h>

#ifndef _JANKYBUTTONPANEL_H
#define _JANKYBUTTONPANEL_H

// include port numbers when we have them

#define ROCKET_MED_CARGO_PORT 4
#define ROCKET_LOW_CARGO_PORT 6
#define ROCKET_HIGH_CARGO_PORT 2
#define ROCKET_LOW_HATCH_PORT 6//hp station, cargo ship hatch
#define ROCKET_MED_HATCH_PORT 8
#define ROCKET_HIGH_HATCH_PORT 10
#define SHIP_CARGO_PORT 8
#define GROUND_PORT 12
#define ELEVATOR_Y_AXIS_CHANNEL 0
#define CARGO_IN_PORT 10
#define CARGO_OUT_PORT 12
#define CARGO_ROLLERS_Y_AXIS_CHANNEL 0
#define BOTTOM_PISTONS_PORT 4
#define TOP_PISTONS_PORT 2

class jankyButtonPanel {
public:
    jankyButtonPanel(int joystickPortA, int joystickPortB, int elevatorBottomLS, int elevatorTopLS, int cargoBottomLS, int cargoTopLS);
	virtual ~jankyButtonPanel(void);

    bool GetRocketCargoLow();
    bool GetRocketCargoMed();
    bool GetRocketCargoHigh();
    bool GetRocketHatchLow(); //human player station, cargo ship hatch
    bool GetRocketHatchMed();
    bool GetRocketHatchHigh();
    bool GetShipCargo();
    bool GetGroundHeight();
    float GetElevatorYAxis();
    //bool GetElevatorLimSwitches();
    bool GetCargoIn();
    bool GetCargoOut();
    float GetRollersYAxis();
    //bool GetCargoLimSwitches();
    bool GetBottomPistons();
    bool GetTopPistons();
    float x;
    float y;

private:
    frc::Joystick * joystick1;
    frc::Joystick * joystick2;
    frc::DigitalInput * elevatorBottomLimSwitch;
    frc::DigitalInput * elevatorTopLimSwitch;
    frc::DigitalInput * cargoBottomLimSwitch;
    frc::DigitalInput * cargoTopLimSwitch;
    //frc::DigitalInput * cargoLeftLimSwitch;
    //frc::DigitalInput * cargoRightLimSwitch;
};
#endif