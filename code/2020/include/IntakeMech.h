#include "Settings.h"
#include "frc/Solenoid.h"
#include "ctre/Phoenix.h"
#include <iostream>
#include <jankyXboxJoystick.h>

#define INTAKEMECH_H_

using namespace frc;
using namespace ctre;

class IntakeMech{
public:
    IntakeMech(int motorRollChannel, int leftPistonChannel, int rightPistonChannel);
    virtual ~IntakeMech();
    bool isMechRunning;
    void RollersIn();
    void RollersOut();
    void RollersStop();
    void MechInRobot();
    void MechOutRobot();
private:
    WPI_VictorSPX * motorRoll;
    Solenoid* leftPiston;
    Solenoid * rightPiston;
};