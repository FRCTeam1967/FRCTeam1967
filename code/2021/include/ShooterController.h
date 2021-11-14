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
        ShooterControllerInfiniteRecharge(int conveyorBeltMotorChannel, int motorRollChannel,int leftPistonChannel,int rightPistonChannel, int flywheelChannelNumber, int bridgeChannel, int turretChannel);
        virtual ~ShooterControllerInfiniteRecharge();

        virtual void Run();

        // FLYWHEEL
        void Target();
        double GetRunningRPM();
        double GetDesiredRPM();
        void StopTarget();
        void SetRPMAuto();
        void SetSelectorVisionDistance(int selectorZone); 
        void FlywheelOut();

        // CONVEYOR BELT
        int GetEncoderCount();
        void ResetEncoderCount();
        void GetOneBall();
        void OuttakeOneBall();
        void SetDesiredCount(int count);
        void StopConveyorBelt();
        void StartConveyorBelt();
        void ConveyorBeltOut();
        
        // INTAKE
        void IntakeStop();
        void IntakeIn();
        void IntakeOut();
        void IntakePistonsDown();
        void IntakePistonsUp();

        // BRIDGE
        void BridgeForward();
        void StopBridge();
        void BridgeBackward();

        // TURRET
        void TurretLeft(float turretSpeed);
        void TurretRight(float turretSpeed);
        void StopTurret();

    private:
        double currentEncoderCount; 
        double desiredEncoderCount;
        bool run;
        bool manualSConveyor;
        float offsetFromVisionTarget;

        WPI_TalonSRX * conveyorBeltMotor;
        FlywheelMechInfiniteRecharge * flywheelmech;
        IntakeMech * intakemech;
        WPI_VictorSPX * bridgeMotor;
        WPI_TalonSRX * turretMotor; 
};