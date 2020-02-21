//INCLUDES
#include <iostream>
// frc
#include <frc/Filesystem.h>
#include "frc/kinematics/DifferentialDriveKinematics.h"
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>
#include <frc/trajectory/TrajectoryConfig.h>
#include <frc/trajectory/TrajectoryUtil.h>
#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc/trajectory/constraint/DifferentialDriveVoltageConstraint.h>
#include "frc/WPILib.h"
//frc2
#include "frc2/command/RamseteCommand.h"
#include <frc2/command/InstantCommand.h>
// wpi
#include <wpi/Path.h>
#include <wpi/SmallString.h>
// custom classes
#include "AutoConstants.h"
#include "AutoDriveSubsystems.h"
#include <ctre/Phoenix.h>

// NAMESPACES
using namespace std;
using namespace frc;
using namespace AutoDriveConstants;
using namespace ctre;

class Robot : public frc::TimedRobot {
  public:
  //JankyPathWeaver * pathweaver;
  AutoDriveSubsystem m_drive;
  frc2::RamseteCommand * rc;

  //constructor
  Robot()
  {
    //pathweaver = NULL;
    rc = NULL;
  }
  //deconstructor
  ~Robot()
  {
    //delete pathweaver;
    delete rc;
  }
  
  virtual void RobotInit() override
  {
    cout << "Creating voltage constraint" << endl;
    // Create a voltage constraint to ensure we don't accelerate too fast
    frc::DifferentialDriveVoltageConstraint autoVoltageConstraint(
        frc::SimpleMotorFeedforward<units::meters>(
            AutoDriveConstants::ks,
            AutoDriveConstants::kv,
            AutoDriveConstants::ka
          ),
          AutoDriveConstants::kDriveKinematics,
          10_V
        );

    cout << "Config Trajectory" << endl;
    // CONFIG TRAJECTORY
    // Set up config for trajectory
    frc::TrajectoryConfig config(AutoConstants::kMaxSpeed, AutoConstants::kMaxAcceleration);
    // Add kinematics to ensure max speed is actually obeyed
    config.SetKinematics(AutoDriveConstants::kDriveKinematics);
    // Apply the voltage constraint
    config.AddConstraint(autoVoltageConstraint);

    // GET TRAJECTORY
    cout << "Get Trajectory" << endl;
    wpi::SmallString<64> deployDirectory;
    frc::filesystem::GetDeployDirectory(deployDirectory);
    wpi::sys::path::append(deployDirectory, "paths");
    wpi::sys::path::append(deployDirectory, "GetBallFromTrench.wpilib.json");

    frc::Trajectory trajectory = frc::TrajectoryUtil::FromPathweaverJson(deployDirectory);

    
    // cout << "Creating pid controller" << endl;
    // // CREATE PID CONTROLLER
    frc2::PIDController leftController(AutoDriveConstants::kPDriveVel, 0,0);
    frc2::PIDController rightController(AutoDriveConstants::kPDriveVel, 0,0);

    leftController.SetTolerance(0.0);
    rightController.SetTolerance(0.0);

    cout << "Instantiate ramsete command" << endl;
    // INSTANTIATE RAMSETE COMMAND
    rc = new frc2::RamseteCommand(
      trajectory, [this]() { return m_drive.GetPose(); },
      frc::RamseteController(AutoConstants::kRamseteB,
                            AutoConstants::kRamseteZeta),
      frc::SimpleMotorFeedforward<units::meters>(
          AutoDriveConstants::ks, AutoDriveConstants::kv, AutoDriveConstants::ka),
      AutoDriveConstants::kDriveKinematics,
      [this] { return m_drive.GetWheelSpeeds(); },
      frc2::PIDController(AutoDriveConstants::kPDriveVel, 0, 0),
      frc2::PIDController(AutoDriveConstants::kPDriveVel, 0, 0),
      [this](auto left, auto right) { m_drive.TankDriveVolts(left, right); },
      {&m_drive});

      rc->Initialize();
  }

  virtual void AutonomousInit() override
  {
      
  }

  virtual void AutonomousPeriodic() override
  {
    // EXECUTE
    cout << " " << endl;
    if(rc->IsFinished())
    {
      cout << "executing" << endl;
      rc->Execute();
    }
    else {
      cout << "end" << endl;
      rc->End(true);
      m_drive.StopAuto();
    }
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

  virtual void DisabledInit() override
  {

  }

  virtual void DisabledPeriodic() override
  {

  }

private:
};
int main() 
{ 
  return frc::StartRobot<Robot>(); 
}