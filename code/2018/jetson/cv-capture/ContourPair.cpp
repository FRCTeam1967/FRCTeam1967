/*
   ContourPairClass.cpp
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
#include "ContourPair.h"
#include "Contour.h"
//#include <ntcore/src/networktables/NetworkTable.h>
//#include "/home/nvidia/FRCTeam1967/code/jetson/ntcore/include/ntcore.h"
//#include <cscore>

// Namespaces
using namespace std;
using namespace cv;

// Constants
const float ROBOT_OFFSET = 13.5;   // In inches, how far is the camera into the robot (from the edge of bumpers)
const int MIN_AREA = 500;          // Pixels
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
const int RIGHT = 1;
const int LEFT = 0;
const int NO_VALUE_TIME = 3;         // Seconds
const bool DEBUG_MODE = false;       // Flag for whether to print out values & messages (true = prints & false = no prints)
const int IMPOSSIBLE_ELEMENT = 1000; // Impossible x value (used later on with sorting algorithm)
int widthThreshold = DEFAULT_WIDTH_THRESHOLD;

ContourPair::ContourPair(Contour c1, Contour c2)
{
   contourPair = {c1, c2};
}

float ContourPair::getOffset(Rect leftRect, Rect rightRect, float T_INCHES_BOTH_WIDTH, int FOV_PIXELS_WIDTH)
{
   lengthWidth = rightRect.tl().x + rightRect.width - leftRect.tl().x; // length from left edge of left tape to right edge of right tape
   pixelsToInches = T_INCHES_BOTH_WIDTH / lengthWidth;
   tapeCenter = leftRect.tl().x + lengthWidth / 2;
   localOffset = (FOV_PIXELS_WIDTH / 2) - tapeCenter;
   offset = localOffset * pixelsToInches;
   return offset;
}

float ContourPair::getDist(float lengthWidth, int widthThreshold, int rectHeight, float frameHeight, float frameWidth, int rectWidth, float leftCornerDist, float rightCornerDist, float offsetInches)
{
   // Checks if tape's height is cut off
   if (lengthWidth < widthThreshold)
   {
      // Uses tape height to find distance
      widthThreshold = DEFAULT_WIDTH_THRESHOLD + 5;
      fovHeight = FOV_PIXELS_HEIGHT * T_INCHES_HEIGHT / rectHeight;
      fovWidth = fovHeight * frameWidth / frameHeight;
      fovDiagonal = sqrt(pow(fovHeight, 2) + pow(fovWidth, 2));
      //float distanceToTape = fovDiagonal / (2 * tan(theta / 1.15));
      verticalDistanceToTape = fovHeight / (2 * tan(MEASURED_VERT_FOV));
      distance = verticalDistanceToTape;
      if (DEBUG_MODE)
      {
         cout << "Height" << endl;
      }
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
   distanceFromRobot = distance - ROBOT_OFFSET
   return distanceFromRobot;
}