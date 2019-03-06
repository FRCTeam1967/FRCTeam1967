#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include "LightsController.h"
#include "LightsKey.h"

using namespace frc;

LightsController::LightsController(int pwmChannel)
{
    lightController = new PWM(pwmChannel);
}

LightsController::~LightsController()
{
    delete lightController;
}

void LightsController::SetColor(int color, int mode)
{
    if (mode==3)
        lightController -> SetRaw(21);
    else
        lightController -> SetRaw((color*200)+(mode*100));
}