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
#define PIXWIDTH 320.0
#define MIN_SCORE 85.0
#define MIN_PARAREA 500.0



typedef struct BogeyInfo_t {
	
	int BogeyBRCX;
	int BogeyBRCY;
	int BogeyVD;
	int BogeyPMCX;
	int BogeyPMCY;
	int BogeySCORE;
	int BogeyTop;
	int BogeyLeft;
	//int BogeyRATIO;
		
} BogeyInfo;


#define RPM_DEADBANDx4 400

class JankyShooter {
public:
	JankyShooter(int JagPort, int EncoderAPort, int EncoderBPort);
	virtual ~JankyShooter(void);
	void setTargetRPM(int desiredrpm);
	void DoCalculations(void);
	int GetCurrentRPM(void) {return CurrentRPMx4 / 4;};
	
	//Member variables
	int CurrentRPMx4;
	int TargetRPMx4;
	Jaguar ShooterMotor;
	Encoder ShooterEncoder;
	float MotorSpeed;
};


class JankyTargeting {
public:
	JankyTargeting(void);  // Initialization goes here - Constructor
	virtual ~JankyTargeting(void); // Destructor - clean up all things before dying.
	bool GetImage(void);
	bool isImageValid(void);
	bool GetParticles (void);
	bool ProcessOneImage(void);
	bool DoImageProcessing(void);
	int GetValues(void);
	void PrintBogey(void); 
	int ChooseBogey(void);
	void MoveTurret(void);
	
	
	// Member variables
	SmartDashboard *smarty;
	BinaryImage *samwise;
	HSLImage hsl;
	int success;
	int tempparticles;
	bool gotparticles;
	int particles;
	float PI; 
	float inpx; 
	float imh;
	int rect_score;
	int BRcenterx;
	int BRcentery;
	int visualdistance;
	vector<ParticleAnalysisReport>* vPAR;
	ParticleAnalysisReport* par;
	int numValidBogies;
	int aspectratio;
	BogeyInfo bogies[4];
	int targetBogey;
	
};

#endif
