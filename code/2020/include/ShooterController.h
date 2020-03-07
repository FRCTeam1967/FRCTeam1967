#include "Settings.h"
#include "ctre/Phoenix.h"
#include <iostream>
#include "Flywheel.h"
#include "IntakeMech.h"
#include <frc/smartdashboard/SmartDashboard.h>

#define SHOOTERCONTROLLERMECH_H_

class ShooterControllerInfiniteRecharge{
    public:
        ShooterControllerInfiniteRecharge(int conveyorBeltMotorChannel, int motorRollChannel,int leftPistonChannel,int rightPistonChannel, int flywheelChannelNumber);
        virtual ~ShooterControllerInfiniteRecharge();
        void Target();
        void GetEncoderCount();
        void ResetEncoderCount();
        void GetOneBall();
        void OuttakeOneBall();
        void SetDesiredCount(double desiredCount);
        void StopConveyorBelt();
        double GetRunningRPM();
        double GetDesiredRPM();
    private:
        double currentEncoderCount; 
        double desiredEncoderCount;
        WPI_TalonSRX * conveyorBeltMotor;
        FlywheelMechInfiniteRecharge * flywheelmech;
        IntakeMech * intakemech;
};