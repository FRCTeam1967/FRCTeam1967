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
        void StartConveyorBelt();
        double GetRunningRPM();
        double GetDesiredRPM();
        void IntakeStop();
        void IntakeIn();
        void IntakeOut();
        void StopTarget();
        void IntakePistonsDown();
        void IntakePistonsUp();
    private:
        double currentEncoderCount; 
        double desiredEncoderCount;
        bool run;
        bool manualSConveyor;
        WPI_TalonSRX * conveyorBeltMotor;
        FlywheelMechInfiniteRecharge * flywheelmech;
        IntakeMech * intakemech;
};