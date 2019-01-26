#include "frc/WPILib.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "ctre/Phoenix.h"
#include "jankyXboxJoystick.h"
#include "jankyDrivestick.h"
#include <frc/drive/DifferentialDrive.h>
#include <frc/Encoder.h>
#include <math.h>

#define FRONT_LEFT_MOTOR_CHANNEL 3
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 5
#define REAR_RIGHT_MOTOR_CHANNEL 4
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1
#define GC_XBOX_CHANNEL 2
#define PISTON_FRONT_LEFT_CHANNEL 3
#define PISTON_FRONT_RIGHT_CHANNEL 3
#define PISTON_BACK_CHANNEL 5

#define ENCODER_UNITS_PER_ROTATION 4096
#define DIAMETER 6
#define CIRCUMFERENCE DIAMETER*M_PI

using namespace std;
using namespace frc;

class Robot : public frc::TimedRobot {
  WPI_TalonSRX*flmotor;
  WPI_TalonSRX*rlmotor;
  WPI_TalonSRX*frmotor;
  WPI_TalonSRX*rrmotor;
  Solenoid*frpiston;
  Solenoid*flpiston;
  Solenoid*bpiston;
  DifferentialDrive*drive;
  SpeedControllerGroup*leftDrive;
  SpeedControllerGroup*rightDrive;
  jankyDrivestick*left;
  jankyDrivestick*right;
  jankyXboxJoystick*gameJoystick;

  public:
  Robot()
  {
    flmotor = NULL;
    rlmotor = NULL;
    frmotor = NULL;
    rrmotor = NULL;
    frpiston = NULL;
    flpiston = NULL;
    bpiston = NULL;
    drive = NULL;
    leftDrive = NULL;
    rightDrive = NULL;
    left = NULL;
    right = NULL;
    gameJoystick = NULL;
  }

  ~Robot()
  {
    delete flmotor;
    delete rlmotor;
    delete frmotor;
    delete rrmotor;
    delete frpiston;
    delete flpiston;
    delete bpiston;
    delete drive;
    delete leftDrive;
    delete rightDrive;
    delete left;
    delete right;
    delete gameJoystick;
  }
  
  virtual void RobotInit() override
  {
    flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
    rlmotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
    frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
    rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
    frpiston = new Solenoid(10, PISTON_FRONT_RIGHT_CHANNEL);
    flpiston = new Solenoid(10, PISTON_FRONT_LEFT_CHANNEL);
    bpiston = new Solenoid(10, PISTON_BACK_CHANNEL);
    drive = new DifferentialDrive(*leftDrive, *rightDrive);
    leftDrive = new SpeedControllerGroup(*flmotor, *rlmotor);
    rightDrive = new SpeedControllerGroup(*frmotor, *rrmotor);
    left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
    right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);
    gameJoystick = new jankyXboxJoystick(GC_XBOX_CHANNEL);
  }

  virtual void AutonomousInit() override
  {
  
  }

  virtual void AutonomousPeriodic() override
  {
    
  }

  virtual void TeleopInit() override
  {
    flmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
    frmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
    flmotor->SetSelectedSensorPosition(0, 0, 10);
    flmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
    frmotor->SetSelectedSensorPosition(0, 0, 10);
    frmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
  }

  virtual void TeleopPeriodic() override
  {
    int i = 0;
    int m = 0;
    
    drive->TankDrive(-left->GetY(), -right->GetY());
    
    double leftEncoderCount = -(flmotor->GetSensorCollection().GetQuadraturePosition());
    double leftEncoderDistance = (leftEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE;
    double rightEncoderCount = frmotor->GetSensorCollection().GetQuadraturePosition();
    double rightEncoderDistance = (rightEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE;
    SmartDashboard::PutNumber("Left Encoder Count", leftEncoderCount);
    SmartDashboard::PutNumber("Left Encoder Distance", leftEncoderDistance);
    SmartDashboard::PutNumber("Right Encoder Count", rightEncoderCount);
    SmartDashboard::PutNumber("Right Encoder Distance", rightEncoderDistance);

    bool buttonY = gameJoystick -> GetButtonY();
    bool buttonA = gameJoystick -> GetButtonA();

    if (buttonY)
    {
      if (i==0)
      {
        frpiston -> Set(true);
        flpiston -> Set(true);
        i++;
      }

      if (i==1)
      {
        frpiston -> Set(false);
        flpiston -> Set(false);
        i--;
      }
    }

    if (buttonA)
    {
      if (m==0)
      {
        bpiston -> Set(true);
        m++;
      }

      if (m==1)
      {
        bpiston -> Set(false);
        m--;
      }
    }
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