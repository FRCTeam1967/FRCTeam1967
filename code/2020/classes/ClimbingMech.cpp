#include "Settings.h"
#include "frc/WPILib.h" 
#include "jankyTask.h"
#include "ctre/Phoenix.h"
#include <iostream>
#include <frc/smartdashboard/SmartDashboard.h>
#include <math.h>
#include <frc/Encoder.h>
#include <jankyXboxJoystick.h>
#include "ClimbingMech.h"
#include "ctre/phoenix/motorcontrol/SensorCollection.h"

#ifdef JANKY_BOT_2020 //told us cims but might be mini cims later
#else
#define L_MOTOR_F_SPEED 1.0
#define L_MOTOR_R_SPEED -1.0
#endif
#define MOTOR_STOP_SPEED 0.0
#define UD_PULSES_PER_REVOLUTION 4096

//preset height measurements in inches 
#define INCHES_OFF_GROUND 2.0
#define GROUND_HEIGHT 0.0 - INCHES_OFF_GROUND 
//#define BALL_HEIGHT 19 - INCHES_OFF_GROUND
#define HIGHEST_HEIGHT 78.875 - INCHES_OFF_GROUND 
#define LEVEL_HEIGHT 63.0 - INCHES_OFF_GROUND

//manual movement + max heights in inches
#define AMNT_TO_MOVE_MANUAL_UP 0.6
#define AMNT_TO_MOVE_MANUAL_DOWN -1.0 
#define MAX_HEIGHT 77.0 
#define MIN_HEIGHT 0.0 
#define AMNT_TO_MOVE_HP 6.0
#define AMNT_TO_MOVE_DOWN -0.75

//variables for pid loop
#define PEAK_OUTPUT_FWD .7 //.6
#define PEAK_OUTPUT_REVERSE -0.08
#define K_P 0.5
#define K_I 0.0
#define K_D 0.0

ClimbingMech::ClimbingMech(int lMotorChannel, int rMotorChannel, int rightSolenoidChannel, int leftSolenoidChannel) {
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
	lmotor -> ConfigPeakOutputForward(PEAK_OUTPUT_FWD, kTimeoutMs);
	lmotor -> ConfigPeakOutputReverse(PEAK_OUTPUT_REVERSE, kTimeoutMs);
 
	lmotor->Config_kF(kPIDLoopIdx, 0.0, kTimeoutMs); //not using feedforward
	lmotor->Config_kP(kPIDLoopIdx, K_P, kTimeoutMs); 
	lmotor->Config_kI(kPIDLoopIdx, K_I, kTimeoutMs);
	lmotor->Config_kD(kPIDLoopIdx, K_D, kTimeoutMs); //needs to be tuned
    lmotor -> SelectProfileSlot(0, kPIDLoopIdx); 

    rmotor -> Set(ControlMode::Follower, lMotorChannel);

    //lim switches
    /*
    lmotor->ConfigForwardLimitSwitchSource(RemoteLimitSwitchSource_RemoteTalonSRX , LimitSwitchNormal_Disabled , 6, 0);
	lmotor->ConfigReverseLimitSwitchSource(RemoteLimitSwitchSource_RemoteTalonSRX , LimitSwitchNormal_Disabled , 6, 0);
    */
}

ClimbingMech::~ClimbingMech(){
    delete lmotor;
    delete rmotor;
}

void ClimbingMech::EnablePID(){
    CalculateDesiredHeight();
    lmotor -> Set(ControlMode::Position, desiredHeightPulses);
    controlMode = "Position, Running Control Loop";
}

//encoder functions
void ClimbingMech::ResetEncoder(){
	lmotor ->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	lmotor -> SetSelectedSensorPosition(0, 0, 10);
    rmotor ->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	rmotor -> SetSelectedSensorPosition(0, 0, 10);
}

void ClimbingMech::GetEncoderCount(){
    leftEncoderCount = lmotor -> GetSensorCollection().GetQuadraturePosition();
    rightEncoderCount = rmotor -> GetSensorCollection().GetQuadraturePosition();
    frc::SmartDashboard::PutNumber("Left Encoder Count", leftEncoderCount);
    frc::SmartDashboard::PutNumber("Right Encoder Count", -rightEncoderCount);
}

void ClimbingMech::GetEncoderDistance(){
    GetEncoderCount();
    leftEncoderDistance = ((((leftEncoderCount*100)/(UD_PULSES_PER_REVOLUTION*GEAR_RATIO))*UD_CIRCUMFERENCE)*THIRD_STAGE_PRESENT)/100;
    rightEncoderDistance = (((-rightEncoderCount*100/(UD_PULSES_PER_REVOLUTION*GEAR_RATIO))*UD_CIRCUMFERENCE)*THIRD_STAGE_PRESENT)/100;
    frc::SmartDashboard::PutNumber("Left Encoder Distance", leftEncoderDistance);
    frc::SmartDashboard::PutNumber("Right Encoder Distance", rightEncoderDistance);   
}

void ClimbingMech::CalculateDesiredHeight(){
    GetEncoderDistance();
    avgEncoderDistance = (leftEncoderDistance + rightEncoderDistance) / 2; 
    amountToMove = (desiredHeight - avgEncoderDistance); 
    desiredHeightPulses = (((((desiredHeight * 100) / THIRD_STAGE_PRESENT) / UD_CIRCUMFERENCE) * (GEAR_RATIO * UD_PULSES_PER_REVOLUTION) / 100));
    frc::SmartDashboard::PutNumber("Desired Height Pulses", desiredHeightPulses);
}

//climbing motor movement functions
void ClimbingMech::ClimbingMotorUp(){
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

void ClimbingMech::ClimbingMotorDown(){
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

void ClimbingMech::ClimbingMotorStop(){
    lmotor -> Set(ControlMode::PercentOutput, MOTOR_STOP_SPEED);
    // lmotor -> Set(MOTOR_STOP_SPEED);
    // controlMode = "PercentOutput, Running on Throttle Power";
}

// lim switch values
/*
bool ClimbingMech::GetBottomLimSwitch(){
    return lmotor->GetSensorCollection().IsFwdLimitSwitchClosed();
}

bool ClimbingMech::GetTopLimSwitch(){
    return lmotor->GetSensorCollection().IsRevLimitSwitchClosed();
    */
}

//preset heights 
void ClimbingMech::HighestHeight(){
    desiredHeight = HIGHEST_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Highest Height";
}

void ClimbingMech::LevelHeight(){
    desiredHeight = LEVEL_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Level Height";
}

void ClimbingMech::GroundHeight(){
    desiredHeight = GROUND_HEIGHT;
    EnablePID();
    isMechanismRunning = true;
    setHeight = "Ground Height";
}

bool ClimbingMech::GetIfMechIsRunning(){
    return isMechanismRunning;
}

// smartdash comments
void ClimbingMech::SmartDashboardComments(){
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

void ClimbingMech::StartUpInit(){
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

    //bottomLimSwitchHasNotBeenPressed = true;
    //topLimSwitchHasNotBeenPressed = true;
}

//estop if lim switch triggered
/*
void ClimbingMech::EmergencyStop(){
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
    */
}

void ClimbingMech::Run(){ 
    EmergencyStop();
    SmartDashboardComments();
}
