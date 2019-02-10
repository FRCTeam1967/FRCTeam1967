/*
   ContourPair.h
   @author - Maddie, Anika, Angela, Layla, Ushoshi
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
#include "Contour.h"
//#include <ntcore/src/networktables/NetworkTable.h>
//#include "/home/nvidia/FRCTeam1967/code/jetson/ntcore/include/ntcore.h"
//#include <cscore>

// Namespaces
using namespace std;
using namespace cv;

#ifndef CONTOURPAIR_H_
#define CONTOURPAIR_H_

class ContourPair
{
 public:
   ContourPair(Contour c1, Contour c2);
   float getDist(int widthThreshold, int rectHeight, float frameHeight, float frameWidth, int rectWidth, float leftCornerDist, float rightCornerDist);
   float getDistFromBot();
   float getOffset(float lRectTlX, float rRectTlX, float rRectWidth, float T_INCHES_BOTH_WIDTH, int FOV_PIXELS_WIDTH);
   float returnOffset();
   float returnDist();
 private:
   vector<Contour> contourPair;
   // variables for distance calculation
   float distance;
   float fovHeight;
   float fovWidth;
   float fovDiagonal;
   float verticalDistanceToTape;
   float distanceToTape;
   float horizDistanceToTapeLeft;
   float horizDistanceToTapeRight;
   float avgDistToTape;
   float distanceFromRobot;
   float lengthWidth;
   // variables for offset calculation
   float offset;
   float pixelsToInches;
   float tapeCenter;
   float localOffset;
};
#endif
