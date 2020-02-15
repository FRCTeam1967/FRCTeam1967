#include "frc/WPILib.h"
#include "ctre/Phoenix.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/Filesystem.h>
#include <frc/trajectory/TrajectoryUtil.h>
#include <wpi/Path.h>
#include <wpi/SmallString.h>
#include "frc/kinematics/DifferentialDriveKinematics.h"
#include <frc/trajectory/constraint/DifferentialDriveVoltageConstraint.h>
#include <frc/trajectory/TrajectoryConfig.h>
#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>

#include "Constants.h"
#include "DriveSubsystem.h"
#include "frc2/command/RamseteCommand.h"
#include <frc2/command/InstantCommand.h>

#define SHOOTING_LEFT_MOTOR_CHANNEL 4
#define INTAKE_LEFT_MOTOR_CHANNEL 0
#define SHOOTING_RIGHT_MOTOR_CHANNEL 1
#define INTAKE_RIGHT_MOTOR_CHANNEL 3

using namespace std;
using namespace frc;
using namespace DriveConstants;

class Robot : public frc::TimedRobot {
  public:
  // declare m_drive
  DriveSubsystem m_drive;
  frc2::RamseteCommand * rc;

  
  //constructor
  Robot()
  {
    rc = NULL;
  }
  //deconstructor
  ~Robot()
  {
    delete rc;
  }
  
  virtual void RobotInit() override
  {

  }

  virtual void AutonomousInit() override
  {
    // Create a voltage constraint to ensure we don't accelerate too fast
    frc::DifferentialDriveVoltageConstraint autoVoltageConstraint(
        frc::SimpleMotorFeedforward<units::meters>(
            DriveConstants::ks,
            DriveConstants::kv,
            DriveConstants::ka
          ),
          DriveConstants::kDriveKinematics,
          10_V
        );

    // CONFIG TRAJECTORY
    // Set up config for trajectory
    frc::TrajectoryConfig config(AutoConstants::kMaxSpeed, AutoConstants::kMaxAcceleration);
    // Add kinematics to ensure max speed is actually obeyed
    config.SetKinematics(DriveConstants::kDriveKinematics);
    // Apply the voltage constraint
    config.AddConstraint(autoVoltageConstraint);

    // GET TRAJECTORY
    wpi::SmallString<64> deployDirectory;
    frc::filesystem::GetDeployDirectory(deployDirectory);
    wpi::sys::path::append(deployDirectory, "paths");
    wpi::sys::path::append(deployDirectory, "GetBallFromTrench.wpilib.json");

    frc::Trajectory trajectory = frc::TrajectoryUtil::FromPathweaverJson(deployDirectory);

    // CREATE PID CONTROLLER
    frc2::PIDController leftController(DriveConstants::kPDriveVel, 0,0);
    frc2::PIDController rightController(DriveConstants::kPDriveVel, 0,0);

    leftController.SetTolerance(0.0);
    rightController.SetTolerance(0.0);

    rc = new frc2::RamseteCommand(
      trajectory, [this]() { return m_drive.GetPose(); },
      frc::RamseteController(AutoConstants::kRamseteB,
                            AutoConstants::kRamseteZeta),
      frc::SimpleMotorFeedforward<units::meters>(
          DriveConstants::ks, DriveConstants::kv, DriveConstants::ka),
      DriveConstants::kDriveKinematics,
      [this] { return m_drive.GetWheelSpeeds(); },
      frc2::PIDController(DriveConstants::kPDriveVel, 0, 0),
      frc2::PIDController(DriveConstants::kPDriveVel, 0, 0),
      [this](auto left, auto right) { m_drive.TankDriveVolts(left, right); },
      {&m_drive});
  }

  virtual void AutonomousPeriodic() override
  {
    rc->Execute();
  }

  virtual void TeleopInit() override
  {

  }

  virtual void TeleopPeriodic() override
  {

  }

  virtual void TestPeriodic() override
  {

  }

private:
};
int main() 
{ 
  return frc::StartRobot<Robot>(); 
}