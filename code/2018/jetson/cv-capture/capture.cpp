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
//#include <ntcore/src/networktables/NetworkTable.h>
//#include "/home/nvidia/FRCTeam1967/code/jetson/ntcore/include/ntcore.h"
//#include <cscore>

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

// Set our HSV values
double hue[] = {69, 77};   //{63, 96};
double sat[] = {112, 255}; //{112, 255};
double val[] = {132, 255}; //{80, 255};

float lengthWidth;

void changeKey(double hsv[], char key, bool plus)
{
    // Determines whether to change the upper or lower bounds of the hsv value + chagnes it
    int upper = 0;
    if (isupper(key))
        upper = 1;
    if (plus == true)
        hsv[upper]++;
    else
        hsv[upper]--;
}

void callibrateHSV(char key)
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
        hue[0] = 0;
        hue[1] = 255;
        sat[0] = 0;
        sat[1] = 255;
        val[0] = 0;
        val[1] = 255;
        break;
    }
    // Print out the HSV values
    cout << "hue: [" << hue[0] << ", " << hue[1] << "]" << endl;
    cout << "sat: [" << sat[0] << ", " << sat[1] << "]" << endl;
    cout << "val: [" << val[0] << ", " << val[1] << "]" << endl;
}

//sorting
void sortContours(vector<Contour> &sortedContours, vector<vector<Point>> contours)
{
    // Create variables & arrays necesarry for code segment below
    int element = 0;
    int index;
    int lowestValue;

    for (int a = 0; a < contours.size(); a++) // Loops through as many times as the size of 'contours'
    {
        lowestValue = FOV_PIXELS_WIDTH + 1;       // Set lowest value to 1 + the maximum x value
        for (int b = 0; b < contours.size(); b++) // Loops through as many times as the size of 'contours'
        {
            if (contours[b].size() == 0) // If the array is empty, continue
            {
                continue;
            }

            if ((lowestValue > (contours[b][0]).x) && ((contours[b][0]).x != 1000))
            {
                lowestValue = (contours[b][0]).x; // Set 'lowestValue' to the new low value
                index = b;                        // Set the index to the index of the lowest value (we need this for later)
            }
        }
        if (lowestValue < (FOV_PIXELS_WIDTH + 1)) // Will sort the contours as long as the values are appropriate
        {
            Contour c = Contour(contours[index]);
            sortedContours.push_back(c); // Add elements to the sorted list
            element++;                                   // increment the element that the next low value will be added to
            (contours[index][0]).x = IMPOSSIBLE_ELEMENT; // Set value at index of last low value to an impossibly large number
        }
    }
}

