#include "WPILib.h"
#include "nivision.h"
#define WRITE_IMAGES
#include "math.h"
#include "jankyRobot.h"
#include "jankyTargeting.h"

/**
 * This is a demo program showing the use of the Gyro for driving in a straight
 * line. The Gyro object in the library returns the heading with 0.0 representing
 * straight and positive or negative angles in degrees for left and right turns.
 * The zero point is established when the gyro object is created so it is important
 * to have the robot still after turning it on.
 * 
 * If you are experiencing drift in the gyro, it can always be reset back to the
 * zero heading by calling Gyro.Reset().
 * 
 * Notice that there is no OperatorControl method. Since this program doesn't supply
 * one the default OperatorControl method will be called from the library.
 */
class GyroSample : public SimpleRobot
{
//	RobotDrive myRobot; // robot drive system
	Gyro gyro;
	Joystick stick;
	JoystickButton button;
	Relay LEDRelay;
//	JoystickButton calbutton; //camera calibration-white balance
	
	
public:
	GyroSample(void):
//		myRobot(1, 2),		// initialize the sensors in initialization list
		gyro(1),
		stick(1),
		button(&stick, 1),
		LEDRelay(FLASHRING_RELAY)

	{
//		myRobot.SetExpiration(0.1);
	}

	void Autonomous(void)
	{
/*		gyro.Reset();
		while (IsAutonomous())
		{
			float angle = gyro.GetAngle();			// current heading (0 = target)
			if (angle > 10)
//				myRobot.Drive (1.0, 0.0);
			else {
				if (angle < -10)
//					myRobot.Drive (-1.0, 0.0);
				else {
//					myRobot.Drive(0.0, 0.0);					
				}
			}
			//myRobot.Drive(-1.0, -angle / 30.0);		// proportionally drive in a straight line
			Wait(0.004);
		}
//		myRobot.Drive(0.0, 0.0); 	// stop robot
*/	}
	
	void OperatorControl(void)
	{ 
		JankyTurret turret(7,11,10);
		JankyTargeting targ(&turret);
		
		JankyShooter shoot(SHOOTER_JAGUAR_CHANNEL,SHOOTER_ENCODER_A,SHOOTER_ENCODER_B);
		SmartDashboard* smarty = SmartDashboard::GetInstance();
		
		while (IsOperatorControl())
		{
//			myRobot.ArcadeDrive(stick);       
	//		shoot.GetCurrentRPM();
			
			if (button.Get()==true)
			{
				LEDRelay.Set(Relay::kForward);
			}
			else 
				LEDRelay.Set(Relay::kOff); 
			
//			float desired=abs(stick.GetY() *1000) + 100;
//			smarty->PutInt("Desired RPM1", (int)desired);
			
			
					
			{
				
				targ.ProcessOneImage();
				targ.ChooseBogey();
				targ.MoveTurret();
				targ.InteractivePIDSetup();
//				shoot.setTargetRPM((int)desired);
				
				
			}
		
		 	
		}		
		
		Wait(0.05);
	}
};

START_ROBOT_CLASS(GyroSample);

