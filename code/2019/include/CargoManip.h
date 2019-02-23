/*       CargoManip.h
*   created on: Jan 19, 2019
*    by: Sandhya, Isha, GC2 
*  logic creds to Maddie (2018)  
*/
#include "Settings.h"
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include <iostream>
#include <frc/Encoder.h>
#include <jankyXboxJoystick.h>

#define CARGOMANIP_H_

class CargoManip {
public:
    CargoManip(int motorRollChannel, int motorPivotChannel);
    virtual ~CargoManip();
    bool isMechRunning;
    //virtual void Run();

    void StartInit(); //starts
    void RollersIn(); // brings cargo into bot
    void RollersOut(); // pushes cargo out of bot
    void CargoMechInRobot(); // retracts up into robot 
    void CargoMechOutRobot(); // extends down out of robot 
    void RollersStop(); //stops roll
    void CargoMechStop();
    //void CargoMechStopWithLimSwitch(); //uses lim switch value to stop mech

    bool getCargoMechPosition(); //gets if claw is up or down (switch to a string to specify??)

	/*int GetLimSwitchOutside(); //get the value of the limit switch for when the claw goes outside the robot(true/false)
	int GetLimSwitchInside();*/ //get the value of the limit switch for when the claw goes inside the robot(true/false)

    double getEncoderCount(); 
    float GetEncoderAngle(); //returns angle movement - type uncertain??
    float GetHatchPanelDistance();
    //void ButtonVals();

   // void PIDSetUp(); //for PID? try for elevator!

private:
    //double encoderCount;
    bool cargoMechExtended; 
    bool cargoMechGoingForward;
    bool cargoMechGoingBackward;
    double encoderCount;
    float encoderAngle;

    WPI_VictorSPX * motorRoll;
    WPI_TalonSRX * pivotMotor;
    //frc::DigitalInput * limSwitchInside;
    //frc::DigitalInput * limSwitchOutside;
    frc::Encoder * pivotEncoder;

}; 

//bool mechExtended: true is out of robot, false is in bot