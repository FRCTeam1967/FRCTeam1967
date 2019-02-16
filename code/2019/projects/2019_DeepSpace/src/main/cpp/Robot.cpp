#include "frc/WPILib.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "AutoDrive.h"
#include "VisionStateMachine.h"
#include "DifferentialDriveSegment.h"
#include "DifferentialTurnSegment.h"
#include "jankyDrivestick.h"
#include "jankyXboxJoystick.h"
#include "CargoManip.h"
#include "ctre/Phoenix.h" 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "HatchIntake.h"
#include "ElevatorMech.h"

#define ROBOT_2019 //CHANGE THIS BASED ON THE ROBOT

//Motors
#ifdef ROBOT_2019
#define FRONT_LEFT_MOTOR_CHANNEL 1
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 4
#define REAR_RIGHT_MOTOR_CHANNEL 3
#define MOTOR_ROLL_CHANNEL 6 
#define MOTOR_PIVOT_CHANNEL 7 // might be switched w/ motor_roll_channel
#define L_ELEVATOR_MOTOR_CHANNEL 0
#define R_ELEVATOR_MOTOR_CHANNEL 5 
#define ELEVATOR_LIM_SWITCH_BOTTOM_CHANNEL 2 //might be 0
#define ELEVATOR_LIM_SWITCH_TOP_CHANNEL 3 //might be 0
#define TOP_HATCH_PISTON 4
#define BOTTOM_CARGO_PISTON 5
#define PISTON_FRONT_CHANNEL 6 //change for real robot
#define PISTON_BACK_CHANNEL 7 //change for real robot

#else //for Lola
#define FRONT_LEFT_MOTOR_CHANNEL 3
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 5
#define REAR_RIGHT_MOTOR_CHANNEL 4
#define MOTOR_ROLL_CHANNEL 8
#define MOTOR_PIVOT_CHANNEL 7
#define L_ELEVATOR_MOTOR_CHANNEL 6
#define R_ELEVATOR_MOTOR_CHANNEL 1
#define ELEVATOR_LIM_SWITCH_BOTTOM_CHANNEL 2 //doesn't exist on Lola
#define ELEVATOR_LIM_SWITCH_TOP_CHANNEL 3 //doesn't exist on Lola
#define TOP_HATCH_PISTON 4 // claw open and close
#define BOTTOM_CARGO_PISTON 5 //doesn't exist on Lola
#define PISTON_FRONT_CHANNEL 6 //doesn't exist on Lola
#define PISTON_BACK_CHANNEL 7 //doesn't exist on Lola
#endif

//Joysticks
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1
#define XBOX_CONTROLLER 2
#define XBOX_CONTROLLER_2 3

using namespace std;
using namespace frc;

class Robot : public frc::TimedRobot {
  public:
    WPI_TalonSRX * flmotor;
    WPI_TalonSRX * rlmotor;
    WPI_TalonSRX * frmotor;
    WPI_TalonSRX * rrmotor;
    //WPI_TalonSRX * cargoPistonMotor;
    frc::SpeedControllerGroup * leftDrive;
    frc::SpeedControllerGroup * rightDrive;
    frc::DifferentialDrive * drive;
    jankyDrivestick * left;
    jankyDrivestick * right;
    jankyXboxJoystick * joystick;
    jankyXboxJoystick * joystick2;
    frc::ADXRS450_Gyro * gyro;
    VisionStateMachine * vision;
    CargoManip * cargomanip;
    HatchIntake * hatch;
    ElevatorMech * elevator;
    //Solenoid*fpiston;
    //Solenoid*bpiston;
    bool buttonPressed;
    bool hatchPistonsIn;
    string setHeight;
    cs::UsbCamera * driveCam;
    bool StartPressed, BackPressed;
    
  //constructor
  Robot(){
    flmotor = NULL;
    rlmotor = NULL;
    frmotor = NULL;
    rrmotor = NULL;
    //cargoPistonMotor = NULL;
    leftDrive = NULL;
    rightDrive = NULL;
    drive = NULL;
    left = NULL;
    right = NULL;
    joystick = NULL;
    joystick2 = NULL;
    gyro = NULL;
    vision = NULL;
    cargomanip = NULL;
    hatch = NULL;
    elevator = NULL;
    driveCam = NULL;
    //fpiston = NULL;
    //bpiston = NULL;
  }

