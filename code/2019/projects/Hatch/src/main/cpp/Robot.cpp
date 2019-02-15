#include "frc/WPILib.h"
//#include "ctre/Phoenix.h"
#include <iostream>
#include <string>
#include <frc/smartdashboard/SmartDashboard.h>
#include <jankyXboxJoystick.h>
#include "HatchIntake.h"

#define TOP_PISTON 4
#define CARGO_PISTON 6
#define CARGO_MOTOR_CHANNEL 2

using namespace std;
using namespace frc;

class Robot : public frc::TimedRobot {
  HatchIntake*hatch;
  jankyXboxJoystick*joystick;
  WPI_TalonSRX*cargoMotor;
  bool buttonPressed;

  public:
  //constructor
  Robot()
  {
    hatch = NULL;
    joystick = NULL;
    cargoMotor = NULL;
    hatch->Start();
  }
  //deconstructor
  ~Robot()
  {
    delete hatch;
    delete joystick;
    delete cargoMotor;
  }
  
  virtual void RobotInit() override
  {
    hatch = new HatchIntake(TOP_PISTON, CARGO_PISTON);
    joystick = new jankyXboxJoystick(2);
    cargoMotor = new WPI_TalonSRX(CARGO_MOTOR_CHANNEL);
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
    cargoMotor -> ConfigSelectedFeedbackSensor(Analog, 0, 0);
  }

  virtual void TeleopPeriodic() override
  {
    int hatchDistance = cargoMotor->GetSensorCollection().GetAnalogIn();
    bool b = joystick -> GetButtonB();
    bool x = joystick -> GetButtonX();
    bool pistonOut;

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

    pistonOut = hatch->GetPistonStatus();
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