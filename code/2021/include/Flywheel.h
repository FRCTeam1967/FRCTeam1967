#include <iostream>
#include "Settings.h"
#include "ctre/Phoenix.h"
#include <frc/smartdashboard/SmartDashboard.h>

class FlywheelMechInfiniteRecharge{
    public:
        FlywheelMechInfiniteRecharge(int flywheelChannelNumber);
        virtual ~FlywheelMechInfiniteRecharge();
        void SetRPM();
        void SetRPMAuto();
        double ReturnDesiredRPM();
        double GetRunningRPM();
        void CalculateDistanceToVisionTarget();
        double GetDistanceToVisionTarget();
        void StopFlywheel();
        void SetSelectorVisionDistance(int selectorDistance); 

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