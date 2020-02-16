// includes
#include <iostream>
#include "Settings.h"
#include "JankyConstants.h"
#include "ctre/Phoenix.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include "jankyXboxJoystick.h"
#include "jankyDrivestick.h"
#include <frc/drive/DifferentialDrive.h>
#include "rev/ColorSensorV3.h"
#include "ColorSensorInfiniteRecharge.h"
#include "frc/TimedRobot.h"
#include "frc/SpeedControllerGroup.h"
#include "AutoDrive.h"
#include "AutoSelector.h"
#include "AutoEntry.h"
#include "AutoSequencer.h"
#include "IntakeMech.h"

using namespace std;
using namespace frc;
using namespace rev;

class Robot : public TimedRobot {
  AutoSelector*autoSelector;
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
  IntakeMech * intakemech;
  WPI_VictorSPX * conveyorBeltMotor;
  WPI_VictorSPX * bridgeMotor;
  
	jankyXboxJoystick * _joy;  
	string _sb;
	int _loops = 0;


  float distanceToVisionTarget;
  float offsetFromVisionTarget;

  public:
  //constructor
  Robot()
  {
    autoSelector = NULL;
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
    intakemech = NULL;
    conveyorBeltMotor = NULL;
    bridgeMotor = NULL;
    _joy = NULL;
  }

  //deconstructor
  ~Robot()
  {
    delete autoSelector;
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
    delete intakemech;
    delete conveyorBeltMotor;
    delete bridgeMotor;
    delete _joy;
  }
  
  virtual void RobotInit() override
  {
    // AUTO
    autoSelector = new AutoSelector();
    autoSelector->DisplayAutoOptions();

    // CHASSIS
    flmotor = new WPI_VictorSPX(SHOOTING_LEFT_MOTOR_CHANNEL);
    rlmotor = new WPI_TalonSRX(INTAKE_LEFT_MOTOR_CHANNEL);
    frmotor = new WPI_TalonSRX(SHOOTING_RIGHT_MOTOR_CHANNEL);
    rrmotor = new WPI_VictorSPX(INTAKE_RIGHT_MOTOR_CHANNEL);

    // CAMERAS
    #ifdef DRIVE_TEAM_CAM_1
      //Run drive team camera
      cs::UsbCamera driveCam1;
      driveCam1 = CameraServer::GetInstance()->StartAutomaticCapture(0);
      driveCam1.SetResolution(160,120);
      driveCam1.SetFPS(5);
      driveCam1.GetProperty("compression").Set(100);
    #endif

    #ifdef DRIVE_TEAM_CAM_2
      //Run drive team camera
      cs::UsbCamera driveCam2;
      driveCam2 = CameraServer::GetInstance()->StartAutomaticCapture(0);
      driveCam2.SetResolution(160,120);
      driveCam2.SetFPS(5);
      driveCam2.GetProperty("compression").Set(100);
    #endif

    // CHASSIS
    leftDrive = new SpeedControllerGroup(*flmotor, *rlmotor);
    rightDrive = new SpeedControllerGroup(*frmotor, *rrmotor);
    drive = new DifferentialDrive(*leftDrive, *rightDrive);
    left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
    right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);

    sensor_fake = new ColorSensorInfiniteRecharge(); // color sensor
    _joy = new jankyXboxJoystick(2); // joystick

    //drive -> SetSafetyEnabled(false);

    // SHOOTER
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

    // GET VISION DATA
    SmartDashboard::PutNumber(VISION_DISTANCE, NO_DATA_DEFAULT);
	  SmartDashboard::PutNumber(VISION_OFFSET, NO_DATA_DEFAULT);

