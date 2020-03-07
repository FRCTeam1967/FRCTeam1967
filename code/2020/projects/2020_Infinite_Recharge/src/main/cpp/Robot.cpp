//INCLUDES
#include <iostream>
#include <math.h>
// color sensor
#include "rev/ColorSensorV3.h"
// Camera
#include <cscore_oo.h>
#include <cameraserver/CameraServer.h>
// ctre
#include "ctre/Phoenix.h"
// frc
#include <frc/drive/DifferentialDrive.h>
#include "frc/TimedRobot.h"
#include <frc/Filesystem.h>
#include "frc/kinematics/DifferentialDriveKinematics.h"
#include <frc/kinematics/DifferentialDriveWheelSpeeds.h>
#include "frc/SpeedControllerGroup.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include "frc/TimedRobot.h"
#include <frc/trajectory/TrajectoryConfig.h>
#include <frc/trajectory/TrajectoryUtil.h>
#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc/trajectory/constraint/DifferentialDriveVoltageConstraint.h>
#include <frc/ADXRS450_Gyro.h>
//frc2
#include "frc2/command/RamseteCommand.h"
#include <frc2/command/InstantCommand.h>
// wpi
#include <wpi/Path.h>
#include <wpi/SmallString.h>
// gyro
#include "adi/ADIS16470_IMU.h"
// custom classes
#include "AutoConstants.h"
#include "AutoDriveSubsystems.h"
#include "AutoSelector.h"
#include "ColorSensorInfiniteRecharge.h"
#include "JankyConstants.h"
// #include "IntakeMech.h"
#include "ShooterController.h"
#include "jankyXboxJoystick.h"
#include "jankyDrivestick.h"
#include "Settings.h"
//#include "JankyPathWeaver.h"

// NAMESPACES
using namespace ctre;
using namespace frc;
using namespace rev;
using namespace std;
using namespace AutoDriveConstants;


class Robot : public TimedRobot {
  //auto
  AutoSelector*autoSelector;
  AutoDriveSubsystem m_drive;
  frc2::RamseteCommand * rc;
  //chassis
  WPI_VictorSPX*flmotor;
  WPI_TalonSRX*frmotor;
  WPI_TalonSRX*rlmotor;
  WPI_VictorSPX*rrmotor;
  DifferentialDrive*drive;
  SpeedControllerGroup*leftDrive;
  SpeedControllerGroup*rightDrive;
  bool shootingSideFront;
  #ifdef CP_ON_ROBOT
    //control panel
    ColorSensorInfiniteRecharge*sensor_fake;
    WPI_TalonSRX*colorMotor;
  #endif
  //shooting
  // TalonFX * _talon = new TalonFX(FLYWHEEL_CHANNEL); //change the channel number on here and id
  // IntakeMech * intakemech;
  // WPI_TalonSRX * conveyorBeltMotor;
  ShooterControllerInfiniteRecharge * shootingcontroller;
  WPI_VictorSPX * bridgeMotor;
  WPI_TalonSRX * turretMotor; 
	string _sb;
	int _loops = 0;
  //joysticks
  #ifdef DRIVING_WITH_2_JOYS
    jankyDrivestick*left;
    jankyDrivestick*right;
  #endif
  #ifdef DRIVING_WITH_XBOX
    jankyXboxJoystick * drivingJoy;
  #endif
	jankyXboxJoystick * _joy; 
  //vision data
  float distanceToVisionTarget;
  float offsetFromVisionTarget;
  int i = 0;
  int j = 0;

  public:
  //constructor
  Robot()
  {
    //auto
    autoSelector = NULL;
    rc = NULL;
    //chassis
    flmotor = NULL;
    rlmotor = NULL;
    frmotor = NULL;
    rrmotor = NULL;
    drive = NULL;
    leftDrive = NULL;
    rightDrive = NULL;
    #ifdef CP_ON_ROBOT
      //color sensor
      sensor_fake = NULL;
      colorMotor = NULL;
    #endif
    //shooting
    // intakemech = NULL;
    // conveyorBeltMotor = NULL;
    shootingcontroller = NULL;
    bridgeMotor = NULL;
    turretMotor = NULL;
    //joysticks
    _joy = NULL;
    #ifdef DRIVING_WITH_2_JOYS
      left = NULL;
      right = NULL;
    #endif
    #ifdef DRIVING_WITH_XBOX
      drivingJoy = NULL;
    #endif
  }

