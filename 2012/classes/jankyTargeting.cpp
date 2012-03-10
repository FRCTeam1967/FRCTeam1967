///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// Targeting for Rebound Rumble 2012

#include "jankyTargeting.h" 

/// Constructor - do initialization here initialization-we are using these, not creating new variables
JankyTargeting::JankyTargeting(JankyTurret* pTurret) :
	PIDTurret(TURRET_P, TURRET_I, TURRET_D, this, pTurret),
	camera(AxisCamera::GetInstance(CAMERA_IP))
{
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
	normalizedHOffset = 0.0;
	numImagesProcessed = 0;
	
	PIDTurret.SetInputRange(-100.0, 100.0);
	PIDTurret.SetOutputRange(-0.1,0.1);
	PIDTurret.SetSetpoint(0.0);
//PID Values: P--0.0014, I--0.0001, D--0.0000045 (possible values--still needs more tuning)
	smarty->PutString("P-turret", "0.008");
	smarty->PutString("I-turret", "0.002");
	smarty->PutString("D-turret", "0.000012");

	Wait(1.0);
}

// Destructor
JankyTargeting::~JankyTargeting(void)
{
	
}

// implement PIDGet for pIDSource functionality
double JankyTargeting::PIDGet(void)
{
	return (double)(normalizedHOffset * -1.0);
}

bool JankyTargeting::GetImage(void)
{
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
	bool isSuccessful = false;
	BinaryImage* firstBinaryImage = NULL;
	BinaryImage* readyForConvexHull = NULL;
	
	firstBinaryImage = hsl.ThresholdHSL(120,186,60,255,0,255);
	
	if (firstBinaryImage !=NULL)
	{
		// Prune down # particles by removing all small stuff before convexHull.
		readyForConvexHull = firstBinaryImage->RemoveSmallObjects(false, 2);

		if (readyForConvexHull != NULL)
		{
			samwise = readyForConvexHull->ConvexHull(false);
			
			if (samwise != NULL)
				isSuccessful = true;
		}
	}
	
	if (readyForConvexHull)
		delete readyForConvexHull;

	if (firstBinaryImage)
		delete firstBinaryImage;
	
	return isSuccessful;
}	

bool JankyTargeting::GetParticles (void)
{
	particles = samwise->GetNumberParticles();
	smarty->PutInt("particles",particles);
	printf("Particles = %d\n", particles);
	
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
		printf("Bogey #=%d,PS=%d,D=%04d ",i,bogies[i].BogeySCORE,bogies[i].BogeyVD);
		printf("BRC=%d,%d,PCM=%d,%d\n",bogies[i].BogeyBRCX,bogies[i].BogeyBRCY, bogies[i].BogeyPMCX,bogies[i].BogeyPMCY);
	}
}

 bool JankyTargeting::ProcessOneImage(void)
{
	 PIDTurret.Enable();
	 numValidBogies = 0;
	 if (GetImage()==true)
	{
		 numImagesProcessed++;
		 smarty->PutInt("Images Processed", numImagesProcessed);
		 
		 if (DoImageProcessing()==true)
			if (GetParticles()==true)
			{
				for (int i=0 ; i<vPAR->size() && i<3 ; i++)
				{
					par = &(*vPAR)[i];
					if (par->particleArea < MIN_PARAREA)
						break;
					GetValues();
					if (rect_score > MIN_SCORE)
					{	
						bogies[numValidBogies].BogeyBRCX=BRcenterx;
						bogies[numValidBogies].BogeyBRCY=BRcentery;
						bogies[numValidBogies].BogeyTop=par->boundingRect.top;
						bogies[numValidBogies].BogeyLeft=par->boundingRect.left;
						bogies[numValidBogies].BogeyVD=visualdistance;
						bogies[numValidBogies].BogeyPMCX=par->center_mass_x;
						bogies[numValidBogies].BogeyPMCY=par->center_mass_y;
						bogies[numValidBogies].BogeySCORE=rect_score;
						//bogies[numValidBogies].BogeyRATIO=aspect_ratio;						

						numValidBogies ++;
					}
				}
				PrintBogey();
			}													
	}
	else
		return false;
	
	delete vPAR;
	delete samwise;
}

int JankyTargeting::ChooseBogey(void)
{
	targetBogey=-1;
	
	if (numValidBogies>=1)
		targetBogey=0; //index starts at 0
}

void JankyTargeting::MoveTurret(void)
{
	if (targetBogey!=-1)
	{
		int widthOffset = (int)(bogies[targetBogey].BogeyBRCX)-(PIXWIDTH/2);
		normalizedHOffset = (widthOffset * 100) / (PIXWIDTH/2);
		smarty->PutInt("Horizontal Offset",normalizedHOffset);
		printf("widthOffset(+/-160)=%d, Horiz Offset(+/-100) = %d\n", widthOffset, normalizedHOffset);
//		printf("Target Bogey=%d,WidthOffset=%d\n",targetBogey,widthOffset);
	}
	
	else
	{
		normalizedHOffset = 0;
	}
//TODO give values to jaguars and move turret to adjust for error	
}

/*
 int JankyTargeting::CalculateShootingSpeed(void)
{
	float gravity = 32.174;
//TODO check to see if code needs to be in radians or degrees! 
	float launchAngle = 1.13446;
	float hoopHeight = 98.0;
	float launchHeight = 36.0;
	int ShooterRPM = (int)(visualdistance * (sqrt(gravity/(visualdistance*tan(launchAngle)-hoopHeight + launchHeight))/(sqrt(2)*cos(launchAngle))));
}
*/

void JankyTargeting::InteractivePIDSetup(void)
{
	float p, i, d;
	std::string tempstring;

	tempstring=smarty->GetString("P-turret");
	sscanf(tempstring.c_str(), "%f", &p);
	
	tempstring=smarty->GetString("I-turret");
	sscanf(tempstring.c_str(), "%f", &i);

	tempstring=smarty->GetString("D-turret");
	sscanf(tempstring.c_str(), "%f", &d);
	
	PIDTurret.SetPID(p,i,d);
}

void JankyTargeting::StopPID(void)
{
	PIDTurret.Disable();
}

