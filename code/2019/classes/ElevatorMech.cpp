/*     ElevatorMech.cpp
*   created on: Jan 25, 2019
*    by: Sandhya, Isha, GC2 */


// elevator cannot run if hatch pisons are in
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
/*#include <frc/PIDController.h>
#include <frc/PIDSource.h>
#include <frc/PIDOutput.h>*/


#ifdef JANKY_BOT_2019 //Speeds for jankybot & realbot need to be different, because jankybot has CIMS & realbot has miniCIMS
#define L_MOTOR_F_SPEED 0.3 
#define L_MOTOR_R_SPEED -0.2
#else
#define L_MOTOR_F_SPEED 0.5 
#define L_MOTOR_R_SPEED -0.4
#endif
#define MOTOR_STOP_SPEED 0.0
#define UD_PULSES_PER_REVOLUTION 4096

//hysteresis: wiggle room for preset height [if mech reaches within an inch of its destination, stop]
#define UD_HYSTERESI_POS 2.0
#define UD_HYSTERESIS_NEG -2.0

//measurements in inches 
#define INCHES_OFF_GROUND 5.0
#define ROCKET_LOW_CARGO_HEIGHT 27.5 - INCHES_OFF_GROUND 
#define ROCKET_MED_CARGO_HEIGHT 55.5 - INCHES_OFF_GROUND 
#define ROCKET_HIGH_CARGO_HEIGHT 83.5 - INCHES_OFF_GROUND 
#define ROCKET_LOW_HATCH_HEIGHT 19 - INCHES_OFF_GROUND 
#define ROCKET_MED_HATCH_HEIGHT 47 - INCHES_OFF_GROUND 
#define ROCKET_HIGH_HATCH_HEIGHT 75 - INCHES_OFF_GROUND 
#define GROUND_HEIGHT 0.0 - INCHES_OFF_GROUND 
#define HP_HEIGHT 19 - INCHES_OFF_GROUND 
#define CARGO_SHIP_CARGO_HEIGHT 36 - INCHES_OFF_GROUND  // one inch over edge of bay
#define CARGO_SHIP_HATCH_HEIGHT 19 - INCHES_OFF_GROUND 

bool done = false; //used to determine completion of tasks
bool hatchPistonsOut = true; //sets up ConditionalRun - if pistons in, class doesn't run

ElevatorMech::ElevatorMech(int lMotorChannel, int rMotorChannel, int limSwitchBottomChannel, int limSwitchTopChannel) {
    //motors + encoder setup (motors need to be in brake mode)
    kTimeoutMs = 50;
    kPIDLoopIdx = 0;
    lmotor = new WPI_TalonSRX(lMotorChannel);
    rmotor = new WPI_TalonSRX(rMotorChannel); 

    //object pid attempt
    /*lmotor -> PIDGet();
    source -> SetPIDSourceType(lmotor);
    source.encoderCount = leftEncoderCount;
    pid = new frc::PIDController(0.01, 0, 0, desiredHeight, CTRE_MagEncoder_Relative, &lmotor, 500); //source: encoder counts, output: motor speed
    */

    //pid setup
    int absolutePosition = lmotor -> GetSelectedSensorPosition(0);
    lmotor -> SetSelectedSensorPosition(absolutePosition, kPIDLoopIdx, kTimeoutMs);
	lmotor -> ConfigSelectedFeedbackSensor(FeedbackDevice::CTRE_MagEncoder_Relative, kPIDLoopIdx, kTimeoutMs);
	lmotor -> SetSensorPhase(false);
    lmotor -> ConfigNominalOutputForward(0, kTimeoutMs);
	lmotor -> ConfigNominalOutputReverse(0, kTimeoutMs);
	lmotor -> ConfigPeakOutputForward(0.6, kTimeoutMs);
	lmotor -> ConfigPeakOutputReverse(-0.6, kTimeoutMs);
 
	lmotor->Config_kF(kPIDLoopIdx, 0.0, kTimeoutMs); //not using feedforward
	lmotor->Config_kP(kPIDLoopIdx, 0.01, kTimeoutMs); //p val: 0.01 (tune)
	lmotor->Config_kI(kPIDLoopIdx, 0, kTimeoutMs); //i val: 0
	lmotor->Config_kD(kPIDLoopIdx, 0, kTimeoutMs); //d val: 0 (use if needed while tuning p)
    lmotor -> SelectProfileSlot(0, kPIDLoopIdx); //kpidloopidx = pidloopidx?

    rmotor -> Set(ControlMode::Follower, lMotorChannel);

    /*lmotor -> SetSelectedSensorPosition(0, 0, 10);
	lmotor -> GetSensorCollection().SetQuadraturePosition(0,10);
    rmotor -> SetSelectedSensorPosition(0, 0, 10);
	rmotor -> GetSensorCollection().SetQuadraturePosition(0,10);*/


    //lim switches
    bottomLimSwitch = new frc::DigitalInput(limSwitchBottomChannel);
    topLimSwitch = new frc::DigitalInput(limSwitchTopChannel);
    
    //pid -> Enable();
    Start();
}

