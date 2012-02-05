///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// Targeting for Rebound Rumble 2012

#include "jankyTargeting.h"

/// Constructor - do initialization here.
JankyTargeting::JankyTargeting(void)
{
	AxisCamera &camera = AxisCamera::GetInstance(CAMERA_IP); 
	smarty = SmartDashboard::GetInstance();
	
	camera.WriteResolution(AxisCamera::kResolution_320x240); 
	camera.WriteBrightness(50); 
//		camera.WriteWhiteBalance(kWhiteBalance_Automatic);		

	Wait(3.0);
	//	printf("Hello from Janky Targeting land.\n");
}

// Destructor
JankyTargeting::~JankyTargeting(void)
{
	
}

bool JankyTargeting::isImageValid(ColorImage* pImage)
{
	if (pImage->GetHeight() && pImage->GetWidth())
		return true;
	else
		return false;
}

void JankyTargeting::ProcessOneImage(void)
{
	int count = 0;
	int fayz = 0; 
	
	HSLImage canon; 
	AxisCamera& camera = AxisCamera::GetInstance();

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
/*	bob				numValidBogies = 0;
					for () <vector>
					numValidBogies ++;

					bogies[i].BRcenterX = alsdkjfas dfjaslkdjf asljf sakjf 
					bogies[i].visualDistance = adslkfjasd fjslk fjasdkfj 
*/

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
	
}
