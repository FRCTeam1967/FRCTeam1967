#include "WPILib.h"
#include "nivision.h"
#define WRITE_IMAGES
#include "math.h"
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
	RobotDrive myRobot; // robot drive system
	Gyro gyro;
//	HSLImage image;
	Joystick stick;
	JoystickButton button;
//	JoystickButton calbutton; //camera calibration-white balance
	
	
public:
	GyroSample(void):
		myRobot(1, 2),		// initialize the sensors in initialization list
		gyro(1),
		stick(1),
		button(&stick, 1)
//		calbutton(&stick, 2)
	{
		myRobot.SetExpiration(0.1);
	}
	
	/**
	 * Drive in a straight line using the gyro.
	 * This short program uses simple proportional to correct for errors in heading.
	 * Proportional control simply means that the amount of correction to the heading is
	 * proportional to the error. Since we are trying to drive straight, then the
	 * desired gyro value is 0.0. The gyro heading, which varies from 0.0 as the robot
	 * turns is supplied as the curve parameter to the Drive method. This method takes
	 * values from -1 to 1 to represent turns left and right. 0 is no turn.
	 * 
	 * While the gyro heading is 0.0 (the desired heading) there will be no turn. As the
	 * heading increases in either direction, it results in a proportionally larger
	 * turn to counteract the error. The angle is divided by 30 to scale the value so
	 * the robot doesn't turn too fast or too slowly while correcting.
	 */
	void Autonomous(void)
	{
		gyro.Reset();
		while (IsAutonomous())
		{
			float angle = gyro.GetAngle();			// current heading (0 = target)
			if (angle > 10)
				myRobot.Drive (1.0, 0.0);
			else {
				if (angle < -10)
					myRobot.Drive (-1.0, 0.0);
				else {
					myRobot.Drive(0.0, 0.0);					
				}
			}
			//myRobot.Drive(-1.0, -angle / 30.0);		// proportionally drive in a straight line
			Wait(0.004);
		}
		myRobot.Drive(0.0, 0.0); 	// stop robot
	}
	
	void OperatorControl(void)
	{ 
//		DriverStationLCD *dslcd = DriverStationLCD::GetInstance();
//		char debugout[100];

		SmartDashboard *smarty = SmartDashboard::GetInstance();
		AxisCamera& camera = AxisCamera::GetInstance("10.19.67.11"); 
		camera.WriteResolution(AxisCamera::kResolution_320x240); 
		camera.WriteBrightness(50); 
//		camera.WriteWhiteBalance(kWhiteBalance_Automatic);		
		int count = 0;
		int fayz = 0; 
		
		HSLImage canon; 
		Wait(3.0);
		
		
		while (IsOperatorControl())
		{
			myRobot.ArcadeDrive(stick);       
			
//			dslcd->Clear();
			
			if (button.Get()==true){ 
				
			camera.IsFreshImage();
			
					if (camera.IsFreshImage()==true)
					{ 
						count++;
								
						smarty->PutInt("Number of fresh images",count);
								
						//sprintf(debugout, "Number of fresh images=%d", count);
						//dslcd->Printf(DriverStationLCD::kUser_Line1, 1, debugout);
														
						int success = camera.GetImage((ColorImage*)&canon);
								
						if ( canon.GetHeight()!=0 && canon.GetWidth()!=0 ) {
								
							fayz++;
									
#ifdef WRITE_IMAGES									
							canon.Write("hsl.jpg");
#endif			
							smarty->PutInt("Number of valid images",fayz);
							//sprintf(debugout, "Number of valid images=%d", fayz);
							//dslcd->Printf(DriverStationLCD::kUser_Line2, 1, debugout);
									
//							BinaryImage* samwise = canon.ThresholdHSL(154,231,33,255,131,255);
//							BinaryImage* samwise = canon.ThresholdHSL(185,255,95,255,65,255);
							
							BinaryImage* samwise = canon.ThresholdHSL(120,170,60,255,100,253);
							
#ifdef WRITE_IMAGES
							int tempparticles = samwise->GetNumberParticles();
									
							smarty->PutInt("Number of threshold particles",tempparticles);
							
							bool gotparticles = false;
									
							if (tempparticles > 0)
								gotparticles = true;
										
							if (gotparticles == true)
								samwise->Write("purple.bmp");	
#endif	
							Image* imaqImage = samwise->GetImaqImage();
									
							imaqConvexHull(imaqImage,imaqImage,TRUE);
#ifdef WRITE_IMAGES									
							if (gotparticles == true)
								samwise->Write("convex.bmp");
#endif
							int particles = samwise->GetNumberParticles();
									
							smarty->PutInt("Number of convex particles",particles);
							//sprintf(debugout, "Number of particles=%d", particles);
							//dslcd->Printf(DriverStationLCD::kUser_Line3, 1, debugout);

							vector<ParticleAnalysisReport>* vPAR = samwise->GetOrderedParticleAnalysisReports();
							if (vPAR) 	// Sanity - did we get a report back?
							{
								printf("Particle Analysis Report for Particles follows:\n");
								
								for (int i=0 ; i<vPAR->size() ; i++)
								{
									//char temp [100];
									
									float d=1.0;
									
									ParticleAnalysisReport& par = (*vPAR)[i];	// Get the i-th entry from the vector - "Report # i"
									// Calculate score
									// Avoid floating point disasters by multiplying before dividing
									
									int rect_score = (int)(((par.particleArea) / (par.boundingRect.width*par.boundingRect.height))*100.0);
									
//									float rect_score = par.particleArea / (par.boundingRect.width*par.boundingRect.height);
									
									int BRcenterx = (int)(par.boundingRect.left + par.boundingRect.width*0.5);
									
									int BRcentery = (int)(par.boundingRect.top + par.boundingRect.height*0.5);
									
									//float distance = 0.00000051169*pow (par.particleArea, 2)-0.01611*par.particleArea+178.2972;
									//find different equation based on BR width, then compare accuracy
									
									//#-particle number, A-particle area, PS-particle score, BRC-bounding rect. center, PCM-particle center, D=distance 
									
									if (i==0)
									{
										float PI = 3.14159265; 
										float inpx = 24.0 / par.boundingRect.width; //inches per pixels (proportion)
										float imw = 320.0 * inpx; //image width in inches
										printf("inches per pixels=%f,image width=%f\n",inpx,imw);
										d = (0.5 * imw) / tan (27.0*PI/180.0); //PI/180 is converting radians to degrees
									}
									
									printf("#=%d,A=%04.2f,PS=%d,D=%04f",i,par.particleArea,rect_score,d);
									
									printf(" BRW=%d,BRC=%d,%d,PCM=%d,%d\n",par.boundingRect.width,BRcenterx,BRcentery,par.center_mass_x,par.center_mass_y);
									
									// Escape hatch - early - just want to deal with first 4 reports MAX.
									if (i == 3)
										break;
								}
								delete vPAR;
							}
//							ParticleAnalysisReport report;

/*							for(int x = 0 ; x < 4 ; x++)
							{
								samwise->GetParticleAnalysisReport(x, &report);
							}
*/							
							delete samwise;
						}
					}
							//dslcd->UpdateLCD();		
									}
			
			//compute distance
			
			Wait(0.005);	}

	}
};

START_ROBOT_CLASS(GyroSample);
