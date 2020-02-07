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

class ClimbingMech : public JankyTask{
public: 
        bool isMechanismRunning;

        ClimbingMech(int lMotorChannel, int rMotorChannel, int leftSolenoidChannel, int rightSolenoidChannel);
        virtual ~ClimbingMech(); 
        virtual void Run();

        void HighestHeight();
        void LevelHeight(); 
        void GroundHeight(); 

        void ResetEncoder(); 
        void SmartDashboardComments(); 
        bool GetIfMechIsRunning(); 
        void StartUpInit();

        void GetEncoderCount(); 
        void GetEncoderDistance();
        void CalculateDesiredHeight();

        void ClimbingMotorUp(); 
        void ClimbingMotorDown();
        void ClimbingMotorStop(); 

        void EnablePID();

protected:
        bool GetleftSolenoid();
        bool GetrightSolenoid();

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
        bool rightSolenoidOff;
        bool leftSolenoidOff;
        string setHeight;
        string controlMode;

        float kPIDLoopIdx;
        float kTimeoutMs;

        WPI_TalonSRX * lmotor;
        WPI_TalonSRX * rmotor;
};