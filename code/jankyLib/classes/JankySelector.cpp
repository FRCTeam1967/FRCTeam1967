#include <iostream>
#include <vector>
#include "frc/smartdashboard/SmartDashboard.h"
#include "frc/smartdashboard/SendableChooser.h"
#include "JankySelector.h"

using namespace std;
using namespace frc;

JankySelector::JankySelector(){ 
	
}

JankySelector::~JankySelector(){
	
}

void JankySelector::PrintValues(){
	for (unsigned int i = 0; i < optionList.size(); i++){
		cout << optionList.at(i) << ": " << selectorOptions[i] << endl;
	}
}

int JankySelector::GetSelectedValue(){
	cout << "Selected option for " << selectorName << ": " << *selector.GetSelected() << endl;
	return *selector.GetSelected();
}

void JankySelector::Initialize(std::vector<std::string> passedInOptions, std::string passedInSelectorName){
	selectorName = passedInSelectorName;
	optionList = passedInOptions;

	selectorOptions[0] = 0;
	selector.SetDefaultOption(passedInOptions.at(0), &selectorOptions[0]);

	for (unsigned int i = 1; i < passedInOptions.size(); i++){
		selectorOptions[i]=i;
		selector.AddOption(passedInOptions[i], &selectorOptions[i]);
	}
	SmartDashboard::PutData(selectorName, &selector);
}