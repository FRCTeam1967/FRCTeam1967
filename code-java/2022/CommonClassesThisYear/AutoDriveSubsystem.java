package org.janksters.CommonClassesThisYear;

import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

//auto imports
import edu.wpi.first.math.geometry.Pose2d;
import edu.wpi.first.math.kinematics.DifferentialDriveOdometry;
import edu.wpi.first.math.kinematics.DifferentialDriveWheelSpeeds;
import edu.wpi.first.wpilibj.ADXRS450_Gyro;
import edu.wpi.first.wpilibj.Encoder;
import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.interfaces.Gyro;
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.motorcontrol.PWMSparkMax;
import edu.wpi.first.wpilibj2.command.SubsystemBase;

//other wpilib imports
import edu.wpi.first.wpilibj.motorcontrol.MotorControllerGroup;
import edu.wpi.first.wpilibj.ADIS16470_IMU;
import edu.wpi.first.math.kinematics.DifferentialDriveOdometry;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.math.geometry.Rotation2d;
import edu.wpi.first.math.util.Units;

//ctre imports
import com.ctre.phoenix.motorcontrol.can.WPI_TalonSRX;
import com.ctre.phoenix.motorcontrol.can.WPI_VictorSPX;
import com.ctre.phoenix.motorcontrol.FeedbackDevice;
public class AutoDriveSubsystem extends SubsystemBase {
  // The motor controllers
  private WPI_TalonSRX m_left1 = new WPI_TalonSRX(AutoConstants.kLeftMotor1Port); //CHECK PORTS
  private WPI_VictorSPX m_left2 = new WPI_VictorSPX (AutoConstants.kLeftMotor2Port);
  private WPI_VictorSPX m_right1 = new WPI_VictorSPX (AutoConstants.kRightMotor1Port);
  private WPI_TalonSRX m_right2 = new  WPI_TalonSRX (AutoConstants.kRightMotor2Port);

  // The motors on the left side of the drive
  private MotorControllerGroup m_leftMotors = new MotorControllerGroup(m_left1, m_left2);

  // The motors on the right side of the drive
  private MotorControllerGroup m_rightMotors = new MotorControllerGroup (m_right1, m_right2);

  // The robot's drive
  private DifferentialDrive m_drive = new DifferentialDrive (m_leftMotors, m_rightMotors);

  // The gyro sensor
  //frc::ADXRS450_Gyro m_gyro{frc::SPI::Port::kOnboardCS0};
  private ADIS16470_IMU m_gyro; //{frc::ADIS16470_IMU::IMUAxis::kY, frc::SPI::Port::kOnboardCS0, frc::ADIS16470CalibrationTime::_1s};
  

  // Odometry class for tracking robot pose
  private DifferentialDriveOdometry m_odometry = new DifferentialDriveOdometry(new Rotation2d(Units.degreesToRadians(getHeading())));

  //Timer
  private Timer time;
  private double prevTime;

  //GetWheelSpeed
  private double prevDistanceLeft;
  private double prevDistanceRight;
  private int getWheelSpeedIteration;
  private double leftRate;
  private double rightRate;