    // INTAKE & CONVEYOR BELT & BRIDGE
    intakemech = new IntakeMech(MOTOR_ROLL_CHANNEL, LEFT_PISTON_CHANNEL, RIGHT_PISTON_CHANNEL);
    conveyorBeltMotor = new WPI_VictorSPX(CONVEYOR_BELT_CHANNEL);
    bridgeMotor = new WPI_VictorSPX(BRIDGE_CHANNEL);
  }

  virtual void AutonomousInit() override
  {
  
  }

  virtual void AutonomousPeriodic() override
  {
    // print values
    //autoSelector->PrintValues();

    // get auto mode
    autoSelector -> GetAutoMode();
  }

  virtual void TeleopInit() override
  {

  }

  virtual void TeleopPeriodic() override
  {
    // CHASSIS
    #ifdef PROG_BOT
    bool drivingToggle = left -> Get10();
    #endif

    #ifdef JANKYCHASSIS
    bool drivingToggle = left -> Get10();
    #endif

    bool buttonPressed = false;

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

    //SmartDashboard::PutNumber("left y axis", left -> GetY());
    //SmartDashboard::PutNumber("right y axis", right -> GetY());
  
    // COLOR SENSOR
    string colorString;
    
    // COMMENTED OUT DUE TO ERRORS --> needs troubleshooting by color sensor group :)
    // switch (sensor_fake -> ReadColor())
    // {
    //   case 0: colorString = "red";
    //     break;
    //   case 1: colorString = "yellow";
    //     break;
    //   case 2: colorString = "blue";
    //     break;
    //   case 3: colorString = "green";
    //     break;
    //   case 4: colorString = "unknown";
    //     break;
    //   default: colorString = "invalid";
    // }

    SmartDashboard::PutString("Color", colorString);

    // SHOOTING
    /* get gamepad axis */
		double leftYstick = _joy->GetLeftThrottle();
		double motorOutput = _talon->GetMotorOutputPercent();
		/* prepare line to print */
		_sb.append("\tout:");
		_sb.append(to_string(motorOutput));
		_sb.append("\tspd:");
		_sb.append(to_string(_talon->GetSelectedSensorVelocity(kPIDLoopIdx)));
		/* while button1 is held down, closed-loop on target velocity */
		string button_status;
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
			_sb.append(to_string(_talon->GetClosedLoopError(kPIDLoopIdx)));
			_sb.append("\ttrg:");
			_sb.append(to_string(targetVelocity_UnitsPer100ms));
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
		SmartDashboard::PutString("button a: ", button_status);

    if (++_loops >= 10) {
			_loops = 0;
			printf("%s\n",_sb.c_str());
		}
		_sb.clear();

    // Set distance & offset --> to give to turret & shooter
    distanceToVisionTarget = SmartDashboard::GetNumber(VISION_DISTANCE, NO_DATA_DEFAULT); 
	  offsetFromVisionTarget = (SmartDashboard::GetNumber(VISION_OFFSET, NO_DATA_DEFAULT)); //positive is to the right
    cout << "Distance to vision target: " << distanceToVisionTarget << endl;
    cout << "offset: " << offsetFromVisionTarget << endl;
    cout << endl;

    string string = "hi";
    SmartDashboard::PutString("Color", string);

    //INTAKE
    bool buttonBack = _joy -> GetButtonBack();
    bool buttonStart = _joy -> GetButtonStart();
    bool buttonRB = _joy -> GetButtonRB();
    bool buttonLB = _joy -> GetButtonLB();

    if(buttonBack){
      intakemech -> RollersIn();
    }
    else if(buttonStart){
      intakemech -> RollersOut(); 
    }
    else{
      intakemech -> RollersStop();
    }
    //check with someone to see if this makes sense 
    if(buttonLB){
      intakemech -> MechInRobot();
    }
    else if(buttonRB){
      intakemech -> MechOutRobot();
    }

    // INTAKE TO TURRET TRANSPORTATION --> for testing; will need changed by intake group
    bool buttonX = _joy -> GetButtonX(); // conveyor belt
    bool buttonY = _joy -> GetButtonY(); // bridge

    if (buttonX) {
      // run conveyor belt
      conveyorBeltMotor->Set(0.6);
    }
    else {
      // stop conveyor belt
      conveyorBeltMotor->Set(0);
    }

    if (buttonY) {
      // run bridge motor
      conveyorBeltMotor->Set(0.6);
    }
    else {
      // stop bridge motor
      conveyorBeltMotor->Set(0);
    }

  }

  virtual void TestPeriodic() override
  {

  }

private:
};
int main() 
{ 
  return StartRobot<Robot>(); 
}