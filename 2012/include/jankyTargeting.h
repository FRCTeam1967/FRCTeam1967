///
/// Team 1967 FIRST FRC - The Janksters - Notre Dame High School San Jose CA
///

/// Targeting for Rebound Rumble 2012

#ifndef _JANKYTARGETING_H
#define _JANKYTARGETING_H

#include "jankyRobot.h"
#include "nivision.h"
#include "math.h"

#define USE_CENTER_OF_MASS
#define WRITE_IMAGES
#define INHEIGHT 18.0
#define PIXHEIGHT 240.0
#define MIN_SCORE 85.0
#define MIN_PARAREA 500.0


typedef struct BogeyInfo_t {
	
	int BogeyBRCX;
	int BogeyBRCY;
	int BogeyVD;
	int BogeyPMCX;
	int BogeyPMCY;
	int BogeySCORE;
	//int BogeyRATIO;
		
} BogeyInfo;


class JankyTargeting {
public:
	JankyTargeting(void);  // Initialization goes here - Constructor
	virtual ~JankyTargeting(void); // Destructor - clean up all things before dying.
	bool GetImage(void);
	bool isImageValid(void);
	bool GetParticles (void);
//	int GetNumValidBogies (void);
	bool ProcessOneImage(void);
	bool DoImageProcessing(void);
	int GetValues(void);
	void PrintBogey(void); 
	
	
	// Member variables
	SmartDashboard *smarty;
	BinaryImage *samwise;
	HSLImage hsl;
	int success;
	int tempparticles;
	bool gotparticles;
	int particles;
	vector<ParticleAnalysisReport>* vPAR;
	ParticleAnalysisReport* par;
	int numValidBogies;
	BogeyInfo bogies[4];
	float PI; 
	float inpx; 
	float imh;
	int rect_score;
	int BRcenterx;
	int BRcentery;
	int visualdistance;
	int aspectratio;
	
};

#endif
