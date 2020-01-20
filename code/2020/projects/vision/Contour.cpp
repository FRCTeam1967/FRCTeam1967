/*
   Contour.cpp
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
#include "Contour.h"

// Namespaces
using namespace std;
using namespace cv;

float getSlope(float x1, float y1, float x2, float y2) //float x1, float y1, float x2, float y2)
{
    // Using two (x, y) coordinates, returns the slope of the line
    return ((y2 - y1) / (x2 - x1));
}

Contour::Contour(vector<Point> points)
{
    this->points = points;
    getLeftOrRight();
}

void Contour::findMaxYs()
{
    maxy.x = 640;
    maxy.y = 480;
    max2y.x = 640;
    max2y.y = 480;
    for (int a = 0; a < points.size(); a++)
    {
        if (points[a].y < maxy.y)
        {
            // Reassign maxy & max2y
            max2y = maxy;
            maxy = points[a];
        }
        else if (points[a].y < max2y.y)
        {
            // Reassign maxy
            max2y = points[a];
        }
    }
}

enum leftOrRight Contour::getLeftOrRight()
{
	enum leftOrRight lr;
    slope = 0;
    findMaxYs();
    slope = getSlope(maxy.x, maxy.y, max2y.x, max2y.y);

    // Print out the x & y coordinates & the slope
    /*cout << "MAX Y : " << maxy << endl;
    cout << "MAX 2 Y : " << max2y << endl;
    cout << "SLOPE : " << slope << endl;*/

    if (maxy != max2y)
    {
        if (slope < 0)
        {
            lr = RIGHT;
        }
        else if (slope > 0)
        {
            lr = LEFT;
        }
        else
        {
            lr = NONE;
        }
    }
    
    //cout << "LR: " << lr << endl;
    return lr;
}
