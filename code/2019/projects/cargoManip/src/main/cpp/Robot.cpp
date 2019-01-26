#include "frc/WPILib.h" 
#include <iostream> 
#include <frc/smartdashboard/SmartDashboard.h>
#include "CargoManip.h"
#include "ctre/Phoenix.h" 
#include <math.h>
#include <hal/Encoder.h>
#include "hal/Constants.h"
#include <jankyXboxJoystick.h>

#define MOTOR_ROLL_CHANNEL 8
#define MOTOR_PIVOT_CHANNEL 7
#define LIM_SWITCH_INSIDE_CHANNEL 2 //inaccurate
#define LIM_SWITCH_OUTSIDE_CHANNEL 4 //inaccurate
#define GAME_JOYSTICK_CHANNEL 2
// placeholder channels

class Robot : public frc::TimedRobot {
    CargoManip * cargomanip;
    jankyXboxJoystick * joystick;
    frc::Encoder * pivotencoder; 
  public:
  //constructor
  Robot(){
    cargomanip = NULL;
    joystick = NULL;
  }
  //deconstructor
  ~Robot(){
    delete cargomanip;
    delete joystick;
  }
  
  virtual void RobotInit() override{
    cargomanip = new CargoManip(MOTOR_ROLL_CHANNEL, MOTOR_PIVOT_CHANNEL, LIM_SWITCH_INSIDE_CHANNEL, LIM_SWITCH_OUTSIDE_CHANNEL); // placeholder motor/lim switch channels
    joystick = new jankyXboxJoystick(2);
  }

  virtual void AutonomousInit() override{
  
  }

  virtual void AutonomousPeriodic() override{
    
  }

  virtual void TeleopInit() override{

  }

  virtual void TeleopPeriodic() override{
    bool buttonB = joystick -> GetButtonB();
    if (buttonB){
      cargomanip -> RollersIn();
    }

    bool buttonA = joystick -> GetButtonA();
    if (buttonA){
      cargomanip -> RollersOut();
    }

    bool buttonLB = joystick -> GetButtonLB(); // TURN MOTOR OFF 
    if (buttonLB){
      cargomanip -> CargoMechInRobot(); //rename to cargomechinrobot
    }

    bool buttonRB = joystick -> GetButtonRB();
    if (buttonRB){
      cargomanip -> CargoMechOutRobot();
    }

    bool buttonX = joystick -> GetButtonX();
    if (buttonX){
      cargomanip -> getCargoMechPosition();
    }

    bool buttonY = joystick -> GetButtonY();
    if (buttonY){
      cargomanip -> CargoMechStop();
    }

    bool buttonBack = joystick -> GetButtonBack();
    if (buttonBack){
      cargomanip -> GetLimSwitchOutside();
    }

    bool buttonStart = joystick -> GetButtonStart();
    if (buttonStart){
      cargomanip -> GetLimSwitchInside();
    }

  }

// WHAT HAPPENED TO RT AND LT SIS?????

  virtual void TestPeriodic() override{

  }

private:
};

int main() 
{ 
  return frc::StartRobot<Robot>(); 
}