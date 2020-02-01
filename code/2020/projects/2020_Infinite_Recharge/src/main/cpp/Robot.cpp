#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "ctre/Phoenix.h"
#include "jankyXboxJoystick.h"
#include "jankyDrivestick.h"
#include <frc/drive/DifferentialDrive.h>
#include "rev/ColorSensorV3.h"
#include "ColorSensorInfiniteRecharge.h"
#include "frc/TimedRobot.h"
#include "frc/SpeedControllerGroup.h"

#define PROG_BOT
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1
#define COLOR_SENSOR_PORT 0x52

#ifdef PROG_BOT
#define FRONT_LEFT_MOTOR_CHANNEL 1
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 4
#define REAR_RIGHT_MOTOR_CHANNEL 3
#endif

using namespace std;
using namespace frc;
using namespace rev;

class Robot : public frc::TimedRobot {
  WPI_TalonSRX*flmotor;
  WPI_TalonSRX*frmotor;
  WPI_VictorSPX*rlmotor;
  WPI_VictorSPX*rrmotor;
  DifferentialDrive*drive;
  SpeedControllerGroup*leftDrive;
  SpeedControllerGroup*rightDrive;
  jankyDrivestick*left;
  jankyDrivestick*right;
  bool shootingSideFront;
  ColorSensorInfiniteRecharge*sensor_fake;

  public:
  //constructor
  Robot()
  {
    flmotor = NULL;
    rlmotor = NULL;
    frmotor = NULL;
    rrmotor = NULL;
    drive = NULL;
    leftDrive = NULL;
    rightDrive = NULL;
    left = NULL;
    right = NULL;
    sensor_fake = NULL;
  }
  //deconstructor
  ~Robot()
  {
    delete flmotor;
    delete rlmotor;
    delete frmotor;
    delete rrmotor;
    delete drive;
    delete leftDrive;
    delete rightDrive;
    delete left;
    delete right;
    delete sensor_fake;
  }
  
  virtual void RobotInit() override
  {
    flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
    rlmotor = new WPI_VictorSPX(REAR_LEFT_MOTOR_CHANNEL);
    frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
    rrmotor = new WPI_VictorSPX(REAR_RIGHT_MOTOR_CHANNEL);
    leftDrive = new SpeedControllerGroup(*flmotor, *rlmotor);
    rightDrive = new SpeedControllerGroup(*frmotor, *rrmotor);
    drive = new DifferentialDrive(*leftDrive, *rightDrive);
    left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
    right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);
    sensor_fake = new ColorSensorInfiniteRecharge();

    drive -> SetSafetyEnabled(false);

    shootingSideFront = true;
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
    //Driving code
    #ifdef PROG_BOT
    bool drivingToggle = left -> Get10();
    #endif

    //double confidence = 0.0;

    if (drivingToggle && shootingSideFront)
    {
      shootingSideFront = false;
    }
    else if (drivingToggle && !shootingSideFront)
    {
      shootingSideFront = true;
    }
    if (shootingSideFront)
    {
      drive -> TankDrive(-left->GetY(), -right->GetY());
    }
    else if (!shootingSideFront)
    {
      drive -> TankDrive(left->GetY(), right->GetY());
    }

    frc::SmartDashboard::PutString("Color", sensor_fake -> ReadColor());
    //frc::SmartDashboard::PutNumber("Confidence", confidence);
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