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

typedef struct BogeyInfo_t {
	int BRcenterX;
	int BRcenterY;
	int visualDistance;
} BogeyInfo;

class JankyTargeting {
public:
	JankyTargeting(void);  // Initialization goes here - Constructor
	virtual ~JankyTargeting(void); // Destructor - clean up all things before dying.

	bool isImageValid(ColorImage* pImage);
	void ProcessOneImage(void);
	
	// Member variables
	SmartDashboard *smarty;
	
	int numValidBogies;
	BogeyInfo bogies[4];
};

#endif