  //deconstructor
  ~Robot(){
    delete flmotor;
    delete rlmotor;
    delete frmotor;
    delete rrmotor;
    //delete cargoPistonMotor;
    delete leftDrive;
    delete rightDrive;
    delete drive;
    delete left;
    delete right; 
    delete joystick;
    delete joystick2;
    delete gyro;
    delete vision;
    delete cargomanip;
    delete hatch;
    delete elevator; 
    delete driveCam;
    //delete fpiston;
    //delete bpiston;
  }

  virtual void RobotInit() override {
    //Run drive team camera
    cs::UsbCamera driveCam = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
    driveCam.SetResolution(640,480);
    driveCam.SetFPS(25); //can't go above 30

    //Motors for driving
    flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
    rlmotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
    frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
    rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
    //cargoPistonMotor = new WPI_TalonSRX(BOTTOM_CARGO_PISTON);    
    flmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
		frmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
    leftDrive = new frc::SpeedControllerGroup(*flmotor, *rlmotor);
    rightDrive = new frc::SpeedControllerGroup(*frmotor, *rrmotor);
    joystick = new jankyXboxJoystick(XBOX_CONTROLLER);
    joystick2 = new jankyXboxJoystick(XBOX_CONTROLLER_2);
    drive = new frc::DifferentialDrive(*leftDrive, *rightDrive);
    left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
    right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);
    gyro = new frc::ADXRS450_Gyro(frc::SPI::Port::kOnboardCS0); //gyro didn't work; maybe try other port options
    vision = new VisionStateMachine(drive, gyro, &(flmotor->GetSensorCollection()), &(frmotor->GetSensorCollection()), flmotor, frmotor);
    cargomanip = new CargoManip(MOTOR_ROLL_CHANNEL, MOTOR_PIVOT_CHANNEL);
    hatch = new HatchIntake(TOP_HATCH_PISTON, BOTTOM_CARGO_PISTON);
    elevator = new ElevatorMech(L_ELEVATOR_MOTOR_CHANNEL, R_ELEVATOR_MOTOR_CHANNEL, ELEVATOR_LIM_SWITCH_BOTTOM_CHANNEL, ELEVATOR_LIM_SWITCH_TOP_CHANNEL);
    //fpiston = new Solenoid(10, PISTON_FRONT_CHANNEL);
    //bpiston = new Solenoid(10, PISTON_BACK_CHANNEL);    
    
    drive->SetSafetyEnabled(false); 
    gyro->Calibrate();
    buttonPressed = false;

    StartPressed=false;
    BackPressed=false;

