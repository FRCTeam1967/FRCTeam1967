/*      ElevatorMech.h
*   created on: Jan 25, 2019
*    by: Sandhya, Isha, GC2 */


#include "Settings.h"
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

        void BallHeight();
        void RocketLowCargoHeight(); 
        void RocketMedCargoHeight(); 
        void RocketHighCargoHeight(); 
        void RocketLowHatchHeight(); 
        void RocketMedHatchHeight(); 
        void RocketHighHatchHeight(); 
        void GroundHeight(); 
        void HPHeight();
        void UpHP();
        void DownHatch();
        void ShipCargoHeight();
        void ShipHatchHeight();

        void ResetEncoder(); 
        void SmartDashboardComments(); 
        bool GetIfMechIsRunning(); 
        void StartUpInit();

        void GetEncoderCount(); 
        void GetEncoderDistance();
        void CalculateDesiredHeight();

        void ElevatorMotorUp(); 
        void ElevatorMotorDown();
        void ElevatorMotorStop(); 

        void EnablePID();


protected:
        bool GetBottomLimSwitch(); 
        bool GetTopLimSwitch(); 

private:
        void EmergencyStop(); 
        void PutMechanismDown(); 

        double desiredHeight;
        int desiredHeightPulses;
        double amountToMove; 
        int leftEncoderCount;
        int rightEncoderCount;
        double leftEncoderDistance;
        double rightEncoderDistance;
        double avgEncoderDistance;
        bool bottomLimSwitchHasNotBeenPressed;
        bool topLimSwitchHasNotBeenPressed;
        string setHeight;
        string controlMode;

        float kPIDLoopIdx;
        float kTimeoutMs;

        WPI_TalonSRX * lmotor;
        WPI_TalonSRX * rmotor;
};