// Includes
#include "Settings.h"
#include "frc/WPILib.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "AutoDrive.h"
#include "VisionStateMachine.h"
#include "DifferentialDriveSegment.h"
#include "DifferentialTurnSegment.h"
#include "jankyDrivestick.h"
#include "jankyXboxJoystick.h"
#include "JankyButtonPanel.h"
#include "CargoManip.h"
#include "ctre/Phoenix.h" 
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include "HatchIntake.h"
#include "ElevatorMech.h"
#include "LightsController.h"
#include "LightsKey.h"

using namespace std;
using namespace frc;

//Joysticks
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1
#define JANKY_BUTTON_PANEL //comment or uncomment depending on if using xbox vs button panel
#ifdef JANKY_BUTTON_PANEL
#define CARGO_SIDE_CHANNEL 3
#define HATCH_SIDE_CHANNEL 4
#else
#define XBOX_CONTROLLER 2
#define XBOX_CONTROLLER_2 3
#endif

class Robot : public frc::TimedRobot {
  public:
    WPI_TalonSRX * flmotor;
    WPI_TalonSRX * frmotor;
    #ifdef JANKY_BOT_2019
    WPI_VictorSPX * rlmotor;
    WPI_VictorSPX * rrmotor;
    #else
    WPI_TalonSRX * rlmotor;
    WPI_TalonSRX * rrmotor;
    #endif
    frc::SpeedControllerGroup * leftDrive;
    frc::SpeedControllerGroup * rightDrive;
    frc::DifferentialDrive * drive;
    jankyDrivestick * left;
    jankyDrivestick * right;
    frc::ADXRS450_Gyro * gyro;
    VisionStateMachine * vision;
    CargoManip * cargomanip;
    HatchIntake * hatch;
    ElevatorMech * elevator;
    Solenoid*fpiston;
    Solenoid*bpiston;
    bool buttonPressedOpen;
    bool buttonPressedClose;
    bool hatchPistonsOut;
    bool hatchPistonDeployed;
    string setHeight;
    bool cargoInPressed;
    bool cargoOutPressed;
    bool chassisFrontButtonPressed, chassisBackButtonPressed;
    LightsController*leds;
    float distance;
    float horizontalOffset;

    #ifdef JANKY_BUTTON_PANEL
    jankyButtonPanel * buttonpanel;
    #else    
    jankyXboxJoystick * joystick;
    jankyXboxJoystick * joystick2;
    #endif
    
    //constructor
    Robot(){
      flmotor = NULL;
      rlmotor = NULL;
      frmotor = NULL;
      rrmotor = NULL;
      leftDrive = NULL;
      rightDrive = NULL;
      drive = NULL;
      left = NULL;
      right = NULL;
      gyro = NULL;
      vision = NULL;
      cargomanip = NULL;
      hatch = NULL;
      elevator = NULL;
      fpiston = NULL;
      bpiston = NULL;
      leds = NULL;
      
      #ifdef JANKY_BUTTON_PANEL
      buttonpanel = NULL;
      #else    
      joystick = NULL;
      joystick2 = NULL;
      #endif
    }

    //deconstructor
    ~Robot(){
      delete flmotor;
      delete rlmotor;
      delete frmotor;
      delete rrmotor;
      delete leftDrive;
      delete rightDrive;
      delete drive;
      delete left;
      delete right; 
      delete gyro;
      delete vision;
      delete cargomanip;
      delete hatch;
      delete elevator; 
      delete fpiston;
      delete bpiston;
      delete leds;
      
      #ifdef JANKY_BUTTON_PANEL
      delete buttonpanel;
      #else    
      delete joystick;
      delete joystick2;
      #endif
    }

