///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// Targeting for Rebound Rumble 2012

#include "jankyTargeting.h" 

JankyShooter::JankyShooter(int JagPort, int EncoderAPort, int EncoderBPort):
	ShooterMotor(JagPort),
	ShooterEncoder(EncoderAPort,EncoderBPort)
{
	ShooterEncoder.Start();
	ShooterMotor.Set(0.0);
	TargetRPMx4 = 0;
	CurrentRPMx4 = 0;
	MotorSpeed = 0;
}

JankyShooter::~JankyShooter(void)
{
	ShooterEncoder.Stop();
}

void JankyShooter::setTargetRPM(int desiredrpm)
{
	TargetRPMx4 = desiredrpm * 4;
	DoCalculations();
}

void JankyShooter::DoCalculations(void)
{
	int suspectRPM = (int)ShooterEncoder.GetRate();
	
	//filtering out noise from the encoder 
	if (suspectRPM < -40000 || suspectRPM > 40000)
		return;
	
	CurrentRPMx4 = ((CurrentRPMx4 * 7) + suspectRPM) / 8;
	
	if (TargetRPMx4 > CurrentRPMx4 + RPM_DEADBANDx4)
	{
		MotorSpeed = MotorSpeed + 0.02;
	}
	else if (TargetRPMx4 < CurrentRPMx4 - RPM_DEADBANDx4)
	{
		MotorSpeed = MotorSpeed - 0.02;
	}
	
	if (MotorSpeed > 1.0)
		MotorSpeed = 1.0;
	
	if (MotorSpeed < 0.0)
		MotorSpeed = 0.0;
	
	SmartDashboard::GetInstance()->PutInt("CurrentRPM",CurrentRPMx4);
	SmartDashboard::GetInstance()->PutDouble("Motor Speed",MotorSpeed);
	
	ShooterMotor.Set(MotorSpeed);
}

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
		printf("Bogey #=%d,PS=%d,D=%04d ",i,bogies[i].BogeySCORE,bogies[i].BogeyVD);
		printf("BRC=%d,%d,PCM=%d,%d\n",bogies[i].BogeyBRCX,bogies[i].BogeyBRCY, bogies[i].BogeyPMCX,bogies[i].BogeyPMCY);
	}
}

 bool JankyTargeting::ProcessOneImage(void)
{
	 numValidBogies = 0;
	 if (GetImage()==true)
	{
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
	
	if (numValidBogies==1)
		targetBogey=0; //index starts at 0
}

void JankyTargeting::MoveTurret(void)
{
	if (targetBogey!=-1)
	{
		int widthOffset = (int)(bogies[targetBogey].BogeyLeft + bogies[targetBogey].BogeyBRCX)-(PIXWIDTH/2);
		int normalizedHOffset = (widthOffset * 100) / (PIXWIDTH/2);
		smarty->PutInt("Horizontal Offset",normalizedHOffset);
//		printf("Target Bogey=%d,WidthOffset=%d\n",targetBogey,widthOffset);
	}
//TODO give values to jaguars and move turret to adjust for error	
}







