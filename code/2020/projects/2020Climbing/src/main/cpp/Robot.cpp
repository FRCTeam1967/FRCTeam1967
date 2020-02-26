#include "frc/WPILib.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "jankyXboxJoystick.h"
#include "ctre/Phoenix.h"
#include "ctre/phoenix/motorcontrol/SensorCollection.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <math.h>
#include <frc/Encoder.h>
#include "ClimbingMech.h"
#include "jankyTask.h"

using namespace std;
using namespace frc;

#define L_MOTOR_CHANNEL 6 //find real numbers for these later
#define R_MOTOR_CHANNEL 1
#define GAME_JOYSTICK_CHANNEL 2

class Robot : public frc::TimedRobot {
  WPI_TalonSRX * lmotor;
  WPI_TalonSRX * rmotor;
  ClimbingMech * climbing;
  jankyXboxJoystick * joystick;
  string setHeight;
  
  public:
  //constructor
  Robot()
  {
    lmotor = NULL;
    rmotor = NULL;
    climbing = NULL;
    joystick = NULL;
  }

  //deconstructor
  ~Robot()
  {
    delete lmotor;
    delete rmotor;
    delete climbing;
    delete joystick;
  }
  
  virtual void RobotInit() override
  {
    climbing = new ClimbingMech(L_MOTOR_CHANNEL, R_MOTOR_CHANNEL, 1, 2);
    joystick = new jankyXboxJoystick(4);
    climbing -> StartUpInit();
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
    bool buttonA = joystick -> GetButtonA();
    bool buttonB = joystick -> GetButtonB();
    bool buttonY = joystick -> GetButtonY();
    bool buttonX = joystick -> GetButtonX();
    float rightVal = joystick -> GetRightYAxis();

  if(buttonX){
    climbing -> ClimbingMotorStop();
  }
  else if(buttonA){
    climbing -> HighestHeight();
  }
  else if(buttonB){
    climbing -> LevelHeight();
  }
  else if(buttonY){
    climbing -> GroundHeight();
  }
  //manual controls
  else {
    if (rightVal >= 0.2){
      climbing -> ClimbingMotorDown();
      setHeight = "None";
    }
    else if (rightVal <= -0.2){
      climbing -> ClimbingMotorUp();
      setHeight = "None";
    }
    else if (rightVal < 0.2 && rightVal > -0.2){
      climbing -> ClimbingMotorStop();
      setHeight = "None";
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