ElevatorMech::~ElevatorMech(){
    delete lmotor;
    delete rmotor;
    delete bottomLimSwitch;
    delete topLimSwitch;
    //delete pid;
}

void ElevatorMech::EnablePID(){
    lmotor -> Set(ControlMode::Position, desiredHeightPulses);
    //lmotor -> Set(desiredHeightPulses); //necessary? is this done in the above line
    //lmotor -> GetSelectedSensorPosition(kPIDLoopIdx);
    controlMode = "Position, Running Control Loop";
}

//encoder functions
void ElevatorMech::ResetEncoder(){
	lmotor ->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	lmotor -> SetSelectedSensorPosition(0, 0, 10);
    rmotor ->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	rmotor -> SetSelectedSensorPosition(0, 0, 10);
}

double ElevatorMech::GetEncoderCount(){
    leftEncoderCount = lmotor -> GetSensorCollection().GetQuadraturePosition();
    rightEncoderCount = rmotor -> GetSensorCollection().GetQuadraturePosition();
    frc::SmartDashboard::PutNumber("Left Encoder Count", leftEncoderCount);
    frc::SmartDashboard::PutNumber("Right Encoder Count", -rightEncoderCount);
}

double ElevatorMech::GetEncoderDistance(){
    GetEncoderCount();
    leftEncoderDistance = ((((leftEncoderCount*100)/(UD_PULSES_PER_REVOLUTION*GEAR_RATIO))*UD_CIRCUMFERENCE)*THIRD_STAGE_PRESENT)/100;
    rightEncoderDistance = (((-rightEncoderCount*100/(UD_PULSES_PER_REVOLUTION*GEAR_RATIO))*UD_CIRCUMFERENCE)*THIRD_STAGE_PRESENT)/100;
    frc::SmartDashboard::PutNumber("Left Encoder Distance", leftEncoderDistance);
    frc::SmartDashboard::PutNumber("Right Encoder Distance", rightEncoderDistance);   
}

//elevator motor movement functions
void ElevatorMech::ElevatorMotorUp(){
    lmotor -> Set(ControlMode::PercentOutput, L_MOTOR_F_SPEED);
    controlMode = "PercentOutput, Running on Throttle Power";
    //lmotor -> Set(L_MOTOR_F_SPEED);
}

void ElevatorMech::ElevatorMotorDown(){
    lmotor -> Set(ControlMode::PercentOutput, L_MOTOR_R_SPEED);
    controlMode = "PercentOutput, Running on Throttle Power";
    //lmotor -> Set(L_MOTOR_R_SPEED);
}

void ElevatorMech::ElevatorMotorStop(){
    lmotor -> Set(MOTOR_STOP_SPEED);
}

// lim switch values
int ElevatorMech::GetBottomLimSwitch(){
    return bottomLimSwitch -> Get();
}

int ElevatorMech::GetTopLimSwitch(){
    return topLimSwitch -> Get();
}

//presets 

//rocket presets
void ElevatorMech::RocketLowCargoHeight(){
    desiredHeight = ROCKET_LOW_CARGO_HEIGHT;
    isMechanismRunning = true;
    setHeight = "Rocket Low Cargo Height";
}

void ElevatorMech::RocketMedCargoHeight(){
    desiredHeight = ROCKET_MED_CARGO_HEIGHT;
    isMechanismRunning = true;
    setHeight = "Rocket Medium Cargo Height";
}

