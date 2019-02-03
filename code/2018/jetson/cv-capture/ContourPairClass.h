/*
   ContourPairClass.h
   @author - Maddie, Anika, Angela
   @version - February 2, 2019
*/

// Includes
#include <chrono>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/photo/photo.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <cstdlib>
#include <stdio.h>
#include <ctype.h>
#include <networktables/NetworkTable.h>
//#include <ntcore/src/networktables/NetworkTable.h>
//#include "/home/nvidia/FRCTeam1967/code/jetson/ntcore/include/ntcore.h"
//#include <cscore>

// Namespaces
using namespace std;
using namespace cv;

#ifndef CONTOURPAIRCLASS_H_
#define CONTOURPAIRCLASS_H_

class ContourPairClass {

public:
Point findCenterOfTape();
float getDist();
float getOffset();

private:
Point centerOfTapes;
float distance;
float distanceFromRobot;
float offset;
};
#endif
