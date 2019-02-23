#include "Settings.h"
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "jankyTask.h"

#define HATCHINTAKE_H_

using namespace frc;

class HatchIntake: public JankyTask
{
    public:
        HatchIntake(int pistonTopChannel, int pistonBottomChannel);
        virtual ~HatchIntake();

        void Go();
        virtual void Run();
        void BottomPistonsSwitch();
        void BottomPistonsOut();
        void BottomPistonsIn();

        bool GetPistonStatus();

    private:
        Solenoid*pistonTop;
        Solenoid*pistonBottom;
        bool actuating;
        bool hatchPistonsIn;
        double fullActuationTime;
        double fullCycleTime;
        int i;
        Timer cycleTimer;
};