void ElevatorMech::RocketHighCargoHeight(){
    desiredHeight = ROCKET_HIGH_CARGO_HEIGHT;
    isMechanismRunning = true;
    setHeight = "Rocket High Cargo Height";
}

void ElevatorMech::RocketLowHatchHeight(){
    desiredHeight = ROCKET_LOW_HATCH_HEIGHT;
    isMechanismRunning = true;
    setHeight = "Rocket Low Hatch Height";
}

void ElevatorMech::RocketMedHatchHeight(){
    desiredHeight = ROCKET_MED_HATCH_HEIGHT;
    isMechanismRunning = true;
    setHeight = "Rocket Medium Hatch Height";
}

void ElevatorMech::RocketHighHatchHeight(){
    desiredHeight = ROCKET_HIGH_HATCH_HEIGHT;
    isMechanismRunning = true;
    setHeight = "Rocket High Hatch Height";
}

//ground + hp presets
void ElevatorMech::GroundHeight(){
    desiredHeight = GROUND_HEIGHT;
    isMechanismRunning = true;
    setHeight = "Ground Height";
}

void ElevatorMech::HPHeight(){
    desiredHeight = HP_HEIGHT;
    isMechanismRunning = true;
    setHeight = "Human Player Station Height";
}

//cargo ship presents
void ElevatorMech::ShipCargoHeight(){
    desiredHeight = CARGO_SHIP_CARGO_HEIGHT;
    isMechanismRunning = true;
    setHeight = "Cargo Ship Cargo Height";
}

void ElevatorMech::ShipHatchHeight(){
    desiredHeight = CARGO_SHIP_HATCH_HEIGHT;
    isMechanismRunning = true;
    setHeight = "Cargo Ship Hatch Hatch Height";
}

bool ElevatorMech::GetIfMechIsRunning(){
    return isMechanismRunning;
}

// smartdash comments
void ElevatorMech::SmartDashboardComments(){
    frc::SmartDashboard::PutNumber("Desired Height", desiredHeight);
	frc::SmartDashboard::PutNumber("Amount To Move", amountToMove);
    frc::SmartDashboard::PutNumber("DH Encoder Pulses", desiredHeightPulses);
    frc::SmartDashboard::PutBoolean("Top Limit Switch Value", GetTopLimSwitch());
	frc::SmartDashboard::PutBoolean("Bottom Limit Switch Value", GetBottomLimSwitch());
    frc::SmartDashboard::PutString("Preset Height:", setHeight);
    if (lmotor -> GetControlMode() == ControlMode::Position){
        frc::SmartDashboard::PutString("Current Control Mode:", controlMode);
    }
    else {
        frc::SmartDashboard::PutString("Current Control Mode:", controlMode);
    }
}

// run + check for hatch piston status

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

	needsToPutDownMechanism = true;
    bottomLimSwitchHasNotBeenPressed = true;
    topLimSwitchHasNotBeenPressed = true;
}

//estop if lim switch triggered

void ElevatorMech::EmergencyStop(){
    if ((GetBottomLimSwitch()==1) && bottomLimSwitchHasNotBeenPressed) {
		ElevatorMotorStop();
		isMechanismRunning = false;
		ResetEncoder();
		bottomLimSwitchHasNotBeenPressed = false;
	}
	else if (GetBottomLimSwitch()==0) {
		bottomLimSwitchHasNotBeenPressed = true;
	}

	if ((GetTopLimSwitch()==1) && topLimSwitchHasNotBeenPressed) {
		ElevatorMotorStop();
		isMechanismRunning = false;
		topLimSwitchHasNotBeenPressed = false;
	}
	else if (GetTopLimSwitch()==0) {
		topLimSwitchHasNotBeenPressed = true;
    }
}
//stage 3's highest point is top of stage 2

void ElevatorMech::PutMechanismDown(){
    ElevatorMotorDown();
    if (GetBottomLimSwitch()==1){
        ElevatorMotorStop();
        needsToPutDownMechanism = false;
    }
}

