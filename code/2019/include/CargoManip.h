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

    void RollersIn();
    void RollersOut(); 
    //void CargoMechIn(); //brings mechanism 1 phase in (0 -> 70 -> 90)
    //void CargoMechOut(); //brings mechanism 1 phase out (90 -> 70 -> 0)
    void RollersStop(); 
    void CargoInRobot();
    void CargoHPAngle();
    void CargoGroundAngle();
    void CargoFiveDegAngle();

    void FindEncoderCount(); 
    void FindEncoderAngle(); 
    void ResetPivotEncoder();
    void SetPIDAngle(float desiredAnglePulses);
    
    float GetHatchPanelDistance();
    bool HasElevatorGoneUp();
    void SetElevatorVar(bool state);

private:
    //double encoderCount;
    bool cargoMechGoingForward;
    bool cargoMechGoingBackward;
    double pivotEncoderCount;
    float encoderAngle;
    float desiredAngle;
    double desiredAnglePulses;
    bool elevatorHasGoneUp;

    WPI_VictorSPX * motorRoll;
    WPI_TalonSRX * pivotMotor;
    float kPIDLoopIdx;
    float kTimeoutMs;
}; 