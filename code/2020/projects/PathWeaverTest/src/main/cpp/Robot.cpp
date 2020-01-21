#include "frc/WPILib.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include <frc/trajectory/Trajectory.h>
#include <frc/trajectory/TrajectoryUtil.h>

#define PATHWEAVER_JSON_DIRECTORY "Pathweaver/output/GetBallFromTrench.wpilib.json"

using namespace std;
using namespace frc;

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
    Trajectory trajectory = TrajectoryUtil::FromPathweaverJson(PATHWEAVER_JSON_DIRECTORY);

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