/*     ElevatorMech.cpp
*   created on: Jan 25, 2019
*    by: Sandhya, Isha, GC2 */

// 2 cim motors
// 2 encoders to return distance (w/ preset heights for rocket + ground  + cargo + hp)
// 2 lim switches on stages 1 and 2, controlling next stage

// elevator cannot run if hatch pisons are in

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

#define L_MOTOR_F_SPEED 0.8
#define L_MOTOR_R_SPEED -0.9
//#define R_MOTOR_F_SPEED 0.8
//#define R_MOTOR_R_SPEED -0.9
#define MOTOR_STOP_SPEED 0.0

#define UD_PULSES_PER_REVOLUTION 4096
#define GEAR_RATIO 12 
#define SPROCKET_TEETH 22 
#define SPROCKET_INCHES_PER_TOOTH 0.25 

//hysteresis: wiggle room for preset height [if mech reaches within an inch of its destination, stop]
#define UD_HYSTERESIS_POS 0.5
#define UD_HYSTERESIS_NEG -0.5

#define UD_CIRCUMFERENCE 5.5 //22 teeth & size 25 chain - recalculate for 2019
//1.8125 * M_PI
#define THIRD_STAGE_PRESENT 2 

//measurements in inches 
#define ROCKET_LOW_CARGO_HEIGHT 27.5 
#define ROCKET_MED_CARGO_HEIGHT 55.5 
#define ROCKET_HIGH_CARGO_HEIGHT 83.5 
#define ROCKET_LOW_HATCH_HEIGHT 19
#define ROCKET_MED_HATCH_HEIGHT 47
#define ROCKET_HIGH_HATCH_HEIGHT 75 
#define GROUND_HEIGHT 0.0
#define HP_HEIGHT 19
#define CARGO_SHIP_CARGO_HEIGHT 36 // one inch over edge of bay
#define CARGO_SHIP_HATCH_HEIGHT 19

// pid values - used for regulation [add later]; p - prop, i - int, d - der
/*#define P_VAL 0.5
#define I_VAL 0.0
#define D_VAL 0.0*/

bool done = false; //used to determine completion of tasks
bool hatchPistonsIn = false; //sets up ConditionalRun - if pistons in, class doesn't run

ElevatorMech::ElevatorMech(int lMotorChannel, int rMotorChannel, int limSwitchBottomChannel, int limSwitchTopChannel) {
    //motors + encoder setup (motors need to be in brake mode)
    lMotor = new WPI_TalonSRX(lMotorChannel);
    rMotor = new WPI_TalonSRX(rMotorChannel);
    rMotor -> Set(ControlMode::Follower, lMotorChannel);
	lMotor ->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	lMotor -> SetSelectedSensorPosition(0, 0, 10);
	lMotor -> GetSensorCollection().SetQuadraturePosition(0,10);

    //lim switches
    bottomLimSwitch = new frc::DigitalInput(limSwitchBottomChannel);
    topLimSwitch = new frc::DigitalInput(limSwitchTopChannel);
}

ElevatorMech::~ElevatorMech(){
    delete lMotor;
    delete rMotor;
    delete bottomLimSwitch;
    delete topLimSwitch;
}

//encoder functions
void ElevatorMech::ResetEncoder(){
	lMotor ->ConfigSelectedFeedbackSensor(CTRE_MagEncoder_Absolute, 0, 0);
	lMotor -> SetSelectedSensorPosition(0, 0, 10);
}

double ElevatorMech::GetEncoderCount(){
    double leftEncoderCount= lMotor -> GetSensorCollection().GetQuadraturePosition();
    double rightEncoderCount= rMotor -> GetSensorCollection().GetQuadraturePosition();
    frc::SmartDashboard::PutNumber("Left Encoder Count", leftEncoderCount);
    frc::SmartDashboard::PutNumber("Right Encoder Count", rightEncoderCount);
}

double ElevatorMech::GetEncoderDistance(){
    GetEncoderCount();
    double leftEncoderDistance = ((leftEncoderCount/(UD_PULSES_PER_REVOLUTION*GEAR_RATIO))*UD_CIRCUMFERENCE)*THIRD_STAGE_PRESENT;
    double rightEncoderDistance = ((rightEncoderCount/(UD_PULSES_PER_REVOLUTION*GEAR_RATIO))*UD_CIRCUMFERENCE)*THIRD_STAGE_PRESENT;
    frc::SmartDashboard::PutNumber("Left Encoder Distance", leftEncoderDistance);
    frc::SmartDashboard::PutNumber("Right Encoder Distance", rightEncoderDistance);
        
}

//elevator motor movement functions
void ElevatorMech::ElevatorMotorUp(){
    lMotor -> Set(L_MOTOR_F_SPEED);
}

void ElevatorMech::ElevatorMotorDown(){
    lMotor -> Set(L_MOTOR_R_SPEED);
}

void ElevatorMech::ElevatorMotorStop(){
    lMotor -> Set(MOTOR_STOP_SPEED);
}

// lim switch values
int ElevatorMech::GetBottomLimSwitch(){
    return bottomLimSwitch -> Get();
}

int ElevatorMech::GetTopLimSwitch(){
    return topLimSwitch -> Get();
}

