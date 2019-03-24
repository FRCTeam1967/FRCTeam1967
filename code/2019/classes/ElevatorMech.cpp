 /*     ElevatorMech.cpp
*   created on: Jan 25, 2019
*    by: Sandhya, Isha, GC2 */

#include "Settings.h"
#include "frc/WPILib.h" 
#include "jankyTask.h"
#include "ctre/Phoenix.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include <math.h>
#include <frc/Encoder.h>
#include <jankyXboxJoystick.h>
#include "ElevatorMech.h"
#include "ctre/phoenix/motorcontrol/SensorCollection.h"

#ifdef JANKY_BOT_2019 //Speeds for jankybot & realbot need to be different, because jankybot has CIMS & realbot has miniCIMS
#define L_MOTOR_F_SPEED 0.7
#define L_MOTOR_R_SPEED -0.6
#else
#define L_MOTOR_F_SPEED 1.0
#define L_MOTOR_R_SPEED -1.0
#endif
#define MOTOR_STOP_SPEED 0.0
#define UD_PULSES_PER_REVOLUTION 4096

//preset height measurements in inches 
#define INCHES_OFF_GROUND 5.0
#define GROUND_HEIGHT 0.0 - INCHES_OFF_GROUND 
#define BALL_HEIGHT 12 - INCHES_OFF_GROUND
#define ROCKET_LOW_CARGO_HEIGHT 20.5 - INCHES_OFF_GROUND 
#define ROCKET_MED_CARGO_HEIGHT 48.5 - INCHES_OFF_GROUND 
#define ROCKET_HIGH_CARGO_HEIGHT 77.0 - INCHES_OFF_GROUND 
#define ROCKET_LOW_HATCH_HEIGHT 2.0 + GROUND_HEIGHT
#define ROCKET_MED_HATCH_HEIGHT 38 - INCHES_OFF_GROUND 
#define ROCKET_HIGH_HATCH_HEIGHT 70.5 - INCHES_OFF_GROUND 
#define HP_HEIGHT 19 - INCHES_OFF_GROUND 
#define CARGO_SHIP_CARGO_HEIGHT 36 - INCHES_OFF_GROUND  // one inch over edge of bay
#define CARGO_SHIP_HATCH_HEIGHT 19 - INCHES_OFF_GROUND 

//manual movement + max heights in inches
#define AMNT_TO_MOVE_MANUAL_UP 0.6
#define AMNT_TO_MOVE_MANUAL_DOWN -1.0 
#define MAX_HEIGHT 70.0 
#define MIN_HEIGHT 0.0 


ElevatorMech::ElevatorMech(int lMotorChannel, int rMotorChannel, int limSwitchBottomChannel, int limSwitchTopChannel) {
    kTimeoutMs = 50;
    kPIDLoopIdx = 0;
    lmotor = new WPI_TalonSRX(lMotorChannel);
    rmotor = new WPI_TalonSRX(rMotorChannel); 

    StartUpInit();

    //pid setup
    int absolutePosition = lmotor -> GetSelectedSensorPosition(0);
    lmotor -> SetSelectedSensorPosition(absolutePosition, kPIDLoopIdx, kTimeoutMs);
	lmotor -> ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, kPIDLoopIdx, kTimeoutMs);
	lmotor -> SetSensorPhase(false);
    lmotor -> ConfigNominalOutputForward(0, kTimeoutMs);
	lmotor -> ConfigNominalOutputReverse(0, kTimeoutMs);
	lmotor -> ConfigPeakOutputForward(0.6, kTimeoutMs);
	lmotor -> ConfigPeakOutputReverse(-0.08, kTimeoutMs);
 
	lmotor->Config_kF(kPIDLoopIdx, 0.0, kTimeoutMs); //not using feedforward
	lmotor->Config_kP(kPIDLoopIdx, 0.5, kTimeoutMs); 
	lmotor->Config_kI(kPIDLoopIdx, 0, kTimeoutMs);
	lmotor->Config_kD(kPIDLoopIdx, 0, kTimeoutMs); //needs to be tuned
    lmotor -> SelectProfileSlot(0, kPIDLoopIdx); 

    rmotor -> Set(ControlMode::Follower, lMotorChannel);

    //lim switches
    lmotor->ConfigForwardLimitSwitchSource(RemoteLimitSwitchSource_RemoteTalonSRX , LimitSwitchNormal_Disabled , 6, 0);
	lmotor->ConfigReverseLimitSwitchSource(RemoteLimitSwitchSource_RemoteTalonSRX , LimitSwitchNormal_Disabled , 6, 0);
}

