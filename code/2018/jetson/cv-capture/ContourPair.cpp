/*
   ContourPair.cpp
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
#include "ContourPair.h"
#include "Contour.h"

// Namespaces
using namespace std;
using namespace cv;

// Constants
const float OFFSET_OF_CAM = -3.5; // In inches, how far the camera is offset left/right
const float ROBOT_OFFSET = 13.5;   // In inches, how far is the camera into the robot (from the edge of bumpers)
const float T_INCHES_HEIGHT = 5.5; // Height of tape in inches
const float T_INCHES_WIDTH = 2;    // Width of tape in inches
const float T_INCHES_LEFT_WIDTH = 11.75;
const float T_INCHES_BOTH_WIDTH = 14.5;
const int FOV_PIXELS_HEIGHT = 480;
const int FOV_PIXELS_WIDTH = 640;
const float theta = 68.5 * M_PI / 360; // Degrees
const float MEASURED_HORIZ_FOV = 51.80498 * M_PI / 360;
const float MEASURED_VERT_FOV = 38.3557 * M_PI / 360;
const int DEFAULT_WIDTH_THRESHOLD = 100; // Number of pixels from left edge to right edge of both tapes before tape gets cut off (lengthwidth)

ContourPair::ContourPair(Contour c1, Contour c2)
{
   contourPair = {c1, c2};
   offset = -1000;
   distance = -1;
}

float ContourPair::getOffset(float lRectTlX, float rRectTlX, float rRectWidth)
{
   lengthWidth = rRectTlX + rRectWidth - lRectTlX; // length from left edge of left tape to right edge of right tape
   pixelsToInches = T_INCHES_BOTH_WIDTH / lengthWidth;
   tapeCenter = lRectTlX + lengthWidth / 2;
   localOffset = (FOV_PIXELS_WIDTH / 2) - tapeCenter;
   offset = (localOffset * pixelsToInches) + OFFSET_OF_CAM;
   return offset;
}

float ContourPair::returnOffset()
{
	return offset;
}

float ContourPair::returnDist()
{
	return distance;
}

float ContourPair::getDist(int widthThreshold, int rectHeight, float frameHeight, float frameWidth, int rectWidth, float leftCornerDist, float rightCornerDist)
{
   // Checks if tape's height is cut off
   if (lengthWidth < widthThreshold)
   {
      // Uses tape height to find distance
      widthThreshold = DEFAULT_WIDTH_THRESHOLD + 5;
      fovHeight = FOV_PIXELS_HEIGHT * T_INCHES_HEIGHT / rectHeight;
      fovWidth = fovHeight * frameWidth / frameHeight;
      fovDiagonal = sqrt(pow(fovHeight, 2) + pow(fovWidth, 2));
      float distanceToTape = fovDiagonal / (2 * tan(theta / 1.15));
      //verticalDistanceToTape = fovHeight / (2 * tan(MEASURED_VERT_FOV));
      //distance = verticalDistanceToTape;
      distance = distanceToTape;
   }
   else
   {
      // Uses tape width to find distance
      widthThreshold = DEFAULT_WIDTH_THRESHOLD;
      fovWidth = FOV_PIXELS_WIDTH * T_INCHES_WIDTH / rectWidth;
      fovHeight = fovWidth * frameHeight / frameWidth;
      fovDiagonal = sqrt(pow(fovHeight, 2) + pow(fovWidth, 2));
      distanceToTape = fovDiagonal / (2 * tan(theta / 1.4));
      // Getting pixel width using top left corners of both tapes
      fovWidth = FOV_PIXELS_WIDTH * T_INCHES_LEFT_WIDTH / leftCornerDist;
      horizDistanceToTapeLeft = fovWidth / (2 * tan(MEASURED_HORIZ_FOV));
      // Getting pixel width using bottom right corners of both tapes
      fovWidth = FOV_PIXELS_WIDTH * T_INCHES_LEFT_WIDTH / rightCornerDist;
      horizDistanceToTapeRight = fovWidth / (2 * tan(MEASURED_HORIZ_FOV));
      // Averaging out left and right distances to be more accurate
      avgDistToTape = (horizDistanceToTapeLeft + horizDistanceToTapeRight) / 2;
      distance = avgDistToTape;
   }
   return distance;
}

float ContourPair::getDistFromBot()
{
   distanceFromRobot = distance - ROBOT_OFFSET;
   return distanceFromRobot;
}
