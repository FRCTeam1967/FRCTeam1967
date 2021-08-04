#include "JankyStateMachineExample.h"

JankyStateMachineExample::JankyStateMachineExample(DifferentialDrive * _pDrive, WPI_TalonSRX * _pFLMotor, WPI_TalonSRX * _pFRMotor, frc::Timer * _pTimer, frc::ADIS16470_IMU * _pGyro, DigitalInput * _pSwitch, VisionSubsystem * _pVision){
    SetMachineName("JankyAutonomousStateMachine");

    SetName(Idle, "Idle");
    SetName(InitialDelay, "InitialDelay");
    SetName(Forward2F, "Forward2F");
    SetName(Right90D, "Right90D");
    SetName(Backward1F, "Backward1F");
    SetName(Pause2S, "Pause2S");
    SetName(Left45D, "Left45D");
    SetName(Right45D, "Right45D");
    SetName(Forward1F, "Forward1F");
    SetName(DoNothing, "DoNothing");

    //sensors:
    pDrive = _pDrive;

    pFLMotor = _pFLMotor;
    pFRMotor = _pFRMotor;
    pTimer = _pTimer;
    pGyro = _pGyro; 
    pSwitch = _pSwitch;
    pVision = _pVision;

    Start();
    
}
JankyStateMachineExample::~JankyStateMachineExample(){

}

void JankyStateMachineExample::StateEngine(int curState, bool onStateEntered){
    switch (curState){
        case Idle:
            //machine stays in idle (doesn't do anything) after initialized until GoAutonomous() is called
            break;
        case InitialDelay:
            if (onStateEntered){
                ResetTimer();
            }
            if (GetTimer() >= STARTUPDELAY){
                NewState(Forward2F, "done with Initial Delay");
            }
            break;
        case Forward2F:
            if (onStateEntered){
                ResetEncoders();
            }
            pDrive -> TankDrive(1, 1); 
            if (GetEncoderAverageDistance() >= 2.0){ 
                NewState(Right90D, "done driving forward 2 feet");
            }
            break;
        case Right90D:
            if (onStateEntered){
                ResetGyro();
            }
            pDrive -> TankDrive(1, 0); 
            if (GetHeading() >= 90){
                NewState(Backward1F, "done turning right 90 degrees");
            }
            break;
        case Backward1F:
            if (onStateEntered){
                ResetEncoders();
            }
            pDrive -> TankDrive (-1, -1);
            if (GetEncoderAverageDistance() <= -1){ 
                NewState(Pause2S, "done driving backward 1 feet");
            }
            break;
        case Pause2S:
            if (onStateEntered){
                ResetTimer();
            }
            if (GetTimer() >= 2.0){
                if (GetVisionOffset()<0){
                    NewState(Left45D, "done pausing for 2 seconds, turn left");
                }
                else if (GetVisionOffset()>0){
                    NewState(Right45D, "done pausing for 2 seconds, turn right");
                }
                else{
                    NewState(Forward1F, "done pausing for 2 seconds");
                }
            }
            break;
        case Left45D:
            if (onStateEntered){
                ResetGyro();
            }
            pDrive -> TankDrive(0, 1);
            if (GetHeading() <= -45){
                NewState(Forward1F, "done turning left 45 degrees");
            }
            break;
        case Right45D:
            if (onStateEntered){
                ResetGyro();
            }
            pDrive -> TankDrive(1,0);
            if (GetHeading() >=45){
                NewState(Forward1F, "done turning right 45 degrees");
            }
            break;
        case Forward1F:
            if (onStateEntered){
                ResetEncoders();
            }
            pDrive -> TankDrive(1,1);
            if (GetEncoderAverageDistance() >= 1){
                if (GetLimitSwitch() == true) {
                    NewState(DoNothing, "done going forward 1 feet, do nothing");
                }
                else{
                    NewState(Pause2S, "done going forward 1 feet, pause");
                }
            }
            break;
    }
}

void JankyStateMachineExample::GoAutonomous(){
	if (GetCurrentState() == Idle){
		NewState(InitialDelay, "leaving idle, starting autonomous");
	}
}

//SENSOR FUNCTIONS:
//encoder
void JankyStateMachineExample::ResetEncoders() {
  pFLMotor -> GetSensorCollection().SetQuadraturePosition(0,10);
  pFRMotor -> GetSensorCollection().SetQuadraturePosition(0,10);
}

double JankyStateMachineExample::GetEncoderAverageDistance(){ 
    //may need to reverse
    double distanceLeft = pFLMotor -> GetSensorCollection().GetQuadraturePosition() * (WHEEL_CIRCUMFERENCE_INCHES / PULSES_PER_REVOLUTION);
    double distanceRight = pFRMotor -> GetSensorCollection().GetQuadraturePosition() * (WHEEL_CIRCUMFERENCE_INCHES / PULSES_PER_REVOLUTION); 

    return ((distanceLeft + distanceRight) / 2) / 12;  //divde by 12 to return feet from inches 
}

//timer:
void JankyStateMachineExample::ResetTimer(){
    pTimer -> Reset();
}

double JankyStateMachineExample::GetTimer(){
    return pTimer -> Get();
}

//gyro:
void JankyStateMachineExample::ResetGyro(){
    pGyro -> Reset();
}

double JankyStateMachineExample::GetHeading(){
    bool kGyroReversed = false;
    return std::remainder(-(pGyro -> GetAngle()), 360) * (kGyroReversed ? -1.0 : 1.0);
}

//vision
double JankyStateMachineExample::GetVisionOffset(){
    return pVision -> GetVisionOffset();
}

//limit switch
bool JankyStateMachineExample::GetLimitSwitch(){
    return pSwitch ->  Get();
}

