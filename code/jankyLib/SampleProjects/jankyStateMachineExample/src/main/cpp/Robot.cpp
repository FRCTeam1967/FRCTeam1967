#include "Robot.h"
#include <iostream>
#include "JankyStateMachineExample.h"
#include "adi/ADIS16470_IMU.h" //gyro
#include "frc/Timer.h" //timer
#include <frc/DigitalInput.h> //limit switch
#include "ctre/Phoenix.h" //talons
#include <frc/drive/DifferentialDrive.h> //drive
#include "frc/SpeedControllerGroup.h" //SpeedControllerGroup

#define FRONT_LEFT_MOTOR_CHANNEL 1
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 3
#define REAR_RIGHT_MOTOR_CHANNEL 4

#define LIMIT_SWITCH_CHANNEL 5

JankyStateMachineExample * jankyStateMachine = NULL;

WPI_TalonSRX * pFLMotor = NULL;
WPI_TalonSRX * pRLMotor = NULL;
WPI_TalonSRX * pFRMotor = NULL;
WPI_TalonSRX * pRRMotor = NULL;
SpeedControllerGroup * pLeftDrive = NULL;
SpeedControllerGroup * pRightDrive = NULL;
DifferentialDrive * pDrive = NULL;

Timer * pTimer = NULL;
ADIS16470_IMU * pGyro = NULL;
DigitalInput * pSwitch = NULL;
VisionSubsystem * pVision = NULL;

void Robot::RobotInit() {
  //chassis setup
  pFLMotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
  pRLMotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
  pFRMotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
  pRRMotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);

  pLeftDrive = new SpeedControllerGroup(*pFLMotor, *pRLMotor);
  pRightDrive = new SpeedControllerGroup(*pFRMotor, *pRRMotor);
  pDrive = new DifferentialDrive(*pLeftDrive, *pRightDrive);
  
  pDrive -> SetSafetyEnabled(false);

  //sensors setup
  pFLMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
  pFLMotor->SetSelectedSensorPosition(0, 0, 10);
  pFLMotor->GetSensorCollection().SetQuadraturePosition(0,10);
  pFRMotor->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
  pFRMotor->SetSelectedSensorPosition(0, 0, 10);
  pFRMotor->GetSensorCollection().SetQuadraturePosition(0,10);

  pTimer = new Timer();

  pGyro = new ADIS16470_IMU();
  pGyro -> SetYawAxis(ADIS16470_IMU::IMUAxis::kY); //for vertical electronic board

  pSwitch = new DigitalInput(LIMIT_SWITCH_CHANNEL);

  pVision = new VisionSubsystem();
}

void Robot::RobotPeriodic() {}


void Robot::AutonomousInit() {
  if (jankyStateMachine == NULL){
    jankyStateMachine = new JankyStateMachineExample(pDrive, pFLMotor, pFRMotor, pTimer, pGyro, pSwitch, pVision);
  }
}

void Robot::AutonomousPeriodic() {
  jankyStateMachine -> GoAutonomous();
}

void Robot::TeleopInit() {}

void Robot::TeleopPeriodic() {}

void Robot::DisabledInit() {}

void Robot::DisabledPeriodic() {}

void Robot::TestInit() {}

void Robot::TestPeriodic() {}

#ifndef RUNNING_FRC_TESTS
int main() {
  return frc::StartRobot<Robot>();
}
#endif
