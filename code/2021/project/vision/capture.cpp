/*
   capture.cpp - code for retro-reflective tape vision processing
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
#include "ContourPair.h"
#include <ntcore.h>
#include "HSV.h"
#include "VisionCalculations.h"

// Namespaces
using namespace std;
using namespace cv;

// Constants
const int MIN_AREA = 500; // Pixels
const float T_INCHES_BOTH_WIDTH = 14.5;
const int FOV_PIXELS_WIDTH = 640;
const int DEFAULT_WIDTH_THRESHOLD = 100; // Number of pixels from left edge to right edge of both tapes before tape gets cut off (lengthwidth)
const int NO_VALUE_TIME = 3;             // Seconds
const int IMPOSSIBLE_ELEMENT = 1000;     // Impossible x value (used later on with sorting algorithm)
int widthThreshold = DEFAULT_WIDTH_THRESHOLD;
float smallestOffset;
float distToSend;
float indexOfOffset;

// Main Function
int main(int argc, char **argv)
{
    // Initialize sortedContours & contourPairs vectors
    vector<Contour> sortedContours;
    vector<ContourPair> contourPairs;

    bool printCalculations = true;
    HSV hsv = HSV();

    VisionCalculations calcs = VisionCalculations();


    //Network tables send data to the roboRIO
    NetworkTable::SetTeam(1967); //set team number
    NetworkTable::SetClientMode();
    NetworkTable::Initialize();

    shared_ptr<NetworkTable> vTable = NetworkTable::GetTable("SmartDashboard");

    // Checks if argument passed
    bool argPassed = true;

    if (argc != 1)
    {
        // There are arguments being passed in
        argPassed = false;
    }
	
    // Change exposure & brightness of camera
    system("v4l2-ctl -d /dev/video0 -c exposure_auto=1 -c exposure_absolute=1 -c brightness=0"); // KEEP
    
    // Opens the camera stream
    VideoCapture cap(0);
    // If camera stream won't open, display this message
    if (!cap.isOpened())
    {
        cerr << "Failed to open USB camera. Check that the camera is plugged into the correct port" << endl;
        return -1;
    }

    //SET RES
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    
    auto start = chrono::high_resolution_clock::now();
    float frames = 0;
    vector<Point> maxContour;
    vector<vector<Point>> contours;
    int counter = 0;
    float average[8];

    // Calibration variable
    bool calibrateHSVOn = false;

    // Clock variables
    auto valueStart = chrono::high_resolution_clock::now();
    double duration = 0;

    for (;;)
    {
        Mat frame, green, outline;
        cap >> frame;
	
	//rotate image
	cv::transpose(frame, frame); // swaps the x & y axis of image, producing a reversed & rotated image
	cv::flip(frame, frame, 1); //flip to correct for the reversal from the transpose

        float frameHeight = frame.size().height;
        float frameWidth = frame.size().width;

        // Convert from brg to hsv
        cvtColor(frame, green, COLOR_BGR2HSV);
        // Filter green taperobot distance: 16.6748
        inRange(green, Scalar(hsv.getHue(HSV_LOWERBOUND), hsv.getSat(HSV_LOWERBOUND), \
	    hsv.getVal(HSV_LOWERBOUND)), Scalar(hsv.getHue(HSV_UPPERBOUND), \
	    hsv.getSat(HSV_UPPERBOUND), hsv.getVal(HSV_UPPERBOUND)), green);
        // Blurs image
        GaussianBlur(green, outline, Size(9, 9), 2, 2);
        // Finds contours
        findContours(outline, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

        vector<vector<Point>> contours_poly(contours.size());
        vector<Rect> boundRect(contours.size());

        int largestContour = -1;
        int largestContour2 = -1;
        bool hasTwoRects = false;
        char key = waitKey(1);

        if (key == ' ')
            break;
        // Turn on hsv calibration if c is pressed
        else if (key == 'c')
        {
            calibrateHSVOn = true;
        }
        // Turn off hsv callibration if x is pressed
        else if (key == 'x')
        {
            calibrateHSVOn = false;
        }

        if (calibrateHSVOn)
        {
            hsv.callibrateHSV(key);
        }

        // Loops through each contour - does more processing if contour area is greater than MIN_AREA
        for (int c = 0; c < contours.size(); c++)
        {
            if (contourArea(contours[c]) < MIN_AREA)
            {
                continue;
            }

            // Draws contours of random colors
            Scalar color = Scalar((rand() % 255) + 1, (rand() % 255) + 1, (rand() % 255) + 1);
            if (argPassed)
            {
                drawContours(frame, contours, c, color, 4);
            }

            approxPolyDP(Mat(contours[c]), contours_poly[c], 10, true);
            for (int i = 0; i < contours_poly[c].size(); i++)
            {

                if (argPassed)
                {
                    Scalar red = Scalar(255, 0, 0);
                    circle(frame, contours_poly[c][i], 3, red, 10);
                    Point p = contours_poly[c][i];
                    //cout << p << endl;
                    putText(frame, format("(%d, %d)", p.x, p.y), p, FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255));
                }
            }

            boundRect[c] = boundingRect(Mat(contours_poly[c]));
            if (argPassed)
            {
                rectangle(frame, boundRect[c].tl(), boundRect[c].br(), color);
            }

	    cout << "Rect Height: " << boundRect[c].height << endl;
	    cout << "Rect Width: " << boundRect[c].width << endl;
	    cout << "Left Corner: " << boundRect[c].tl().x << endl;
	    cout << endl << endl;

	    // cout << "Contours Size: " << contours.size() << endl;
	    //Send Offset & distance to Smart Dashboard
            //if(contourPairs.size() > 0)
	    if(contours.size() == 0)
	    {
	        cout << "Skipped a frame with zero contours" << endl;
		vTable->PutNumber("Offset", -1000);
            	vTable->PutNumber("Distance to Tape", -1000);
		continue;
	    }
	    calcs.calculateOffset(boundRect[c].tl().x, boundRect[c].width);
	    float offset = calcs.returnOffset();
	    cout << "Offset: " << offset << endl;

	    calcs.calculateDist(boundRect[c].width);
 	    float dist = calcs.returnDist();
	    cout << "Distance: " << dist << endl;

	    // Send data to smart dash
	    vTable->PutNumber("Offset", offset);
            vTable->PutNumber("Distance to Tape", dist);
	    vTable->PutNumber("Velocity", calcs.returnVelocity(boundRect[c].width));
	    vTable->PutNumber("Acceleration", calcs.returnAcceleration(boundRect[c].width));

	    cout << "Velocity: " << calcs.returnVelocity(boundRect[c].width) << endl;
	    cout << "Acceleration: " << calcs.returnAcceleration(boundRect[c].width) << endl;

        }

	if (contours.size() == 0) {
	    vTable->PutNumber("Offset", -1000);
            vTable->PutNumber("Distance to Tape", -1000);
	}

	// Decides whether or not to run the camera streams
        if (argPassed)
        {
            circle(frame, Point(0, 0), 3, Scalar(255, 0, 0), 10);
            imshow("camera feed", frame);
            imshow("filtered green", green);
        } 

        frames++;

        // Calculate the msec_duration & fps
        auto end = chrono::high_resolution_clock::now();
        auto msec_duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        float fps = (frames / msec_duration.count()) * 1000;
	//cout << "FPS" << endl;
	//cout << fps << endl;
    }

    cap.release();

    // Calculate the msec_duration & fps
    auto end = chrono::high_resolution_clock::now();
    auto msec_duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    float fps = (frames / msec_duration.count()) * 1000;
}
