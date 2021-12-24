#ifndef _AUTOSELECTOR2021V2_H
#define _AUTOSELECTOR2021V2_H

#include <iostream>
#include <vector>
#include "JankySelector.h"

class PathSelector : public JankySelector 
{
    public:
        std::string selectorName = "Auto Paths";
        std::vector<std::string> optionsList 
            {"Do Nothing", "Simple Path", "Bounce Path", "Barrel Path", "Slalom Path", "Test Path 1", "Test Path 2", "Test Path 3"};
        enum class Options {doNothing, simplePath, bouncePath, barrelPath, slalomPath, testPath1, testPath2, testPath3};

        PathSelector();
        ~PathSelector();
        Options GetSelection();
};

class RobotSelector : public JankySelector 
{
    public:
        std::string selectorName = "Selected Robot";
        std::vector<std::string> optionsList 
            {"Jankybot", "Luca"};
        enum class Options {jankybot, luca};

        RobotSelector();
        ~RobotSelector();
        Options GetSelection();
};
#endif

