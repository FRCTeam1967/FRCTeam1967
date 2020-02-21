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
#include "VisionCalculations.h"

// Constants
const float ROBOT_OFFSET = 0;   // In inches, how far is the camera into the robot (from the edge of bumpers)
const float TAPE_HEIGHT_IN_INCHES = 17; // Height of tape in inches
const float TAPE_WIDTH_IN_INCHES = 2;    // Width of tape in inches
const float TAPE_WIDTH_FROM_TOP_CORNERS = 39;
const float TAPE_WIDTH_FROM_BOTTOM_CORNERS = 19.5;
const float FOCAL_LENGTH_TEN_FEET = 606.15;
const float FOCAL_LENGTH_SEVENTEEN_FEET = 669.54;
const float FOCAL_LENGTH = (FOCAL_LENGTH_TEN_FEET + FOCAL_LENGTH_SEVENTEEN_FEET) / 2;
const int CAMERA_PIXELS_HEIGHT = 640;
const int CAMERA_PIXELS_WIDTH = 480;
const float ANGLE = 68.5 * M_PI / 360; // Degrees
const float MEASURED_HORIZ_FOV = 51.80498 * M_PI / 360;
const float MEASURED_VERT_FOV = 38.3557 * M_PI / 360;
const int DEFAULT_WIDTH_THRESHOLD = 100; // Number of pixels from left edge to right edge of both tapes before tape gets cut off (lengthwidth)

using namespace std;
using namespace cv;

VisionCalculations::VisionCalculations(){
   offsetInInches = -10000;
   distance = -1;
}

float VisionCalculations::calculateDist(float boundRectWidth){
   distance = (TAPE_WIDTH_FROM_TOP_CORNERS * FOCAL_LENGTH) / boundRectWidth; //distance using focal length

   //distance = (-0.0673 * boundRectWidth) + 126.1538; //distance using linear

   //distance = (-0.001309 * pow(boundRectWidth, 2)) - (0.025 * boundRectWidth) + 125.9524; //distance using quadratic regression
   //distance = distance * 2;

   //distance = (-0.000083 * pow(boundRectWidth, 3)) + (0.0025 * pow(boundRectWidth, 2)) - (0.06667 * boundRectWidth) + 126; //distance using cubic regression

   //distance = 126.6202 / (1 + (0.00534 * pow(2.71828, 0.04567 * boundRectWidth))); //distance using logistic regression

   return distance;
}

int VisionCalculations::calculateOffset(int boundRectLeftCorner, float boundRectWidth){
   pixelsToInches = TAPE_WIDTH_FROM_TOP_CORNERS / boundRectWidth;
   tapeCenter = (boundRectWidth / 2) + boundRectLeftCorner;
   offsetInPixels = (CAMERA_PIXELS_WIDTH / 2) - tapeCenter;
   offsetInInches = offsetInPixels * pixelsToInches;
   cout << "Offset in Pixels: " << offsetInPixels << endl;
   return offsetInInches;
}

float VisionCalculations::returnDist(){
   return distance;
}

int VisionCalculations::returnOffset(){
   return offsetInInches;
}