/*void ElevatorMech::FindLocation(double amountToMove){
        if (amountToMove > UD_HYSTERESIS_1_POS) {
            ElevatorMotorDown();
        }
        else if (amountToMove < UD_HYSTERESIS_1_NEG) {
            ElevatorMotorUp();
        }
        else {
            if (amountToMove > UD_HYSTERESIS_2_POS){
                ElevatorMotorDownSpeed2();
            }
            else if (amountToMove < UD_HYSTERESIS_2_NEG){
                ElevatorMotorUpSpeed2();
            }
            else {
                if (amountToMove > UD_HYSTERESIS_3_POS){
                    ElevatorMotorDownSpeed3();
                }
                else if (amountToMove < UD_HYSTERESIS_3_NEG){
                    ElevatorMotorUpSpeed3();
                }
                else if ((amountToMove <= UD_HYSTERESIS_3_POS) && (amountToMove >= UD_HYSTERESIS_3_NEG)){
                    ElevatorMotorStop();
                }
            }
        }
    }
*/

//run functions if piston not out
void ElevatorMech::Run(){ 
    GetEncoderDistance();
    avgEncoderDistance = (leftEncoderDistance + rightEncoderDistance) / 2; //averages left and right encoders to get one uniform variable
    amountToMove = (desiredHeight - avgEncoderDistance); //finds distance to travel - return changing value by calculating it every time
    desiredHeightPulses = ((((((desiredHeight * 100) / THIRD_STAGE_PRESENT) / UD_CIRCUMFERENCE) / GEAR_RATIO) * UD_PULSES_PER_REVOLUTION) / 100);
    EnablePID();
    SmartDashboardComments();
   // if (GetBottomLimSwitch()==0 && GetTopLimSwitch()==0){
        //lmotor -> Set(ControlMode::Position, desiredHeightPulses);
    lmotor -> GetSelectedSensorPosition(kPIDLoopIdx);
    /*}
    else {
        lmotor -> Set(ControlMode::PercentOutput, 0);
        ElevatorMotorStop();
    }*/
    /*if (isMechanismRunning) { //uhp: 0.5, uhn: -0.5
         if (amountToMove > UD_HYSTERESIS_POS) {
             ElevatorMotorDown();
         }
         else if (amountToMove < UD_HYSTERESIS_NEG) {
             ElevatorMotorUp();
         }
         else if ((amountToMove <= UD_HYSTERESIS_POS) && (amountToMove >= UD_HYSTERESIS_NEG)){
             ElevatorMotorStop();
             isMechanismRunning = false;
             if (!done){
                 cout << "Elevator at Desired Height";
                 done = true;
             }
        }
    }*/
}

// PID setup - done in constructor
//void ElevatorMech::PIDSetup() {
	//int absolutePosition = lmotor->GetSelectedSensorPosition(0); /* mask out the bottom12 bits, we don't care about the wrap arounds */
	/* use the low level API to set the quad encoder signal */
	//(lmotor->SetSelectedSensorPosition(absolutePosition, kPIDLoopIdx, kTimeoutMs);

	/* choose the sensor and sensor direction */
	/*lmotor->ConfigSelectedFeedbackSensor(
			FeedbackDevice::CTRE_MagEncoder_Relative, kPIDLoopIdx,
			kTimeoutMs);
	lmotor->SetSensorPhase(false);*/

	/* set the peak and nominal outputs, 12V means full */
	/*lmotor->ConfigNominalOutputForward(0, kTimeoutMs);
	lmotor->ConfigNominalOutputReverse(0, kTimeoutMs);
	lmotor->ConfigPeakOutputForward(1, kTimeoutMs);
	lmotor->ConfigPeakOutputReverse(-1, kTimeoutMs);*/

	/* set closed loop gains in slot0 */
	/*lmotor->Config_kF(kPIDLoopIdx, 0.0, kTimeoutMs);
	lmotor->Config_kP(kPIDLoopIdx, P_VAL, kTimeoutMs);
	lmotor->Config_kI(kPIDLoopIdx, I_VAL, kTimeoutMs);
	lmotor->Config_kD(kPIDLoopIdx, D_VAL, kTimeoutMs);
}*/

//https://www.chiefdelphi.com/t/pid-output-to-a-variable/109158/6
//https://www.chiefdelphi.com/t/pid-setsetpoint/148342