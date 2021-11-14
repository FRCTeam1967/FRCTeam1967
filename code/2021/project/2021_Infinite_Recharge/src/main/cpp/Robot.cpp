//INCLUDES
#include <iostream>
#include <math.h>
// color sensor
//#include "rev/ColorSensorV3.h"
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
#include "frc/livewindow/LiveWindow.h"
#include <frc/trajectory/TrajectoryConfig.h>
#include <frc/trajectory/TrajectoryUtil.h>
#include <frc/trajectory/TrajectoryGenerator.h>
#include <frc/trajectory/constraint/DifferentialDriveVoltageConstraint.h>
// #include <frc/ADXRS450_Gyro.h>
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
#include "AutoSelector2021V2.h"
#include "AutoSettings2021.h"
//#include "ColorSensorInfiniteRecharge.h"
#include "JankyConstants.h"
// #include "IntakeMech.h"
#include "AutoSanDiego.h"
// #include "ShooterController.h"
#include "jankyXboxJoystick.h"
#include "jankyDrivestick.h"
#include "Settings.h"
//#include "JankyPathWeaver.h"
//climbing
#include "ClimbingMech.h"
//#include "frc/WPILib.h"
#include <frc/smartdashboard/SmartDashboard.h>
#include "ctre/phoenix/motorcontrol/SensorCollection.h"
#include <frc/Encoder.h>
#include "jankyTask.h"

//Flywheel 
#include "FlywheelSelector.h"

// to display states in auto
//#include <frc/trajectory/Trajectory.h>
//#include <frc/geometry/Pose2d.h>
//#include <frc/geometry/Rotation2d.h>
//#include <frc/geometry/Translation2d.h>

// NAMESPACES
using namespace ctre;
using namespace frc;
//using namespace rev;
using namespace std;
// using namespace AutoDriveConstants;

//Climbing Defines
#define L_MOTOR_CHANNEL 9 
#define R_MOTOR_CHANNEL 10



class Robot : public TimedRobot {
  //auto
  AutoDriveSubsystem m_drive;
  #ifdef INFINITE_RECHARGE_AT_HOME
  LiveWindow*lw;
  PathSelector*pathSelector;
  RobotSelector*robotSelector;
  frc2::RamseteCommand * rc;
  frc::Trajectory * trajectory;
  #endif
  #ifdef INFINITE_RECHARGE
  AutoSanDiego * autoSD;
  AutoSDDelaySelector * autoSDDelaySelector;
  AutoIntakeSelector * autoIntakeSelector;
  #endif
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
  WPI_TalonSRX * conveyorBeltMotor;
  ShooterControllerInfiniteRecharge * shootingcontroller;
  FlywheelSelector * flywheelSelector;
  // WPI_VictorSPX * bridgeMotor;
  // WPI_TalonSRX * turretMotor; 
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

  bool yWasPressed = false;
  bool xWasPressed = false;
  bool aWasPressed = false;
  bool bWasPressed = false;
  bool rightXAxisWasPressed = false;
  bool rightYAxisWasPressed = false;

  //climbing
  #ifdef CLIMBING_MECH
  WPI_TalonSRX * lmotor;
  WPI_TalonSRX * rmotor;
  ClimbingMech * climbing;
  jankyXboxJoystick * joystick;
  string setHeight;
  #endif