    cargomanip -> StartInit();
    hatch -> Start();
    elevator -> Start();
  }

  virtual void AutonomousInit() override {
    gyro->Reset();
    hatch->BottomPistonsOut(); //THIS IS SO THAT THE ELEVATOR CAN GO UP
    flmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
    frmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
    flmotor->SetSelectedSensorPosition(0, 0, 10);
    flmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
    frmotor->SetSelectedSensorPosition(0, 0, 10);
    frmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
  }

  virtual void AutonomousPeriodic() override {
    if(left->Get3()){ //replace later to use the button board
      vision->StartSequenceTest(); //test mode 
    }
    else if(vision->IsIdle()){ 
      drive->TankDrive(-joystick->GetLeftYAxis(), -joystick->GetRightYAxis());
      //drive->TankDrive(-left->GetY(), -right->GetY());
    }

    frc::SmartDashboard::PutNumber("angle", gyro->GetAngle());
    if(left->Get2()){
      vision->Cancel();
    }

  }

  virtual void TeleopInit() override {
    //cargoPistonMotor -> ConfigSelectedFeedbackSensor(Analog, 0, 0);
  }

  virtual void TeleopPeriodic() override {
  //drive TODO: add vision logic here
    //drive->TankDrive(-left->GetY(), -right->GetY());
    drive->TankDrive(-joystick->GetLeftYAxis(), -joystick->GetRightYAxis());

  //TODO: rename buttons according to function
  //buttons -- joystick 1: hatch + cargo + chassis pistons, joystick 2: chassis + elevator
    bool buttonB = joystick -> GetButtonB(); //cargo rollers out
    bool buttonX = joystick -> GetButtonX(); //cargo rollers in
    bool buttonA = joystick -> GetButtonA(); //cargo mech into robot
    bool buttonY = joystick -> GetButtonY(); //cargo mech out of robot
    //TODO: CHANGE CHASSIS PISTON BUTTONS TO BE ON JANKYDRIVESTICK
    bool buttonStart = joystick -> GetButtonStart(); //chassis piston?
    bool buttonBack = joystick -> GetButtonBack(); //chassis piston?
    bool buttonLB = joystick -> GetButtonLB(); //hatch bottom pistons
    bool buttonRB = joystick -> GetButtonRB(); //hatch in-out pistons
    float rightVal = joystick2 -> GetRightYAxis(); //manual elevator

    bool buttonB2 = joystick2 -> GetButtonB(); //rocket low hatch height + hp station + cargo ship hatch
    bool buttonX2 = joystick2 -> GetButtonX(); //rocket medium cargo height
    bool buttonA2 = joystick2 -> GetButtonA(); //rocket high cargo height
    bool buttonY2 = joystick2 -> GetButtonY(); //rocket low cargo height
    bool buttonStart2 = joystick2 -> GetButtonStart(); //cargo ship cargo height
    bool buttonBack2 = joystick2 -> GetButtonBack(); //rocket ship high hatch
    bool buttonLB2 = joystick2 -> GetButtonLB(); //ground height
    bool buttonRB2 = joystick2 -> GetButtonRB(); //hard stop

  //ELEVATOR
    //conditional run
    hatchPistonsIn = hatch -> GetPistonStatus();

    frc::SmartDashboard::PutBoolean("Bottom Piston In:", hatchPistonsIn);

    if (hatchPistonsIn){
      elevator -> Pause();
    }
    else {
      elevator -> Start();
    }
    
    //hard stop, overrides everything
    if (buttonRB2){ // stop button overrides everything 
      elevator -> ElevatorMotorStop();
    }

    //presets
    else if (buttonStart2){ 
      elevator -> ShipCargoHeight();
    }

    else if (buttonY2){
      elevator -> RocketLowCargoHeight();
    }

    else if (buttonA2){
      elevator -> RocketHighCargoHeight();
    }

    else if (buttonB2){
      elevator -> RocketLowHatchHeight();
    }
   
    else if (buttonBack2){
      elevator -> RocketHighHatchHeight();
    }  
   
    else if (buttonX2){
      elevator -> RocketMedCargoHeight();
    }
   
    else if (buttonLB2){
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


  //cargo
    if (buttonB){
      cargomanip -> RollersOut();
    }
    else if (buttonX){
      cargomanip -> RollersIn();
    }
    else {
      cargomanip -> RollersStop();
    }

    if (buttonA){
      cargomanip -> CargoMechInRobot(); 
    }
    else if (buttonY){
      cargomanip -> CargoMechOutRobot();
    }
    else {
      cargomanip -> CargoMechStop();
    }

  //hatch
    //int hatchDistance = cargoPistonMotor->GetSensorCollection().GetAnalogIn();
    //bool pistonOut;
    //SmartDashboard::PutNumber("Distance to hatch panel", hatchDistance);

    if (buttonRB)
    {
      hatch->Go();
    }

    if (buttonLB && !buttonPressed)
    {
      hatch->BottomPistonsSwitch();
      buttonPressed = true;
    }
    else if (!buttonLB && buttonPressed){
      buttonPressed = false;
    }

    //chassis lifting pistons
    //front piston
    /*if (buttonStart && StartPressed==false)
    {
      if(fpiston->Get()==true)
        fpiston -> Set(false);

      else if(fpiston->Get()==false)
        fpiston -> Set(true);
      StartPressed=true;
    }
    else if (!buttonStart && StartPressed==true)
      StartPressed=false;

    //back pistons
    if (buttonBack && BackPressed==false)
    {
      if(bpiston->Get()==true)
        bpiston -> Set(false);
      
      else
        bpiston -> Set(true);
      BackPressed=true;
    }
    else if (!buttonBack && BackPressed==true)
      BackPressed=false;*/
    
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