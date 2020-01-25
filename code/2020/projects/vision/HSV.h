/*
   HSV.h
   @author - Maddie
   @version - January 23, 2020
*/

#ifndef HSV_H
#define HSV_H

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

#define HSV_LOWERBOUND 0
#define HSV_UPPERBOUND 1

using namespace std;
using namespace cv;

class HSV {
    public: 
	HSV(); // Constructor

        // Calibration functions
        void changeKey(double hsv[], char key, bool plus); // Determines whether to change the upper or lower bounds of the hsv value + chagnes it
        void callibrateHSV(char key); // Sets hsv values baed on human input


        // HSV Setting & getting
        void setHue(double hueLowerBound, double hueUpperBound); // Set hue value range
        double getHue(int lowerOrUpperBound); // Get hue value range

        void setSat(double satLowerBound, double satUpperBound); // Set saturation value range
        double getSat(int lowerOrUpperBound); // Get hue value range

        void setVal(double valLowerBound, double valUpperBound); // Set value value range
        double getVal(int lowerOrUpperBound); // Get hue value range

    private:
	double hue[2] = {73, 84};
	double sat[2] = {212, 255};
	double val[2] = {135, 255};
};

#endif /* HSV_H */