  //deconstructor
  ~Robot()
  {
    //auto
    delete autoSelector;
    delete rc;
    //chassis
    delete flmotor;
    delete rlmotor;
    delete frmotor;
    delete rrmotor;
    delete drive;
    delete leftDrive;
    delete rightDrive;
    #ifdef CP_ON_ROBOT
      //color sensor
      delete sensor_fake;
      delete colorMotor;
    #endif
    //shooting
    // delete intakemech;
    // delete conveyorBeltMotor;
    delete shootingcontroller;
    delete bridgeMotor;
    delete turretMotor;
    //joysticks
    delete _joy;
    #ifdef DRIVING_WITH_2_JOYS
      delete left;
      delete right;
    #endif
    #ifdef DRIVING_WITH_XBOX
      delete drivingJoy;
    #endif
  }
  
  virtual void RobotInit() override
  {
    // AUTONOMOUS SET-UP
    // Auto Selector
    autoSelector = new AutoSelector();
    autoSelector->DisplayAutoOptions();

    // Create a voltage constraint to ensure we don't accelerate too fast
    cout << "Creating voltage constraint" << endl;
    frc::DifferentialDriveVoltageConstraint autoVoltageConstraint(
        frc::SimpleMotorFeedforward<units::meters>(
            AutoDriveConstants::ks,
            AutoDriveConstants::kv,
            AutoDriveConstants::ka
          ),
          AutoDriveConstants::kDriveKinematics,
          10_V
        );

    // Configure trajectory
    cout << "Config Trajectory" << endl;
    frc::TrajectoryConfig config(AutoConstants::kMaxSpeed, AutoConstants::kMaxAcceleration); // Set up config for trajectory
    config.SetKinematics(AutoDriveConstants::kDriveKinematics); // Add kinematics to ensure max speed is actually obeyed
    config.AddConstraint(autoVoltageConstraint); // Apply the voltage constraint

    // Get Trajectory
    cout << "Get Trajectory" << endl;
    wpi::SmallString<64> deployDirectory;
    frc::filesystem::GetDeployDirectory(deployDirectory);
    wpi::sys::path::append(deployDirectory, "output");
    wpi::sys::path::append(deployDirectory, "Unnamed.wpilib.json");
    frc::Trajectory trajectory = frc::TrajectoryUtil::FromPathweaverJson(deployDirectory);

    // Create PID controller & set tolerance
    cout << "Creating pid controller" << endl;
    frc2::PIDController leftController(AutoDriveConstants::kPDriveVel, 0, AutoDriveConstants::kDDriveVel); // left
    frc2::PIDController rightController(AutoDriveConstants::kPDriveVel, 0, AutoDriveConstants::kDDriveVel); // right
    leftController.SetTolerance(0.0);
    rightController.SetTolerance(0.0);

    // Instantiate ramsete command
    cout << "Instantiate ramsete command" << endl;
    rc = new frc2::RamseteCommand(
      trajectory, [this]() { return m_drive.GetPose(); },
      frc::RamseteController(AutoConstants::kRamseteB,
                            AutoConstants::kRamseteZeta),
      frc::SimpleMotorFeedforward<units::meters>(
          AutoDriveConstants::ks, AutoDriveConstants::kv, AutoDriveConstants::ka),
      AutoDriveConstants::kDriveKinematics,
      [this] { return m_drive.GetWheelSpeeds(); },
      leftController,
      rightController,
      [this](auto left, auto right) { m_drive.TankDriveVolts(left, right); },
      {&m_drive});

    // CHASSIS
    flmotor = new WPI_VictorSPX(SHOOTING_LEFT_MOTOR_CHANNEL);
    rlmotor = new WPI_TalonSRX(INTAKE_LEFT_MOTOR_CHANNEL);
    frmotor = new WPI_TalonSRX(SHOOTING_RIGHT_MOTOR_CHANNEL);
    rrmotor = new WPI_VictorSPX(INTAKE_RIGHT_MOTOR_CHANNEL);

    // CHASSIS ENCODERS
    rlmotor->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
    rlmotor->SetSelectedSensorPosition(0, 0, 10);
    rlmotor->GetSensorCollection().SetQuadraturePosition(0,10);
    frmotor->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Absolute, 0, 0);
    frmotor->SetSelectedSensorPosition(0, 0, 10);
    frmotor->GetSensorCollection().SetQuadraturePosition(0,10);

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
    #ifdef DRIVING_WITH_2_JOYS
      left = new jankyDrivestick(LEFT_JOYSTICK_CHANNEL);
      right = new jankyDrivestick(RIGHT_JOYSTICK_CHANNEL);
    #endif
    #ifdef DRIVING_WITH_XBOX
      drivingJoy = new jankyXboxJoystick(XBOX_DRIVE_CHANNEL);
    #endif
    
