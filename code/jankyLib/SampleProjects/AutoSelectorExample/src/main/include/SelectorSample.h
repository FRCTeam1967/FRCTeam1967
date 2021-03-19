#ifndef _SELECTORSAMPLE_H
#define _SELECTORSAMPLE_H

#include <iostream>
#include <vector>
#include "JankySelector.h"

//rename "SelectorSample" to a class name that fits your selector
class SelectorSample : public JankySelector 
{
    public:
        //customize variables based on your selector's name and options
        std::string selectorName = "Auto Paths";
        std::vector<std::string> optionsList 
            {"Path 1", "Path 2", "Path 3"};
        enum class Options {Path1, Path2, Path3};

        SelectorSample();
        ~SelectorSample();
        Options GetSelection();
};

#endif