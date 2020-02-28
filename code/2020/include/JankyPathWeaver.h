#include <iostream>
#include <frc/trajectory/TrajectoryGenerator.h>
#include "frc2/command/RamseteCommand.h"

#ifndef JANKY_PATHWEAVER_H
#define JANKY_PATHWEAVER_H

class JankyPathWeaver
{
    public:
        JankyPathWeaver();
        virtual ~JankyPathWeaver();

        frc::Trajectory GeneratePath(int autoMode);
        // frc2::RamseteCommand GetRamsetteCommand(frc::Trajectory traj);

    private:

};
#endif