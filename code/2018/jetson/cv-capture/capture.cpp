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
const float T_INCHES_LEFT_WIDTH = 11.25;
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

// Set our HSV values
double hue[] = {62, 79};
double sat[] = {149, 255};
double val[] = {73, 236};

bool correctData = true;

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
        hue[0] = 62;
        hue[1] = 79;
        sat[0] = 149;
        sat[1] = 255;
        val[0] = 73;
        val[1] = 236;
        break;
    }
    //if (DEBUG_MODE)
    //{
    // Print out the HSV values
    cout << "hue: [" << hue[0] << ", " << hue[1] << "]" << endl;
    cout << "sat: [" << sat[0] << ", " << sat[1] << "]" << endl;
    cout << "val: [" << val[0] << ", " << val[1] << "]" << endl;
    //}
}

float findAverage(float average[])
{
    // Finds average of values in an array of length 8
    int m;
    float sum;

    sum = 0;
    for (m = 0; m < 8; m++)
    {
        sum += average[m];
    }

    return sum / 8.0;
}

float findSlope(float x1, float y1, float x2, float y2)
{
    // Using two (x, y) coordinates, returns the slope of the line
    return ((y2 - y1) / (x2 - x1));
}

//filter tape (hsv stuff & bounding box)
void filterTape(Mat frame, Mat green, Mat outline)
{
    // Convert from brg to hsv
    cvtColor(frame, green, COLOR_BGR2HSV);
    // Filter green taperobot distance: 16.6748
    inRange(green, Scalar(hue[0], sat[0], val[0]), Scalar(hue[1], sat[1], val[1]), green);
    // Blurs image
    GaussianBlur(green, outline, Size(9, 9), 2, 2);
    // Finds contours
    findContours(outline, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
}

//finding polygons
findPolygons(vector<Points> contours, vector<Points> contours_poly, Mat frame)
{
    approxPolyDP(Mat(contours), contours_poly, 10, true);
    for (int i = 0; i < contours_poly.size(); i++)
    {
        if (argPassed)
        {
            Scalar red = Scalar(255, 0, 0);
            circle(frame, contours_poly[i], 3, red, 10);
            Point p = contours_poly[i];
            putText(frame, format("(%d, %d)", p.x, p.y), p, FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255));
        }
    }
}

//sorting
void sortContours(vector<vector<Points>> contourCopy, int element, vector<vector<Point>> sortedContours)
{
    // Create variables & arrays necesarry for code segment below
    int element = 0;
    int index;
    int lowestValue;
    for (int a = 0; a < contourCopy.size(); a++) // Loops through as many times as the size of 'contourCopy'
    {
        lowestValue = 641;                           // Set lowest value to 1 + the maximum x value
        for (int b = 0; b < contourCopy.size(); b++) // Loops through as many times as the size of 'contourCopy'
        {
            if (contourCopy[b].size() == 0) // If the array is empty, continue
            {
                continue;
            }

            if (DEBUG_MODE)
            {
                // Print out contourCopy's first X coordinate
                cout << "Contour Copy X Coordinate 1: " << (contourCopy[b][0]).x << endl;
            }

            if ((lowestValue > (contourCopy[b][0]).x) && ((contourCopy[b][0]).x != 1000))
            {
                lowestValue = (contourCopy[b][0]).x; // Set 'lowestValue' to the new low value
                index = b;                           // Set the index to the index of the lowest value (we need this for later)
            }
        }
        if (lowestValue < 641) // Will sort the contours as long as the values are appropriate
        {
            sortedContours[element] = contourCopy[index]; // Add elements to the sorted list
            if (DEBUG_MODE)
            {
                // Print out sortedContours array
                cout << "Sorted Contours: " << sortedContours[element] << endl;
            }
            //cout << "Sorted Contours: " << sortedContours[element] << endl;
            element++;                                      // increment the element that the next low value will be added to
            (contourCopy[index][0]).x = IMPOSSIBLE_ELEMENT; // Set valye at index of last low value to an impossibly large number
        }
    }
}

