#ifndef _FLYWHEELSELECTOR_H
#define _FLYWHEELSELECTOR_H
#include <iostream>
#include <vector>
#include "JankySelector.h"
class FlywheelSelector
{
    public:
        FlywheelSelector(); // constructor
        ~FlywheelSelector(); // deconstructor
        // Methods
        void DisplayShootingOptions(); 
        int GetShootingZone(); 
        void PrintValues(); // print values of auto selector for debugging purposes
    private:
        int shootingZone;
};
#endif 