int main(int argc, char **argv)
{
    // Initialize sortedContours & contourPairs vectors
    vector<Contour> sortedContours;
    vector<ContourPair> contourPairs;

    //Network tables send data to the roboRIO
    /*NetworkTable::SetTeam(1967); //set team number
    NetworkTable::SetClientMode();
    NetworkTable::Initialize();
    shared_ptr<NetworkTable> vTable = NetworkTable::GetTable("SmartDashboard");*/

    // Checks if argument passed
    bool argPassed = true;

    if (argc != 1)
    {
        // There are arguments being passed in
        argPassed = true;
    }

    // Change exposure & brightness of camera
    system("v4l2-ctl -d /dev/video0 -c exposure_auto=1 -c exposure_absolute=1 -c brightness=1"); // KEEP
    // Opens the camera stream
    VideoCapture cap(0);
    // If camera stream won't open, display this message
    if (!cap.isOpened())
    {
        cerr << "Failed to open USB camera. Check that the camera is plugged into the correct port" << endl;
        return -1;
    }

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

        float frameHeight = frame.size().height;
        float frameWidth = frame.size().width;

        // Convert from brg to hsv
        cvtColor(frame, green, COLOR_BGR2HSV);
        // Filter green taperobot distance: 16.6748
        inRange(green, Scalar(hue[0], sat[0], val[0]), Scalar(hue[1], sat[1], val[1]), green);
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
            callibrateHSV(key);
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

            // Finds largest and second largest contours
            if (largestContour == -1)
            {
                largestContour = c;
                continue;
            }

            if (largestContour2 == -1)
            {
                largestContour2 = c;
            }

            // We have 2 contours to check
            if (contourArea(contours[c]) > contourArea(contours[largestContour]))
            {
                int temp = largestContour;
                largestContour = c;
                largestContour2 = temp;
            }
            else if (contourArea(contours[c]) > contourArea(contours[largestContour2]))
            {
                largestContour2 = c;
            }
            hasTwoRects = true;
        }

        // Finds distance only if 2 pieces of tape are detected
        if (hasTwoRects)
        {
            sortedContours.clear();
            contourPairs.clear();
            
            // Sort through contours & create a new sorted list of them --> can use later when pairing up the tapes
            sortContours(sortedContours, contours_poly);

            // Resets timer because calculating distance to tape again
            duration = 0;

            for (int k = 0; k < sortedContours.size() - 1; k++)
            {
                if (sortedContours[k].getLeftOrRight() != LEFT)
                {
                    //cout << "continuing" << endl;
                    continue;
                }
                if (sortedContours[k + 1].getLeftOrRight() != RIGHT)
                {
                    //cout << "continuing" << endl;
                    continue;
                }

                ContourPair cp = ContourPair(sortedContours[k], sortedContours[k + 1]);
                contourPairs.push_back(cp);

                // Initializes variables
                float finalDistInInches;
                int rectHeight = boundRect[k].height;
                int rectWidth = boundRect[k].width;

                Rect largestRect = boundRect[k];
                Rect largestRect2 = boundRect[k + 1];
                float leftCornerDist = abs(largestRect.tl().x - largestRect2.tl().x);
                float rightCornerDist = abs(largestRect.br().x - largestRect2.br().x);

                // Distinguishes left and right tape
                Rect leftRect = largestRect, rightRect = largestRect2;
                if (largestRect.tl().x > largestRect2.tl().x)
                {
                    leftRect = largestRect2;
                    rightRect = largestRect;
                }

                contourPairs[contourPairs.size() - 1].getOffset(leftRect.tl().x, rightRect.tl().x, rightRect.width, T_INCHES_BOTH_WIDTH, FOV_PIXELS_WIDTH);
                contourPairs[contourPairs.size() - 1].getDist(lengthWidth, widthThreshold, rectHeight, frameHeight, frameWidth, rectWidth, leftCornerDist, rightCornerDist);

                average[counter] = finalDistInInches;

                counter++;

                if (counter == 8)
                {
                    counter = 0;
                }
            }
            
            //Send Offset & distance to Smart Dashboard
            float smallestOffset = fabs(contourPairs[0].returnOffset());
            float distToSend = contourPairs[0].returnDist();
            int indexOfOffset = 0;

            for (int a = 0; a < contourPairs.size(); a++)
            {
                if (fabs(smallestOffset) > fabs(contourPairs[a].returnOffset()))
                {
                	indexOfOffset = a;
                    smallestOffset = fabs(contourPairs[a].returnOffset());
                    distToSend = contourPairs[a].returnDist();
                }
            }
            
            //Print out distances & offsets
            for(int b = 0; b < contourPairs.size(); b++)
            {
            	cout << "Distance of " << b << ": " << contourPairs[b].returnDist() << endl;
     			cout << "Offset of " << b << ": " << contourPairs[b].returnOffset() << endl;       		
            }
            cout << "Smallest offset (Target # " << indexOfOffset << "): " << smallestOffset << endl;
     		cout << "Distance to send (Target # " << indexOfOffset << "): " << smallestOffset << endl;
     		cout << endl;
            if(!isinf(distToSend))
            {
            	//Send data to smart dashboard
            	//vTable->PutNumber("Offset", smallestOffset);
            	//vTable->PutNumber("Distance to Tape", distToSend);
            }
        }
        //If not calculating distance to tape
        else
        {
            cout << "no dist" << endl;
            // Start the clock if not started
            if (duration == 0)
            {
                auto valueStart = chrono::high_resolution_clock::now();
                duration = .001;
            }
            else
            {
                // Calculate duration
                auto valueEnd = chrono::high_resolution_clock::now();
                auto valueDuration = chrono::duration_cast<chrono::milliseconds>(valueEnd - valueStart);
                duration = valueDuration.count() * 1000;

                // Send -1 to distance if time not calculating new values is more than 2 seconds
                if (duration >= NO_VALUE_TIME)
                {
                    //vTable->PutNumber("Distance to Tape", -1);
                    // vTable->PutNumber("Averaged Distance to Tape", -1);
                }
            }
        }

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
    }

    cap.release();

    // Calculate the msec_duration & fps
    auto end = chrono::high_resolution_clock::now();
    auto msec_duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    float fps = (frames / msec_duration.count()) * 1000;
}
