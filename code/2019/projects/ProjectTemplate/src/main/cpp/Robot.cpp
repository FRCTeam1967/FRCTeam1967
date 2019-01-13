#include "frc/WPILib.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>

class Robot : public frc::TimedRobot {
  public:
  //constructor
  Robot()
  {

  }
  //deconstructor
  ~Robot()
  {

  }
  
  virtual void RobotInit() override
  {

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