    virtual void RobotInit() override {
      #ifdef DRIVE_TEAM_CAM
      //Run drive team camera
      //frc::CameraServer cs;
      //driveCam = cs::UsbCamera("USB Camera 0", 0);
      cs::UsbCamera driveCam;
      //driveCam.AddCamera(cam);
      driveCam = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
      driveCam.SetResolution(160,120);
      driveCam.SetFPS(5);
      driveCam.GetProperty("compression").Set(100);
      #endif

      //Motors for driving
      flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
      frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
      #ifdef JANKY_BOT_2019
      rlmotor = new WPI_VictorSPX(REAR_LEFT_MOTOR_CHANNEL);
      rrmotor = new WPI_VictorSPX(REAR_RIGHT_MOTOR_CHANNEL);
      #else
      rlmotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
      rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
      #endif
      flmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
      frmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
      leftDrive = new frc::SpeedControllerGroup(*flmotor, *rlmotor);
      rightDrive = new frc::SpeedControllerGroup(*frmotor, *rrmotor);
      drive = new frc::DifferentialDrive(*leftDrive, *rightDrive);
      left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
      right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);
      gyro = new frc::ADXRS450_Gyro(frc::SPI::Port::kOnboardCS0); //gyro didn't work; maybe try other port options
      vision = new VisionStateMachine(drive, gyro, &(flmotor->GetSensorCollection()), &(frmotor->GetSensorCollection()), flmotor, frmotor, rlmotor, rrmotor);
      cargomanip = new CargoManip(MOTOR_ROLL_CHANNEL, MOTOR_PIVOT_CHANNEL);
      hatch = new HatchIntake(TOP_HATCH_PISTON, BOTTOM_CARGO_PISTON);
      elevator = new ElevatorMech(L_ELEVATOR_MOTOR_CHANNEL, R_ELEVATOR_MOTOR_CHANNEL, ELEVATOR_LIM_SWITCH_BOTTOM_CHANNEL, ELEVATOR_LIM_SWITCH_TOP_CHANNEL);
      fpiston = new Solenoid(10, PISTON_FRONT_CHANNEL);
      bpiston = new Solenoid(10, PISTON_BACK_CHANNEL);
      leds = new LightsController(3);      
      //Button Panel
      #ifdef JANKY_BUTTON_PANEL
      buttonpanel = new jankyButtonPanel(CARGO_SIDE_CHANNEL, HATCH_SIDE_CHANNEL);
      #else    
      joystick = new jankyXboxJoystick(XBOX_CONTROLLER);
      joystick2 = new jankyXboxJoystick(XBOX_CONTROLLER_2);
      #endif

      drive->SetSafetyEnabled(false); 
      gyro->Calibrate();
      buttonPressedOpen = false;
      buttonPressedClose = false;
      hatchPistonDeployed = false;
      cargoInPressed = false;
      cargoOutPressed = false;

      chassisFrontButtonPressed = false;
      chassisBackButtonPressed = false;