ElevatorMech::~ElevatorMech(){
    delete lmotor;
    delete rmotor;
}

void ElevatorMech::EnablePID(){
    CalculateDesiredHeight();
    lmotor -> Set(ControlMode::Position, desiredHeightPulses);
    controlMode = "Position, Running Control Loop";
}

//encoder functions
void ElevatorMech::ResetEncoder(){
	lmotor ->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	lmotor -> SetSelectedSensorPosition(0, 0, 10);
    rmotor ->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	rmotor -> SetSelectedSensorPosition(0, 0, 10);
}

void ElevatorMech::GetEncoderCount(){
    leftEncoderCount = lmotor -> GetSensorCollection().GetQuadraturePosition();
    rightEncoderCount = rmotor -> GetSensorCollection().GetQuadraturePosition();
    frc::SmartDashboard::PutNumber("Left Encoder Count", leftEncoderCount);
    frc::SmartDashboard::PutNumber("Right Encoder Count", -rightEncoderCount);
}

void ElevatorMech::GetEncoderDistance(){
    GetEncoderCount();
    leftEncoderDistance = ((((leftEncoderCount*100)/(UD_PULSES_PER_REVOLUTION*GEAR_RATIO))*UD_CIRCUMFERENCE)*THIRD_STAGE_PRESENT)/100;
    rightEncoderDistance = (((-rightEncoderCount*100/(UD_PULSES_PER_REVOLUTION*GEAR_RATIO))*UD_CIRCUMFERENCE)*THIRD_STAGE_PRESENT)/100;
    frc::SmartDashboard::PutNumber("Left Encoder Distance", leftEncoderDistance);
    frc::SmartDashboard::PutNumber("Right Encoder Distance", rightEncoderDistance);   
}

void ElevatorMech::CalculateDesiredHeight(){
    GetEncoderDistance();
    avgEncoderDistance = (leftEncoderDistance + rightEncoderDistance) / 2; 
    amountToMove = (desiredHeight - avgEncoderDistance); 
    desiredHeightPulses = (((((desiredHeight * 100) / THIRD_STAGE_PRESENT) / UD_CIRCUMFERENCE) * (GEAR_RATIO * UD_PULSES_PER_REVOLUTION) / 100));
    frc::SmartDashboard::PutNumber("Desired Height Pulses", desiredHeightPulses);
}

//elevator motor movement functions
void ElevatorMech::ElevatorMotorUp(){
    if ((desiredHeight + AMNT_TO_MOVE_MANUAL_UP) <= MAX_HEIGHT){
        desiredHeight += AMNT_TO_MOVE_MANUAL_UP;
        EnablePID();
        isMechanismRunning = true;
        setHeight = "Manual Control";
    }
    // lmotor -> Set(ControlMode::PercentOutput, L_MOTOR_F_SPEED);
    // controlMode = "PercentOutput, Running on Throttle Power";
    //lmotor -> Set(L_MOTOR_F_SPEED);
}

void ElevatorMech::ElevatorMotorDown(){
    if ((desiredHeight + AMNT_TO_MOVE_MANUAL_DOWN) >= MIN_HEIGHT){
        desiredHeight += AMNT_TO_MOVE_MANUAL_DOWN;
        EnablePID();
        isMechanismRunning = true;
        setHeight = "Manual Control";
    }
    // lmotor -> Set(ControlMode::PercentOutput, L_MOTOR_R_SPEED);
    // controlMode = "PercentOutput, Running on Throttle Power";
    //lmotor -> Set(L_MOTOR_R_SPEED);
}

void ElevatorMech::ElevatorMotorStop(){
    lmotor -> Set(ControlMode::PercentOutput, MOTOR_STOP_SPEED);
    // lmotor -> Set(MOTOR_STOP_SPEED);
    // controlMode = "PercentOutput, Running on Throttle Power";
}

// lim switch values
bool ElevatorMech::GetBottomLimSwitch(){
    return lmotor->GetSensorCollection().IsFwdLimitSwitchClosed();
}

bool ElevatorMech::GetTopLimSwitch(){
    return lmotor->GetSensorCollection().IsRevLimitSwitchClosed();
}

//preset heights 

//rocket presets
void ElevatorMech::BallHeight()
{
    desiredHeight = BALL_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Rocket Low Cargo Height";
}

