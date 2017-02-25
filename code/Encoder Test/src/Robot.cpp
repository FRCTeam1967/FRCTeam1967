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
#include "jankyXboxJoystick.h"

#define MOTOR_CHANNEL_FL 1
#define MOTOR_CHANNEL_RL 2
#define MOTOR_CHANNEL_FR 6
#define MOTOR_CHANNEL_RR 5
#define MOTOR_CHANNEL 7
#define ENCODERA_CHANNEL_A 3
#define ENCODERA_CHANNEL_B 4
#define ENCODERB_CHANNEL_A 1
#define ENCODERB_CHANNEL_B 2
#define ENCODERC_CHANNEL 5
#define JOYSTICK_CHANNEL 1
#define ACCELEROMETER 0

class Robot: public IterativeRobot {
    CANTalon*flMotor;
    CANTalon*rlMotor;
    CANTalon*frMotor;
    CANTalon*rrMotor;
    CANTalon*sideMotor;
    RobotDrive*arcadeDrive;
    Joystick*stick;
    AnalogAccelerometer*accel;
    Encoder*encoderA;
    Encoder*encoderB;
    Counter*encoderC;
    jankyXboxJoystick * xbox;

public:
    Robot(){
        flMotor = NULL;
        rlMotor = NULL;
        frMotor = NULL;
        rrMotor = NULL;
        sideMotor = NULL;
        arcadeDrive = NULL;
        stick = NULL;
        accel = NULL;
        encoderA = NULL;
        encoderB = NULL;
        encoderC = NULL;
        xbox = NULL;
    }
    ~Robot(){
        delete flMotor;
        delete rlMotor;
        delete frMotor;
        delete rrMotor;
        delete sideMotor;
        delete arcadeDrive;
        delete stick;
        delete accel;
        delete encoderA;
        delete encoderB;
        delete encoderC;
        delete xbox;
    }
    void RobotInit() {
        flMotor = new CANTalon(MOTOR_CHANNEL_FL);
        rlMotor = new CANTalon(MOTOR_CHANNEL_RL);
        frMotor = new CANTalon(MOTOR_CHANNEL_FR);
        rrMotor = new CANTalon(MOTOR_CHANNEL_RR);
        sideMotor = new CANTalon(MOTOR_CHANNEL);
        arcadeDrive = new RobotDrive(flMotor, rlMotor, frMotor, rrMotor);
        stick = new Joystick(JOYSTICK_CHANNEL);
        accel = new AnalogAccelerometer(ACCELEROMETER);
        encoderA = new Encoder(ENCODERA_CHANNEL_A, ENCODERA_CHANNEL_B);
        encoderB = new Encoder(ENCODERB_CHANNEL_A, ENCODERB_CHANNEL_B);
        encoderC = new Counter(Encoder::EncodingType::k1X, ENCODERC_CHANNEL);
        xbox = new jankyXboxJoystick(0);

    	encoderA->Reset();
    	encoderB->Reset();
    	encoderC->Reset();

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
        SmartDashboard::PutNumber("encoderC get: ", encoderC->Get());

        if(xbox->GetButtonA())
        {
        	sideMotor->Set(.2);
        }
        else
        {
        	sideMotor->Set(0.0);
        }
        //arcadeDrive -> ArcadeDrive(yVal, rotate);
    }
    void TestPeriodic() {
    }
private:
};
START_ROBOT_CLASS(Robot)
