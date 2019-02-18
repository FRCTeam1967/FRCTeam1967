/*      JankyButtonPanel.h
*    Created on: Feb 15, 2019
*    Author: Sandhya Ganesan
*/


#include <frc/WPILib.h>
#include <frc/Joystick.h>

#ifndef _JANKYBUTTONPANEL_H
#define _JANKYBUTTONPANEL_H

// include port numbers when we have them

#define ROCKET_MED_CARGO_PORT 0
#define ROCKET_LOW_CARGO_PORT 0
#define ROCKET_HIGH_CARGO_PORT 0
#define ROCKET_LOW_HATCH_PORT 0//hp station, cargo ship hatch
#define ROCKET_MED_HATCH_PORT 0
#define ROCKET_HIGH_HATCH_PORT 0
#define SHIP_CARGO_PORT 0
#define GROUND_PORT 0
#define ELEVATOR_Y_AXIS_CHANNEL 0
#define CARGO_IN_PORT 0
#define CARGO_OUT_PORT 0
#define CARGO_ROLLERS_Y_AXIS_CHANNEL 0
#define BOTTOM_PISTONS_PORT 0
#define TOP_PISTONS_PORT 0

class jankyButtonPanel : public frc::Joystick {
public:
    jankyButtonPanel(int portNumber);
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
    bool GetCargoIn();
    bool GetCargoOut();
    float GetRollersYAxis();
    bool GetBottomPistons();
    bool GetTopPistons();
};
#endif