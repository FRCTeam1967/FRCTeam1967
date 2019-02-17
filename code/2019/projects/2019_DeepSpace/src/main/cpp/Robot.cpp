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
#include "Settings.h"

using namespace std;
using namespace frc;

//Joysticks
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1
#define XBOX_CONTROLLER 2
#define XBOX_CONTROLLER_2 3

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
    bool hatchPistonsOut;
    string setHeight;
    bool chassisFrontButtonPressed, chassisBackButtonPressed;
    
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
    //delete fpiston;
    //delete bpiston;
  }

  virtual void RobotInit() override {
    //Run drive team camera
    cs::UsbCamera driveCam;
    driveCam = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
    driveCam.SetResolution(360,240);
    driveCam.SetFPS(15);

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

    chassisFrontButtonPressed=false;
    chassisBackButtonPressed=false;

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
      drive->TankDrive(-left->GetY(), -right->GetY());
    }

    frc::SmartDashboard::PutNumber("angle", gyro->GetAngle());
    if(left->Get2()){
      vision->Cancel();
    }
    //TODO: ADD ALL THE GC LOGIC HERE SO THAT IT CAN BE USED WHEN OPERATOR CONTROLLED
  }

  virtual void TeleopInit() override {
    //cargoPistonMotor -> ConfigSelectedFeedbackSensor(Analog, 0, 0);
    hatch->BottomPistonsOut();
  }

  virtual void TeleopPeriodic() override {
  //drive TODO: add vision logic here
    drive->TankDrive(-left->GetY(), -right->GetY());

  //TODO: rename buttons according to function
  //buttons -- joystick 1: hatch + cargo + chassis pistons, joystick 2: chassis + elevator
    bool rollersOut = joystick -> GetButtonB();
    bool rollersIn = joystick -> GetButtonX(); 
    bool cargoIn = joystick -> GetButtonA(); 
    bool cargoOut = joystick -> GetButtonY(); 
    //TODO: CHANGE CHASSIS PISTON BUTTONS TO BE ON JANKYDRIVESTICK
    bool chassisFront = right->Get3();
    bool chassisBack = right->Get2(); 
    bool cargoPistons = joystick -> GetButtonLB(); 
    bool hatchPistons = joystick -> GetButtonRB(); 
    float manualElevator = joystick2 -> GetRightYAxis(); 

    bool rocketLowHatchHPShipHatch = joystick2 -> GetButtonB(); //rocket low hatch height + hp station + cargo ship hatch god i hate this name so much
    bool rocketMedCargo = joystick2 -> GetButtonX(); 
    bool rocketHighCargo = joystick2 -> GetButtonA();
    bool rocketLowCargo = joystick2 -> GetButtonY(); 
    bool cargoShipCargo = joystick2 -> GetButtonStart(); 
    bool rocketHighHatch = joystick2 -> GetButtonBack(); 
    bool groundHeight = joystick2 -> GetButtonLB(); 
    bool elevatorStop = joystick2 -> GetButtonRB();

  //ELEVATOR
    //conditional run
    hatchPistonsOut = hatch -> GetPistonStatus();

    frc::SmartDashboard::PutBoolean("Bottom Piston Out:", hatchPistonsOut);

    if (!hatchPistonsOut){
      elevator -> Pause();
    }
    else {
      elevator -> Start();
    }
    
    //hard stop, overrides everything
    if (elevatorStop){ // stop button overrides everything 
      elevator -> ElevatorMotorStop();
    }
    //presets
    else if (cargoShipCargo){ 
      elevator -> ShipCargoHeight();
    }

    else if (rocketLowCargo){
      elevator -> RocketLowCargoHeight();
    }

    else if (rocketHighCargo){
      elevator -> RocketHighCargoHeight();
    }

    else if (rocketLowHatchHPShipHatch){
      elevator -> RocketLowHatchHeight();
    }
   
    else if (rocketHighHatch){
      elevator -> RocketHighHatchHeight();
    }  
   
    else if (rocketMedCargo){
      elevator -> RocketMedCargoHeight();
    }
   
    else if (groundHeight){
      elevator -> GroundHeight();
    }

   //manual controls
    else { 
      if (manualElevator >= 0.2){
        elevator -> ElevatorMotorDown();
        setHeight = "None";
      }
      else if (manualElevator <= -0.2){
        elevator -> ElevatorMotorUp();
        setHeight = "None";
      }
      else if (manualElevator < 0.2 && manualElevator > -0.2){
        elevator -> ElevatorMotorStop();
        setHeight = "None";
      }
    }


  //cargo
    if (rollersOut){
      cargomanip -> RollersOut();
    }
    else if (rollersIn){
      cargomanip -> RollersIn();
    }
    else {
      cargomanip -> RollersStop();
    }

    if (cargoIn){
      cargomanip -> CargoMechInRobot(); 
    }
    else if (cargoOut){
      cargomanip -> CargoMechOutRobot();
    }
    else {
      cargomanip -> CargoMechStop();
    }

  //hatch
    //int hatchDistance = cargoPistonMotor->GetSensorCollection().GetAnalogIn();
    //bool pistonOut;
    //SmartDashboard::PutNumber("Distance to hatch panel", hatchDistance);

    if (hatchPistons)
    {
      hatch->Go();
    }

    if (cargoPistons && !buttonPressed)
    {
      hatch->BottomPistonsSwitch();
      buttonPressed = true;
    }
    else if (!cargoPistons && buttonPressed){
      buttonPressed = false;
    }

    //chassis lifting pistons
    //front piston
    /*if (chassisFront && chassisFrontButtonPressed==false)
    {
      if(fpiston->Get()==true)
        fpiston -> Set(false);

      else if(fpiston->Get()==false)
        fpiston -> Set(true);
      chassisFrontButtonPressed=true;
    }
    else if (!chassisFront && chassisFrontButtonPressed==true)
      chassisStartButtonPressed=false;

    //back pistons
    if (chassisBack && chassisBackButtonPressed==false)
    {
      if(bpiston->Get()==true)
        bpiston -> Set(false);
      
      else
        bpiston -> Set(true);
      chassisBackButtonPressed=true;
    }
    else if (!chassisBack && chassisBackButtonPressed==true)
      chassisBackButtonPressed=false;*/
    
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