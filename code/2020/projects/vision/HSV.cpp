/*
   HSV.cpp
   @author - 
   @version - January 23, 2020
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
#include "HSV.h"

using namespace std;
using namespace cv;

HSV::HSV() {
    cout << "HSV VALUES" << endl << "Hue: " << hue[HSV_LOWERBOUND] << " " << hue[HSV_UPPERBOUND] << endl;
    cout << "HSV VALUES" << endl << "Sat: " << sat[HSV_LOWERBOUND] << " " << sat[HSV_UPPERBOUND] << endl;
    cout << "HSV VALUES" << endl << "Val: " << val[HSV_LOWERBOUND] << " " << val[HSV_UPPERBOUND] << endl;

}

void HSV::setHue(double hueLowerBound, double hueUpperBound)
{
    hue[HSV_LOWERBOUND] = hueLowerBound;
    hue[HSV_UPPERBOUND] = hueUpperBound;
}

double HSV::getHue(int lowerOrUpperBound)
{
    return hue[lowerOrUpperBound];
}

void HSV::setSat(double satLowerBound, double satUpperBound)
{
    sat[HSV_LOWERBOUND] = satLowerBound;
    sat[HSV_UPPERBOUND] = satUpperBound;
}

double HSV::getSat(int lowerOrUpperBound)
{
    return sat[lowerOrUpperBound];
}

void HSV::setVal(double valLowerBound, double valUpperBound)
{
    val[HSV_LOWERBOUND] = valLowerBound;
    val[HSV_UPPERBOUND] = valUpperBound;
}

double HSV::getVal(int lowerOrUpperBound)
{
    return val[lowerOrUpperBound];
}

void HSV::changeKey(double hsv[], char key, bool plus) // Determines whether to change the upper or lower bounds of the hsv value + chagnes it
{
    int upper = 0;
    if (isupper(key))
        upper = 1;
    if (plus == true)
        hsv[upper]++;
    else
        hsv[upper]--;
}

void HSV::callibrateHSV(char key) // Sets hsv values baed on human input
{
    // Gets the +/- (which determines if hsv is added/subtracted) and upper/lower case
    // Letter (upper/lower bound + hue/sat/val) from user input and passes it to changeKey
    static bool plus;

    switch (key)
    {
    case '+': // Increase vale
        plus = true;
        break;
    case '-': // Decrease value
        plus = false;
        break;
    case 'h':
    case 'H': // For hue
        changeKey(hue, key, plus);
        break;
    case 's':
    case 'S': // For saturation
        changeKey(sat, key, plus);
        break;
    case 'v':
    case 'V': // For value
        changeKey(val, key, plus);
        break;
    case 'r': // For reset
        hue[HSV_LOWERBOUND] = 0;
        hue[HSV_UPPERBOUND] = 255;
        sat[HSV_LOWERBOUND] = 0;
        sat[HSV_UPPERBOUND] = 255;
        val[HSV_LOWERBOUND] = 0;
        val[HSV_UPPERBOUND] = 255;
        break;
    }
    // Print out the HSV values
    cout << "hue: [" << hue[HSV_LOWERBOUND] << ", " << hue[HSV_UPPERBOUND] << "]" << endl;
    cout << "sat: [" << sat[HSV_LOWERBOUND] << ", " << sat[HSV_UPPERBOUND] << "]" << endl;
    cout << "val: [" << val[HSV_LOWERBOUND] << ", " << val[HSV_UPPERBOUND] << "]" << endl;
}

