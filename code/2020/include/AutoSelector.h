#include <iostream>
#include "frc/WPILib.h"

#ifndef AUTO_SELECTOR_H
#define AUTO_SELECTOR_H

class AutoSelector
{
    public:
        AutoSelector();
        ~AutoSelector();

        // Methods
        void DisplayAutoOptions(); // displays different options for auto modes on Smart Dashboard
        int GetAutoMode(); // gets user's input for what auto mode will be
        int GetDelayTime(); // get amount of time  the robot will wait until it starts executing autonomus mode
        void PrintValues(); // print values of auto selector for debugging purposes

    private:

};
#endif 