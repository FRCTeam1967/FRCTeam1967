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

    //void StartInit(); //starts
    void RollersIn(); // brings cargo into bot
    void RollersOut(); // pushes cargo out of bot
    /*void CargoMechInRobot(); // retracts up into robot 
    void CargoMechOutRobot(); // extends down out of robot */
    void CargoMechIn(); //brings mechanism 1 phase in (0 -> 70 -> 90)
    void CargoMechOut(); //brings mechanism 1 phase out (90 -> 70 -> 0)
    void RollersStop(); //stops roll
    void CargoMechStop();
    //void CargoMechStopWithLimSwitch(); //uses lim switch value to stop mech

    bool GetCargoMechPosition(); //gets if claw is up or down (switch to a string to specify??)

	/*int GetLimSwitchOutside(); //get the value of the limit switch for when the claw goes outside the robot(true/false)
	int GetLimSwitchInside();*/ //get the value of the limit switch for when the claw goes inside the robot(true/false)

    double GetEncoderCount(); 
    float GetEncoderAngle(); 
    void SetPIDAngle(float pivotangle);
    float GetHatchPanelDistance();
    //void ButtonVals();

   //void EnablePID(); 

private:
    //double encoderCount;
    bool cargoMechExtended; 
    bool cargoMechGoingForward;
    bool cargoMechGoingBackward;
    double encoderCount;
    float encoderAngle;
    //float desiredAngle;
    double desiredAnglePulses;

    WPI_VictorSPX * motorRoll;
    WPI_TalonSRX * pivotMotor;
    //frc::DigitalInput * limSwitchInside;
    //frc::DigitalInput * limSwitchOutside;
    //frc::Encoder * pivotEncoder;
    float kPIDLoopIdx;
    float kTimeoutMs;
}; 

//bool mechExtended: true is out of robot, false is in bot