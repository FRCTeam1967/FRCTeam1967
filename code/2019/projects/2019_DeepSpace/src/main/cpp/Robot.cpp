#include "frc/WPILib.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "AutoDrive.h"
#include "VisionStateMachine.h"
#include "DifferentialDriveSegment.h"
#include "DifferentialTurnSegment.h"
#include "jankyDrivestick.h"
#include "jankyXboxJoystick.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

//Motors
// 4 = left elevator
//2 = right elevator
#define FRONT_LEFT_MOTOR_CHANNEL 1
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 4
#define REAR_RIGHT_MOTOR_CHANNEL 3

//Joysticks
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1
#define XBOX_CONTROLLER 2

class Robot : public frc::TimedRobot {
  public:
    WPI_TalonSRX*flmotor;
    WPI_TalonSRX*rlmotor;
    WPI_TalonSRX*frmotor;
    WPI_TalonSRX*rrmotor;
    frc::SpeedControllerGroup*leftDrive;
    frc::SpeedControllerGroup*rightDrive;
    frc::DifferentialDrive*drive;
    jankyDrivestick*left;
    jankyDrivestick*right;
    jankyXboxJoystick*xbox;
    frc::ADXRS450_Gyro*gyro;
    VisionStateMachine*vision;
    
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
    xbox = NULL;
    gyro = NULL;
    vision = NULL;
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
    delete xbox;
    delete gyro;
    delete vision;
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
    xbox = new jankyXboxJoystick(XBOX_CONTROLLER);
    drive = new frc::DifferentialDrive(*leftDrive, *rightDrive);
    left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
    right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);
    gyro = new frc::ADXRS450_Gyro(frc::SPI::Port::kOnboardCS0); //gyro didn't work; maybe try other port options
    vision = new VisionStateMachine(drive, gyro, &(flmotor->GetSensorCollection()), &(frmotor->GetSensorCollection()), flmotor, frmotor);

    drive->SetSafetyEnabled(false); 
    gyro->Calibrate();
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
    if(xbox->GetButtonY()){ //replace later to use the button board
      vision->StartSequenceTest(); //test mode 
    }
    else if(vision->IsIdle()){ 
      drive->TankDrive(-left->GetY(), -right->GetY());
    }

    frc::SmartDashboard::PutNumber("angle", gyro->GetAngle());
    if(xbox->GetButtonX()){
      vision->Cancel();
    }

  }

  virtual void TeleopInit() override
  {

  }

  virtual void TeleopPeriodic() override
  {
    drive->TankDrive(-left->GetY(), -right->GetY());

    //TODO: add hatch mech, chassis lifting pistons, elevator, ultrasonic, and cargo mech code
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