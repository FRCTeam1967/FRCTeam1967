#include <iostream>
#include <vector>
#include "JankySelector.h"
#include "SelectorSample.h"

SelectorSample::SelectorSample(){
    Initialize(optionsList, selectorName);
}
SelectorSample::~SelectorSample(){

}
SelectorSample::Options SelectorSample::GetSelection(){
    Options selectedPath = (Options) GetSelectedValue();
    return selectedPath;
}