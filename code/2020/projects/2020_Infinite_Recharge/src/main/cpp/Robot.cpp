#include "frc/WPILib.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "ctre/Phoenix.h"
#include "jankyXboxJoystick.h"
#include "jankyDrivestick.h"
#include <frc/drive/DifferentialDrive.h>

#define FRONT_LEFT_MOTOR_CHANNEL 5
#define REAR_LEFT_MOTOR_CHANNEL 4
#define FRONT_RIGHT_MOTOR_CHANNEL 3
#define REAR_RIGHT_MOTOR_CHANNEL 2
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1
#define GC_XBOX_CHANNEL 2
#define PISTON_CHANNEL 5

using namespace std;
using namespace frc;

class Robot : public frc::TimedRobot {
  WPI_TalonSRX*flmotor;
  WPI_TalonSRX*frmotor;
  WPI_TalonSRX*rlmotor;
  WPI_TalonSRX*rrmotor;
  Solenoid*piston;
  DifferentialDrive*drive;
  SpeedControllerGroup*leftDrive;
  SpeedControllerGroup*rightDrive;
  jankyDrivestick*left;
  jankyDrivestick*right;
  jankyXboxJoystick*gameJoystick;

  public:
  //constructor
  Robot()
  {
    flmotor = NULL;
    rlmotor = NULL;
    frmotor = NULL;
    rrmotor = NULL;
    piston = NULL;
    drive = NULL;
    leftDrive = NULL;
    rightDrive = NULL;
    left = NULL;
    right = NULL;
    gameJoystick = NULL;
  }
  //deconstructor
  ~Robot()
  {
    delete flmotor;
    delete rlmotor;
    delete frmotor;
    delete rrmotor;
    delete piston;
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
    rrmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
    rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
    piston = new Solenoid(10, PISTON_CHANNEL);
    leftDrive = new SpeedControllerGroup(*flmotor, *rlmotor);
    rightDrive = new SpeedControllerGroup(*frmotor, *rrmotor);
    drive = new DifferentialDrive(*leftDrive, *rightDrive);
    left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
    right = new jankyDrivestick(REAR_RIGHT_MOTOR_CHANNEL);
    gameJoystick = new jankyXboxJoystick(GC_XBOX_CHANNEL);

    drive -> SetSafetyEnabled(false);
  }

  virtual void AutonomousInit() override
  {
  
  }

  virtual void AutonomousPeriodic() override
  {
    
  }

  virtual void TeleopInit() override
  {

  }

  virtual void TeleopPeriodic() override
  {
    drive -> TankDrive(-left->GetY(), -right->GetY());

    bool buttonY = gameJoystick -> GetButtonY();

    if (buttonY == true)
      piston -> Set(true);

    else
      piston -> Set(false);
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