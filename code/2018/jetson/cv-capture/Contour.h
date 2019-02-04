/*
   Contour.h
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
//#include <ntcore/src/networktables/NetworkTable.h>
//#include "/home/nvidia/FRCTeam1967/code/jetson/ntcore/include/ntcore.h"
//#include <cscore>

// Namespaces
using namespace std;
using namespace cv;

#ifndef CONTOUR_H_
#define CONTOUR_H_

enum leftOrRight{LEFT, RIGHT, NONE};

class Contour
{
 public:
   Contour(vector<Point> points);
   enum leftOrRight getLeftOrRight();
   void findMaxYs();
 private:
   vector<Point> points;
   float slope;
   Point maxy;
   Point max2y;
};
#endif