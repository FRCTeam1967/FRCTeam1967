#ifndef _JANKYSELECTOR_H
#define _JANKYSELECTOR_H

#include <iostream>
#include <vector>
#include "frc/smartdashboard/SendableChooser.h"

#define MAX_ARRAY_SIZE 30


class JankySelector
{
    public:
        JankySelector(); //constructor
        ~JankySelector(); // deconstructor

        // Methods
        void PrintValues(); // print values of optionNumber for debugging purposes
        void Initialize(std::vector<std::string> passedInOptions, std::string passedInSelectorName);

    protected:
        int GetSelectedValue(); //returns the value based on selection
		int selectorOptions[MAX_ARRAY_SIZE];  
		frc::SendableChooser<int*> selector;
		std::string selectorName;
        std::vector<std::string> optionList;

};
#endif