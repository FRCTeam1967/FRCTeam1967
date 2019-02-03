/*
   ContourClass.cpp
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
#include "ContourClass.h"
//#include <ntcore/src/networktables/NetworkTable.h>
//#include "/home/nvidia/FRCTeam1967/code/jetson/ntcore/include/ntcore.h"
//#include <cscore>

// Namespaces
using namespace std;
using namespace cv;

ContourClass::ContourClass()
{
	
}

ContourClass::~ContourClass()
{
	
}

float ContourClass::getSlope(float x1, float y1, float x2, float y2)
{
    // Using two (x, y) coordinates, returns the slope of the line
    return ((y2 - y1) / (x2 - x1));
}

enum ContourClass::getLeftOrRight()
