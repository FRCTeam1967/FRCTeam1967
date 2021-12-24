#ifndef _JANKYSTATEMACHINEEXAMPLE_H
#define _JANKYSTATEMACHINEEXAMPLE_H

#include <iostream>
#include "jankyStateMachine.h"

#include "adi/ADIS16470_IMU.h" //gyro
#include "frc/Timer.h" //timer
#include <frc/DigitalInput.h> //limit switch
#include "ctre/Phoenix.h" //talons
#include <frc/drive/DifferentialDrive.h> //drive
#include <wpi/math> //for pi

#define STARTUPDELAY 3 //3 seconds

//for encoders:
#define WHEEL_CIRCUMFERENCE_INCHES 6 * wpi::math::pi
#define PULSES_PER_REVOLUTION 4096

using namespace std;
using namespace frc; //do I need to include?

//VisionSubsystem class - fake class that doesn't actually use vision. This class is just used to get "vision input" for JankyStateMachineExample
class VisionSubsystem {
public:
    VisionSubsystem(){ return; };
    ~VisionSubsystem(){ return; };

    double GetVisionOffset(){
        return 0.0;
    }
};

//JankyStateMachineExample class
class JankyStateMachineExample : public JankyStateMachine {
  
public:
    JankyStateMachineExample(DifferentialDrive * _pDrive, WPI_TalonSRX * _pFLMotor, WPI_TalonSRX * _pFRMotor, frc::Timer * _pTimer, frc::ADIS16470_IMU * _pGyro, DigitalInput * _pSwitch, VisionSubsystem * _pVision);
    ~JankyStateMachineExample();

    void StateEngine(int curState, bool onStateEntered);
    enum AutoStateValues {
        Idle,
        InitialDelay,
        Forward2F,
        Right90D,
        Backward1F,
        Pause2S,
        Left45D,
        Right45D, 
		Forward1F,
        DoNothing,
	};
    void GoAutonomous();

    //functions for sensors
    void ResetEncoders();
    double GetEncoderAverageDistance(); //return in feet
    void ResetGyro();
    double GetHeading();
    double GetVisionOffset();
    bool GetLimitSwitch();
    void ResetTimer();
    double GetTimer();

private:
    //chassis:
    DifferentialDrive * pDrive;

    //sensors:
    WPI_TalonSRX * pFLMotor;
    WPI_TalonSRX * pFRMotor;
    frc::Timer * pTimer;
    frc::ADIS16470_IMU * pGyro; 
    DigitalInput * pSwitch;
    VisionSubsystem * pVision;
};


#endif
