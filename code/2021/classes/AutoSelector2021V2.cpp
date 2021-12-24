#include <iostream>
#include <vector>
#include "JankySelector.h"
#include "AutoSelector2021V2.h"

PathSelector::PathSelector(){
    Initialize(optionsList, selectorName);
}
PathSelector::~PathSelector(){

}
PathSelector::Options PathSelector::GetSelection(){
    Options selectedPath = (Options) GetSelectedValue();
    return selectedPath;
}

RobotSelector::RobotSelector(){
    Initialize(optionsList, selectorName);
}
RobotSelector::~RobotSelector(){

}
RobotSelector::Options RobotSelector::GetSelection(){
    Options selectedRobot = (Options) GetSelectedValue();
    return selectedRobot;
}