//find left & right
void findLeftRightTape(Point maxy, Point max2y, vector<vector<Points>> sortedContours, vector<float> lr, bool correctData)
{
    // Create vars for slope & if data is correct
    float slope = 0;

    // Find slope of each tape
    for (int a = 0; a < sortedContours.size(); a++)
    {
        // Print cycle # of for loop
        //cout << "A : " << a << endl;

        for (int b = 0; b < sortedContours[a].size(); b++)
        {
            if (sortedContours[a][b].y < maxy.y)
            {
                // Reassign maxy & max2y
                max2y = maxy;
                maxy = sortedContours[a][b];
            }
            else if (sortedContours[a][b].y < max2y.y)
            {
                // Reassign maxy
                max2y = sortedContours[a][b];
            }
        }

        // Find slope of the tape
        slope = findSlope(maxy.x, maxy.y, max2y.x, max2y.y);

        // Print out the x & y coordinates & the slope
        //cout << "MAX Y : (" << maxy.x << ", " << maxy.y << ") " << endl;
        //cout << "MAX 2 Y : (" << max2y.x << ", " << max2y.y << ") " << endl;
        //cout << "SLOPE : " << slope << endl;

        if (slope < 0)
        {
            lr[a] = 1; //right tape
        }
        else
        {
            lr[a] = 0; //left tape
        }

        // Print out if tape is left / right
        //cout << "LEFT OR RIGHT " << lr[a] << endl;
    }

    // Decide if left / right data is correct
    correctData = true;
    for (int g = 1; g < sortedContours.size(); g += 2)
    {
        if (lr[g - 1] == lr[g])
        {
            correctData = false;
        }
    }
}

void find2Largest(int largestContour, int largestContour2, int c, vector<vector<Points>> contours)
{
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
}

//find offset
void findOffset(Rect leftRect, Rect rightRect, float T_INCHES_BOTH_WIDTH, int FOV_PIXELS_WIDTH)
{
    float lengthWidth = rightRect.tl().x + rightRect.width - leftRect.tl().x; // length from left edge of left tape to right edge of right tape
    float pixelsToInches = T_INCHES_BOTH_WIDTH / lengthWidth;
    float tapeCenter = leftRect.tl().x + lengthWidth / 2;
    float localOffset = (FOV_PIXELS_WIDTH / 2) - tapeCenter;
    float offsetInches = localOffset * pixelsToInches;
}

//find distanc
void findDist(float lengthWidth, int widthThreshold, int rectHeight, Mat frame, float finalDistInInches, int rectWidth, float leftCornerDist, float rightCornerDist, float offsetInches)
{
    // Checks if tape's height is cut off
    if (lengthWidth < widthThreshold)
    {
        // Uses tape height to find distance
        widthThreshold = DEFAULT_WIDTH_THRESHOLD + 5;
        float fovHeight = FOV_PIXELS_HEIGHT * T_INCHES_HEIGHT / rectHeight;
        float fovWidth = fovHeight * frame.size().width / frame.size().height;
        float fovDiagonal = sqrt(pow(fovHeight, 2) + pow(fovWidth, 2));
        //float distanceToTape = fovDiagonal / (2 * tan(theta / 1.15));
        float verticalDistanceToTape = fovHeight / (2 * tan(MEASURED_VERT_FOV));
        finalDistInInches = verticalDistanceToTape;
        if (DEBUG_MODE)
        {
            cout << "Height" << endl;
        }
    }
    else
    {
        // Uses tape width to find distance
        widthThreshold = DEFAULT_WIDTH_THRESHOLD;
        float fovWidth = FOV_PIXELS_WIDTH * T_INCHES_WIDTH / rectWidth;
        float fovHeight = fovWidth * frame.size().height / frame.size().width;
        float fovDiagonal = sqrt(pow(fovHeight, 2) + pow(fovWidth, 2));
        float distanceToTape = fovDiagonal / (2 * tan(theta / 1.4));

        // Getting pixel width using top left corners of both tapes
        fovWidth = FOV_PIXELS_WIDTH * T_INCHES_LEFT_WIDTH / leftCornerDist;
        float horizDistanceToTapeLeft = fovWidth / (2 * tan(MEASURED_HORIZ_FOV));

        // Getting pixel width using bottom right corners of both tapes
        fovWidth = FOV_PIXELS_WIDTH * T_INCHES_LEFT_WIDTH / rightCornerDist;
        float horizDistanceToTapeRight = fovWidth / (2 * tan(MEASURED_HORIZ_FOV));

        // Averaging out left and right distances to be more accurate
        float avgDistToTape = (horizDistanceToTapeLeft + horizDistanceToTapeRight) / 2;
        finalDistInInches = avgDistToTape;

        // Angle calculation (NEEDS FIXED TO WORK PROPERLY)
        float angle = (atan((offsetInches / finalDistInInches))) * (180 / M_PI);
        //cout << "ANGLE: " << angle << endl;
    }
    float robotDistance = finalDistInInches - ROBOT_OFFSET;
}