      //cargomanip -> StartInit();
      //hatch -> Start();
      elevator -> Start();
      elevator -> StartUpInit();
      elevator -> ResetEncoder();
    }

    virtual void RobotCode() 
    {
      // Vision Code
      if(left->Get3())
      { 
        vision->StartSequence();
      }
      else if(vision->IsIdle())
      { 
        drive->TankDrive(-left->GetY(), -right->GetY());
      }

      if(left->Get2())
      {
        vision->Cancel();
      }

      // Vision LEDs
      if(!vision->IsIdle())
      {
        leds->SetColor(GREEN, SOLID);
      }

      /*
      distance=SmartDashboard::GetNumber("Distance to Tape", -100);
      horizontalOffset=(SmartDashboard::GetNumber("Offset", -100)) + 10;
      if((distance != -1) && (distance != -100) && (horizontalOffset != -100))
        leds->SetColor(GREEN, FLASHING);
      else
        leds->SetColor(RED, SOLID);
      */

      // Buttons -- joystick 1: hatch + cargo + chassis pistons, joystick 2: chassis + elevator
      bool chassisFront = right->Get3();
      bool chassisBack = right->Get2(); 
      bool cargoGround = right -> Get10();
    
      #ifdef JANKY_BUTTON_PANEL
      float rollers = buttonpanel -> GetRollersYAxis();
      bool cargoInBot = buttonpanel -> GetCargoIn();
      
      bool cargoHPAngle = buttonpanel -> GetCargoOut();

      bool pistonsOpen = buttonpanel -> GetBottomPistons();
      bool pistonsClose = buttonpanel -> GetTopPistons(); 

      // bool cargoPistons = buttonpanel -> GetBottomPistons();
      // bool hatchPistons = buttonpanel -> GetTopPistons(); 

      float manualElevator = buttonpanel -> GetElevatorYAxis();
      bool rocketLowHatchHPShipHatch = buttonpanel -> GetRocketHatchLow(); //rocket low hatch height + hp station + cargo ship hatch god i hate this name so much
      bool rocketMedCargo = buttonpanel -> GetRocketCargoMed(); 
      bool rocketHighCargo = buttonpanel -> GetRocketCargoHigh();
      bool rocketLowCargo = buttonpanel -> GetRocketCargoLow(); 
      bool cargoShipCargo = buttonpanel -> GetShipCargo(); 
      bool rocketMedHatch = buttonpanel -> GetRocketHatchMed();
      bool rocketHighHatch = buttonpanel -> GetRocketHatchHigh(); 
      bool groundHeight = buttonpanel -> GetGroundHeight(); 

      #else
      bool rollersOut = joystick -> GetButtonB();
      bool rollersIn = joystick -> GetButtonX(); 
      bool cargoIn = joystick -> GetButtonA(); 
      bool cargoOut = joystick -> GetButtonY(); 


      bool pistonsOpen = joystick -> GetButtonLB(); 
      bool pistonsClose = joystick -> GetButtonRB(); 

      // bool cargoPistons = joystick -> GetButtonLB(); 
      // bool hatchPistons = joystick -> GetButtonRB(); 

      float manualElevator = joystick2 -> GetRightYAxis(); 
      bool rocketLowHatchHPShipHatch = joystick2 -> GetButtonB(); //rocket low hatch height + hp station + cargo ship hatch god i hate this name so much
      bool rocketMedCargo = joystick2 -> GetButtonX(); 
      bool rocketHighCargo = joystick2 -> GetButtonA();
      bool rocketLowCargo = joystick2 -> GetButtonY(); 
      bool cargoShipCargo = joystick2 -> GetButtonStart(); 
      bool rocketHighHatch = joystick2 -> GetButtonBack(); 
      bool groundHeight = joystick2 -> GetButtonLB(); 
      bool elevatorStop = joystick2 -> GetButtonRB();
      #endif

      // Elevator
      hatchPistonsOut = hatch -> GetBottomPistonStatus();
      frc::SmartDashboard::PutBoolean("Bottom Piston Out:", hatchPistonsOut);
      elevator -> GetEncoderDistance();

      // if (!hatchPistonsOut){
      //   elevator -> Pause();
      // }
      // else {
      //   elevator -> Start();
      // }
      
      //hard stop, overrides everything

      /*if (elevatorStop){ // stop button overrides everything 
        elevator -> ElevatorMotorStop();
      }
      //presets
      else */

      // Elevator Preset Heights
      if (cargoShipCargo){ 
        //printf("cargo ship cargo button pressed \n");
        elevator -> ShipCargoHeight();
        if(!cargomanip->HasElevatorGoneUp())
        {
          cargomanip->SetElevatorVar(true);
        }
      }
      else if (rocketLowCargo){
        //printf("rocket low cargo button pressed \n");
        elevator -> RocketLowCargoHeight();
        if(!cargomanip->HasElevatorGoneUp())
        {
          cargomanip->SetElevatorVar(true);
        }
      }
      else if (rocketHighCargo){
        //printf("rocket high cargo button pressed \n");
        elevator -> RocketHighCargoHeight();
        if(!cargomanip->HasElevatorGoneUp())
        {
          cargomanip->SetElevatorVar(true);
        }
      }
      else if (rocketLowHatchHPShipHatch){
        //printf("rocket low hatch button pressed \n");
        elevator -> RocketLowHatchHeight();
        if(!cargomanip->HasElevatorGoneUp())
        {
          cargomanip->SetElevatorVar(true);
        }
      }
      else if (rocketHighHatch){
        //printf("rocket high hatch button pressed \n");
        elevator -> RocketHighHatchHeight();
        if(!cargomanip->HasElevatorGoneUp())
        {
          cargomanip->SetElevatorVar(true);
        }
      }  
      else if (rocketMedCargo){
        //printf("rocket medium cargo button pressed \n");
        elevator -> RocketMedCargoHeight();
        if(!cargomanip->HasElevatorGoneUp())
        {
          cargomanip->SetElevatorVar(true);
        }
      }
      else if (rocketMedHatch){
        //printf("rocket medium hatch button pressed \n");
        elevator -> RocketMedHatchHeight();
        if(!cargomanip->HasElevatorGoneUp())
        {
          cargomanip->SetElevatorVar(true);
        }
      }
      else if (groundHeight){
        //printf("ground button pressed \n");
        elevator -> GroundHeight();
      }
      else if(left->Get10())
      {
        elevator->UpHP();
      }
      else if(left->Get7())
      {
        elevator->DownHatch();
      }

      // Manual Elevator Controls
      else { 
        if (manualElevator <= -0.2){
          elevator -> ElevatorMotorDown();
          // leds->SetColor(DARK_BLUE, CHASING);
          //printf("elevator down triggered \n");
          // setHeight = "None";
        }
        else if (manualElevator >= 0.2){
          elevator -> ElevatorMotorUp();
          if(!cargomanip->HasElevatorGoneUp())
        {
          cargomanip->SetElevatorVar(true);
        }
          // leds->SetColor(RED, FLASHING);
          //printf("elevator up triggered \n");
          // setHeight = "None";
        }
        // else if (manualElevator < 0.2 && manualElevator > -0.2){
        //   elevator -> ElevatorMotorStop();
        //   // setHeight = "None";
        // }
      }

      // Cargo
      #ifdef JANKY_BUTTON_PANEL
        if (rollers <= -0.2){
          //printf("rollers in triggered \n");
          cargomanip -> RollersIn();
        }
        else if (rollers >= 0.2){
          //printf("rollers out triggered \n");
          cargomanip -> RollersOut();
        }
        else if (rollers < 0.2 && rollers > -0.2){
          cargomanip -> RollersStop();
        }
      #else
        if (rollersOut){
          cargomanip -> RollersOut();
        }
        else if (rollersIn){
          cargomanip -> RollersIn();
        }
        else {
          cargomanip -> RollersStop();
        }
      #endif

      cargomanip -> FindEncoderAngle();

      // if (cargoGround){
      //   cargomanip -> CargoGroundAngle();
      //   //elevator->BallHeight();
      // }
      if (cargoHPAngle){
        elevator->HPHeight();
        //cargomanip -> CargoHPAngle();
      }
      // else if (cargoInBot){
      //   cargomanip -> CargoInRobot();
      // }

      // Hatch
      // Side Pistons
      // if (hatchPistons && !hatchPistonDeployed)
      // {
      //   hatch->TopPistonSwitch();
      //   hatchPistonDeployed = true;
      //   //printf("hatch top pistons button pressed \n");
      //   //hatch->Go();
      // }
      // else if (!hatchPistons && hatchPistonDeployed)
      // {
      //   hatchPistonDeployed = false;
      // }
      
      // // Side Piston LEDs
      // if((hatch->GetTopPistonStatus()) && (vision->IsIdle()))
      // {
      //   leds->SetColor(ORANGE, FLASHING);
      // }
      // else if ((!hatch->GetTopPistonStatus()) && (vision->IsIdle()))
      // {
      //   leds->SetColor(PINK, FLASHING);
      // }

      //Bottom pistons
      // if (cargoPistons && !buttonPressed)
      // {
      //   //printf("cargo/hatch bottom pistons button pressed \n");
      //   hatch->BottomPistonSwitch();
      //   buttonPressed = true;
      // }
      // else if (!cargoPistons && buttonPressed)
      // {
      //   buttonPressed = false;
      // }

      //Pistons open
      if(pistonsOpen && !buttonPressedOpen)
      {
        hatch->TopPistonOut();
        buttonPressedOpen = true;
      }
      else if (!pistonsOpen && buttonPressedOpen)
      {
        buttonPressedOpen = false;
      }

      //Pistons close
      if(pistonsClose && !buttonPressedClose)
      {
        hatch->TopPistonIn();
        buttonPressedClose = true;
      }
      else if (!pistonsClose && buttonPressedClose)
      {
        buttonPressedClose = false;
      }

      //chassis lifting pistons
      //front piston
      /*if (chassisFront && chassisFrontButtonPressed==false)
      {
        if(fpiston->Get()==true)
        {
          fpiston -> Set(false);
        }
        else if(fpiston->Get()==false)  
        {
          fpiston -> Set(true);
        }
        chassisFrontButtonPressed=true;
      }
      else if (!chassisFront && chassisFrontButtonPressed==true)
      {
        chassisFrontButtonPressed=false;
      }*/

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
      {
        chassisBackButtonPressed=false;
      }
    }

    virtual void AutonomousInit() override 
    {
      gyro->Reset();
      //hatch->BottomPistonOut(); //THIS IS SO THAT THE ELEVATOR CAN GO UP
      flmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
      frmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
      flmotor->SetSelectedSensorPosition(0, 0, 10);
      flmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
      frmotor->SetSelectedSensorPosition(0, 0, 10);
      frmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
    }

    virtual void AutonomousPeriodic() override 
    {
      RobotCode();
    }

    virtual void TeleopInit() override 
    {
      //hatch->BottomPistonOut();
    }

    virtual void TeleopPeriodic() override 
    {
      RobotCode();
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