  /** Creates a new DriveSubsystem. */
  public AutoDriveSubsystem() {
      
  // configure left encoder
  m_left1.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute, 0, 0);
  m_left1.setSelectedSensorPosition(0, 0, 10);
  m_left1.getSensorCollection().setQuadraturePosition(0,10);

// configure right encoder
  m_right2.configSelectedFeedbackSensor(FeedbackDevice.CTRE_MagEncoder_Absolute, 0, 0);
  m_right2.setSelectedSensorPosition(0, 0, 10);
  m_right2.getSensorCollection().setQuadraturePosition(0,10);

// set differential drive safety to false
m_drive.setSafetyEnabled(false); 

prevTime = 0;

// reset time
time = new Timer();
time.reset();
time.start();

// reset encoders
resetEncoders();

//gyro

m_gyro.setYawAxis(ADIS16470_IMU.IMUAxis.kX);//kY on Luca
}

  @Override
  public void periodic() {
    // Update the odometry in the periodic block
    //m_odometry.update(
    //    m_gyro.getRotation2d(), m_leftEncoder.getDistance(), m_rightEncoder.getDistance());

    double distanceLeft = getLeftEncoder() * (AutoConstants.WHEEL_CIRCUMFERENCE / AutoConstants.PULSES_PER_REVOLUTION);
    double distanceRight = getRightEncoder() * (AutoConstants.WHEEL_CIRCUMFERENCE / AutoConstants.PULSES_PER_REVOLUTION);
  
    // cout << "Left distance: " << distanceLeft << endl;
    // cout << "Right distance: " << distanceRight << endl;
    SmartDashboard.putNumber("Left distance: " , distanceLeft);
    SmartDashboard.putNumber("Right distance: " , distanceRight);
  
    m_odometry.update(new Rotation2d(Units.degreesToRadians(getHeading())),
                      distanceLeft, // QUESTION: WHY IS THIS IN METERS? :(
                      distanceRight); // QUESTION: WHY IS THIS IN METERS? :(
    SmartDashboard.putNumber("Heading: ", getHeading());

  }

  /**
   * Returns the currently-estimated pose of the robot.
   *
   * @return The pose.
   */
  public Pose2d getPose() {
    return m_odometry.getPoseMeters();
  }

  /**
   * Returns the current wheel speeds of the robot.
   *
   * @return The current wheel speeds.
   */
  public DifferentialDriveWheelSpeeds getWheelSpeeds() {
    if (getWheelSpeedIteration++%50==0) {
      // get distance
      double distanceLeft = getLeftEncoder() * (AutoConstants.WHEEL_CIRCUMFERENCE / AutoConstants.PULSES_PER_REVOLUTION);
      double distanceRight = getRightEncoder() * (AutoConstants.WHEEL_CIRCUMFERENCE / AutoConstants.PULSES_PER_REVOLUTION);
  
      //cout << "Left distance: " << distanceLeft << endl;
      //cout << "Right distance: " << distanceRight << endl;
  
      // get time
      double t = time.get();
      double elapsedTime = t - prevTime;
  
      //
      double elapsedDistanceLeft = distanceLeft - prevDistanceLeft;
      double elapsedDistanceRight = distanceRight - prevDistanceRight;
  
      //cout << "time: " << t << endl; // print statement
      SmartDashboard.putNumber("time", t);
  
      // get rate (divide distance by time)
      //double leftRate = distanceLeft / elapsedTime;
      //double rightRate = distanceRight / elapsedTime;
      leftRate = elapsedDistanceLeft / elapsedTime;
      rightRate = elapsedDistanceRight / elapsedTime;
  
    
      prevTime = t;
      prevDistanceLeft=distanceLeft;
      prevDistanceRight=distanceRight;
    }
    return new DifferentialDriveWheelSpeeds(leftRate, rightRate);
    // return {units.meters_per_second_t(leftRate), // units of distance per second
    // units.meters_per_second_t(rightRate)}; // units of distance per second

  }

  /**
   * Resets the odometry to the specified pose.
   *
   * @param pose The pose to which to set the odometry.
   */
  public void resetOdometry(Pose2d pose) {
    resetEncoders();
    
    m_odometry.resetPosition(pose, new Rotation2d (Units.degreesToRadians(getHeading())));

  }

  /**
   * Drives the robot using arcade controls.
   *
   * @param fwd the commanded forward movement
   * @param rot the commanded rotation
   */
  public void arcadeDrive(double fwd, double rot) {
    m_drive.arcadeDrive(fwd, rot);
  }

  /**
   * Controls the left and right sides of the drive directly with voltages.
   *
   * @param leftVolts the commanded left output
   * @param rightVolts the commanded right output
   */
  public void tankDriveVolts(double leftVolts, double rightVolts) {
    m_leftMotors.setVoltage(leftVolts);
    m_rightMotors.setVoltage(rightVolts);
    m_drive.feed(); //wasn't in c++?
  }

  /** Resets the drive encoders to currently read a position of 0. */
  public void resetEncoders() {
    m_left1.getSensorCollection().setQuadraturePosition(0,10);
    m_right2.getSensorCollection().setQuadraturePosition(0,10);
    time.reset();
  }

  /**
   * Gets the average distance of the two encoders.
   *
   * @return the average of the two encoder readings
   */
  public double getAverageEncoderDistance() {
   //return (m_leftEncoder.getDistance() + m_rightEncoder.getDistance()) / 2.0;
   double distanceLeft = getLeftEncoder() * (AutoConstants.WHEEL_CIRCUMFERENCE / AutoConstants.PULSES_PER_REVOLUTION);
    double distanceRight = getRightEncoder() * (AutoConstants.WHEEL_CIRCUMFERENCE / AutoConstants.PULSES_PER_REVOLUTION);
    double avgDist = distanceLeft + distanceRight / 2;
    return avgDist;
  }

  /**
   * Gets the left drive encoder.
   *
   * @return the left drive encoder
   */
  //retur type was origially Ecoder
  //public Encoder getLeftEncoder() {
  //  return m_leftEncoder;
  //}
  public double getLeftEncoder() { 
    //cout << "Left Encoder: " << (-1 * m_left1.GetSensorCollection().GetQuadraturePosition()) << endl; // print statement
    return (-1.0 * m_left1.getSensorCollection().getQuadraturePosition()); 
  }

  /**
   * Gets the right drive encoder.
   *
   * @return the right drive encoder
   */
  public double getRightEncoder() { 
    //cout << "Right Encoder: " << (-1.0 * m_right2.GetSensorCollection().GetQuadraturePosition()) << endl; // print statement
    return (m_right2.getSensorCollection().getQuadraturePosition()); 
  }

  

  /**
   * Sets the max output of the drive. Useful for scaling the drive to drive more slowly.
   *
   * @param maxOutput the maximum output to which the drive will be constrained
   */
  public void setMaxOutput(double maxOutput) {
    m_drive.setMaxOutput(maxOutput);
  }

  /** Zeroes the heading of the robot. */
  public void zeroHeading() {
    m_gyro.reset();
  }

  /**
   * Returns the heading of the robot.
   *
   * @return the robot's heading in degrees, from -180 to 180
   */
  public double getHeading() {
    //cout << "Heading: " << std::remainder(-(m_gyro.GetAngle()), 360) * (kGyroReversed ? -1.0 : 1.0) << endl; // print statement
    return (-(m_gyro.getAngle()%360)) * (AutoConstants.kGyroReversed ? -1.0 : 1.0);
  }
  

  /**
   * Returns the turn rate of the robot.
   *
   * @return The turn rate of the robot, in degrees per second
   */
  public double getTurnRate() {
    return -(m_gyro.getRate()) * (AutoConstants.kGyroReversed ? -1.0 : 1.0);
  }

  public double returnGyroAngle() {
    return m_gyro.getAngle();
  }
  
  public void stopAuto() {
    System.out.println("stopping subsystems");
    time.stop();
    time.reset();
    m_gyro.reset();
  }


}