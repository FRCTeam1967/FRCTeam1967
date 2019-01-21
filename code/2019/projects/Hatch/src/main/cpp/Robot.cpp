#include "frc/WPILib.h"
//#include "ctre/Phoenix.h"
#include <iostream>
#include <string>
#include <frc/smartdashboard/SmartDashboard.h>
#include <jankyXboxJoystick.h>
#include "HatchIntake.h"

#define LEFTPISTON 4
#define RIGHTPISTON 4

using namespace std;
using namespace frc;

class Robot : public frc::TimedRobot {
  HatchIntake*hatch;
  jankyXboxJoystick*joystick;
  
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
    hatch = new HatchIntake(LEFTPISTON, RIGHTPISTON);
    joystick = new jankyXboxJoystick(2);
    hatch->Start();
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
    if (joystick -> GetButtonB())
    {
      printf("button pressed \n");
      hatch->Go();
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