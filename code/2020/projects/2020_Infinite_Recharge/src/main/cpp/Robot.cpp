#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include "ctre/Phoenix.h"
#include "jankyXboxJoystick.h"
#include "jankyDrivestick.h"
#include <frc/drive/DifferentialDrive.h>
#include "rev/ColorSensorV3.h"
#include "ColorSensorInfiniteRecharge.h"
#include "frc/TimedRobot.h"
#include "frc/SpeedControllerGroup.h"
//#include "Constants.h"

//move these values into a Constants.h
enum Constants {
	/**
	 * Which PID slot to pull gains from.  Starting 2018, you can choose
	 * from 0,1,2 or 3.  Only the first two (0,1) are visible in web-based configuration.
	 */
	kSlotIdx = 0,

	/* Talon SRX/ Victor SPX will supported multiple (cascaded) PID loops.
	 * For now we just want the primary one.
	 */
	kPIDLoopIdx = 0,

	/*
	 * set to zero to skip waiting for confirmation, set to nonzero to wait
	 * and report to DS if action fails.
	 */
	kTimeoutMs = 30
};

//#define PROG_BOT
#define JANKYCHASSIS
#define LEFT_JOYSTICK_CHANNEL 0
#define RIGHT_JOYSTICK_CHANNEL 1
#define COLOR_SENSOR_PORT 0x52

#ifdef PROG_BOT
#define FRONT_LEFT_MOTOR_CHANNEL 1
#define REAR_LEFT_MOTOR_CHANNEL 2
#define FRONT_RIGHT_MOTOR_CHANNEL 3
#define REAR_RIGHT_MOTOR_CHANNEL 4
#endif

#ifdef JANKYCHASSIS
#define SHOOTING_LEFT_MOTOR_CHANNEL 4
#define INTAKE_LEFT_MOTOR_CHANNEL 0
#define SHOOTING_RIGHT_MOTOR_CHANNEL 1
#define INTAKE_RIGHT_MOTOR_CHANNEL 3
#endif
#include "Settings.h"

using namespace std;
using namespace frc;
using namespace rev;

class Robot : public frc::TimedRobot {
  WPI_VictorSPX*flmotor;
  WPI_TalonSRX*frmotor;
  WPI_TalonSRX*rlmotor;
  WPI_VictorSPX*rrmotor;
  DifferentialDrive*drive;
  SpeedControllerGroup*leftDrive;
  SpeedControllerGroup*rightDrive;
  jankyDrivestick*left;
  jankyDrivestick*right;
  bool shootingSideFront;
  ColorSensorInfiniteRecharge*sensor_fake;
  TalonFX * _talon = new TalonFX(0); //change the channel number on here and id
	jankyXboxJoystick * _joy;  
	std::string _sb;
	int _loops = 0;


  float distanceToVisionTarget;
  float offsetFromVisionTarget;

  public:
  //constructor
  Robot()
  {
    flmotor = NULL;
    rlmotor = NULL;
    frmotor = NULL;
    rrmotor = NULL;
    drive = NULL;
    leftDrive = NULL;
    rightDrive = NULL;
    left = NULL;
    right = NULL;
    sensor_fake = NULL;
    _joy = NULL;
  }

  //deconstructor
  ~Robot()
  {
    delete flmotor;
    delete rlmotor;
    delete frmotor;
    delete rrmotor;
    delete drive;
    delete leftDrive;
    delete rightDrive;
    delete left;
    delete right;
    delete sensor_fake;
    delete _joy;
  }
  
  virtual void RobotInit() override
  {
    flmotor = new WPI_VictorSPX(SHOOTING_LEFT_MOTOR_CHANNEL);
    rlmotor = new WPI_TalonSRX(INTAKE_LEFT_MOTOR_CHANNEL);
    frmotor = new WPI_TalonSRX(SHOOTING_RIGHT_MOTOR_CHANNEL);
    rrmotor = new WPI_VictorSPX(INTAKE_RIGHT_MOTOR_CHANNEL);

    #ifdef DRIVE_TEAM_CAM_1
      //Run drive team camera
      cs::UsbCamera driveCam1;
      driveCam1 = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
      driveCam1.SetResolution(160,120);
      driveCam1.SetFPS(5);
      driveCam1.GetProperty("compression").Set(100);
    #endif

    #ifdef DRIVE_TEAM_CAM_2
      //Run drive team camera
      cs::UsbCamera driveCam2;
      driveCam2 = frc::CameraServer::GetInstance()->StartAutomaticCapture(0);
      driveCam2.SetResolution(160,120);
      driveCam2.SetFPS(5);
      driveCam2.GetProperty("compression").Set(100);
    #endif

    flmotor = new WPI_TalonSRX(FRONT_LEFT_MOTOR_CHANNEL);
    rlmotor = new WPI_VictorSPX(REAR_LEFT_MOTOR_CHANNEL);
    frmotor = new WPI_TalonSRX(FRONT_RIGHT_MOTOR_CHANNEL);
    rrmotor = new WPI_VictorSPX(REAR_RIGHT_MOTOR_CHANNEL);
    leftDrive = new SpeedControllerGroup(*flmotor, *rlmotor);
    rightDrive = new SpeedControllerGroup(*frmotor, *rrmotor);
    drive = new DifferentialDrive(*leftDrive, *rightDrive);
    left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
    right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);
    sensor_fake = new ColorSensorInfiniteRecharge();
    _joy = new jankyXboxJoystick(2);

