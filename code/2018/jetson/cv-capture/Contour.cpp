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
#include "Contour.h"
//#include <ntcore/src/networktables/NetworkTable.h>
//#include "/home/nvidia/FRCTeam1967/code/jetson/ntcore/include/ntcore.h"
//#include <cscore>

// Namespaces
using namespace std;
using namespace cv;

float getSlope(Point maxy, Point max2y) //float x1, float y1, float x2, float y2)
{
    // Using two (x, y) coordinates, returns the slope of the line
    return ((maxy.y - max2y.y) / (maxy.x - max2y.x));
}

Contour::Contour(vector<Point> points)
{
    points = points;
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
    // Find slope of the tape
    findMaxYs();
    slope = getSlope(maxy, max2y);

    // Print out the x & y coordinates & the slope
    //cout << "A: " << a << endl;
    //cout << "MAX Y : " << maxy << endl;
    //cout << "MAX 2 Y : " << max2y << endl;
    //cout << "SLOPE : " << slope << endl;

    if (maxy != max2y)
    {
        if (slope < 0)
        {
            lr = RIGHT; //right tape
        }
        else if (slope > 0)
        {
            lr = LEFT; //left tape
        }
        else
        {
            lr = NONE;
        }
    }
    return lr;
}
