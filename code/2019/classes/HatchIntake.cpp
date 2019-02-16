#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "HatchIntake.h"
#include <frc/Solenoid.h>
#include "jankyTask.h"

using namespace frc;

HatchIntake::HatchIntake(int pistonTopChannel, int pistonBottomChannel)
{
    actuating = false;
    fullActuationTime = 1;
    fullCycleTime = 2;
    i = 0;
    cycleTimer.Reset();

    pistonTop = new Solenoid(10, pistonTopChannel);
    pistonBottom = new Solenoid(10, pistonBottomChannel);
}

HatchIntake::~HatchIntake()
{
    delete pistonTop;
    delete pistonBottom;
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
            pistonTop->Set(true);
        }

        else
        {
            pistonTop->Set(false);
            cycleTimer.Stop();
            cycleTimer.Reset();
            actuating = false;
        }
    }
}

void HatchIntake::BottomPistonsOut()
{
    if (pistonBottom->Get()==true)
        pistonBottom->Set(false);

    else if (pistonBottom->Get()==false)
        pistonBottom->Set(true);
}

bool HatchIntake::GetPistonStatus()
{
    if (pistonBottom->Get()==true)
        hatchPistonsIn = true;

    else if (pistonBottom->Get()==false)
        hatchPistonsIn = false;
}