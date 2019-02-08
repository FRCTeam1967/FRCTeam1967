#include "frc/WPILib.h"
//#include "ctre/Phoenix.h"
#include <iostream>
#include <string>
#include <frc/smartdashboard/SmartDashboard.h>
#include <jankyXboxJoystick.h>
#include "HatchIntake.h"

<<<<<<< HEAD
#define TOP_PISTONS 6
#define CARGO_PISTON 4
=======
#define TOP_PISTON 4
#define CARGO_PISTON 6
#define HATCH_ULTRASON_CHANNEL 2
>>>>>>> 0e277e0... - added ultrasonic sensor analog input channel to code

using namespace std;
using namespace frc;

class Robot : public frc::TimedRobot {
  HatchIntake*hatch;
  jankyXboxJoystick*joystick;
  AnalogInput*hatchDetector;
  bool buttonPressed;

  public:
  //constructor
  Robot()
  {
    hatch = NULL;
    joystick = NULL;
    hatchDetector = NULL;
    hatch->Start();
  }
  //deconstructor
  ~Robot()
  {
    delete hatch;
    delete joystick;
    delete hatchDetector;
  }
  
  virtual void RobotInit() override
  {
    hatch = new HatchIntake(TOP_PISTONS, CARGO_PISTON);
    joystick = new jankyXboxJoystick(2);
    hatchDetector = new AnalogInput(HATCH_ULTRASON_CHANNEL);
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
    int hatchDistance = hatchDetector -> GetValue();
    bool b = joystick -> GetButtonB();
    bool x = joystick -> GetButtonX();

    SmartDashboard::PutNumber("Distance to hatch panel", hatchDistance);

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