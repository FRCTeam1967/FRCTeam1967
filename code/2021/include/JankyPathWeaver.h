#include <iostream>
#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc/trajectory/constraint/DifferentialDriveVoltageConstraint.h>
#include "frc/kinematics/DifferentialDriveKinematics.h"
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>
#include "frc2/command/RamseteCommand.h"
#include "AutoEntry.h"
#include "AutoDriveSubsystems.h"

#ifndef JANKY_PATHWEAVER_H
#define JANKY_PATHWEAVER_H

class JankyPathWeaver : public AutoEntry
{
    public:
        JankyPathWeaver(AutoDriveSubsystem * drive, int autoMode);
        virtual ~JankyPathWeaver();

        // auto entry methods
        void Start();
        bool JobDone();
        void RunAction();
        void End();

        void GeneratePath(int autoMode);
        void SetUpRamseteCommand();

    private:
        AutoDriveSubsystem * m_drive;
        frc2::PIDController leftController{AutoDriveConstants::kPDriveVel, 0,0};
        frc2::PIDController rightController{AutoDriveConstants::kPDriveVel, 0,0};
        frc::DifferentialDriveVoltageConstraint autoVoltageConstraint{
            frc::SimpleMotorFeedforward<units::meters>(
            AutoDriveConstants::ks,
            AutoDriveConstants::kv,
            AutoDriveConstants::ka
          ),
          AutoDriveConstants::kDriveKinematics,
          10_V
        };
        frc::Trajectory trajectory;
        //frc2::RamseteCommand * rc;
};
#endif