    //drive -> SetSafetyEnabled(false);

    shootingSideFront = true;

    _talon->ConfigFactoryDefault();
     /* first choose the sensor */
		_talon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, kTimeoutMs);
		_talon->SetSensorPhase(true);
		/* set the peak and nominal outputs */
		_talon->ConfigNominalOutputForward(0, kTimeoutMs);
		_talon->ConfigNominalOutputReverse(0, kTimeoutMs);
		_talon->ConfigPeakOutputForward(1, kTimeoutMs);
		_talon->ConfigPeakOutputReverse(-1, kTimeoutMs);
		/* set closed loop gains in slot0 */
		_talon->Config_kF(kPIDLoopIdx, 0.1097, kTimeoutMs);
		_talon->Config_kP(kPIDLoopIdx, 0.22, kTimeoutMs);
		_talon->Config_kI(kPIDLoopIdx, 0.0, kTimeoutMs);
		_talon->Config_kD(kPIDLoopIdx, 0.0, kTimeoutMs);
    frc::SmartDashboard::PutNumber(VISION_DISTANCE, NO_DATA_DEFAULT);
	  frc::SmartDashboard::PutNumber(VISION_OFFSET, NO_DATA_DEFAULT);
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
    //Driving code
    #ifdef PROG_BOT
    bool drivingToggle = left -> Get10();
    #endif

    #ifdef JANKYCHASSIS
    bool drivingToggle = left -> Get10();
    #endif

    bool buttonPressed = false;

    if (drivingToggle && !buttonPressed && shootingSideFront)
    //double confidence = 0.0;

    if (drivingToggle && shootingSideFront)
    {
      shootingSideFront = false;
      buttonPressed = true;
    }
    else if(drivingToggle && !buttonPressed && !shootingSideFront)
    {
      shootingSideFront = true;
      buttonPressed = true;
    }
    else if (!drivingToggle && buttonPressed)
    {
      buttonPressed = false;
    }

    if (shootingSideFront)
    {
      drive -> TankDrive(-left->GetY(), -right->GetY());
    }
    else if (!shootingSideFront)
    {
      drive -> TankDrive(left->GetY(), right->GetY());
    }

    frc::SmartDashboard::PutNumber("left y axis", left -> GetY());
    frc::SmartDashboard::PutNumber("right y axis", right -> GetY());
  
    std::string colorString;
    switch (sensor_fake -> ReadColor())
    {
      case 0: colorString = "red";
        break;
      case 1: colorString = "yellow";
        break;
      case 2: colorString = "blue";
        break;
      case 3: colorString = "green";
        break;
      case 4: colorString = "unknown";
        break;
      default: colorString = "invalid";
    }

    frc::SmartDashboard::PutString("Color", colorString);

    /* get gamepad axis */
		double leftYstick = _joy->GetLeftThrottle();
		double motorOutput = _talon->GetMotorOutputPercent();
		/* prepare line to print */
		_sb.append("\tout:");
		_sb.append(std::to_string(motorOutput));
		_sb.append("\tspd:");
		_sb.append(std::to_string(_talon->GetSelectedSensorVelocity(kPIDLoopIdx)));
		/* while button1 is held down, closed-loop on target velocity */
		std::string button_status;
    double targetVelocity_UnitsPer100ms = 0;
    if (_joy->GetButtonA()) 
    {
        	/* Speed mode */
			/* Convert 500 RPM to units / 100ms.
			 * 4096 Units/Rev * 500 RPM / 600 100ms/min in either direction:
			 * velocity setpoint is in units/100ms
			 */
      button_status = "pushed";
			targetVelocity_UnitsPer100ms = leftYstick * 5000.0 * 2048 / 600; //change 4096 to 2048 for unit per rev
      //double targetVelocity_UnitsPer100ms = 0.75 * 500.0 * 2048 / 600; //change 4096 to 2048 for unit per rev
			/* 500 RPM in either direction */
			/* append more signals to print when in speed mode. */
			_sb.append("\terrNative:");
			_sb.append(std::to_string(_talon->GetClosedLoopError(kPIDLoopIdx)));
			_sb.append("\ttrg:");
			_sb.append(std::to_string(targetVelocity_UnitsPer100ms));
      } 
      else
      {
        button_status = "not pushed";
      }
      _talon->Set(ControlMode::Velocity, targetVelocity_UnitsPer100ms); 

      //else {
			/* Percent voltage mode */
		//_talon->Set(ControlMode::PercentOutput, leftYstick);
		//}
		/* print every ten loops, printing too much too fast is generally bad for performance */
		frc::SmartDashboard::PutString("button a: ", button_status);

    if (++_loops >= 10) {
			_loops = 0;
			printf("%s\n",_sb.c_str());
		}
		_sb.clear();

    // Set distance & offset --> to give to turret & shooter
    distanceToVisionTarget = frc::SmartDashboard::GetNumber(VISION_DISTANCE, NO_DATA_DEFAULT); 
	  offsetFromVisionTarget = (frc::SmartDashboard::GetNumber(VISION_OFFSET, NO_DATA_DEFAULT)); //positive is to the right

    std::string string = "hi";
    frc::SmartDashboard::PutString("Color", string);
    //frc::SmartDashboard::PutNumber("Confidence", confidence);
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