#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "ctre/Phoenix.h"
#include "jankyXboxJoystick.h"
#include "jankyDrivestick.h"
#include <frc/drive/DifferentialDrive.h>
#include "ColorSensorV3.h"
#include "ColorSensorInfiniteRecharge.h"
#include "frc/TimedRobot.h"
#include "frc/SpeedControllerGroup.h"
#include "Settings.h"

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

  float distanceToVisionTarget;
  float offsetFromVisionTarget;

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

    #ifdef DRIVE_TEAM_CAM_1
      //Run drive team camera
      cs::UsbCamera driveCam1;
      driveCam1 = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
      driveCam1.SetResolution(160,120);
      driveCam1.SetFPS(5);
      driveCam1.GetProperty("compression").Set(100);
    #endif

    #ifdef DRIVE_TEAM_CAM_2
      //Run drive team camera
      cs::UsbCamera driveCam2;
      driveCam2 = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
      driveCam2.SetResolution(160,120);
      driveCam2.SetFPS(5);
      driveCam2.GetProperty("compression").Set(100);
    #endif

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

    frc::SmartDashboard::PutNumber(VISION_DISTANCE, NO_DATA_DEFAULT);
	  frc::SmartDashboard::PutNumber(VISION_OFFSET, NO_DATA_DEFAULT);
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

    // Set distance & offset --> to give to turret & shooter
    distanceToVisionTarget = frc::SmartDashboard::GetNumber(VISION_DISTANCE, NO_DATA_DEFAULT); 
	  offsetFromVisionTarget = (frc::SmartDashboard::GetNumber(VISION_OFFSET, NO_DATA_DEFAULT)); //positive is to the right

    ColorSensorInfiniteRecharge::InfiniteRechargeColors color = sensor_fake -> ReadColor();

    frc::SmartDashboard::PutString("Color", sensor_fake -> GetColorString(color));
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