  public:
  //constructor
  Robot()
  {
    // lw = NULL;
    //auto
    #ifdef INFINITE_RECHARGE_ATHOME
    pathSelector = NULL;
    robotSelector = NULL;
    rc = NULL;
    trajectory = NULL;
    #endif
    #ifdef INFINITE_RECHARGE
    autoSD = NULL;
    autoSDDelaySelector = NULL;
    autoIntakeSelector = NULL;
    #endif
    //chassis
    flmotor = NULL;
    rlmotor = NULL;
    frmotor = NULL;
    rrmotor = NULL;
    drive = NULL;
    leftDrive = NULL;
    rightDrive = NULL;
    //climbing
    #ifdef CLIMBING_MECH
    lmotor = NULL;
    rmotor = NULL;
    climbing = NULL;
    joystick = NULL;
    #endif
    //flywheel 
    flywheelSelector = NULL;

    #ifdef CP_ON_ROBOT
      //color sensor
      sensor_fake = NULL;
      colorMotor = NULL;
    #endif
    //shooting
    // intakemech = NULL;
    conveyorBeltMotor = NULL;
    shootingcontroller = NULL;
    // bridgeMotor = NULL;
    // turretMotor = NULL;
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
    // delete lw;
    //auto
    #ifdef INFINITE_RECHARGE_AT_HOME
    delete pathSelector;
    delete robotSelector;
    delete rc;
    delete trajectory;
    #endif
    #ifdef INFINITE_RECHARGE
    delete autoSD;
    delete autoSDDelaySelector;
    delete autoIntakeSelector;
    #endif
    //chassis
    delete flmotor;
    delete rlmotor;
    delete frmotor;
    delete rrmotor;
    delete drive;
    delete leftDrive;
    delete rightDrive;
    //climbing
    #ifdef CLIMBING_MECH
    delete lmotor;
    delete rmotor;
    delete climbing;
    delete joystick;
    #endif
    #ifdef CP_ON_ROBOT
      //color sensor
      delete sensor_fake;
      delete colorMotor;
    #endif
    //shooting
    // delete intakemech;
    delete conveyorBeltMotor;
    delete shootingcontroller;
    // delete bridgeMotor;
    //delete conveyorBeltMotor;
    // delete turretMotor;
    //joysticks
    delete _joy;
    #ifdef DRIVING_WITH_2_JOYS
      delete left;
      delete right;
    #endif
    #ifdef DRIVING_WITH_XBOX
      delete drivingJoy;
    #endif
    delete flywheelSelector;
  }
  
  virtual void RobotInit() override
  {
    // AUTONOMOUS SET-UP
    // Auto Selector
    #ifdef INFINITE_RECHARGE_AT_HOME
    pathSelector = new PathSelector();
    robotSelector = new RobotSelector();
    #endif

    #ifdef INFINITE_RECHARGE
    autoSDDelaySelector = new AutoSDDelaySelector();
    autoIntakeSelector  = new AutoIntakeSelector();
    #endif

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
      // driveCam1.SetPixelFormat(cs::PixelFormat::kMJPEG);
      driveCam1.SetResolution(160,120);
      driveCam1.SetFPS(5);
      driveCam1.GetProperty("compression").Set(100);
    #endif
    #ifdef DRIVE_TEAM_CAM_2
      //Run drive team camera
      cs::UsbCamera driveCam2;
      driveCam2 = CameraServer::GetInstance()->StartAutomaticCapture(1);
      // driveCam1.SetPixelFormat(cs::PixelFormat::kMJPEG);
      driveCam2.SetResolution(50,50);
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
    shootingcontroller = new ShooterControllerInfiniteRecharge(CONVEYOR_BELT_CHANNEL, MOTOR_ROLL_CHANNEL, LEFT_PISTON_CHANNEL, RIGHT_PISTON_CHANNEL, FLYWHEEL_CHANNEL, BRIDGE_CHANNEL, TURRET_MOTOR_CHANNEL);
    flywheelSelector = new FlywheelSelector();
    
    // GET VISION DATA
    SmartDashboard::PutNumber(VISION_DISTANCE, NO_DATA_DEFAULT);
	  SmartDashboard::PutNumber(VISION_OFFSET, NO_DATA_DEFAULT);

    // INTAKE & CONVEYOR BELT & BRIDGE
    conveyorBeltMotor = new WPI_TalonSRX(CONVEYOR_BELT_CHANNEL);
    // bridgeMotor = new WPI_VictorSPX(BRIDGE_CHANNEL);

    // TURRET 
    // turretMotor = new WPI_TalonSRX(TURRET_MOTOR_CHANNEL);
    //climbing
    #ifdef CLIMBING_MECH
    climbing = new ClimbingMech(L_MOTOR_CHANNEL, R_MOTOR_CHANNEL);
    joystick = new jankyXboxJoystick(4);
    climbing -> StartUpInit();
    #endif

    autoSD = new AutoSanDiego(flmotor, frmotor, rlmotor, rrmotor, shootingcontroller);
    // lw = frc::LiveWindow::GetInstance();
  }

