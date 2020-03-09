#include <iostream>
#include "Settings.h"
#include <frc/drive/DifferentialDrive.h>
#include "ShooterController.h"
#include "frc/SpeedControllerGroup.h"
#include "ctre/Phoenix.h"
#include "frc/Timer.h"

using namespace frc;
using namespace ctre;

class AutoSanDiego {
    public:
        AutoSanDiego(WPI_VictorSPX*flmotor, WPI_TalonSRX*frmotor, WPI_TalonSRX*rlmotor, WPI_VictorSPX*rrmotor, ShooterControllerInfiniteRecharge * shootercontroller);
        ~AutoSanDiego();

        void Initialize();

        void ResetEncoders();
        int GetRightEncoder();
        int GetLeftEncoder();

        void StartShootingTimer();
        void EndShootingTimer();
        void StartFlywheelTimer();
        void EndFlywheelTimer();

        void DriveStraight(int distancePulses, int forwardOrBack);
        void CenterTurretAutomatically();
        void MoveTurretStart();

        void RunAuto();
        
    private:
        ShooterControllerInfiniteRecharge * shootercontroller;
        WPI_VictorSPX * flmotor;
        WPI_TalonSRX * frmotor;
        WPI_TalonSRX * rlmotor;
        WPI_VictorSPX * rrmotor;
        SpeedControllerGroup * leftDrive;
        SpeedControllerGroup * rightDrive;
        DifferentialDrive*drive;
        Timer * shootingTimer;
        Timer * flywheelTimer;

        float offsetFromVisionTarget;
        bool isFinishedDriving;
        int caseNum;
};