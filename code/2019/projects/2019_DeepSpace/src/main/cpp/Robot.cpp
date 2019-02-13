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

//Motors
// 4 = left elevator
//2 = right elevator
#define FRONT_LEFT_MOTOR_CHANNEL 1
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 4
#define REAR_RIGHT_MOTOR_CHANNEL 3
#define MOTOR_ROLL_CHANNEL 6 
#define MOTOR_PIVOT_CHANNEL 7 // might be switched w/ motor_roll_channel
#define LIM_SWITCH_INSIDE_CHANNEL 7 //on whatever the pivot channel is
#define LIM_SWITCH_OUTSIDE_CHANNEL 7 //lim switch channels might be on 7

//Joysticks
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1
#define XBOX_CONTROLLER 2
//define XBOX_CONTROLLER_2 - if we need a second one

class Robot : public frc::TimedRobot {
  public:
    WPI_TalonSRX * flmotor;
    WPI_TalonSRX * rlmotor;
    WPI_TalonSRX * frmotor;
    WPI_TalonSRX * rrmotor;
    frc::SpeedControllerGroup * leftDrive;
    frc::SpeedControllerGroup * rightDrive;
    frc::DifferentialDrive * drive;
    jankyDrivestick * left;
    jankyDrivestick * right;
    jankyXboxJoystick * joystick;
    //jankyXboxJoystick * joystick2; for if we need a second gc joystick
    frc::ADXRS450_Gyro * gyro;
    VisionStateMachine * vision;
    CargoManip * cargomanip;
    
  //constructor
  Robot()
  {
    flmotor = NULL;
    rlmotor = NULL;
    frmotor = NULL;
    rrmotor = NULL;
    leftDrive = NULL;
    rightDrive = NULL;
    drive = NULL;
    left = NULL;
    right = NULL;
    joystick = NULL;
    //joystick2 = NULL;
    gyro = NULL;
    vision = NULL;
    cargomanip = NULL;
  }

  //deconstructor
  ~Robot()
  {
    delete flmotor;
    delete rlmotor;
    delete frmotor;
    delete rrmotor;
    delete leftDrive;
    delete rightDrive;
    delete drive;
    delete left;
    delete right; 
    delete joystick;
    //delete joystick2;
    delete gyro;
    delete vision;
    delete cargomanip;
  }
  
  static void DriveTeamCameraThread()
    {
        cs::UsbCamera driveTeamCamera = frc::CameraServer::GetInstance()->StartAutomaticCapture();
        driveTeamCamera.SetResolution(640, 480);
        cs::CvSink cvSink = frc::CameraServer::GetInstance()->GetVideo();
        cs::CvSource outputStream = frc::CameraServer::GetInstance()->PutVideo("Drive Team Camera", 640, 480);
        cv::Mat source;
        cv::Mat output;
        while(true) {
            cvSink.GrabFrame(source);
            cvtColor(source, output, cv::COLOR_BGR2GRAY);
            outputStream.PutFrame(output);
        }
    }

  virtual void RobotInit() override
  {
    //Run drive team camera
    std::thread driveTeamCameraThread(DriveTeamCameraThread);
    driveTeamCameraThread.detach();

    flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
    rlmotor = new WPI_TalonSRX(REAR_LEFT_MOTOR_CHANNEL);
    frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
    rrmotor = new WPI_TalonSRX(REAR_RIGHT_MOTOR_CHANNEL);
    flmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
		frmotor->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
    leftDrive = new frc::SpeedControllerGroup(*flmotor, *rlmotor);
    rightDrive = new frc::SpeedControllerGroup(*frmotor, *rrmotor);
    joystick = new jankyXboxJoystick(XBOX_CONTROLLER);
    //joystick2 = new jankyXboxJoystick(XBOX_CONTROLLER_2);
    drive = new frc::DifferentialDrive(*leftDrive, *rightDrive);
    left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
    right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);
    gyro = new frc::ADXRS450_Gyro(frc::SPI::Port::kOnboardCS0); //gyro didn't work; maybe try other port options
    vision = new VisionStateMachine(drive, gyro, &(flmotor->GetSensorCollection()), &(frmotor->GetSensorCollection()), flmotor, frmotor);
    cargomanip = new CargoManip(MOTOR_ROLL_CHANNEL, MOTOR_PIVOT_CHANNEL, LIM_SWITCH_INSIDE_CHANNEL, LIM_SWITCH_OUTSIDE_CHANNEL);

    drive->SetSafetyEnabled(false); 
    gyro->Calibrate();
    cargomanip -> StartInit();
  }

  virtual void AutonomousInit() override
  {
    gyro->Reset();
   	flmotor->SetSelectedSensorPosition(0, 0, 10);
		flmotor->GetSensorCollection().SetQuadraturePosition(0, 10);
		frmotor->SetSelectedSensorPosition(0, 0, 10);
		frmotor->GetSensorCollection().SetQuadraturePosition(0, 10);  
  }

  virtual void AutonomousPeriodic() override
  {
    if(left->Get3()){ //replace later to use the button board
      vision->StartSequenceTest(); //test mode 
    }
    else if(vision->IsIdle()){ 
      drive->TankDrive(-left->GetY(), -right->GetY());
    }

    frc::SmartDashboard::PutNumber("angle", gyro->GetAngle());
    if(left->Get4()){
      vision->Cancel();
    }

  }

  virtual void TeleopInit() override
  {

  }

  virtual void TeleopPeriodic() override
  {
    //drive 
    drive->TankDrive(-left->GetY(), -right->GetY());


    //cargo
    bool buttonB = joystick -> GetButtonB();
    bool buttonX = joystick -> GetButtonX();
    bool buttonLB = joystick -> GetButtonLB(); 
    bool buttonRB = joystick -> GetButtonRB();

    frc::SmartDashboard::PutBoolean("Outside Limit Switch Pressed:", cargomanip -> GetLimSwitchOutside());
    frc::SmartDashboard::PutBoolean("Inside Limit Switch Pressed:", cargomanip -> GetLimSwitchInside());  

    if (buttonB){
      cargomanip -> RollersOut();
    }
    else if (buttonX){
      cargomanip -> RollersIn();
    }
    else {
      cargomanip -> RollersStop();
    }

    if (buttonLB){
      cargomanip -> CargoMechInRobot(); 
    }
    else if (buttonRB){
      cargomanip -> CargoMechOutRobot();
    }
    else {
      cargomanip -> CargoMechStop();
    }

    //TODO: add hatch mech, chassis lifting pistons, elevator, & ultrasonic code
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