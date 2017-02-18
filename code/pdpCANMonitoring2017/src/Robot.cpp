/*
 * PDP CAN Monitoring 2017
 * prints pdp current values on SmartDashboard
 * has one talon w/an encoder
 * prints output current of talon -- gives accurate values on the board
 */
#include <PowerDistributionPanel.h>
#include <SampleRobot.h>
#include <SmartDashboard/SmartDashboard.h>
#include <Timer.h>
#include "WPILib.h"
#include "jankyXboxJoystick.h"
#include "CANTalon.h"

#define FL_MOTOR_CHANNEL 1
#define RL_MOTOR_CHANNEL 2
#define FR_MOTOR_CHANNEL 5
#define RR_MOTOR_CHANNEL 6
#define JOYSTICK_CHANNEL 0
#define MOTOR_CHANNEL 7
#define ENCODER_A_CHANNEL 3
#define ENCODER_B_CHANNEL 4

/**
 * This is a sample program showing how to retrieve information from the Power
 * Distribution Panel via CAN. The information will be displayed under variables
 * through the SmartDashboard.
 */

class Robot: public frc::IterativeRobot {
	//CANTalon*flMotor;
	//CANTalon*rlMotor;
	//CANTalon*frMotor;
	CANTalon*rrMotor;
	RobotDrive*arcadeDrive;
	Joystick*joystick;
	CANTalon*motor;
	Encoder*encoder;
	PowerDistributionPanel*pdp;
	double max12 = 0;
	double max13 = 0;

public:

	Robot(){
		//flMotor = NULL;
		//rlMotor = NULL;
		//frMotor = NULL;
		rrMotor = NULL;
		arcadeDrive = NULL;
		joystick = NULL;
		motor = NULL;
		encoder = NULL;
		pdp = NULL;


	}

	~Robot(){
		//delete flMotor;
		//delete rlMotor;
		//delete frMotor;
		delete rrMotor;
		delete arcadeDrive;
		delete joystick;
		delete motor;
		delete encoder;
		delete pdp;
	}

	/**
	 * Retrieve information from the PDP over CAN and displays it on the
	 * SmartDashboard interface. SmartDashboard::PutNumber takes a string (for a
	 * label) and a double; GetCurrent takes a channel number and returns a
	 * double for current, in Amperes. Channel numbers are printed on the PDP
	 * and range from 0-15.
	 */

	void RobotInit(){
		//flMotor = new CANTalon(FL_MOTOR_CHANNEL);
		//rlMotor = new CANTalon(RL_MOTOR_CHANNEL);
		//frMotor = new CANTalon(FR_MOTOR_CHANNEL);
		rrMotor = new CANTalon(RR_MOTOR_CHANNEL);
		//arcadeDrive = new RobotDrive(flMotor, rlMotor, frMotor, rrMotor);
		joystick = new Joystick(JOYSTICK_CHANNEL);
		motor = new CANTalon(MOTOR_CHANNEL);
		encoder = new Encoder(ENCODER_A_CHANNEL, ENCODER_B_CHANNEL);
		pdp = new PowerDistributionPanel();
	}


	void TeleopPeriodic() {
		while (IsOperatorControl() && IsEnabled()) {
			float yval = joystick -> GetY();
			//float rotate = joystick -> GetX();

			printf("Total Current: %f", pdp -> GetTotalCurrent());
			printf("     Get Current 12: %f", pdp -> GetCurrent(12));
			printf("     Get Current 13: %f", pdp -> GetCurrent(13));
			printf("     Get Current 14: %f", pdp -> GetCurrent(14));
			printf("     Get Current 15: %f", pdp -> GetCurrent(15));

			if(pdp -> GetCurrent(12) > max12){
				max12 = pdp -> GetCurrent(12);
			}

			if(pdp -> GetCurrent(13) > max13){
				max13 = pdp -> GetCurrent(13);
			}





			//arcadeDrive -> ArcadeDrive(yval, rotate);

			rrMotor -> Set(yval);
			/* Get the current going through channel 7, in Amperes. The PDP
			 * returns the current in increments of 0.125A. At low currents the
			 * current readings tend to be less accurate.
			 */

			if(yval > 0.5){
				rrMotor -> Set(1);
			}

			if(yval < -0.5){
				rrMotor -> Set(-1);
			}

			if(yval < -0.5 && yval > 0.5){
				rrMotor -> Set(0);
			}

			//rrMotor -> Set(0);


			//motor -> Set(0.2);

			frc::SmartDashboard::PutNumber("Current Channel 0", pdp -> GetCurrent(0));
			frc::SmartDashboard::PutNumber("Current Channel 1", pdp -> GetCurrent(1));
			frc::SmartDashboard::PutNumber("Current Channel 2", pdp -> GetCurrent(2));
			frc::SmartDashboard::PutNumber("Current Channel 3", pdp -> GetCurrent(3));
			frc::SmartDashboard::PutNumber("Current Channel 4", pdp -> GetCurrent(4));
			frc::SmartDashboard::PutNumber("Current Channel 5", pdp -> GetCurrent(5));
			frc::SmartDashboard::PutNumber("Current Channel 6", pdp -> GetCurrent(6));
			frc::SmartDashboard::PutNumber("Current Channel 7", pdp -> GetCurrent(7));
			frc::SmartDashboard::PutNumber("Current Channel 8", pdp -> GetCurrent(8));
			frc::SmartDashboard::PutNumber("Current Channel 9", pdp -> GetCurrent(9));
			frc::SmartDashboard::PutNumber("Current Channel 10", pdp -> GetCurrent(10));
			frc::SmartDashboard::PutNumber("Current Channel 11", pdp -> GetCurrent(11));
			frc::SmartDashboard::PutNumber("Current Channel 12", pdp -> GetCurrent(12));
			frc::SmartDashboard::PutNumber("Current Channel 13", pdp -> GetCurrent(13));
			frc::SmartDashboard::PutNumber("Current Channel 14", pdp -> GetCurrent(14));
			frc::SmartDashboard::PutNumber("Current Channel 15", pdp -> GetCurrent(15));

			frc::SmartDashboard::PutNumber("Total Current", pdp -> GetTotalCurrent());

			frc::SmartDashboard::PutNumber("Encoder Value", encoder -> Get());

			frc::SmartDashboard::PutNumber("RR Talon Current", rrMotor -> GetOutputCurrent());

			frc::SmartDashboard::PutNumber("Max Current", max12);



			/* Get the voltage going into the PDP, in Volts.
			 * The PDP returns the voltage in increments of 0.05 Volts.
			 */

			frc::SmartDashboard::PutNumber("Voltage", pdp -> GetVoltage());

			// Retrieves the temperature of the PDP, in degrees Celsius.
			frc::SmartDashboard::PutNumber("Temperature",
					pdp -> GetTemperature());

			frc::Wait(kUpdatePeriod);
		}
	}

private:
	// Object for dealing with the Power Distribution Panel (PDP).

	// Update every 5milliseconds/0.005 seconds.
	static constexpr double kUpdatePeriod = 0.005;
};

START_ROBOT_CLASS(Robot)
