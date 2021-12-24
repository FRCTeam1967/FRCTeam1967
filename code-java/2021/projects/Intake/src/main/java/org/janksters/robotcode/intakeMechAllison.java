package org.janksters.robotcode;
 
 
//package frc.robot;
//import edu.wpi.first.wpilibj.TimedRobot;
//import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
//import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
//import edu.wpi.first.wpilibj.XboxController;
import edu.wpi.first.wpilibj.Solenoid;
//import edu.wpi.first.wpilibj.SolenoidBase;
 
import org.janksters.robotcode.intakeMechAllison;
//import edu.wpi.first.wpilibj.Joystick;
//import edu.wpi.first.wpilibj.PWMVictorSPX;
//import edu.wpi.first.wpilibj.PWMSparkMax;
//import edu.wpi.first.wpilibj.drive.DifferentialDrive;
import edu.wpi.first.wpilibj.PWMVictorSPX;
//import edu.wpi.first.wpilibj.SpeedController;
//import edu.wpi.first.wpilibj.SpeedControllerGroup;
 
//import Desktop.JankyXboxJoystick;

public class intakeMechAllison {
  //motor speed
      //roller intake speed
  final double MOTOR_ROLL_F_SPEED=-0.75;
  //roller outtake speed
  final double MOTOR_ROLL_R_SPEED=0.75;
  //stops motor
  final double MOTOR_STOP_SPEED=0.0;
 
  PWMVictorSPX motorRoll;
  Solenoid leftPiston;
  Solenoid rightPiston;
 
  public intakeMechAllison(int motorRollChannel,int leftPistonChannel, int rightPistonChannel){
       motorRoll = new PWMVictorSPX(motorRollChannel);
       leftPiston = new Solenoid(11, leftPistonChannel);
       rightPiston = new Solenoid(11, rightPistonChannel);
  }
 
  public void RollersIn() {
      motorRoll.set(MOTOR_ROLL_F_SPEED);
 
  }
  public void RollersOut() {
   motorRoll.set(MOTOR_ROLL_R_SPEED);
   }

  public void RollersStop() {
   motorRoll.set(MOTOR_STOP_SPEED);
 
  }
 
  public void MechInRobot(){
      leftPiston.set(false);
      rightPiston.set(false);
  }
  public void MechOutRobot(){
      leftPiston.set(true);
      rightPiston.set(true);
    }
}