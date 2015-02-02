#include "WPIlib.h"
//#include "jankyTask.h"

class JankyForklift {
public:

JankyForklift(int talonOneChannel, int talonTwoChannel);

virtual ~JankyForklift(void);

// member functions
void pullIn();
void pushOut();


//member variables
Talon * left;
Talon * right;
};

