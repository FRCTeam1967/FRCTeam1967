/*      ElevatorMech.h
*   created on: Jan 25, 2019
*    by: Sandhya, Isha, GC2 */

#include "frc/WPILib.h" 
#include "jankyTask.h"
#include "ctre/Phoenix.h"
#include <iostream> 
#include <frc/smartdashboard/SmartDashboard.h>
#include <math.h>
#include <frc/Encoder.h>
#include "hal/Constants.h"
#include <jankyXboxJoystick.h>
#include "ctre/phoenix/motorcontrol/SensorCollection.h"

using namespace std;

class ElevatorMech : public JankyTask{
public: 
        bool isMechanismRunning;

        ElevatorMech(int lMotorChannel, int rMotorChannel, int limSwitchBottomChannel, int limSwitchTopChannel);
        virtual ~ElevatorMech(); 
        virtual void Run();

        void ConditionalRun();

        void RocketLowCargoHeight(); 
        void RocketMedCargoHeight(); 
        void RocketHighCargoHeight(); 
        void RocketLowHatchHeight(); 
        void RocketMedHatchHeight(); 
        void RocketHighHatchHeight(); 
        void GroundHeight(); 
        void HPHeight();
        void ShipCargoHeight();
        void ShipHatchHeight();

        void ResetEncoder(); 
        void SmartDashboardComments(); 
        bool GetIfMechIsRunning(); 
        void StartUpInit();

        double GetEncoderCount(); 
        double GetEncoderDistance(); 

        void ElevatorMotorUp(); 
        void ElevatorMotorDown();
        void ElevatorMotorStop(); 
        //PID
        //void PIDSetup(); //only if required at the moment not sure yet

protected:
        int GetBottomLimSwitch(); 
        int GetTopLimSwitch(); 

private:
        void EmergencyStop(); 
        void PutMechanismDown(); 

        double desiredHeight;
        double amountToMove; 
        bool reachedMaxHeight;
        bool reachedMinHeight;
        bool needsToPutDownMechanism; 
        double leftEncoderCount;
        double rightEncoderCount;
        double leftEncoderDistance;
        double rightEncoderDistance;
        double avgEncoderCount;
        bool bottomLimSwitchHasNotBeenPressed;
        bool topLimSwitchHasNotBeenPressed;
        bool done;
        bool hatchPistonsIn;



        WPI_TalonSRX * lMotor;
        WPI_TalonSRX * rMotor;
        frc::DigitalInput * bottomLimSwitch;
        frc::DigitalInput * topLimSwitch;
};

