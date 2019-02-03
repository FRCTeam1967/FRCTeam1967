#include "frc/WPILib.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include <jankyXboxJoystick.h>
#include "ctre/Phoenix.h"
#include "ctre/phoenix/motorcontrol/SensorCollection.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include <math.h>
#include <frc/Encoder.h>
#include "jankyTask.h"
#include "ElevatorMech.h"

#define L_MOTOR_CHANNEL 6
#define R_MOTOR_CHANNEL 1
#define GAME_JOYSTICK_CHANNEL 2

class Robot : public frc::TimedRobot {
  WPI_TalonSRX * fMotor;
  WPI_TalonSRX * rMotor;
  ElevatorMech * elevator;
  jankyXboxJoystick * joystick;
  bool hatchPistonsIn = false;

  public:
  //constructor
  Robot(){
    fMotor = NULL;
    rMotor = NULL;
    elevator = NULL;
    joystick = NULL;
  }

  //deconstructor
  ~Robot(){
    delete fMotor;
    delete rMotor;
    delete elevator;
    delete joystick;

  }
  
  virtual void RobotInit() override {
    joystick = new jankyXboxJoystick(2);
    elevator = new ElevatorMech(L_MOTOR_CHANNEL, R_MOTOR_CHANNEL, 1, 2); //lim switch channels unknown atm
    elevator -> StartUpInit();
  }

  virtual void AutonomousInit() override {
  
  }

  virtual void AutonomousPeriodic() override {
    
  }

  virtual void TeleopInit() override {

  }

  virtual void TeleopPeriodic() override {
    elevator -> ConditionalRun();
    
    bool buttonB = joystick -> GetButtonB();
    if (buttonB){
      elevator -> RocketLowCargoHeight();
    }

    bool buttonA = joystick -> GetButtonA();
    if (buttonA){
      elevator -> ShipCargoHeight();
    }

    bool buttonY = joystick -> GetButtonY(); 
    if (buttonY){
      elevator -> RocketHighCargoHeight();
    }

    bool buttonX = joystick -> GetButtonX();
    if (buttonX){
      elevator -> GroundHeight();
    }

    bool buttonLB = joystick -> GetButtonLB();
    if (buttonLB){
      elevator -> RocketLowHatchHeight();
    }

    bool buttonStart = joystick -> GetButtonStart();
    if (buttonStart){
      elevator -> HPHeight();
    }

    bool buttonRB = joystick -> GetButtonRB();
    if (buttonRB){
      elevator -> RocketHighHatchHeight();
    }
  }

  virtual void TestPeriodic() override {

  }

private:
};
int main() { 
  return frc::StartRobot<Robot>(); 
}