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

    pistonTopLeft = new Solenoid(10, pistonTopChannel);
    pistonTopRight = new Solenoid(10, pistonTopChannel);
    pistonBottomLeft = new Solenoid(10, pistonBottomChannel);
    pistonBottomRight = new Solenoid(10, pistonBottomChannel);
}

HatchIntake::~HatchIntake()
{
    delete pistonTopLeft;
    delete pistonTopRight;
    delete pistonBottomLeft;
    delete pistonBottomRight;
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
            pistonTopLeft->Set(true);
            pistonTopRight->Set(true); 
        }

        else
        {
            pistonTopLeft->Set(false);
            pistonTopRight->Set(false);
            cycleTimer.Stop();
            cycleTimer.Reset();
            actuating = false;
        }
    }
}

void HatchIntake::BottomPistonsOut()
{
    if (pistonBottomLeft->Get()==true && pistonBottomRight->Get()==true)
    {
        pistonBottomLeft->Set(false);
        pistonBottomRight->Set(false);
    }

    else if (pistonBottomLeft->Get()==false && pistonBottomRight->Get()==false)
    {
        pistonBottomLeft->Set(true);
        pistonBottomRight->Set(true);
    }
}