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
        void BottomPistonsOut();

    private:
        Solenoid*pistonTopLeft;
        Solenoid*pistonTopRight;
        Solenoid*pistonBottomLeft;
        Solenoid*pistonBottomRight;
        bool actuating;
        double fullActuationTime;
        double fullCycleTime;
        int i;
        Timer cycleTimer;
};