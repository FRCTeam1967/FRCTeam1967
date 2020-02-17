#include "AutoDriveSubsystems.h"
#include <frc/geometry/Rotation2d.h>
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>
#include <ctre/Phoenix.h>

using namespace AutoDriveConstants;
using namespace ctre;

AutoDriveSubsystem::AutoDriveSubsystem()
    : m_left1{kLeftMotor1Port},
      m_left2{kLeftMotor2Port},
      m_right1{kRightMotor1Port},
      m_right2{kRightMotor2Port},
      //m_leftEncoder{kLeftEncoderPorts[0], kLeftEncoderPorts[1]},
      //m_rightEncoder{kRightEncoderPorts[0], kRightEncoderPorts[1]},
      m_odometry{frc::Rotation2d(units::degree_t(GetHeading()))} {
  // Set the distance per pulse for the encoders
  //m_leftEncoder.SetDistancePerPulse(kEncoderDistancePerPulse);
  //m_rightEncoder.SetDistancePerPulse(kEncoderDistancePerPulse);
  m_left1.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
	m_left1.SetSelectedSensorPosition(0, 0, 10);
	m_left1.GetSensorCollection().SetQuadraturePosition(0,10);

  m_right2.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
	m_right2.SetSelectedSensorPosition(0, 0, 10);
	m_right2.GetSensorCollection().SetQuadraturePosition(0,10);

  // reset time
  time = new frc::Timer();
  time -> Reset();

  ResetEncoders();
}

void AutoDriveSubsystem::Periodic() {
  // Implementation of subsystem periodic method goes here.
  // m_odometry.Update(frc::Rotation2d(units::degree_t(GetHeading())),
  //                   units::meter_t(m_leftEncoder.GetDistance()), // units of s
  //                   units::meter_t(m_rightEncoder.GetDistance()));

  double distanceLeft = GetLeftEncoder() * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION);
  double distanceRight = GetRightEncoder() * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION);
  m_odometry.Update(frc::Rotation2d(units::degree_t(GetHeading())),
                    units::meter_t(distanceLeft), // QUESTION: WHY IS THIS IN METERS? :(
                    units::meter_t(distanceRight)); // QUESTION: WHY IS THIS IN METERS? :(
}

void AutoDriveSubsystem::ArcadeDrive(double fwd, double rot) {
  m_drive.ArcadeDrive(fwd, rot);
}

void AutoDriveSubsystem::TankDriveVolts(units::volt_t left, units::volt_t right) {
  m_leftMotors.SetVoltage(left);
  m_rightMotors.SetVoltage(-right);
}

void AutoDriveSubsystem::ResetEncoders() {
  // m_leftEncoder.Reset();
  // m_rightEncoder.Reset();
  m_left1.GetSensorCollection().SetQuadraturePosition(0,10);
  m_right2.GetSensorCollection().SetQuadraturePosition(0,10);

  // reset time
  time -> Reset();
}

double AutoDriveSubsystem::GetAverageEncoderDistance() {
  //return (m_leftEncoder.GetDistance() + m_rightEncoder.GetDistance()) / 2.0;
  return (m_left1.GetSensorCollection().GetQuadraturePosition() + m_right2.GetSensorCollection().GetQuadraturePosition()) / 2.0;
}

// frc::Encoder& AutoDriveSubsystem::GetLeftEncoder() { return m_leftEncoder; }
// frc::Encoder& AutoDriveSubsystem::GetRightEncoder() { return m_rightEncoder; }

double AutoDriveSubsystem::GetLeftEncoder() { return m_left1.GetSensorCollection().GetQuadraturePosition(); }
double AutoDriveSubsystem::GetRightEncoder() { return m_right2.GetSensorCollection().GetQuadraturePosition(); }

void AutoDriveSubsystem::SetMaxOutput(double maxOutput) {
  m_drive.SetMaxOutput(maxOutput);
}

double AutoDriveSubsystem::GetHeading() {
  return std::remainder(m_gyro.GetAngle(), 360) * (kGyroReversed ? -1.0 : 1.0);
}

double AutoDriveSubsystem::GetTurnRate() {
  return m_gyro.GetRate() * (kGyroReversed ? -1.0 : 1.0);
}

frc::Pose2d AutoDriveSubsystem::GetPose() { return m_odometry.GetPose(); }

frc::DifferentialDriveWheelSpeeds AutoDriveSubsystem::GetWheelSpeeds() {
  // return {units::meters_per_second_t(m_leftEncoder.GetRate()), // units of distance per second
  //         units::meters_per_second_t(m_rightEncoder.GetRate())}; // units of distance per second

  // get distance
  double distanceLeft = GetLeftEncoder() * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION);
  double distanceRight = GetRightEncoder() * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION);

  double t = time->Get();

  // get time
  double leftRate = distanceLeft / t;
  double rightRate = distanceRight / t;

  //divide distance by time
  return {units::meters_per_second_t(leftRate), // units of distance per second
          units::meters_per_second_t(rightRate)}; // units of distance per second
}

void AutoDriveSubsystem::ResetOdometry(frc::Pose2d pose) {
  ResetEncoders();
  m_odometry.ResetPosition(pose,
                           frc::Rotation2d(units::degree_t(GetHeading())));
}