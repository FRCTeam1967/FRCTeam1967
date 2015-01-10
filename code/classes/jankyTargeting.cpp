///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// Targeting for Rebound Rumble 2012

#include "jankyTargeting.h" 

/// Constructor - do initialization here initialization-we are using these, not creating new variables
JankyTargeting::JankyTargeting(JankyTurret* pTurret) :
	PIDTurret(TURRET_P, TURRET_I, TURRET_D, this, pTurret),
	camera(CAMERA_IP)
{
	vPAR = NULL;
	samwise = NULL;
//	smarty = SmartDashboard::GetInstance();
	camera.WriteResolution(AxisCamera::kResolution_320x240); 
	camera.WriteBrightness(50); 	
//	camera.WriteWhiteBalance(kWhiteBalance_Automatic);
	success = 0;
	gotparticles = false;
	numValidBogies = 0;
	visualdistance = 1; //1.0
	BRcenterx = 0; //0.0
	BRcentery = 0; //0.0
	normalizedHOffset = 0; //0.0
	numImagesProcessed = 0;
	preferredLMH = -1;
	
	PIDTurret.SetInputRange(-100.0, 100.0);
	PIDTurret.SetOutputRange(-1.0, 1.0);
	PIDTurret.SetSetpoint(-6.0);
//PID Values: P--0.0014, I--0.0001, D--0.0000045 (possible values--still needs more tuning)
//PID valules (3.22): P-0.0008 I-0.0001, D-0	
//Madera PID Values P--0.005000, I--0.000020, D--0.001000
//Madera PID Values P--0.009000, I--0.000020, D--0.001000

	char initstr[20];
	sprintf(initstr, "%f", Preferences::GetInstance()->GetDouble("P-turret", TURRET_P));
	SmartDashboard::PutString("P-turret", initstr);
	sprintf(initstr, "%f", Preferences::GetInstance()->GetDouble("I-turret", TURRET_I));
	SmartDashboard::PutString("I-turret", initstr);
	sprintf(initstr, "%f", Preferences::GetInstance()->GetDouble("D-turret", TURRET_D));
	SmartDashboard::PutString("D-turret", initstr);

	SmartDashboard::PutString("Hot Goal","not hot");
	
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
	static bool isColorWritten = false;
	if (camera.IsFreshImage()==true)
	{ 
		success = camera.GetImage((ColorImage*)&hsl);
		if (success==1)
		{
			if (isImageValid()==true)
			{
				hsl.Write("ColorImage.jpg");
				isColorWritten = true;
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
	static bool isBinaryWritten = false;

	BinaryImage* firstBinaryImage = NULL;
	BinaryImage* readyForConvexHull = NULL;
	
	//Threshold values for Matt robot
		//firstBinaryImage = hsl.ThresholdHSL(120,186,60,255,0,255);
	firstBinaryImage = hsl.ThresholdHSL(100,150,60,255,0,255);

	SmartDashboard::PutBoolean("Binary Image Written",isBinaryWritten);
	
	if(!isBinaryWritten && firstBinaryImage != NULL)
	{
		firstBinaryImage->Write("BinaryImage.bmp"); //or gif
		isBinaryWritten = true;
	}
	
	if (firstBinaryImage !=NULL)
	{
		// Prune down # particles by removing all small stuff before convexHull.
		readyForConvexHull = firstBinaryImage->RemoveSmallObjects(false, 2);

		if (readyForConvexHull != NULL)
		{
			samwise = readyForConvexHull->ConvexHull(false);
			
			if (samwise != NULL)
			{
				isSuccessful = true;
			}
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
	SmartDashboard::PutNumber("particles",particles);
	printf("Particles = %d\n", particles);
	
	if (particles >0)
	{	
		vPAR = samwise->GetOrderedParticleAnalysisReports();
		return true;
	}
	else 
		return false;
}

void JankyTargeting::GetValues(void)
{
	if (vPAR)
	{
		rect_score = (int)(((par->particleArea) / (par->boundingRect.width*par->boundingRect.height))*100.0);
		BRcenterx = (int)(par->boundingRect.left + par->boundingRect.width*0.5);
		BRcentery = (int)(par->boundingRect.top + par->boundingRect.height*0.5);
		PI = 3.14159265;
		inpx = (INHEIGHT) / par->boundingRect.height; // inches per pixels (proportion) 
		imh = PIXHEIGHT * inpx; // image height in inches
		visualdistance = (int)(1.12*(0.5 * imh) / 0.302); // PI/180 is converting radians to degrees
		//visualdistance = (int)((0.5 * imh) / tan (17.5*PI/180.0)); // PI/180 is converting radians to degrees
	}		
}

void JankyTargeting::PrintBogey(void)
{
	//printf("New Bogey Report - # Valid bogies=%d\n", numValidBogies);
	
	for (int i=0;i<numValidBogies; i++)
	{
		//printf("Bogey #=%d,PS=%d,D=%04d ",i,bogies[i].BogeySCORE,bogies[i].BogeyVD);
		//printf("BRC=%d,%d,PCM=%d,%d\n",bogies[i].BogeyBRCX,bogies[i].BogeyBRCY, bogies[i].BogeyPMCX,bogies[i].BogeyPMCY);
	}
}

bool JankyTargeting::ProcessOneImage(void)
{
	bool success = true;
	
	 //RMW - it may be bad to call Enable() on every image inbound...
	 PIDTurret.Enable();
	 numValidBogies = 0;
	 if (GetImage()==true)
	{
		 numImagesProcessed++;
		 SmartDashboard::PutNumber("Images Processed", numImagesProcessed);
		 
		 if (DoImageProcessing()==true)
			if (GetParticles()==true)
			{
				for (unsigned int i=0 ; i<vPAR->size() && i<3 ; i++)
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
				ChooseLMH();
			}
		 
	}
	else
		success = false;

	 if (vPAR)
		 delete vPAR;
	 
	 if (samwise)
		 delete samwise;
	 
	vPAR = NULL;
	samwise = NULL;
	
	return success;
}

void JankyTargeting::GetRectScore(void)
{
	if (vPAR)
	{
		rect_score = (int)(((par->particleArea) / (par->boundingRect.width*par->boundingRect.height/2))*100.0);
		//Do I have to divide this area by 2 because it is a triangle once convex hull has occurred?
		BRcenterx = (int)(par->boundingRect.left + par->boundingRect.width*0.5);
	}
}

void JankyTargeting::PrintReport(void)
{
	printf("New Bogey Report - # Valid bogies=%d\n", numValidBogies);
		
	for (int i=0;i<numValidBogies; i++)
	{
		printf("Bogey #=%d,PS=%d\n",i,bogies[i].BogeySCORE);
		printf("BRCX=%d,PMCX=%d\n",bogies[i].BogeyBRCX,bogies[i].BogeyPMCX);
	}
}

bool JankyTargeting::ProcessNewImage(void)
{
	bool success = false;
	numValidBogies = 0;
	
	if(GetImage() == true)
	{
		numImagesProcessed++;
		SmartDashboard::PutNumber("Number of Images Processed", numImagesProcessed);
		if(DoImageProcessing() == true)
		{
			if(GetParticles() == true)
			{
				for(unsigned int i=0; i<vPAR->size() && i<3; i++)
				{
					par = &(*vPAR)[i];
					if(par->particleArea < MIN_PARAREA)
					{
						printf("Particle area less than minimum\n");
						break;
					}
					GetRectScore();
					if(rect_score > MIN_SCORE)
					{
						bogies[numValidBogies].BogeyPMCX = par->center_mass_x;
						bogies[numValidBogies].BogeyBRCX = BRcenterx;
						bogies[numValidBogies].BogeySCORE = rect_score;
						numValidBogies++;
					}
				}
				//CheckHotGoal();
				PrintReport();
			}
		}
	}
	else
		success = false;
	if(vPAR)
		delete vPAR;
	if(samwise)
		delete samwise;
	
	vPAR = NULL;
	samwise = NULL;
	
	return success;
}

int JankyTargeting::CheckHotGoal(void)
{
	for (int i=0 ; i<numValidBogies ; i++)
	{
		if(bogies[i].BogeyPMCX < CENTER_X)
		{
			bogies[i].BogeyLR = L_BOGEY;
			return L_BOGEY;
		}
		else if(bogies[i].BogeyPMCX > CENTER_X)
		{
			bogies[i].BogeyLR = R_BOGEY;
			return R_BOGEY;
		}
		else
			printf("Neither left nor right goal\n");
	}
	
	std::string goal = "";
	std::string out = "";
	
	for (int i=0 ; i<numValidBogies ; i++)
	{
		if(bogies[i].BogeyLR == L_BOGEY)
		{
			goal = "left";
			out = out + goal;
		}
		else if(bogies[i].BogeyLR == R_BOGEY)
		{
			goal = "right";
			out = out + goal;
		}
		else
			printf("No goal hot\n");
	}
	SmartDashboard::PutString("Hot Goal",out);
}

void JankyTargeting::SetLMHTarget(int ChosenTarget)
{
	preferredLMH = ChosenTarget;
}
 
void JankyTargeting::ChooseBogey(void)
{
	targetBogey=-1;

	int found[4];
	int numFound = 0;
	
	if (numValidBogies>=1)
	{
		targetBogey=0;
		
		// 
		// Specifically disallowing LOW targets.
		//
		if (preferredLMH==BOGEY_L)
		{
			preferredLMH=BOGEY_M;
		}
		
		if (preferredLMH==BOGEY_H)
		{
			for (int i=0; i<numValidBogies; i++)
			{
				if (bogies[i].BogeyLMH == BOGEY_H)
					targetBogey = i;
			}
		}
		else if (preferredLMH==BOGEY_M)
		{
			for (int i=0; i<numValidBogies; i++)
			{
				if(bogies[i].BogeyLMH == BOGEY_M)
				{
					found[numFound++] = i;
				}
			}
			if (numFound > 0)
			{
				if (numFound == 1)
				{
					targetBogey = found[0];
				}
				else 
				{
					if (bogies[found[0]].BogeyPMCX > bogies[found[1]].BogeyPMCX)
						targetBogey = found[0];
					else
						targetBogey = found[1];
				}
			}
		}
	}
	
	SmartDashboard::PutNumber("Visual Distance",bogies[targetBogey].BogeyVD);
}

void JankyTargeting::ChooseLMH(void)
{
	for (int i=0 ; i<numValidBogies ; i++)
	{
		if (bogies[i].BogeyPMCY > (int)CENTER_Y - DEADBAND_Y && bogies[i].BogeyPMCY < (int)CENTER_Y + DEADBAND_Y)
		{
			bogies[i].BogeyLMH = BOGEY_M;
		}
		else if (bogies[i].BogeyPMCY < (int)CENTER_Y - DEADBAND_Y)
		{
			bogies[i].BogeyLMH = BOGEY_H;
		}
		else
//		(bogies[i].BogeyPMCY > (int)CENTER_Y + DEADBAND_Y)
		{
			bogies[i].BogeyLMH = BOGEY_L;
		}
	}
	
	std::string out = "";
	std::string level = "";
	
	for (int i=0 ; i<numValidBogies ; i++)
	{
		if (bogies[i].BogeyLMH == BOGEY_H)
		{
			level = "H";
			out = out + level;
		}
		else if (bogies[i].BogeyLMH == BOGEY_M)
		{
			level = "M";
			out = out + level;
		}
		else
		{
			level = "L";
			out = out + level;
		}
	}
	smarty->PutString("Hoops in View",out);
}

void JankyTargeting::MoveTurret(void)
{
	if (targetBogey!=-1)
	{
		int widthOffset = (int)(bogies[targetBogey].BogeyBRCX)-(int)(PIXWIDTH/2);
		normalizedHOffset = (widthOffset * 100) / (int)(PIXWIDTH/2);
		SmartDashboard::PutNumber("Horizontal Offset",normalizedHOffset);
		printf("widthOffset(+/-160)=%d, Horiz Offset(+/-100) = %d\n", widthOffset, normalizedHOffset);
//		printf("Target Bogey=%d,WidthOffset=%d\n",targetBogey,widthOffset);
	}
	
	else
	{
		normalizedHOffset = 0;
		SmartDashboard::PutNumber("Horizontal Offset",normalizedHOffset);
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

	//
	// Pull values from the smart dashboard and then put them into preferences.
	//
	
	tempstring=smarty->GetString("P-turret");
	sscanf(tempstring.c_str(), "%f", &p);
	Preferences::GetInstance()->PutDouble("P-turret", p);
	
	tempstring=smarty->GetString("I-turret");
	sscanf(tempstring.c_str(), "%f", &i);
	Preferences::GetInstance()->PutDouble("I-turret", i);

	tempstring=smarty->GetString("D-turret");
	sscanf(tempstring.c_str(), "%f", &d);
	Preferences::GetInstance()->PutDouble("D-turret", d);
	
	PIDTurret.SetPID(p,i,d);

/*	float P= PIDTurret.GetP();
	float I= PIDTurret.GetI();
	float D= PIDTurret.GetD();
	
	float E= PIDTurret.GetError();

	printf("P=%f, I=%f, D=%f, E=%f\n", P, I, D, E);
*/
}

void JankyTargeting::StopPID(void)
{
	//RMW May want to set a flag which makes our GetPID() return a zero error instead.
	PIDTurret.Disable();
}

int JankyTargeting::VisToActDist(void) 
{
	// RMW move these to the class level -- proving difficult? Make em static.
	// Or just make these static so they are not re-allocated on the stack each time.
	
	//static int visArr[]={72,74,78,84,92,102};
	//static int actvisArr[]={108,120,132,144,156,178};
	//int numEntries = 6;//sizeof(visArr / sizeof(int));
	//TODO change entry number
	
//	smarty->PutInt("Number of Elements-visArray", numEntries);
	
	int visdist = bogies[targetBogey].BogeyVD;	
	
	/*if (visdist<visArr[0])
		return (actvisArr[0]);
	
	if (visdist>visArr[numEntries-1])
		return (actvisArr[numEntries-1]);	
	
	for (int i=0; i<numEntries; i++)
	{		
		if (visdist>=visArr[i] && visdist<visArr[i+1])
		 {	
			float R=((visdist-visArr[i])/(visArr[i+1]-visArr[i]));
		
			return (actvisArr[i]+ (int)(R*(actvisArr[i+1]-actvisArr[i])));
		 }
	}	
	*/
	// Return something 'sane' if we were to get here. Shouldnt ever happen though.
	//return actvisArr[0]; 
	return visdist;
}
//TODO control-end of non-void

int JankyTargeting::ActDisttoRPM(int actdist)
{
	// Table for use with conveyer - not kicker
	static int MedDisArr[]={144,156,168,180,192};
	static int MedRPMArr[]={1865,1920,2000,2080,2120};
//	static int MedRPMArr[]={1915,1970,2050,2130,2170};
//	static int MedRPMArr[]={1615,1670,1750,1830,1870};
	
	// Table for use with conveyer - not kicker
	static int HighDisArr[]={108,120,132,144,156,168,180,192,204,216,228};
	static int HighRPMArr[]={1950,2000,2050,2100,2150,2250,2330,2240,2350,2520,2580};
//	static int HighRPMArr[]={2000,2050,2100,2150,2200,2300,2380,2290,2400,2570,2630};
//	static int HighRPMArr[]={1700,1750,1800,1850,1900,2000,2080,1990,2100,2270,2330};
	
	int numEntries;
	int *outputRPM;
	int *actArr;
		
	if (preferredLMH==BOGEY_H)
	{	
		actArr = HighDisArr;
		outputRPM = HighRPMArr;
		numEntries=11;
	}	
	else	// We're only allowing High and Medium - so the rest is medium...
	{	
		actArr = MedDisArr;
		outputRPM = MedRPMArr;
        numEntries=5;
	}
	
	if (actdist<actArr[0])
		return (int)((double)outputRPM[0]*0.93);
			
	if(actdist>actArr[numEntries-1])
		return (int)((double)outputRPM[numEntries-1]*0.93);
			
	for (int i=0; i<numEntries; i++)
	{					
			if (actdist>=actArr[i] && actdist<actArr[i+1])
				{			
					float R=((actdist-actArr[i])/(actArr[i+1]-actArr[i]));
					
					return (int)((outputRPM[i] + (int)(R*(double)(outputRPM[i+1]-outputRPM[i])))*0.93);
				}	
	}

	// Return something 'sane' if we get here which should never happen.
	return (int)((double)outputRPM[0]*0.93);
}

int JankyTargeting::GetCalculatedRPM(void)
{
	int actDistance = VisToActDist();
	int calcRPM = ActDisttoRPM(actDistance);
	
	SmartDashboard::PutNumber("Actual distance calculated", actDistance);
	SmartDashboard::PutNumber("Actual RPM calculated", calcRPM);
	
	return calcRPM;
}