  virtual void AutonomousInit() override
  {
  #ifdef INFINITE_RECHARGE_AT_HOME
    auto ksSelected = AutoDriveConstants::jankybotKS;
    auto kvSelected = AutoDriveConstants::jankybotKV;
    auto kaSelected = AutoDriveConstants::jankybotKA; 

    auto kPDriveVel = AutoDriveConstants::jankybotKPDriveVel;
    auto kDDriveVel = AutoDriveConstants::jankybotKDDriveVel;

    // get auto mode
    PathSelector::Options selectedPath = pathSelector->GetSelection();
    RobotSelector::Options selectedRobot = robotSelector -> GetSelection();

    std::string pathName = "doNothing.wpilib.json";
    if (selectedPath == PathSelector::Options::simplePath){
      pathName = "simpleTest.wpilib.json";
    }
    else if (selectedPath == PathSelector::Options::bouncePath){
      pathName = "bounceTest.wpilib.json";
    }
    else if (selectedPath == PathSelector::Options::barrelPath){
      pathName = "barrelTest.wpilib.json";
    }
    else if (selectedPath == PathSelector::Options::slalomPath){
      pathName = "slalomTest.wpilib.json";
    }
    else if (selectedPath == PathSelector::Options::testPath1){
      pathName = "TestPath1.wpilib.json";
    }
    else if (selectedPath == PathSelector::Options::testPath2){
      pathName = "TestPath2.wpilib.json";;
    }
    else if (selectedPath == PathSelector::Options::testPath3){
      pathName = "TestPath3.wpilib.json";
    } 
    SmartDashboard::PutString("Trajectory Path: ", pathName);

    if (selectedRobot == RobotSelector::Options::jankybot){
      ksSelected = AutoDriveConstants::jankybotKS;
      kvSelected = AutoDriveConstants::jankybotKV;
      kaSelected = AutoDriveConstants::jankybotKA;

      kPDriveVel = AutoDriveConstants::jankybotKPDriveVel;
      kDDriveVel = AutoDriveConstants::jankybotKDDriveVel;
    }
    else if (selectedRobot == RobotSelector::Options::luca){
      ksSelected = AutoDriveConstants::lucaKS;
      kvSelected = AutoDriveConstants::lucaKV;
      kaSelected = AutoDriveConstants::lucaKA;

      kPDriveVel = AutoDriveConstants::lucaKPDriveVel;
      kDDriveVel = AutoDriveConstants::lucaKDDriveVel;
    }
    
    //Create a voltage constraint to ensure we don't accelerate too fast
    cout << "Creating voltage constraint" << endl;
    frc::DifferentialDriveVoltageConstraint autoVoltageConstraint(
        frc::SimpleMotorFeedforward<units::meters>(
            ksSelected,
            kvSelected,
            kaSelected
          ),
          AutoDriveConstants::kDriveKinematics,
          10_V
        );

    //Configure trajectory
    cout << "Config Trajectory" << endl;
    frc::TrajectoryConfig config(AutoConstants::kMaxSpeed, AutoConstants::kMaxAcceleration); // Set up config for trajectory
    config.SetKinematics(AutoDriveConstants::kDriveKinematics); // Add kinematics to ensure max speed is actually obeyed
    config.AddConstraint(autoVoltageConstraint); // Apply the voltage constrain

    //Get Trajectory
    cout << "Get Trajectory" << endl;
    wpi::SmallString<64> deployDirectory;
    frc::filesystem::GetDeployDirectory(deployDirectory);
    wpi::sys::path::append(deployDirectory, "output");
    wpi::sys::path::append(deployDirectory, pathName);
    //wpi::sys::path::append(deployDirectory, "TestPath1.wpilib.json");
    //frc::Trajectory trajectory;
    trajectory = new frc::Trajectory();
    
    try{
      *trajectory = frc::TrajectoryUtil::FromPathweaverJson(deployDirectory);
    } catch(...){
      printf("Failed to pass path into trajectory \n");
      exit(-999);
    }
    

   //try to manually make basic trajextory
   //*trajectory = frc::TrajectoryGenerator::GenerateTrajectory(frc::Pose2d(0_m,0_m, frc::Rotation2d(0_deg)), {frc::Translation2d(1_m, 0_m)}, frc::Pose2d(2_m,0_m,frc::Rotation2d(0_deg)),config);

    /*
    //try manually making basic trajectory to see if pathweaver is the issue
    auto trajectory2 = frc::TrajectoryGenerator::GenerateTrajectory(frc::Pose2d(0_m,0_m,frc::Rotation2d(0_deg)),
    {frc::Translation2d(1_m, 0_m)},
    frc::Pose2d(3_m,0_m, frc::Rotation2d(0_deg)),
    config);
    */


    //Create PID controller & set tolerance
    cout << "Creating pid controller" << endl;
    frc2::PIDController leftController(kPDriveVel, 0, kDDriveVel); // left
    frc2::PIDController rightController(kPDriveVel, 0, kDDriveVel); // right
    leftController.SetTolerance(0.0);
    rightController.SetTolerance(0.0);

    //frc::Transform2d transform = Pose2d(0_m, 0_m, Rotation2d(0_deg))-trajectory->InitialPose();
    //*trajectory = trajectory->TransformBy(transform);

    //Instantiate ramsete command
    cout << "Instantiate ramsete command" << endl;
    rc = new frc2::RamseteCommand(
      *trajectory, [this]() { return m_drive.GetPose(); },
      frc::RamseteController(AutoConstants::kRamseteB,
                            AutoConstants::kRamseteZeta),
      frc::SimpleMotorFeedforward<units::meters>(
          ksSelected, kvSelected, kaSelected),
      AutoDriveConstants::kDriveKinematics,
      [this] { return m_drive.GetWheelSpeeds(); },
      leftController,
      rightController,
      [this](auto left, auto right) { m_drive.TankDriveVolts(left, right); },
      {&m_drive});

    SmartDashboard::PutNumber("Robot Heading before reset Odometry: ", m_drive.GetHeading());
    
    //Reset odometry to starting pose of the trajectory
    m_drive.ResetOdometry(trajectory->InitialPose());
    
    //displaying other information about trajectory;
    SmartDashboard::PutNumber("Testing SmartDashboard, ", 5.0);
    SmartDashboard::PutNumber("Trajectory Total Time: ", trajectory->TotalTime().to<float>());
    SmartDashboard::PutNumber("Trajectory Initial Pose Degrees: ", trajectory->InitialPose().Rotation().Degrees().to<float>());
    SmartDashboard::PutNumber("Trajectory Initial Pose X: ", trajectory->InitialPose().Translation().X().to<float>());
    SmartDashboard::PutNumber("Trajectory Initial Pose Y: ", trajectory->InitialPose().Translation().Y().to<float>());
    SmartDashboard::PutNumber("Robot Initial Pose Degrees: ", m_drive.GetPose().Rotation().Degrees().to<float>());
    SmartDashboard::PutNumber("Robot Initial Pose X: ", m_drive.GetPose().Translation().X().to<float>());
    SmartDashboard::PutNumber("Robot Initial Pose Y: ", m_drive.GetPose().Translation().Y().to<float>());
    SmartDashboard::PutNumber("Robot Heading after reset Odometry: ", m_drive.GetHeading());
    
    i = 0;
    j = 0;

    // Initialize ramsete command
    rc->Initialize();
  #endif

  #ifdef INFINITE_RECHARGE
    autoSD -> setInitialDelayTime(autoSDDelaySelector->GetSelection());
    autoSD -> setIntakeUpDown(autoIntakeSelector -> GetSelection());
  #endif
  }

