/*
    AutoSelector.h

    Modified on: February 8, 2020
    Authors: Maddie, Kara, Angela, Sofia
*/

#include <iostream>
#include "frc/WPILib.h"

#ifndef AUTO_SELECTOR_H
#define AUTO_SELECTOR_H

class AutoSelector
{
    public:
        AutoSelector(); // constructor
        ~AutoSelector(); // deconstructor

        // Methods
        void DisplayAutoOptions(); // displays different options for auto modes on Smart Dashboard
        int GetAutoMode(); // gets user's input for what auto mode will be
        int GetDelayTime(); // get amount of time  the robot will wait until it starts executing autonomus mode
        void PrintValues(); // print values of auto selector for debugging purposes

    private:

};
#endif 