package org.janksters.CommonClassesThisYear;
 
 
//package frc.robot;
//import edu.wpi.first.wpilibj.TimedRobot;
//import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
//import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
//import edu.wpi.first.wpilibj.XboxController;
//import edu.wpi.first.wpilibj.Solenoid; //2021
//import edu.wpi.first.wpilibj.SolenoidBase;
 
//import org.janksters.robotcode.IntakeMech; //???
//import edu.wpi.first.wpilibj.Joystick;
//import edu.wpi.first.wpilibj.PWMVictorSPX;
//import edu.wpi.first.wpilibj.PWMSparkMax;
//import edu.wpi.first.wpilibj.drive.DifferentialDrive;
//import edu.wpi.first.wpilibj.PWMVictorSPX; //2021
import edu.wpi.first.wpilibj.motorcontrol.PWMVictorSPX; //2022
//import edu.wpi.first.wpilibj.SpeedController;
//import edu.wpi.first.wpilibj.SpeedControllerGroup;
 
//import edu.wpi.first.wpilibj.PneumaticsControlModule;
import edu.wpi.first.wpilibj.Solenoid;
import edu.wpi.first.wpilibj.PneumaticsModuleType;

//import Desktop.JankyXboxJoystick;

public class IntakeMech {
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
  
  public IntakeMech (int PCMChannel, int motorRollChannel,int leftPistonChannel, int rightPistonChannel){
    //PneumaticsControlModule PCM = new PneumaticsControlModule(PCMChannel); //not sure if needed
    motorRoll = new PWMVictorSPX(motorRollChannel);
    leftPiston = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, leftPistonChannel);
    rightPiston = new Solenoid(PCMChannel, PneumaticsModuleType.CTREPCM, rightPistonChannel);
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