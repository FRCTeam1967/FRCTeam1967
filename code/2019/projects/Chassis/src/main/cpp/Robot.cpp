#include "frc/WPILib.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "ctre/Phoenix.h"
#include "jankyXboxJoystick.h"
#include "jankyDrivestick.h"
#include <frc/drive/DifferentialDrive.h>
#include <frc/Encoder.h>
#include <math.h>
#include <frc/AnalogInput.h>

#define FRONT_LEFT_MOTOR_CHANNEL 5
#define REAR_LEFT_MOTOR_CHANNEL 4
#define FRONT_RIGHT_MOTOR_CHANNEL 3
#define REAR_RIGHT_MOTOR_CHANNEL 2
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1
#define GC_XBOX_CHANNEL 2
#define PISTON_FRONT_LEFT_CHANNEL 4
#define PISTON_FRONT_RIGHT_CHANNEL 4
#define PISTON_BACK_CHANNEL 5
#define CARGO_ULTRASON_CHANNEL 3
#define PWM_CHANNEL 3
#define MIN_PWM_NUM 1000
#define MAX_PWM_NUM 2000
#define NUM_CMDS 10
#define SPACING (MAX_PWM_NUM-MIN_PWM_NUM)/NUM_CMDS
#define ROUNDING_NUM SPACING/2

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
  Solenoid*fpiston;
  Solenoid*bpiston;
  DifferentialDrive*drive;
  SpeedControllerGroup*leftDrive;
  SpeedControllerGroup*rightDrive;
  jankyDrivestick*left;
  jankyDrivestick*right;
  jankyXboxJoystick*gameJoystick;
  AnalogInput*distSensor;
  PWM*lightController;
  int i, m;
  bool ypressed, apressed;

  public:
  Robot()
  {
    flmotor = NULL;
    rlmotor = NULL;
    frmotor = NULL;
    rrmotor = NULL;
    fpiston = NULL;
    bpiston = NULL;
    drive = NULL;
    leftDrive = NULL;
    rightDrive = NULL;
    left = NULL;
    right = NULL;
    gameJoystick = NULL;
    distSensor = NULL;
    lightController = NULL;
  }

  ~Robot()
  {
    delete flmotor;
    delete rlmotor;
    delete frmotor;
    delete rrmotor;
    delete fpiston;
    delete bpiston;
    delete drive;
    delete leftDrive;
    delete rightDrive;
    delete left;
    delete right;
    delete gameJoystick;
    delete distSensor;
    delete lightController;
  }
  
  virtual void RobotInit() override
  {
    flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
    rlmotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
    frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
    rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
    fpiston = new Solenoid(10, PISTON_FRONT_RIGHT_CHANNEL);
    bpiston = new Solenoid(10, PISTON_BACK_CHANNEL);
    leftDrive = new SpeedControllerGroup(*flmotor, *rlmotor);
    rightDrive = new SpeedControllerGroup(*frmotor, *rrmotor);
    drive = new DifferentialDrive(*leftDrive, *rightDrive);
    left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
    right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);
    gameJoystick = new jankyXboxJoystick(GC_XBOX_CHANNEL);
    distSensor = new AnalogInput(CARGO_ULTRASON_CHANNEL);
    lightController = new PWM(PWM_CHANNEL);

    drive -> SetSafetyEnabled(false);

    i = 0;
    m = 0;
    ypressed=false;
    apressed=false;
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
    drive->TankDrive(-left->GetY(), -right->GetY());

    if(-left->GetY()!=0 && -right->GetY()!=0)
      lightController -> SetRaw(1300);

    else if(-left->GetY()!=0)
      lightController -> SetRaw(1200);

    else if(-right->GetY()!=0)
      lightController -> SetRaw(1100);

    else
      lightController -> SetRaw(1000);
    
    double leftEncoderCount = -(flmotor->GetSensorCollection().GetQuadraturePosition());
    double leftEncoderDistance = (leftEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE;
    double rightEncoderCount = frmotor->GetSensorCollection().GetQuadraturePosition();
    double rightEncoderDistance = (rightEncoderCount/ENCODER_UNITS_PER_ROTATION)*CIRCUMFERENCE;
    int distance = distSensor->GetValue();
    SmartDashboard::PutNumber("Left Encoder Count", leftEncoderCount);
    SmartDashboard::PutNumber("Left Encoder Distance", leftEncoderDistance);
    SmartDashboard::PutNumber("Right Encoder Count", rightEncoderCount);
    SmartDashboard::PutNumber("Right Encoder Distance", rightEncoderDistance);
    SmartDashboard::PutNumber("Distance to object in front of robot", distance);
    SmartDashboard::PutNumber("PWM value", lightController -> GetRaw());

    bool buttonY = gameJoystick -> GetButtonY();
    bool buttonA = gameJoystick -> GetButtonA();

    if (buttonY && ypressed==false)
    {
      if(fpiston->Get()==true)
        fpiston -> Set(false);

      else if(fpiston->Get()==false)
        fpiston -> Set(true);

      ypressed=true;
    }

    else if (!buttonY && ypressed==true)
      ypressed=false;

    if (buttonA && apressed==false)
    {
      if(bpiston->Get()==true)
        bpiston -> Set(false);
      
      else
        bpiston -> Set(true);
      
      apressed=true;
    }
 
    if (!buttonA && apressed==true)
      apressed=false;
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