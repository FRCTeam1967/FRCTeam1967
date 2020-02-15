#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/trajectory/constraint/DifferentialDriveKinematicsConstraint.h>
#include <units/units.h>
#include "AutoConstants.h"

using namespace AutoDriveConstants;

const frc::DifferentialDriveKinematics DriveConstants::kDriveKinematics(
    kTrackwidth);
