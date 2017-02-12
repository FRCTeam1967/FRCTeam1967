/**
 * Encoder Test - January 2017 encoder test on programming chassis with four motors and two encoders
 */
#include <iostream>
#include <memory>
#include <string>
#include <AnalogAccelerometer.h>
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "WPILib.h"
#include "CANTalon.h"

#define MOTOR_CHANNEL_FL 1
#define MOTOR_CHANNEL_RL 2
#define MOTOR_CHANNEL_FR 6
#define MOTOR_CHANNEL_RR 5
#define ENCODERA_CHANNEL_A 7
#define ENCODERA_CHANNEL_B 8
#define ENCODERB_CHANNEL_A 1
#define ENCODERB_CHANNEL_B 2
#define JOYSTICK_CHANNEL 1
#define ACCELEROMETER 0

class Robot: public IterativeRobot {
    CANTalon*flMotor;
    CANTalon*rlMotor;
    CANTalon*frMotor;
    CANTalon*rrMotor;
    RobotDrive*arcadeDrive;
    Joystick*stick;
    AnalogAccelerometer*accel;
    Encoder*encoderA;
    Encoder*encoderB;

public:
    Robot(){
        flMotor = NULL;
        rlMotor = NULL;
        frMotor = NULL;
        rrMotor = NULL;
        arcadeDrive = NULL;
        stick = NULL;
        accel = NULL;
        encoderA = NULL;
        encoderB = NULL;
    }
    ~Robot(){
        delete flMotor;
        delete rlMotor;
        delete frMotor;
        delete rrMotor;
        delete arcadeDrive;
        delete stick;
        delete accel;
        delete encoderA;
        delete encoderB;
    }
    void RobotInit() {
        flMotor = new CANTalon(MOTOR_CHANNEL_FL);
        rlMotor = new CANTalon(MOTOR_CHANNEL_RL);
        frMotor = new CANTalon(MOTOR_CHANNEL_FR);
        rrMotor = new CANTalon(MOTOR_CHANNEL_RR);
        arcadeDrive = new RobotDrive(flMotor, rlMotor, frMotor, rrMotor);
        stick = new Joystick(JOYSTICK_CHANNEL);
        accel = new AnalogAccelerometer(ACCELEROMETER);
        encoderA = new Encoder(ENCODERA_CHANNEL_A, ENCODERA_CHANNEL_B);
        encoderB = new Encoder(ENCODERB_CHANNEL_A, ENCODERB_CHANNEL_B);

    	encoderA->Reset();
    	encoderB->Reset();

    }
    void AutonomousInit() override {
    }
    void AutonomousPeriodic() {
    }
    void TeleopInit() {

    }
    void TeleopPeriodic() {
        float yVal = stick -> GetY();
        float rotate = stick -> GetX();
        SmartDashboard::PutNumber("yVal", yVal);
        SmartDashboard::PutNumber("xVal", rotate);
       // SmartDashboard::PutNumber("accelerometer", accel->GetAcceleration());
        SmartDashboard::PutNumber("encoderA get: ", encoderA->Get());
        SmartDashboard::PutNumber("encoderB get: ", encoderB->Get());


        arcadeDrive -> ArcadeDrive(yVal, rotate);
    }
    void TestPeriodic() {
    }
private:
};
START_ROBOT_CLASS(Robot)
