#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "jankyTask.h"

#define HATCHINTAKE_H_

using namespace frc;

class HatchIntake: public JankyTask
{
    public:
        HatchIntake(int pistonLeftChannel, int pistonRightChannel);
        virtual ~HatchIntake();

        void Go();
        virtual void Run();

    private:
        Solenoid*pistonLeft;
        Solenoid*pistonRight;
        bool actuating;
        double fullActuationTime;
        double fullCycleTime;
        Timer cycleTimer;
};