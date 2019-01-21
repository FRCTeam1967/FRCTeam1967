#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "HatchIntake.h"
#include <frc/Solenoid.h>

using namespace frc;

HatchIntake::HatchIntake(int pistonLeftChannel, int pistonRightChannel)
{
    pistonLeft = new Solenoid(10, pistonLeftChannel);
    pistonRight = new Solenoid(10, pistonRightChannel);
}

HatchIntake::~HatchIntake()
{
    delete pistonLeft;
    delete pistonRight;
}

void HatchIntake::PickUp()
{
    pistonLeft->Set(true);
    pistonRight->Set(true);
    Wait(1);
    pistonLeft->Set(false);
    pistonRight->Set(false);
}