//presets

//rocket presents
void ElevatorMech::RocketLowCargoHeight(){
    desiredHeight = ROCKET_LOW_CARGO_HEIGHT;
    isMechanismRunning = true;
}

void ElevatorMech::RocketMedCargoHeight(){
        desiredHeight = ROCKET_MED_CARGO_HEIGHT;
    isMechanismRunning = true;
}

void ElevatorMech::RocketHighCargoHeight(){
    desiredHeight = ROCKET_HIGH_CARGO_HEIGHT;
    isMechanismRunning = true;
}

void ElevatorMech::RocketLowHatchHeight(){
    desiredHeight = ROCKET_LOW_HATCH_HEIGHT;
    isMechanismRunning = true;
}

void ElevatorMech::RocketMedHatchHeight(){
    desiredHeight = ROCKET_MED_HATCH_HEIGHT;
    isMechanismRunning = true;
}

void ElevatorMech::RocketHighHatchHeight(){
    desiredHeight = ROCKET_HIGH_HATCH_HEIGHT;
    isMechanismRunning = true;
}

//ground + hp presets
void ElevatorMech::GroundHeight(){
    desiredHeight = GROUND_HEIGHT;
    isMechanismRunning = true;
}

void ElevatorMech::HPHeight(){
    desiredHeight = HP_HEIGHT;
    isMechanismRunning = true;
}

//cargo ship presents
void ElevatorMech::ShipCargoHeight(){
    desiredHeight = CARGO_SHIP_CARGO_HEIGHT;
    isMechanismRunning = true;
}

void ElevatorMech::ShipHatchHeight(){
    desiredHeight = CARGO_SHIP_HATCH_HEIGHT;
    isMechanismRunning = true;
}

bool ElevatorMech::GetIfMechIsRunning(){
    return isMechanismRunning;
}

// smartdash comments
void ElevatorMech::SmartDashboardComments(){
    frc::SmartDashboard::PutNumber("Desired Height", desiredHeight);
	frc::SmartDashboard::PutNumber("Amount To Move", amountToMove);
    frc::SmartDashboard::PutBoolean("Top Limit Switch Value", GetTopLimSwitch());
	frc::SmartDashboard::PutBoolean("Bottom Limit Switch Value", GetBottomLimSwitch());
}

// run + check for hatch piston status

void ElevatorMech::StartUpInit(){
    lMotor -> GetSensorCollection().SetQuadraturePosition(0,10);
	lMotor -> SetSelectedSensorPosition(0, 0, 10);
	isMechanismRunning = false;
	desiredHeight = 0.0;
	amountToMove = 0.0;

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

//run functions if piston not out
void ElevatorMech::Run(){
    GetEncoderCount();
    avgEncoderCount = ((leftEncoderCount + rightEncoderCount) / 2); //averages left and right encoders to get one uniform variable
    amountToMove = (desiredHeight - avgEncoderCount * -1); //finds distance to travel - return changing value by calculating it every time?
    SmartDashboardComments();
    if (isMechanismRunning) {
        if (amountToMove > UD_HYSTERESIS_POS) {
            ElevatorMotorDown();
        }
        else if (amountToMove < UD_HYSTERESIS_NEG) {
            ElevatorMotorUp();
        }
        else if ((amountToMove < UD_HYSTERESIS_POS) && (amountToMove > UD_HYSTERESIS_NEG)){
            ElevatorMotorStop();
            isMechanismRunning = false;
            if (!done){
                cout << "Elevator at Desired Height";
                done = true;
            }

        }
    }
}

void ElevatorMech::ConditionalRun(){
    if (hatchPistonsIn){
        Pause();
    }
} // call in robot.cpp


// PID setup - add if necessary
/*void ElevatorMech::PIDSetup() {
	int absolutePosition = lMotor->GetSelectedSensorPosition(0); /* mask out the bottom12 bits, we don't care about the wrap arounds */
	/* use the low level API to set the quad encoder signal */
	//(lMotor->SetSelectedSensorPosition(absolutePosition, kPIDLoopIdx,
	//		kTimeoutMs);
    

	/* choose the sensor and sensor direction 
	lMotor->ConfigSelectedFeedbackSensor(
			FeedbackDevice::CTRE_MagEncoder_Relative, kPIDLoopIdx,
			kTimeoutMs);
	lMotor->SetSensorPhase(false);*/

	/* set the peak and nominal outputs, 12V means full */
	/*lMotor->ConfigNominalOutputForward(0, kTimeoutMs);
	lMotor->ConfigNominalOutputReverse(0, kTimeoutMs);
	lMotor->ConfigPeakOutputForward(1, kTimeoutMs);
	lMotor->ConfigPeakOutputReverse(-1, kTimeoutMs);*/

	/* set closed loop gains in slot0 */
	/*lMotor->Config_kF(kPIDLoopIdx, 0.0, kTimeoutMs);
	lMotor->Config_kP(kPIDLoopIdx, P_VAL, kTimeoutMs);
	lMotor->Config_kI(kPIDLoopIdx, I_VAL, kTimeoutMs);
	lMotor->Config_kD(kPIDLoopIdx, D_VAL, kTimeoutMs);*/
//}