    #ifdef CP_ON_ROBOT
      sensor_fake = new ColorSensorInfiniteRecharge(); // color sensor
    #endif

    _joy = new jankyXboxJoystick(2); // joystick

    drive -> SetSafetyEnabled(false);

    // SHOOTER
    shootingSideFront = true;
    shootingcontroller = new ShooterControllerInfiniteRecharge(CONVEYOR_BELT_CHANNEL, MOTOR_ROLL_CHANNEL, LEFT_PISTON_CHANNEL, RIGHT_PISTON_CHANNEL, FLYWHEEL_CHANNEL);
    //_talon->ConfigFactoryDefault();
     /* first choose the sensor */
		//_talon->ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, 0, kTimeoutMs);
		//_talon->SetSensorPhase(true);
		/* set the peak and nominal outputs */
		//_talon->ConfigNominalOutputForward(0, kTimeoutMs);
		//_talon->ConfigNominalOutputReverse(0, kTimeoutMs);
		//_talon->ConfigPeakOutputForward(1, kTimeoutMs);
		//_talon->ConfigPeakOutputReverse(-1, kTimeoutMs);
		/* set closed loop gains in slot0 */
		//_talon->Config_kF(kPIDLoopIdx, 0.045, kTimeoutMs);
		//_talon->Config_kP(kPIDLoopIdx, 0.0012, kTimeoutMs);
		//_talon->Config_kI(kPIDLoopIdx, 0.0, kTimeoutMs);
		//_talon->Config_kD(kPIDLoopIdx, 0.0001, kTimeoutMs);

    // GET VISION DATA
    SmartDashboard::PutNumber(VISION_DISTANCE, NO_DATA_DEFAULT);
	  SmartDashboard::PutNumber(VISION_OFFSET, NO_DATA_DEFAULT);

    // INTAKE & CONVEYOR BELT & BRIDGE
    //intakemech = new IntakeMech(MOTOR_ROLL_CHANNEL, LEFT_PISTON_CHANNEL, RIGHT_PISTON_CHANNEL);
    //conveyorBeltMotor = new WPI_TalonSRX(CONVEYOR_BELT_CHANNEL);
    bridgeMotor = new WPI_VictorSPX(BRIDGE_CHANNEL);

