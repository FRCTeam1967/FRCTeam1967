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

    //void StartInit(); //starts
    void RollersIn(); // brings cargo into bot
    void RollersOut(); // pushes cargo out of bot
    void CargoMechIn(); //brings mechanism 1 phase in (0 -> 70 -> 90)
    void CargoMechOut(); //brings mechanism 1 phase out (90 -> 70 -> 0)
    void RollersStop(); //stops roll
    //void CargoMechStop();

    bool GetCargoMechPosition(); //gets if claw is up or down (switch to a string to specify??)

    void FindEncoderCount(); 
    void FindEncoderAngle(); 
    void SetPIDAngle(float pivotangle);
    float GetHatchPanelDistance();

private:
    //double encoderCount;
    bool cargoMechGoingForward;
    bool cargoMechGoingBackward;
    double encoderCount;
    float encoderAngle;
    float desiredAngle;
    double desiredAnglePulses;

    WPI_VictorSPX * motorRoll;
    WPI_TalonSRX * pivotMotor;
    //frc::Encoder * pivotEncoder;
    float kPIDLoopIdx;
    float kTimeoutMs;
}; 

//bool mechExtended: true is out of robot, false is in bot