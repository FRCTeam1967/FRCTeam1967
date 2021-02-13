#include <frc/kinematics/DifferentialDriveKinematics.h>
#include <frc/trajectory/constraint/DifferentialDriveKinematicsConstraint.h>
//#include <units/units.h>
#include <units/velocity.h>
#include <units/voltage.h>
#include <wpi/math>
#include "Settings.h"

#pragma once

#define CHASSIS_LEFT_ENCODER_CHANNEL 0
#define CHASSIS_RIGHT_ENCODER_CHANNEL 1

#define CHASSIS_WIDTH 0.69_m
#define WHEEL_DIAMETER_INCHES 0.1524 //6

namespace AutoDriveConstants {
constexpr int kLeftMotor1Port = INTAKE_LEFT_MOTOR_CHANNEL;
constexpr int kLeftMotor2Port = SHOOTING_LEFT_MOTOR_CHANNEL;
constexpr int kRightMotor1Port = INTAKE_RIGHT_MOTOR_CHANNEL;
constexpr int kRightMotor2Port = SHOOTING_RIGHT_MOTOR_CHANNEL;

//Try:
// constexpr int kLeftMotor1Port = SHOOTING_LEFT_MOTOR_CHANNEL;
// constexpr int kLeftMotor2Port = INTAKE_LEFT_MOTOR_CHANNEL;
// constexpr int kRightMotor1Port = SHOOTING_RIGHT_MOTOR_CHANNEL;
// constexpr int kRightMotor2Port = INTAKE_RIGHT_MOTOR_CHANNEL;

//constexpr int kLeftEncoderPorts[]{0, 1};
//constexpr int kRightEncoderPorts[]{2, 3};
constexpr bool kLeftEncoderReversed = false;
constexpr bool kRightEncoderReversed = false; //true;

constexpr auto kTrackwidth = CHASSIS_WIDTH; //232.9723996;
extern const frc::DifferentialDriveKinematics kDriveKinematics;

constexpr int kEncoderCPR = 4096;
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
constexpr auto ks = 1.04_V; //1.52_V;
constexpr auto kv = 0.00814 * 1_V * 1_s / 1_in;  //0.00935 * 1_V * 1_s / 1_m; 
constexpr auto ka = 0.00215 * 1_V * 1_s * 1_s / 1_in; //0.000222 * 1_V * 1_s * 1_s / 1_m; 

// Example value only - as above, this must be tuned for your drive!
constexpr double kPDriveVel = 0.0000559;
constexpr double kDDriveVel = 0.000027;
}  // namespace AutoDriveConstants

namespace AutoConstants {
constexpr auto kMaxSpeed = 15_mps;
constexpr auto kMaxAcceleration = 15_mps_sq;

// Reasonable baseline values for a RAMSETE follower in units of meters and
// seconds
constexpr double kRamseteB = 2;
constexpr double kRamseteZeta = 0.7 ;
}  // namespace AutoConstants

namespace OIConstants {
constexpr int kDriverControllerPort = 1;
}  // namespace OIConstants


// constants for distance calculation
#define PULSES_PER_REVOLUTION 4096
#define WHEEL_DIAMETER 6
#define WHEEL_CIRCUMFERENCE WHEEL_DIAMETER * wpi::math::pi