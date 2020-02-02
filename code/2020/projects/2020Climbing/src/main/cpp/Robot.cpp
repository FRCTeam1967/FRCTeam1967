#include "frc/WPILib.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include <jankyXboxJoystick.h>
#include "ctre/Phoenix.h"
#include "ctre/phoenix/motorcontrol/SensorCollection.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <math.h>
#include <frc/Encoder.h>
//#include "jankyTask.h"


using namespace std;
using namespace frc;

class Robot : public frc::TimedRobot {
  WPI_TalonSRX * lmotor;
  WPI_TalonSRX * rmotor;
  climbingMech * climbing;
  jankyXboxJoystick * joystick;
  Solenoid * rpiston;
  Solenoid * lpiston;
  Encoder * zero;
  Encoder * level;
  Encoder * highest;
  string setHeight;
  
  public:
  //constructor
  Robot()
    lmotor = NULL;
    rmotor = NULL;
    climbing = NULL;
    joystick = NULL;
    rpiston = NULL;
    lpiston = NULL;
    zero = NULL;
    level = NULL;
    highest = NULL;
  {

  }
  //deconstructor
  ~Robot()
  {
    delete lmotor;
    delete rmotor;
    delete climbing;
    delete joystick;
    delete rpiston;
    delete lpiston;
    delete zero;
    delete level;
    delete highest;
  }
  
  virtual void RobotInit() override
  {
    lmotor = new WPI_TalonSRX(1);//name these later
    rmotor = new WPI_TalonSRX(2);
    climbing = new climbingMech(3);
    joystick = new jankyXboxJoystick(4);
    rpiston = new Solenoid(5);
    lpiston = new Solenoid(6);
    zero = new Encoder(7);
    level = new Encoder(8);
    highest = new Encoder(9);
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
      climbing -> ClimbingMotorStopp();
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