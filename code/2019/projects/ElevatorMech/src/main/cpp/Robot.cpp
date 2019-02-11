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

#define L_MOTOR_CHANNEL 6 // 2 on realbot
#define R_MOTOR_CHANNEL 1 //4 on realbot
#define GAME_JOYSTICK_CHANNEL 2

class Robot : public frc::TimedRobot {
  WPI_TalonSRX * fMotor;
  WPI_TalonSRX * rMotor;
  ElevatorMech * elevator;
  jankyXboxJoystick * joystick;
  bool hatchPistonsIn = false;
  string setHeight;

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
    bool buttonB = joystick -> GetButtonB();
    bool buttonA = joystick -> GetButtonA();
    bool buttonY = joystick -> GetButtonY(); 
    bool buttonX = joystick -> GetButtonX();
    bool buttonLB = joystick -> GetButtonLB();
    bool buttonStart = joystick -> GetButtonStart();
    bool buttonRB = joystick -> GetButtonRB();
    float rightVal = joystick -> GetRightYAxis();
    bool buttonBack = joystick -> GetButtonBack();

    //elevator -> Run();


  
   if (buttonRB){ // stop button overrides everything 
     elevator -> ElevatorMotorStop();
   }
   
   // presets - overrides manual control
   else if (buttonX){ 
      elevator -> ShipCargoHeight();
   }
   else if (buttonY){
      elevator -> RocketLowCargoHeight();
   }
   else if (buttonA){
     elevator -> HPHeight();
   }
   else if (buttonB){
     elevator -> RocketLowHatchHeight();
   }
   else if (buttonBack){
     elevator -> ShipHatchHeight();
   }
   else if (buttonStart){
     elevator -> RocketMedCargoHeight();
   }
   else if (buttonLB){
     elevator -> GroundHeight();
   }

   //manual controls
   else { 
     if (rightVal >= 0.2){
      elevator -> ElevatorMotorDown();
      setHeight = "None";
    }
    else if (rightVal <= -0.2){
      elevator -> ElevatorMotorUp();
      setHeight = "None";
    }
    else if (rightVal < 0.2 && rightVal > -0.2){
      elevator -> ElevatorMotorStop();
      setHeight = "None";
    }
   }





    /*if (buttonA){
      elevator -> ElevatorMotorUp();
    }
    else if (buttonB){
      elevator -> ElevatorMotorDown();
    }
    else {
      elevator -> ElevatorMotorStop();
    }*/ 


    /*(if (buttonX){
      elevator -> ShipCargoHeight();
    }/*

    else if (buttonY){
      elevator -> RocketMedCargoHeight();
    }

    else if (buttonLB){
      elevator -> GroundHeight();
    }

    if (buttonRB){
      elevator -> ElevatorMotorStop();
    }

    /*else if (buttonX){
      elevator -> GroundHeight();
    }*/

    /*if (button){
      elevator -> HPHeight();
    }*/

    /*if (buttonY){
      elevator -> ElevatorMotorStop();
    }*/

    /*else if (buttonRB){
      elevator -> ElevatorMotorStop();
    }*/
  }

  virtual void TestPeriodic() override {

  }

private:
};
int main() { 
  return frc::StartRobot<Robot>(); 
}