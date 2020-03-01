#include "AutoDriveSubsystems.h"
#include <frc/geometry/Rotation2d.h>
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>
#include <ctre/Phoenix.h>
#include "frc2/command/RamseteCommand.h"

using namespace AutoDriveConstants;
using namespace ctre;
using namespace std;

AutoDriveSubsystem::AutoDriveSubsystem()
    : m_left1{kLeftMotor1Port},
      m_left2{kLeftMotor2Port},
      m_right1{kRightMotor1Port},
      m_right2{kRightMotor2Port},
      m_odometry{frc::Rotation2d(units::degree_t(GetHeading()))} {

  // configure left encoder
  m_left1.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
	m_left1.SetSelectedSensorPosition(0, 0, 10);
	m_left1.GetSensorCollection().SetQuadraturePosition(0,10);

  // configure right encoder
  m_right2.ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
	m_right2.SetSelectedSensorPosition(0, 0, 10);
	m_right2.GetSensorCollection().SetQuadraturePosition(0,10);

  // set differential drive safety to false
  m_drive.SetSafetyEnabled(false); 

  prevTime = 0;

  // reset time
  time = new frc::Timer();
  time -> Reset();
  time->Start();

  // reset encoders
  ResetEncoders();
}

void AutoDriveSubsystem::Periodic() {
  double distanceLeft = GetLeftEncoder() * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION);
  double distanceRight = GetRightEncoder() * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION);

  cout << "Left distance: " << distanceLeft << endl;
  cout << "Right distance: " << distanceRight << endl;
  cout << "Gyro: " << m_gyro.GetAngle() << endl;
  cout << " " << endl;
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
  // reset encoders
  m_left1.GetSensorCollection().SetQuadraturePosition(0,10);
  m_right2.GetSensorCollection().SetQuadraturePosition(0,10);

  // reset time
  time -> Reset();
}

double AutoDriveSubsystem::GetAverageEncoderDistance() {
  cout << "Average Encoder Distance: " << ((m_left1.GetSensorCollection().GetQuadraturePosition()) + m_right2.GetSensorCollection().GetQuadraturePosition()) / 2.0 << endl; // print statement
  return ((m_left1.GetSensorCollection().GetQuadraturePosition()) + m_right2.GetSensorCollection().GetQuadraturePosition()) / 2.0;
}

double AutoDriveSubsystem::GetLeftEncoder() { 
  cout << "Left Encoder: " << (m_left1.GetSensorCollection().GetQuadraturePosition()) << endl; // print statement
  cout << "Gyro: " << m_gyro.GetAngle() << endl;
  cout << " " << endl;
  return (m_left1.GetSensorCollection().GetQuadraturePosition()); 
}

double AutoDriveSubsystem::GetRightEncoder() { 
  cout << "Right Encoder: " << m_right2.GetSensorCollection().GetQuadraturePosition() << endl; // print statement
  return m_right2.GetSensorCollection().GetQuadraturePosition(); 
}

void AutoDriveSubsystem::SetMaxOutput(double maxOutput) {
  cout << "Max Output: " << maxOutput << endl; // print statement
  m_drive.SetMaxOutput(maxOutput);
}

double AutoDriveSubsystem::GetHeading() {
  cout << "Heading: " << std::remainder(m_gyro.GetAngle(), 360) * (kGyroReversed ? -1.0 : 1.0) << endl; // print statement
  return std::remainder(m_gyro.GetAngle(), 360) * (kGyroReversed ? -1.0 : 1.0);
}

double AutoDriveSubsystem::GetTurnRate() {
  cout << "Turn Rate: " << m_gyro.GetRate() * (kGyroReversed ? -1.0 : 1.0) << endl; // print statement
  return m_gyro.GetRate() * (kGyroReversed ? -1.0 : 1.0);
}

frc::Pose2d AutoDriveSubsystem::GetPose() { 
  return m_odometry.GetPose(); 
}

frc::DifferentialDriveWheelSpeeds AutoDriveSubsystem::GetWheelSpeeds() {
  // get distance
  double distanceLeft = GetLeftEncoder() * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION);
  double distanceRight = GetRightEncoder() * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION);

  cout << "Left distance: " << distanceLeft << endl;
  cout << "Right distance: " << distanceRight << endl;

  // get time
  double t = time->Get();
  double elapsedTime = t - prevTime;

  cout << "time: " << t << endl; // print statement

  // get rate (divide distance by time)
  double leftRate = distanceLeft / elapsedTime;
  double rightRate = distanceRight / elapsedTime;

  prevTime = t;

  return {units::meters_per_second_t(leftRate), // units of distance per second
          units::meters_per_second_t(rightRate)}; // units of distance per second
}

void AutoDriveSubsystem::ResetOdometry(frc::Pose2d pose) {
  ResetEncoders();
  m_odometry.ResetPosition(pose,
                           frc::Rotation2d(units::degree_t(GetHeading())));

}

void AutoDriveSubsystem::StopAuto() {
  cout << "stopping subsystems" << endl;
  time -> Stop();
  time -> Reset();
}