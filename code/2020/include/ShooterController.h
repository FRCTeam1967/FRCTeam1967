#include "Settings.h"
#include "ctre/Phoenix.h"
#include <iostream>
#include "Flywheel.h"
#include "IntakeMech.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include "jankyTask.h"

#define SHOOTERCONTROLLERMECH_H_

class ShooterControllerInfiniteRecharge : public JankyTask {
    public:
        ShooterControllerInfiniteRecharge(int conveyorBeltMotorChannel, int motorRollChannel,int leftPistonChannel,int rightPistonChannel, int flywheelChannelNumber);
        virtual ~ShooterControllerInfiniteRecharge();

        virtual void Run();

        void Target();
        int GetEncoderCount();
        void ResetEncoderCount();
        void GetOneBall();
        void OuttakeOneBall();
        void SetDesiredCount(int count);
        void StopConveyorBelt();
        double GetRunningRPM();
        double GetDesiredRPM();
    private:
        double currentEncoderCount; 
        double desiredEncoderCount;
        bool run;
        WPI_TalonSRX * conveyorBeltMotor;
        FlywheelMechInfiniteRecharge * flywheelmech;
        IntakeMech * intakemech;
};