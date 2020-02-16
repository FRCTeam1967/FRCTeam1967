#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/trajectory/constraint/DifferentialDriveKinematicsConstraint.h>
#include <units/units.h>
#include <wpi/math>

#pragma once

#define SHOOTING_LEFT_MOTOR_CHANNEL 4
#define INTAKE_LEFT_MOTOR_CHANNEL 0
#define SHOOTING_RIGHT_MOTOR_CHANNEL 1
#define INTAKE_RIGHT_MOTOR_CHANNEL 3

#define CHASSIS_LEFT_ENCODER_CHANNEL 0
#define CHASSIS_RIGHT_ENCODER_CHANNEL 1

#define CHASSIS_WIDTH 0.69_m
#define WHEEL_DIAMETER_INCHES 6

namespace AutoDriveConstants {
constexpr int kLeftMotor1Port = SHOOTING_LEFT_MOTOR_CHANNEL;
constexpr int kLeftMotor2Port = INTAKE_LEFT_MOTOR_CHANNEL;
constexpr int kRightMotor1Port = SHOOTING_RIGHT_MOTOR_CHANNEL;
constexpr int kRightMotor2Port = INTAKE_RIGHT_MOTOR_CHANNEL;

constexpr int kLeftEncoderPorts[]{0, 1};
constexpr int kRightEncoderPorts[]{2, 3};
constexpr bool kLeftEncoderReversed = false;
constexpr bool kRightEncoderReversed = true;

constexpr auto kTrackwidth = 0.CHASSIS_WIDTH;
extern const frc::DifferentialDriveKinematics kDriveKinematics;

constexpr int kEncoderCPR = 1024;
constexpr double kWheelDiameterInches = WHEEL_DIAMETER_INCHES;
constexpr double kEncoderDistancePerPulse =
    // Assumes the encoders are directly mounted on the wheel shafts
    (kWheelDiameterInches * wpi::math::pi) / static_cast<double>(kEncoderCPR);

constexpr bool kGyroReversed = true;

// These are example values only - DO NOT USE THESE FOR YOUR OWN ROBOT!
// These characterization values MUST be determined either experimentally or
// theoretically for *your* robot's drive. The Robot Characterization
// Toolsuite provides a convenient tool for obtaining these values for your
// robot.
constexpr auto ks = 0.22_V;
constexpr auto kv = 1.98 * 1_V * 1_s / 1_m;
constexpr auto ka = 0.2 * 1_V * 1_s * 1_s / 1_m;

// Example value only - as above, this must be tuned for your drive!
constexpr double kPDriveVel = 8.5;
}  // namespace AutoDriveConstants

namespace AutoConstants {
constexpr auto kMaxSpeed = 3_mps;
constexpr auto kMaxAcceleration = 3_mps_sq;

// Reasonable baseline values for a RAMSETE follower in units of meters and
// seconds
constexpr double kRamseteB = 2;
constexpr double kRamseteZeta = 0.7;
}  // namespace AutoConstants

namespace OIConstants {
constexpr int kDriverControllerPort = 1;
}  // namespace OIConstants