  virtual void AutonomousPeriodic() override
  {

  #ifdef INFINITE_RECHARGE_AT_HOME
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
         
       m_drive.Periodic();

      units::second_t currentTime = (units::second_t)SmartDashboard::GetNumber("time", 0.0); //get time from AutoDriveSubsystem, cast to second_t

      char currentTrajectoryInfo[160]; //should be exactly 145
      //what it returns in order: running iteration count, time from SmartDashboard, seconds from state, velocity, acceleration, curvature
      //trajectory pose rotation in degrees, trajectory pose x translation, trajectory pose y translation, 
      //real robot heading degrees, robot heading radians, robot pose rotation , robot left wheel speed, robot right wheel speed, robot pose x translation, robot pose y translation
      sprintf (currentTrajectoryInfo, "I:%3d SDT:%4.0f S:%4.0f V:%6.3f A:%6.3f C:%5.0f TR:%5.1f TX,Y:(%5.2f, %5.2f), RHD:%5.1f,RHR: %5.1f, RR:%5.1f, RLWS: %7.1f, RRWS: %7.1f, RX,Y:(%5.2f, %5.2f)",
              i, currentTime.to<float>(), trajectory->Sample(currentTime).t.to<float>(), trajectory->Sample(currentTime).velocity.to<float>(), trajectory->Sample(currentTime).acceleration.to<float>(), trajectory->Sample(currentTime).curvature.to<float>(),
              trajectory->Sample(currentTime).pose.Rotation().Degrees().to<float>(), trajectory->Sample(currentTime).pose.Translation().X().to<float>(), trajectory->Sample(currentTime).pose.Translation().Y().to<float>(), 
              m_drive.GetHeading(), m_drive.GetHeading()*(M_PI/180), m_drive.GetPose().Rotation().Degrees().to<float>(), m_drive.GetWheelSpeeds().left.to<float>(), m_drive.GetWheelSpeeds().right.to<float>(), m_drive.GetPose().Translation().X().to<float>(), m_drive.GetPose().Translation().Y().to<float>());
      cout << currentTrajectoryInfo << endl;

       if (currentTime.to<int>()<=0.1){
         SmartDashboard::PutNumber("Robot distance X at start: ", m_drive.GetPose().Translation().X().to<float>());
         SmartDashboard::PutNumber("Robot distance Y at start: ", m_drive.GetPose().Translation().Y().to<float>());
       }

       if (currentTime.to<int>()<=5){
         SmartDashboard::PutNumber("Robot distance X at 5 sec: ", m_drive.GetPose().Translation().X().to<float>());
         SmartDashboard::PutNumber("Robot distance Y at 5 sec: ", m_drive.GetPose().Translation().Y().to<float>());
       }

       if (currentTime.to<int>()<=10){
         SmartDashboard::PutNumber("Robot distance X at 10 sec: ", m_drive.GetPose().Translation().X().to<float>());
         SmartDashboard::PutNumber("Robot distance Y at 10 sec: ", m_drive.GetPose().Translation().Y().to<float>());
       }

      SmartDashboard::PutNumber("Trajectory Velocity: ", trajectory->Sample(currentTime).acceleration.to<float>());
      SmartDashboard::PutNumber("Left Wheel Spped: ",m_drive.GetWheelSpeeds().left.to<float>());
      SmartDashboard::PutNumber("Right Wheel Speed: ", m_drive.GetWheelSpeeds().right.to<float>());

      SmartDashboard::PutNumber("Auto Left Wheel Distance: ", m_drive.GetLeftEncoder() * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION));
      SmartDashboard::PutNumber("Auto Right Wheel Distance: ", m_drive.GetRightEncoder() * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION));


     }
     else {
       SmartDashboard::PutNumber("End COUNT???", j);
       j++;
       cout << "end" << endl;
       //rc->End(true);  
       m_drive.StopAuto();
     }

    SmartDashboard::PutNumber("gyro heading", m_drive.GetHeading());
  #endif

  #ifdef INFINITE_RECHARGE 
    autoSD->RunAuto();
    i = 0;
    j = 0;
  #endif
  }

  virtual void TeleopInit() override
  {
    flywheelSelector ->DisplayShootingOptions();
  }

  virtual void TeleopPeriodic() override
  {
    //climbing
    #ifdef CLIMBING_MECH
    float rightVal = joystick -> GetRightYAxis();
    #endif
    // CHASSIS ENCODERS
    cout << "Left Encoder: " << rlmotor->GetSensorCollection().GetQuadraturePosition() << endl;
    cout << "Right Encoder: " << (-1.0 * frmotor->GetSensorCollection().GetQuadraturePosition()) << endl;
    SmartDashboard::PutNumber("Left Encoder Teleop:", rlmotor->GetSensorCollection().GetQuadraturePosition());
    SmartDashboard::PutNumber("Right Encoder Teleop: ", (-1.0 * frmotor->GetSensorCollection().GetQuadraturePosition()));
    double rlMotorEncoder=rlmotor->GetSensorCollection().GetQuadraturePosition();
    double rlMotorEncoderInches=rlMotorEncoder * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION);
    double frMotorEncoder=-1.0*frmotor->GetSensorCollection().GetQuadraturePosition();
    double frMotorEncoderInches=frMotorEncoder *  (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION);

    SmartDashboard::PutNumber("Heading in Teleop: ", m_drive.GetHeading());

    SmartDashboard::PutNumber("Left Distance in Inches: ", rlMotorEncoderInches);
    SmartDashboard::PutNumber("Right Distance in Inches: ", frMotorEncoderInches);

    SmartDashboard::PutNumber("Left Wheel Speed Teleop: ",m_drive.GetWheelSpeeds().left.to<float>());
    SmartDashboard::PutNumber("Right Wheel Speed Teleop: ", m_drive.GetWheelSpeeds().right.to<float>());

    SmartDashboard::PutNumber("Teleop Left Wheel Distance: ", m_drive.GetLeftEncoder() * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION));
    SmartDashboard::PutNumber("Teleop Right Wheel Distance: ", m_drive.GetRightEncoder() * (WHEEL_CIRCUMFERENCE / PULSES_PER_REVOLUTION));

    // CHASSIS
    #ifdef DRIVING_WITH_2_JOYS
      #ifdef PROG_BOT
      bool drivingToggle = left -> Get4();
      #endif

      #ifdef JANKYCHASSIS
      bool drivingToggle = left -> Get4();
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
    else if (!shootingSideFront)
    {
      #ifdef DRIVING_WITH_2_JOYS
        drive -> TankDrive(right->GetY(), left->GetY());
      #endif
      #ifdef DRIVING_WITH_XBOX
        drive -> TankDrive(pow(drivingJoy->GetLeftYAxis(), 2), pow(drivingJoy->GetRightYAxis(), 2));
      #endif
    }

    bool driveStraightButton = right->Get5();
    if(driveStraightButton && shootingSideFront) {
      float avg = ((left->GetY()) + (right->GetY())) / 2;
      drive -> TankDrive(-avg, -avg);
    }
    else if (driveStraightButton && !shootingSideFront) {
      float avg = ((left->GetY()) + (right->GetY())) / 2;
      drive -> TankDrive(avg, avg);
    }
  
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

    // Set offset --> to give to turret
	  offsetFromVisionTarget = (SmartDashboard::GetNumber(VISION_OFFSET, NO_DATA_DEFAULT)); //positive is to the right
    
    //INTAKE
    bool buttonRB = _joy -> GetButtonRB(); // rollers in
    bool buttonLB = _joy -> GetButtonLB(); // rollers out
    if(buttonLB){
      shootingcontroller -> IntakePistonsUp();
    }
    else if(buttonRB){
      shootingcontroller -> IntakePistonsDown();
    }

    // INTAKE TO TURRET TRANSPORTATION
    bool buttonY = _joy -> GetButtonY(); // bridge
    //SmartDashboard::PutBoolean("button Y", buttonY);
    //SmartDashboard::PutBoolean("yWasPressed", yWasPressed);

    // CONVEYOR BELT & BRIDGE
     if (buttonY && !yWasPressed) { 
      // run bridge motor
        shootingcontroller->BridgeForward();
        //  bridgeMotor->Set(1.0);
         //shootingcontroller->StartConveyorBelt();
         yWasPressed = true;
         //SmartDashboard::PutNumber("Bridge running", 0);
     }
     else if (!buttonY && yWasPressed) {
      //SmartDashboard::PutNumber("Bridge running", 1);
      // stop bridge motor
      shootingcontroller->StopBridge();
      // bridgeMotor->Set(0);
      shootingcontroller->StopConveyorBelt();
      yWasPressed = false; //correct verrsion
     }
     
    //bridge out
     float rightYAxis = _joy->GetRightYAxis();//NOTE: currently uses the same as climbing; hard when both uses flywehel and bridge out right axis?
     if (rightYAxis >=0.2 && !rightYAxisWasPressed){
       shootingcontroller->BridgeBackward();
       rightYAxisWasPressed = true;
     } 
     else if (!(rightYAxis >=0.2) && rightYAxisWasPressed){
       shootingcontroller->StopBridge();
       rightYAxisWasPressed = false;
     }
     

    bool buttonA = _joy->GetButtonA();
    bool buttonX = _joy->GetButtonX();
    bool buttonB = _joy->GetButtonB();
    double desiredRPM = shootingcontroller->GetDesiredRPM();
    double runningRPM = shootingcontroller->GetRunningRPM();
    cout << "desired rpm: " << desiredRPM << endl;
    cout << "running rpm: " << runningRPM << endl; 

    // FLYWHEEL
    bool leftButton = _joy->GetLeftStickButton();
    bool rightButton = _joy->GetRightStickButton();
    float leftThrottle = _joy->GetLeftThrottle();
    float rightThrottle = _joy->GetRightThrottle();

    #ifdef MANUAL_DISTANCE_FOR_FLYWHEEL
    static int shootingZone = 1; //this doesn't change shootingZone to 1 during later calls, right?
    
    if (rightButton)
    {
      shootingZone = 1;
    }
    else if (leftThrottle > 0.2)
    {
      shootingZone = 2;
    }
    else if(rightThrottle > 0)
    {
      shootingZone = 3;
    }
    else if(leftButton)
    {
      shootingZone = 4;
    }

    shootingcontroller->SetSelectorVisionDistance(shootingZone);
    #endif

    if(buttonA) {
      shootingcontroller->Target();
      aWasPressed = true;
    }
    else if (aWasPressed){
      shootingcontroller->StopTarget();
      aWasPressed = false;
    }

    //flywheel out
    
     float rightXAxis = _joy->GetRightXAxis();
     if (rightXAxis >=0.2 && !rightXAxisWasPressed){ //is it 0.0?  
      shootingcontroller->FlywheelOut();
      rightXAxisWasPressed = true; 
     }
     else if (rightXAxis <=0.2 && rightXAxisWasPressed){
       shootingcontroller->StopTarget();
       rightXAxisWasPressed = false;
     }

     
    if(buttonX && !xWasPressed) {
      shootingcontroller->SetDesiredCount(-100); //was -4096
      //shootingcontroller->ConveyorBeltOut(); //not pulsing
      xWasPressed = true;
    }
    else if(buttonB && !bWasPressed) {
      shootingcontroller->SetDesiredCount(100); //was 4096
      //shootingcontroller->StartConveyorBelt();  //not pulsing
      bWasPressed = true;
    }
    else {
      xWasPressed = false;  //reset encoder count?
      bWasPressed = false;
    }

    // INTAKE
    float intakeJoy = _joy->GetLeftYAxis();

    if(intakeJoy > 0.2) {
      shootingcontroller->IntakeOut();
    }
    else if (intakeJoy < -0.2) {
      shootingcontroller->IntakeIn();
    }
    else {
      shootingcontroller->IntakeStop();
    }

    // TURRET
    bool buttonStart = _joy->GetButtonStart();
    bool buttonBack = _joy->GetButtonBack();

    SmartDashboard::PutNumber("gyro heading", m_drive.GetHeading());

    if (buttonStart) {
      shootingcontroller-> TurretRight(TURRET_SPEED_MANUAL);
      // turretMotor -> Set(0.2); // run to right
    }
    else if (buttonBack) {
      shootingcontroller-> TurretLeft(TURRET_SPEED_MANUAL);
      // turretMotor -> Set(-0.2); // run to left
    }
    else if ((offsetFromVisionTarget == BAD_DATA) && (!_joy->GetButtonY()))
    {
      shootingcontroller->StopTurret();
      // turretMotor -> Set(0); // within bounds
    }
    else if ((offsetFromVisionTarget < LOWER_BOUND) && (!_joy->GetButtonY())) {
      shootingcontroller-> TurretRight(TURRET_USING_VISION);
      // turretMotor -> Set(TURRET_USING_VISION); // run motor to right
    }
    else if ((offsetFromVisionTarget > UPPER_BOUND) && (!_joy->GetButtonY()))
    {
      shootingcontroller-> TurretLeft(TURRET_USING_VISION);
      // turretMotor -> Set(-TURRET_USING_VISION); // run motor to left
    }
    else {
      shootingcontroller->StopTurret();
      // turretMotor -> Set(0); // within bounds
    }
    //climbing
    //manual controls
    #ifdef CLIMBING_MECH
    if (rightVal >= 0.2){
      climbing -> ClimbingMotorDown();
      setHeight = "None";
    }
    else if (rightVal <= -0.2){
      climbing -> ClimbingMotorUp();
      setHeight = "None";
    }
    else if (rightVal < 0.2 && rightVal > -0.2){
      climbing -> ClimbingMotorStop();
      setHeight = "None";
    }
    #endif
  
  

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