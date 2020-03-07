#include <iostream>
#include "Settings.h"
#include "ctre/Phoenix.h"
#include <frc/smartdashboard/SmartDashboard.h>

class FlywheelMechInfiniteRecharge{
    public:
        FlywheelMechInfiniteRecharge(int flywheelChannelNumber);
        virtual ~FlywheelMechInfiniteRecharge();
        void SetRPM();
        double ReturnDesiredRPM();
        double GetRunningRPM();
        void CalculateDistanceToVisionTarget();
        double GetDistanceToVisionTarget();

    private:
        TalonFX * flywheelMotor; 
        int kSlotIdx;
        int kPIDLoopIdx; 
        int kTimeoutMs; 
        double desiredRPM;
        double runningRPM; 
        double targetVelocity_UnitsPer100ms;
        double distanceToVisionTarget;
};