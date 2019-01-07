#include "frc/WPILib.h"
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
frc::Talon * left;
frc::Talon * right;
NatSolenoidTestJig * sold;
};

