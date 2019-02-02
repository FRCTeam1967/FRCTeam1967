#include "frc/WPILib.h"
//#include "ctre/Phoenix.h"
#include <iostream>
#include <string>
#include <frc/smartdashboard/SmartDashboard.h>
#include <jankyXboxJoystick.h>
#include "HatchIntake.h"

#define LEFT_PISTON 4
#define RIGHT_PISTON 4
#define CARGO_PISTON 6

using namespace std;
using namespace frc;

class Robot : public frc::TimedRobot {
  HatchIntake*hatch;
  jankyXboxJoystick*joystick;
  Solenoid*pistonBL;
  Solenoid*pistonBR;
  bool buttonPressed;

  public:
  //constructor
  Robot()
  {
    hatch = NULL;
    joystick = NULL;
  }
  //deconstructor
  ~Robot()
  {
    delete hatch;
    delete joystick;
  }
  
  virtual void RobotInit() override
  {
    hatch = new HatchIntake(LEFT_PISTON, CARGO_PISTON);
    joystick = new jankyXboxJoystick(2);
    hatch->Start();
    buttonPressed = false;
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
    bool b = joystick -> GetButtonB();
    bool x = joystick -> GetButtonX();

    if (b)
    {
      hatch->Go();
    }

    if (x && !buttonPressed)
    {
      hatch->BottomPistonsOut();
      buttonPressed = true;
    }

    else if (!x && buttonPressed)
      buttonPressed = false;
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