    // TURRET 
    turretMotor = new WPI_TalonSRX(TURRET_MOTOR_CHANNEL);
  }

  virtual void AutonomousInit() override
  {
    // get auto mode
    autoSelector -> GetAutoMode();
    i = 0;
    j = 0;
    // Initialize ramsete command
    rc->Initialize();
  }

  virtual void AutonomousPeriodic() override
  {
    // Execute ramsete command
    cout << " " << endl;
    if(!rc->IsFinished() )
    {
      SmartDashboard::PutNumber("Execute COUNT???", i);
      i++;
      rc->Execute();
      SmartDashboard::PutNumber("Chassis Left Encoder: ", m_drive.GetLeftEncoder());
      SmartDashboard::PutNumber("Chassis Right Encoder: ", m_drive.GetRightEncoder());
      SmartDashboard::PutNumber("Avg Dist: ", m_drive.GetAverageEncoderDistance());
    }
    else {
      SmartDashboard::PutNumber("End COUNT???", j);
      j++;
      cout << "end" << endl;
      rc->End(true);
      m_drive.StopAuto();
    }
  }

  virtual void TeleopInit() override
  {

  }

  virtual void TeleopPeriodic() override
  {
    //assert(_joy);
    // cout << "GYRO" << m_drive.ReturnGyroAngle() << endl;

    // CHASSIS ENCODERS
    cout << "Left Encoder: " << rlmotor->GetSensorCollection().GetQuadraturePosition() << endl;
    cout << "Right Encoder: " << (-1.0 * frmotor->GetSensorCollection().GetQuadraturePosition()) << endl;

    // CHASSIS
    #ifdef DRIVING_WITH_2_JOYS
      #ifdef PROG_BOT
      bool drivingToggle = left -> Get10();
      #endif

      #ifdef JANKYCHASSIS
      bool drivingToggle = left -> Get10();
      #endif
    #endif
    
    #ifdef DRIVING_WITH_2_JOYS
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
    #endif

    if (shootingSideFront)
    {
      #ifdef DRIVING_WITH_2_JOYS
        drive -> TankDrive(-left->GetY(), -right->GetY());
      #endif

      #ifdef DRIVING_WITH_XBOX
        drive -> TankDrive(-drivingJoy->GetLeftYAxis(), -drivingJoy->GetRightYAxis());
      #endif
    }
    // else if (!shootingSideFront)
    // {
    //   #ifdef DRIVING_WITH_2_JOYS
    //     drive -> TankDrive(left->GetY(), right->GetY());
    //   #endif
    //   #ifdef DRIVING_WITH_XBOX
    //     drive -> TankDrive(pow(drivingJoy->GetLeftYAxis(), 2), pow(drivingJoy->GetRightYAxis(), 2));
    //   #endif
    // }
  
    // COLOR SENSOR
    #ifdef CP_ON_ROBOT
      ColorSensorInfiniteRecharge::InfiniteRechargeColors color = sensor_fake -> ReadColor();
      frc::SmartDashboard::PutString("Color", sensor_fake -> GetColorString(color));
      //frc::SmartDashboard::PutNumber("Confidence", confidence);
      int halfRotations = sensor_fake -> ReadHalfRotations();
      frc::SmartDashboard::PutNumber("Half Rotations", sensor_fake -> ReadHalfRotations());
      if(sensor_fake -> DetectedColor() || halfRotations <= 9)
      {
        colorMotor -> Set(1.0);
      }
    #endif

    // SHOOTING
    /* get gamepad axis */
    //double leftYstick = _joy->GetLeftThrottle();
		// double motorOutput = _talon->GetMotorOutputPercent();
		/* prepare line to print */
		// _sb.append("\tout:");
		// _sb.append(to_string(motorOutput));
		// _sb.append("\tspd:");
		// _sb.append(to_string(_talon->GetSelectedSensorVelocity(kPIDLoopIdx)));
		/* while button1 is held down, closed-loop on target velocity */
		// string button_status;
    // double targetVelocity_UnitsPer100ms = 0;
    // if (_joy->GetButtonA()) 
    // {
        	/* Speed mode */
			/* Convert 500 RPM to units / 100ms.
			 * 4096 Units/Rev * 500 RPM / 600 100ms/min in either direction:
			 * velocity setpoint is in units/100ms
			 */
      // button_status = "pushed";
      // calculate rpm:
      // double desiredRPM = (3.7274 * distanceToVisionTarget) + 4951.4266; // linear regression
      //double desiredRPM = (0.1099* pow(distanceToVisionTarget, 2)) - (33.1961 * distanceToVisionTarget)  + 7968.2759; // quadratic regression
      //double desiredRPM = (0.0017 * pow(distanceToVisionTarget, 3)) - (0.7705 * pow(distanceToVisionTarget, 2)) + (112.4296 * distanceToVisionTarget) + 73.1639; // cubic regression
      //double desiredRPM = (-5.4012 * pow(distanceToVisionTarget, 4)) + (0.0054 * pow(distanceToVisionTarget, 3)) + (-1.6742 * pow(distanceToVisionTarget, 2)) + (211.3463 * distanceToVisionTarget) - 3939.8026; // quartic regression

			// targetVelocity_UnitsPer100ms = desiredRPM * 2048 / 600; //change 4096 to 2048 for unit per rev
      //double targetVelocity_UnitsPer100ms = 0.75 * 500.0 * 2048 / 600; //change 4096 to 2048 for unit per rev
			/* 500 RPM in either direction */
			/* append more signals to print when in speed mode. */
			// _sb.append("\terrNative:");
			// _sb.append(to_string(_talon->GetClosedLoopError(kPIDLoopIdx)));
			// _sb.append("\ttrg:");
			// _sb.append(to_string(targetVelocity_UnitsPer100ms));
      // } 
      // else
      // {
      //   button_status = "not pushed";
      // }
      // _talon->Set(ControlMode::Velocity, targetVelocity_UnitsPer100ms); 

      //else {
			/* Percent voltage mode */
		//_talon->Set(ControlMode::PercentOutput, leftYstick);
		//}
		/* print every ten loops, printing too much too fast is generally bad for performance */
		// SmartDashboard::PutString("button a: ", button_status);

    // if (++_loops >= 10) {
		// 	_loops = 0;
		// 	printf("%s\n",_sb.c_str());
		// }
		// _sb.clear();

    // Set distance & offset --> to give to turret & shooter
    // distanceToVisionTarget = SmartDashboard::GetNumber(VISION_DISTANCE, NO_DATA_DEFAULT); 
	  offsetFromVisionTarget = (SmartDashboard::GetNumber(VISION_OFFSET, NO_DATA_DEFAULT)); //positive is to the right
    // cout << "Distance to vision target: " << distanceToVisionTarget << endl;
    // cout << "offset: " << offsetFromVisionTarget << endl;
    // cout << endl;
    
    //INTAKE
    // bool buttonRB = _joy -> GetButtonRB(); // rollers in
    // bool buttonLB = _joy -> GetButtonLB(); // rollers out

    // if(buttonRB){
    //   intakemech -> RollersIn();
    // }
    // else if(buttonLB){
    //   intakemech -> RollersOut(); 
    // }
    // else{
    //   intakemech -> RollersStop();
    // }
    // if(buttonLB){
    //   intakemech -> MechInRobot();
    // }
    // else if(buttonRB){
    //   intakemech -> MechOutRobot();
    // }

    // INTAKE TO TURRET TRANSPORTATION
    // bool buttonX = _joy -> GetButtonX(); // conveyor belt
    // bool buttonB = _joy -> GetButtonB(); // conveyor belt back
    // bool buttonY = _joy -> GetButtonY(); // bridge

    // if (buttonX) {
    //   // run conveyor belt
    //   conveyorBeltMotor->Set(1.0);
    // }
    // else if (buttonB) {
    //   conveyorBeltMotor->Set(-1.0);
    // }
    // else {
    //   // stop conveyor belt
    //   conveyorBeltMotor->Set(0);
    // }

    // if (buttonY) {
    //   // run bridge motor
    //   bridgeMotor->Set(1.0);
    // }
    // else {
    //   // stop bridge motor
    //   bridgeMotor->Set(0);
    // }

    bool buttonA = _joy->GetButtonA();
    bool buttonX = _joy->GetButtonX();
    bool buttonY = _joy->GetButtonY();
    double desiredRPM = shootingcontroller->GetDesiredRPM();
    double runningRPM = shootingcontroller->GetRunningRPM();

    if(buttonA) {
      shootingcontroller->Target();
      if((runningRPM > (0.95 * desiredRPM)) || (runningRPM > (1.05 * desiredRPM)))
      {
        bridgeMotor->Set(1.0);
        shootingcontroller->StopConveyorBelt();
      }
      else
      {
        bridgeMotor->Set(0.0);
        shootingcontroller->StopConveyorBelt();
      }
    }

    if(buttonX) {
      shootingcontroller->GetOneBall();
    }
    if(buttonY) {
      shootingcontroller->OuttakeOneBall();
    }

    // TURRET (to be integrated into shootercontroller)
    bool buttonStart = _joy->GetButtonStart();
    bool buttonBack = _joy->GetButtonBack();
    if (buttonStart) {
      turretMotor -> Set(0.2); // run to right
    }
    else if (buttonBack) {
      turretMotor -> Set(-0.2); // run to left
    }
    else if ((offsetFromVisionTarget == BAD_DATA) && (!_joy->GetButtonY()))
    {
      turretMotor -> Set(0); // within bounds
    }
    else if ((offsetFromVisionTarget < LOWER_BOUND) && (!_joy->GetButtonY())) {
      turretMotor -> Set(TURRET_SPEED_W_VISION); // run motor to right
    }
    else if ((offsetFromVisionTarget > UPPER_BOUND) && (!_joy->GetButtonY()))
    {
      turretMotor -> Set(-TURRET_SPEED_W_VISION); // run motor to left
    }
    else {
      turretMotor -> Set(0); // within bounds
    }
  }

  virtual void TestPeriodic() override
  {

  }

  virtual void DisabledInit() override
  {

  }

  virtual void DisabledPeriodic() override
  {

  }

private:
};
int main() 
{ 
  return StartRobot<Robot>(); 
}