#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/trajectory/constraint/DifferentialDriveKinematicsConstraint.h>
//#include <units/units.h>
#include <units/velocity.h>
#include <units/voltage.h>
#include "AutoConstants.h"

using namespace AutoDriveConstants;

const frc::DifferentialDriveKinematics AutoDriveConstants::kDriveKinematics(
    kTrackwidth);
