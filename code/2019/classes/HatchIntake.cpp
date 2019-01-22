#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "HatchIntake.h"
#include <frc/Solenoid.h>
#include "jankyTask.h"

using namespace frc;

HatchIntake::HatchIntake(int pistonLeftChannel, int pistonRightChannel)
{
    actuating = false;
    fullActuationTime = 1;
    fullCycleTime = 2;
    cycleTimer.Reset();

    pistonLeft = new frc::Solenoid(10, pistonLeftChannel);
    pistonRight = new frc::Solenoid(10, pistonRightChannel);
}

HatchIntake::~HatchIntake()
{
    delete pistonLeft;
    delete pistonRight;
}

void HatchIntake::Go()
{
    if (actuating == false)
    {
        actuating = true;

        cycleTimer.Reset();
        cycleTimer.Start();
    }
}

void HatchIntake::Run()
{
   if (actuating == true)
    {
        if (cycleTimer.Get() < fullActuationTime)
        {
            pistonLeft->Set(true);
            pistonRight->Set(true);
        }

        else
        {
            pistonLeft->Set(false);
            pistonRight->Set(false);
            cycleTimer.Stop();
            cycleTimer.Reset();
            actuating = false;
        }
    }
}