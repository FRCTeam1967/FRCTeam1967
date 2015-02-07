#include "WPIlib.h"
#include "NatSolenoidTestJig.h"
//#include "jankyTask.h"

class JankyForklift {
public:

JankyForklift(int talonOneChannel, int talonTwoChannel, int pistonOneChannel);

virtual ~JankyForklift(void);

// member functions
void pullIn();
void pushOut();


//member variables
Talon * left;
Talon * right;
NatSolenoidTestJig * sold;
};

