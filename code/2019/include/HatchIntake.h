#include "frc/WPILib.h"
#include "ctre/Phoenix.h"

#define HATCHINTAKE_H_

using namespace frc;

class HatchIntake
{
    public:
        HatchIntake(int pistonLeftChannel, int pistonRightChannel);
        virtual ~HatchIntake();

        void PickUp(); //picks up hatch panel

    private:
        Solenoid*pistonLeft;
        Solenoid*pistonRight;
};