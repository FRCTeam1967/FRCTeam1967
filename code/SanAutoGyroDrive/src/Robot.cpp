#include "WPIlib.h"
#include <iostream>
#include <memory>
#include <string>
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "CANTalon.h"
#include <ADXRS450_Gyro.h>
#include <SPI.h>
#define MOTOR_CHANNEL_FL 1
#define MOTOR_CHANNEL_RL 2
#define MOTOR_CHANNEL_FR 6
#define MOTOR_CHANNEL_RR 5
#define JOYSTICK_CHANNEL 0
#include "PIDDrive.h"

//so this is using the ADXRS450 class
class Robot : public frc::IterativeRobot {
    ADXRS450_Gyro*gyro;
    CANTalon*flMotor;
    CANTalon*rlMotor;
    CANTalon*frMotor;
    CANTalon*rrMotor;
    Joystick*stick;
    PIDDrive*myRobot;
    float kP;
    PIDController * PID;

public:
    Robot() {
        gyro = NULL;
        flMotor = NULL;
        rlMotor = NULL;
        frMotor = NULL;
        rrMotor = NULL;
        stick = NULL;
        myRobot = NULL;
        kP = 0.03;
        PID=NULL;

    }
    ~Robot() {
        delete gyro;
        delete flMotor;
        delete rlMotor;
        delete frMotor;
        delete rrMotor;
        delete stick;
        delete myRobot;
        delete PID;

    }
    void RobotInit() {
        gyro = new ADXRS450_Gyro(SPI::Port::kOnboardCS0);
        printf("gyro \n");

        flMotor = new CANTalon(MOTOR_CHANNEL_FL);
        printf("motorFL \n");

        rlMotor = new CANTalon(MOTOR_CHANNEL_RL);
        printf("motorRL\n");

        frMotor = new CANTalon(MOTOR_CHANNEL_FR);
        printf("motorFR\n");

        rrMotor = new CANTalon(MOTOR_CHANNEL_RR);
        printf("motorRR\n");

        myRobot = new PIDDrive(flMotor, rlMotor, frMotor, rrMotor);
        printf("myRobot\n");


        stick = new Joystick(JOYSTICK_CHANNEL);
        printf("stick\n");

        gyro->Calibrate();
        printf("calibrate\n");








    }
    void AutonomousInit() {

    	gyro->Reset();


        PID = new PIDController(kP, 0.005, 0.009, gyro, myRobot);

        PID->Enable();
    /*	if (gyro==NULL) {
    		printf("gyroisnull \n");
    	}
    	else {
    		printf("notnull \n");
    	} */


    	 //printf("reset \n");
    }
    void AutonomousPeriodic() {
        /*    float angle = gyro->GetAngle(); //get heading

       	 printf("get angle \n");
          //  myRobot->Drive(-0.25, -angle * kP); //turn to correct heading
          //  float x = -angle*kP;
       	 float x=30-angle;
       	 if(x==0){
           		 myRobot->Drive(0.0,0.0);
           	 }
       	 else{
            myRobot->Drive(-0.5, -x*kP);
       	 printf("drive \n");
       	 }
        SmartDashboard::PutNumber("Angle_of_Gyro", angle);
        SmartDashboard::PutNumber("Angle_of_Gyro2", angle);
        //returns the actual angle in degrees that the robot is facing
        SmartDashboard::PutNumber("Rate_of_Gyro", gyro->GetRate());
        //returns the rate of rotation of the gyro*/
    	PID->SetInputRange(-180,180);
    	PID->SetOutputRange(-1,1);
        PID->SetSetpoint(30.0);

    }
    void TeleopInit() {
        gyro->Reset();
        //resets the heading to zero
    }
    void TeleopPeriodic() {
    }



    void TestPeriodic() {
//    	gyro->Reset();
//    	    	//NetworkTable table = NetworkTable.getTable("PID Values!!!!!");
//    	    	//table.putDouble("p value", kP);
//
//    	        PID = new PIDController(kP, 0.0, 0.0, gyro, myRobot);
//
//    	        PID->Enable();
//    	    /*	if (gyro==NULL) {
//    	    		printf("gyroisnull \n");
//    	    	}
//    	    	else {
//    	    		printf("notnull \n");
//    	    	} */
//
//
//    	    	 //printf("reset \n");
//    	    	PID->SetInputRange(-180,180);
//    	        	PID->SetOutputRange(-1,1);
//    	            PID->SetSetpoint(30.0);
    }
private:
};
START_ROBOT_CLASS(Robot)