int main(int argc, char **argv)
{
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
    system("v4l2-ctl -d /dev/video0 -c exposure_auto=1 -c exposure_absolute=1 -c brightness=10"); // KEEP
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
    float lastAverage = 0;

    // Calibration variable
    bool calibrateHSVOn = false;

    // Clock variables
    auto valueStart = chrono::high_resolution_clock::now();
    double duration = 0;

    for (;;)
    {
        if (DEBUG_MODE)
        {
            // Print out the duration & last avg
            cout << "Duration: " << duration << endl;
            cout << "Last Average: " << lastAverage << endl;
        }
        Mat frame, green, outline;
        if (DEBUG_MODE)
        {
            // Print out the height & width
            cout << "Height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
            cout << "Width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
        }
        cap >> frame;

        filterTape(frame, green);

        vector<vector<Point>> contours_poly(contours.size());
        vector<Rect> boundRect(contours.size());
        vector<vector<Point>> contourCopy(contours.size());
        vector<vector<Point>> sortedContours(contours.size());
        // Create array for left & right tapes
        vector<bool> lr(sortedContours.size());

        bool hasTwoRects = false;
        int largestContour = -1;
        int largestContour2 = -1;

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
            //double currentArea = contourArea(contours[c]);

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

            findPolygons(contours[c], contours_poly[c], frame);

            boundRect[c] = boundingRect(Mat(contours_poly[c]));
            if (argPassed)
            {
                rectangle(frame, boundRect[c].tl(), boundRect[c].br(), color);
            }
            if (DEBUG_MODE)
            {
                // Print out the contours
                cout << "Contours: " << maxContour << endl;
            }

            //vector<vector<Point>> contourCopy(contours.size());
            //vector<vector<Point>> sortedContours(contours.size());
            contourCopy = contours_poly;

            if (DEBUG_MODE)
            {
                // Print this when starting to make the sorted list
                cout << "Started Sorting" << endl;
            }

            // Sort through contours & create a new sorted list of them --> can use later when pairing up the tapes
            sortContours(contourCopy, element, sortedContours);

            if (DEBUG_MODE)
            {
                // Print this when done making the sorted list
                cout << "Finished Sorting" << endl;
            }

            // Create array for left & right tapes
            //vector<bool> lr(sortedContours.size());

            // Create the 2 pts that we'll use below
            Point maxy;
            maxy.x = 640;
            maxy.y = 480;
            Point max2y;
            max2y.x = 640;
            max2y.y = 480;

            findLeftRightTape(maxy, max2y, sortedContours, lr, correctData);
            find2Largest(largestContour, largestContour2, c, contours);

            hasTwoRects = true;
        }

        // Finds distance only if 2 pieces of tape are detected
        if (hasTwoRects)
        {
            // Resets timer because calculating distance to tape again
            duration = 0;

            int k = 0;
            if (lr[0] == RIGHT)
            {
                k = 1;
            }

            //cout << "start of for loop" << endl;
            for (; k < sortedContours.size(); k += 2)
            {
                //cout << "K VALUE: " << k << endl;
                // Initializes variables
                float finalDistInInches;
                int rectHeight = boundRect[k].height;
                int rectWidth = boundRect[k].width;

                if (DEBUG_MODE)
                {
                    // Print out the height and width (in pixels)
                    cout << "Height: " << rectHeight << endl;
                    cout << "Width: " << rectWidth << endl;
                }

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

                findOffset(leftRect, rightRect, T_INCHES_BOTH_WIDTH, FOV_PIXELS_WIDTH);

                if (DEBUG_MODE)
                {
                    // Print out offset & widthThreshold
                    cout << "Offset: " << offsetInches << endl;
                    cout << "Width Threshold: " << widthThreshold << endl;
                }

                findDist(lengthWidth, widthThreshold, rectHeight, frame, finalDistInInches, rectWidth, leftCornerDist, rightCornerDist, offsetInches);

                if (correctData)
                {
                    cout << "Final Dist Inches: " << finalDistInInches << endl;
                    //cout << k << ": " << boundRect[k] << endl;
                    //cout << " " << endl;
                }
                else
                {
                    //cout << "BAD DATA" << endl;
                    //cout << k << ": " << boundRect[k] << endl;
                    //cout << " " << endl;
                }

                if (DEBUG_MODE)
                {
                    // Print out distance from edge of robot to tape
                    cout << "Final Dist Inches: " << finalDistInInches << endl;
                }

                if (correctData)
                {
                    // Sends distance and offset to robot (through network tables)
                    //vTable->PutNumber("Horizontal Offset", offsetInches);
                    //vTable->PutNumber("Distance to Tape", finalDistInInches);
                    //vTable->PutNumber("Robot Distance", robotDistance);
                }

                average[counter] = finalDistInInches;

                counter++;

                if (counter == 8)
                {
                    lastAverage = findAverage(average);
                    if (correctData)
                    {
                        //vTable->PutNumber("Averaged Distance to Tape", lastAverage);
                    }
                    counter = 0;

                    if (DEBUG_MODE)
                    {
                        // Print out avg distance to tape & offset
                        cout << "Averaged Distance to Tape: " << lastAverage << endl;
                        cout << "Horizontal Offset: " << offsetInches << endl;
                        cout << " " << endl;
                    }
                }
                if (DEBUG_MODE)
                {
                    // Printing out values
                    cout << "Horizontal Offset: " << offsetInches << endl;
                    cout << "rectWidth: " << rectWidth << endl;
                    cout << "lengthWidth: " << lengthWidth << endl;
                    cout << "Distance to Tape: " << finalDistInInches << endl;
                    cout << "Robot Distance: " << robotDistance << endl;
                    cout << "Average Counter: " << counter << ", Average Value: " << average[counter] << endl;
                    cout << " " << endl;
                }
            }
        }
        // If not calculating distance to tape
        else
        {
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
                    if (correctData)
                    {
                        //vTable->PutNumber("Distance to Tape", -1);
                    }
                    if (DEBUG_MODE)
                    {
                        cout << "Averaged Distance to Tape: -1 " << endl;
                    }
                    if (correctData)
                    {
                        // vTable->PutNumber("Averaged Distance to Tape", -1);
                    }
                }
                else
                {
                    if (DEBUG_MODE)
                    {
                        cout << "Averaged Distance to Tape: " << lastAverage << endl;
                    }
                    if (correctData)
                    {
                        //vTable->PutNumber("Averaged Distance to Tape", lastAverage);
                    }
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

        if (DEBUG_MODE)
        {
            // Print out how long the cam feed has been running & the camera FPS
            cout << "msec_duration: " << msec_duration.count() << endl;
            cout << "frames: " << frames << " FPS: " << fps << endl;
        }
    }

    //}

    cap.release();

    // Calculate the msec_duration & fps
    auto end = chrono::high_resolution_clock::now();
    auto msec_duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    float fps = (frames / msec_duration.count()) * 1000;

    if (DEBUG_MODE)
    {
        // Print out how long the cam feed has been running & the camera FPS
        cout << "msec_duration: " << msec_duration.count() << endl;
        cout << "frames: " << frames << " FPS: " << fps << endl;
    }
}
