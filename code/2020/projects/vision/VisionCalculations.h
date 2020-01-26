/*
   HSV.h
   @author - Kara
   @version - January 25, 2020
*/

#ifndef VISION_CALCULATIONS_H
#define VISION_CALCULATIONS_H

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

using namespace std;
using namespace cv;

class VisionCalculations { //hi
    public: 
	VisionCalculations(); //Contructor
	float  calculateDist(float boundRectWidth);
	int calculateOffset(int boundRectLeftCorner, float boundRectWidth);
	float returnDist();
	int returnOffset();
    private:
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
   	float widthOfTape;
   	// variables for offset calculation
   	float offsetInInches;
   	float pixelsToInches;
   	float tapeCenter;
   	float offsetInPixels;
};
#endif /* VISION_CALCULATIONS_H */
