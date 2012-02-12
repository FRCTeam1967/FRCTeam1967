///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// Targeting for Rebound Rumble 2012

#include "jankyTargeting.h" 

/// Constructor - do initialization here initialization-we are using these, not creating new variables
JankyTargeting::JankyTargeting(void)
{
	AxisCamera& camera = AxisCamera::GetInstance(CAMERA_IP); 
	smarty = SmartDashboard::GetInstance();
	camera.WriteResolution(AxisCamera::kResolution_320x240); 
	camera.WriteBrightness(50); 	
//	camera.WriteWhiteBalance(kWhiteBalance_Automatic);
	success = 0;
	gotparticles = false;
	numValidBogies = 0;
	visualdistance = 1.0;
	BRcenterx = 0.0;
	BRcentery = 0.0; 
	
	
	Wait(1.0);
}

// Destructor
JankyTargeting::~JankyTargeting(void)
{
	
}

bool JankyTargeting::GetImage(void)
{
	AxisCamera& camera = AxisCamera::GetInstance(CAMERA_IP); 
	
		if (camera.IsFreshImage()==true)
		{ 
			success = camera.GetImage((ColorImage*)&hsl);
			
			if (success==1)
			{
				if (isImageValid()==true)
				{
					return true;
				}
			}	
		}
		
		return false;		
}

bool JankyTargeting::isImageValid()
{
	if (hsl.GetHeight()!=0 && hsl.GetWidth()!=0)  
		return true;
	else
		return false;
}

bool JankyTargeting::DoImageProcessing(void)
{
	samwise = hsl.ThresholdHSL(120,170,60,255,100,253);
	
	if (samwise !=NULL)
	{
		Image* imaqImage = samwise->GetImaqImage();
		if (imaqImage!=NULL)
		{
			imaqConvexHull(imaqImage,imaqImage,TRUE);
			return true;
		}			
	}
	
	return false;

}	

bool JankyTargeting::GetParticles (void)
{
	particles = samwise->GetNumberParticles();
	
	smarty->PutInt("particles",particles);
	if (particles >0)
	{	
		vPAR = samwise->GetOrderedParticleAnalysisReports();
		return true;
	}
	else 
		return false;
}

int JankyTargeting::GetValues(void)
{
	if (vPAR)
	{
		rect_score = (int)(((par->particleArea) / (par->boundingRect.width*par->boundingRect.height))*100.0);
		BRcenterx = (int)(par->boundingRect.left + par->boundingRect.width*0.5);
		BRcentery = (int)(par->boundingRect.top + par->boundingRect.height*0.5);
		PI = 3.14159265;
		
		inpx = (INHEIGHT) / par->boundingRect.height; // inches per pixels (proportion) 
		imh = PIXHEIGHT * inpx; // image height in inches
		visualdistance = (int)(0.5 * imh) / tan (27.0*PI/180.0); // PI/180 is converting radians to degrees
	}		
}

void JankyTargeting::PrintBogey(void)
{
	printf("New Bogey Report - # Valid bogies=%d\n", numValidBogies);
	
	for (int i=0;i<numValidBogies; i++)
	{
		
		printf("Bogey #=%d, PS=%d,D=%04d ",i,bogies[i].BogeySCORE,bogies[i].BogeyVD);
		printf("BRC=%d,%d,PCM=%d,%d\n ",bogies[i].BogeyBRCX,bogies[i].BogeyBRCY, bogies[i].BogeyPMCX,bogies[i].BogeyPMCY);
	
	}
}






//TODO Remember to delete vpar, samwise

 bool JankyTargeting::ProcessOneImage(void)
{
	// HOW DO YOU PROGRAM A FUNCTION THAT USES OTHER FUNCTIONS (THEY ARE ALL DEFINITIONS!)???
	 numValidBogies = 0;
	 if (GetImage()==true)
	{
		if (DoImageProcessing()==true)
			if (GetParticles()==true)
			{
				
				for (int i=0 ; i<vPAR->size() && i<3 ; i++)
				{
					par = &(*vPAR)[i];
					GetValues();
					if (rect_score>MIN_SCORE)
					{	
						bogies[numValidBogies].BogeyBRCX=BRcenterx;
						bogies[numValidBogies].BogeyBRCY=BRcentery;
						bogies[numValidBogies].BogeyVD=visualdistance;
						bogies[numValidBogies].BogeyPMCX=par->center_mass_x;
						bogies[numValidBogies].BogeyPMCY=par->center_mass_y;
						bogies[numValidBogies].BogeySCORE=rect_score;
						//bogies[numValidBogies].BogeyRATIO=aspect_ratio;
						
						numValidBogies ++;
					}	
					
					if(par->particleArea<MIN_PARAREA)
						break;
				}
				
				PrintBogey();
			}													
	}
	else
		return false;
	
	delete vPAR;
	delete samwise;
	
}
	


/*	int count = 0;
	int fayz = 0; 
	AxisCamera& camera = AxisCamera::GetInstance(CAMERA_IP); 
		if (camera.IsFreshImage()==true)
		{ 
			count++;		
			smarty->PutInt("Number of fresh images",count);
			int success = camera.GetImage((ColorImage*)&canon);
			if ( canon.GetHeight()!=0 && canon.GetWidth()!=0 ) {
				fayz++;
	#ifdef WRITE_IMAGES									
				canon.Write("hsl.jpg");
	#endif			
				smarty->PutInt("Number of valid images",fayz);
	//			BinaryImage* samwise = canon.ThresholdHSL(154,231,33,255,131,255);
	//			BinaryImage* samwise = canon.ThresholdHSL(185,255,95,255,65,255);
	//			BinaryImage* samwise = canon.ThresholdHSL(120,170,60,255,100,253);
	//last threshold is correct one for blue color	
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
					float rect_score = par.particleArea / (par.boundingRect.width*par.boundingRect.height);
						int BRcenterx = (int)(par.boundingRect.left + par.boundingRect.width*0.5);
						int BRcentery = (int)(par.boundingRect.top + par.boundingRect.height*0.5);
		bob				bogies[i].BRcenterX = alsdkjfas dfjaslkdjf asljf sakjf 
						bogies[i].visualDistance = adslkfjasd fjslk fjasdkfj 
	
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
				{
					samwise->GetParticleAnalysisReport(x, &report);
				}				
				delete samwise;
			}
		}
						
}

*/






