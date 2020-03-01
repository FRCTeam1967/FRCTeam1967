#include <iostream>
#include <frc/trajectory/TrajectoryGenerator.h>
#include "frc2/command/RamseteCommand.h"
#include "AutoEntry.h"
#include "AutoDriveSubsystems.h"

#ifndef JANKY_PATHWEAVER_H
#define JANKY_PATHWEAVER_H

class JankyPathWeaver : public AutoEntry
{
    public:
        JankyPathWeaver(AutoDriveSubsystem * driveSubsystem, int autoMode);
        virtual ~JankyPathWeaver();

        // auto entry methods
        void Start();
        bool JobDone();
        void RunAction();
        void End();

        frc::Trajectory GeneratePath(int autoMode);
        //frc2::RamseteCommand GetRamsetteCommand();

    private:
        AutoDriveSubsystem * m_drive;
        frc::Trajectory  trajectory;
        //frc2::RamseteCommand rc;
};
#endif