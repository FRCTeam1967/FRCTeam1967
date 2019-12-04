#include "Settings.h"
#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "HatchIntake.h"
#include <frc/Solenoid.h>
#include "jankyTask.h"

using namespace frc;

HatchIntake::HatchIntake(int pistonTopChannel, int pistonBottomChannel)
{
    //Unused
    /*actuating = false;
    fullActuationTime = 1;
    fullCycleTime = 2;
    i = 0;
    cycleTimer.Reset();*/

    pistonTop = new Solenoid(10, pistonTopChannel);
    pistonBottom = new Solenoid(10, pistonBottomChannel);
}

HatchIntake::~HatchIntake()
{
    delete pistonTop;
    delete pistonBottom;
}

// Bottom Pistons
void HatchIntake::BottomPistonSwitch()
{
    if (pistonBottom->Get()==true)
    {
        BottomPistonIn();
    }
    else if (pistonBottom->Get()==false)
    {
        BottomPistonOut();
    }
}

void HatchIntake::BottomPistonOut()
{
    pistonBottom->Set(true);
}

void HatchIntake::BottomPistonIn()
{
    pistonBottom->Set(false);
}

bool HatchIntake::GetBottomPistonStatus()
{
    return(pistonBottom->Get());
}

// Top Pistons
void HatchIntake::TopPistonSwitch()
{
    if (GetTopPistonStatus()==true)
    {
        TopPistonIn();
    }
    else if (GetTopPistonStatus()==false)
    {
        TopPistonOut();
    }
}

void HatchIntake::TopPistonOut()
{
    pistonTop->Set(true);
}

void HatchIntake::TopPistonIn()
{
    pistonTop->Set(false);
}

bool HatchIntake::GetTopPistonStatus()
{
    return(pistonTop->Get());
}

//Unused
/*void HatchIntake::Go()
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
}*/