void ElevatorMech::RocketLowCargoHeight(){
    desiredHeight = ROCKET_LOW_CARGO_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Rocket Low Cargo Height";
}

void ElevatorMech::RocketMedCargoHeight(){
    desiredHeight = ROCKET_MED_CARGO_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Rocket Medium Cargo Height";
}

void ElevatorMech::RocketHighCargoHeight(){
    desiredHeight = ROCKET_HIGH_CARGO_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Rocket High Cargo Height";
}

void ElevatorMech::RocketLowHatchHeight(){
    desiredHeight = ROCKET_LOW_HATCH_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Rocket Low Hatch Height";
}

void ElevatorMech::RocketMedHatchHeight(){
    desiredHeight = ROCKET_MED_HATCH_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Rocket Medium Hatch Height";
}

void ElevatorMech::RocketHighHatchHeight(){
    desiredHeight = ROCKET_HIGH_HATCH_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Rocket High Hatch Height";
}

//ground + hp presets
void ElevatorMech::GroundHeight(){
    desiredHeight = GROUND_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Ground Height";
}

void ElevatorMech::HPHeight(){
    desiredHeight = HP_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Human Player Station Height";
}

//cargo ship presents
void ElevatorMech::ShipCargoHeight(){
    desiredHeight = CARGO_SHIP_CARGO_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Cargo Ship Cargo Height";
}

void ElevatorMech::ShipHatchHeight(){
    desiredHeight = CARGO_SHIP_HATCH_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Cargo Ship Hatch Hatch Height";
}

bool ElevatorMech::GetIfMechIsRunning(){
    return isMechanismRunning;
}

// smartdash comments
void ElevatorMech::SmartDashboardComments(){
    GetEncoderCount();
    GetEncoderDistance();
    frc::SmartDashboard::PutNumber("Encoder Count", leftEncoderCount);
    frc::SmartDashboard::PutNumber("Encoder Distance", leftEncoderDistance);
    frc::SmartDashboard::PutNumber("Desired Height", desiredHeight);
	frc::SmartDashboard::PutNumber("Amount To Move", amountToMove);
    frc::SmartDashboard::PutNumber("DH Encoder Pulses", desiredHeightPulses);
    frc::SmartDashboard::PutString("Preset Height:", setHeight);
    if (lmotor -> GetControlMode() == ControlMode::Position){
        frc::SmartDashboard::PutString("Current Control Mode:", controlMode);
    }
    else {
        frc::SmartDashboard::PutString("Current Control Mode:", controlMode);
    }
}

void ElevatorMech::StartUpInit(){
    lmotor -> GetSensorCollection().SetQuadraturePosition(0,10);
	lmotor -> SetSelectedSensorPosition(0, 0, 10);
    rmotor -> GetSensorCollection().SetQuadraturePosition(0,10);
	rmotor -> SetSelectedSensorPosition(0, 0, 10);
    SmartDashboardComments();
    setHeight = "None";
	isMechanismRunning = false;
	desiredHeight = 0.0;
	amountToMove = 0.0;
    avgEncoderDistance = 0.0;
    leftEncoderCount = 0.0;
    rightEncoderCount = 0.0;
    leftEncoderDistance = 0.0;
    rightEncoderDistance = 0.0;
    desiredHeight = 0;
    desiredHeightPulses = 0;

    bottomLimSwitchHasNotBeenPressed = true;
    topLimSwitchHasNotBeenPressed = true;
}

//estop if lim switch triggered
void ElevatorMech::EmergencyStop(){
    if ((GetBottomLimSwitch()==true) && bottomLimSwitchHasNotBeenPressed) {
		ElevatorMotorStop();
		isMechanismRunning = false;
		ResetEncoder();
        printf("Bottom limit switch pressed\n");
		bottomLimSwitchHasNotBeenPressed = false;
	}
	else if (GetBottomLimSwitch()==false) {
		bottomLimSwitchHasNotBeenPressed = true;
	}

	if ((GetTopLimSwitch()==true) && topLimSwitchHasNotBeenPressed) {
        GetEncoderCount();
        lmotor -> Set(ControlMode::Position, leftEncoderCount);
		isMechanismRunning = false;
        printf("Top limit switch pressed\n");
		topLimSwitchHasNotBeenPressed = false;
	}
	else if (GetTopLimSwitch()==false) {
		topLimSwitchHasNotBeenPressed = true;
    }
}

void ElevatorMech::Run(){ 
    EmergencyStop();
    SmartDashboardComments();
}
