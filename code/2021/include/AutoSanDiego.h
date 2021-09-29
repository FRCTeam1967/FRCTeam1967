#include <iostream>
#include "Settings.h"
#include <frc/drive/DifferentialDrive.h>
#include "ShooterController.h"
#include "frc/SpeedControllerGroup.h"
#include "ctre/Phoenix.h"
#include "frc/Timer.h"
//for selector:
#include <vector>
#include "JankySelector.h"

using namespace frc;
using namespace ctre;

//auto delay selector
class AutoSDDelaySelector : public JankySelector{
    public:
        std::string selectorName = "AutoSDDelaySelector";
        std::vector<std::string> optionsList 
            {"0_sec", "1_sec", "2_sec", "3_sec", "4_sec", "5_sec", "6_sec"};
        enum class Options {zeroSec, oneSec, twoSec, threeSec, fourSec, fiveSec, sixSec};

        AutoSDDelaySelector();
        ~AutoSDDelaySelector();
        Options GetSelection();
};

class AutoIntakeSelector : public JankySelector{
    public:
        std::string selectorName = "IntakeDownAuto";
        std::vector<std::string> optionsList 
            {"Intake Up", "Intake Down"};
        enum class Options {intakeUp, intakeDown};

        AutoIntakeSelector();
        ~AutoIntakeSelector();
        Options GetSelection();
};


//main auto code
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
        void StartDelayTimer();
        void EndDelayTimer();

        void DriveStraight(int distancePulses, int forwardOrBack);
        void CenterTurretAutomatically();
        void MoveTurretStart();

        void setInitialDelayTime(AutoSDDelaySelector::Options delay);
        void setIntakeUpDown(AutoIntakeSelector::Options upDown);

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
        Timer * drivingTimer;
        Timer * delayTimer;

        float offsetFromVisionTarget;
        bool isFinishedDriving;
        int caseNum;
        int initialDelay;
        bool intakeUp;
};
