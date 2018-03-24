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
//#include <ntcore/src/networktables/NetworkTable.h>
//#include "/home/nvidia/FRCTeam1967/code/jetson/ntcore/include/ntcore.h"
//#include <cscore>
#include <networktables/NetworkTable.h>

using namespace std;
using namespace cv;

const float ROBOT_OFFSET = 19; //inches
const int MIN_AREA =500; //pixels
const float T_INCHES_HEIGHT = 16;
const float T_INCHES_WIDTH = 2;
const float T_INCHES_LEFT_WIDTH = 6;
const float T_INCHES_BOTH_WIDTH = 8;
const int FOV_PIXELS_HEIGHT = 480;
const int FOV_PIXELS_WIDTH = 640;
const float theta = 68.5 * M_PI / 360; //degrees
const float MEASURED_HORIZ_FOV = 51.80498 * M_PI / 360;
const float MEASURED_VERT_FOV = 38.3557 * M_PI / 360;
const int DEFAULT_WIDTH_THRESHOLD = 100; // number of pixels from left edge to right edge of both tapes before tape gets cut off (lengthwidth)
const int NO_VALUE_TIME = 3; // seconds
int widthThreshold = DEFAULT_WIDTH_THRESHOLD;

double hue[] = {50,81};
double sat[] = {163,255};
double val[] = {34,185};

void changeKey(double hsv[], char key, bool plus) 
{
	// determines whether to change the upper or lower bounds of the hsv value + chagnes it
	int upper = 0;
	if (isupper(key))
		upper = 1;
	if(plus == true)
		hsv[upper]++;
	else
		hsv[upper]--;
}

void callibrateHSV(char key) 
{
	// gets the +/- (which determines if hsv is added/subtracted) and upper/lower case
	// letter (upper/lower bound + hue/sat/val) from user input and passes it to changeKey
	static bool plus;
	
	switch (key)
	{
		case '+':
			plus=true;
			break;
		case '-':
			plus=false;
			break;
		case 'h':
		case 'H':
			changeKey(hue, key, plus);
			break;
		case 's':
		case 'S':
			changeKey(sat, key, plus);
			break;
		case 'v':
		case 'V':
			changeKey(val, key, plus);
			break;
		case 'r':
			hue[0] = 50;
			hue[1] = 81;
			sat[0] = 163;
			sat[1] = 255;
			val[0] = 72;
			val[1] = 144;
			break;
	}

	cout << "hue: [" << hue[0] << ", " << hue[1] << "]" << endl;
	cout << "sat: [" << sat[0] << ", " << sat[1] << "]"<< endl;
	cout << "val: [" << val[0] << ", " << val[1] << "]" << endl;
}

float findAverage(float average[])
{
	// finds average of values in an array of length 8
	int m;
	float sum;

	sum = 0;
	for (m=0; m<8; m++)
	{
		sum+=average[m];
	}

	return sum/8.0;
}

int main(int argc, char** argv)
{
	// Network tables send data to the roboRIO
	NetworkTable::SetTeam(1967);
	NetworkTable::SetClientMode();
	NetworkTable::Initialize();
	shared_ptr<NetworkTable> vTable = NetworkTable::GetTable("SmartDashboard");
	
	// checks if argument passed
	bool argPassed = false;
	
	if (argc != 1)
	{
		// There are arguments being passed in
		argPassed = true;
	}
	
	system("v4l2-ctl -d /dev/video1 -c exposure_auto=1 -c exposure_absolute=1 -c brightness=10"); // KEEP

    VideoCapture cap(1);
 
	if (!cap.isOpened())
	{
            cerr << "Failed to open USB camera" << endl;
            return -1;
	}

	auto start = chrono::high_resolution_clock::now();
	float frames = 0;
	vector<Point> maxContour;
	vector<vector<Point>> contours;
	int counter=0;
	float average[8];
    float lastAverage = 0;
    
    // calibration variable
    bool calibrateHSVOn = false;
    
    // clock variables
    auto valueStart = chrono::high_resolution_clock::now();
    double duration = 0;

	for(;;)
	{
 //       cout << "duration: " << duration << endl;
 //       cout << "last average: " << lastAverage << endl;
		Mat gray, frame, green, outline;
		//cout << "height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
		//cout << "width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
        cap >> frame;
		// convert from brg to hsv
		cvtColor(frame, green, COLOR_BGR2HSV);
		// filter green taperobot distance: 16.6748

		inRange(green,Scalar(hue[0], sat[0], val[0]), Scalar(hue[1], sat[1], val[1]), green);
		// blurs image
		GaussianBlur(green, outline, Size(9, 9), 2, 2);
		// finds contours
		findContours(outline, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
		vector<vector<Point>> contours_poly(contours.size());
		vector<Rect> boundRect(contours.size());
        bool hasTwoRects = false;
		int largestContour = -1;
		int largestContour2 = -1;

        char key = waitKey(1);

		if (key == ' ')
            		break;
        // turn on hsv calibration if c is pressed
        else if (key == 'c') {
            calibrateHSVOn = true;
        }
        // turn off hsv callibration if x is pressed
        else if (key == 'x') {
            calibrateHSVOn = false;
        }
        
        if (calibrateHSVOn) {
            callibrateHSV(key);
        }


		// loops through each contour - does more processing if contour area is greater than MIN_AREA
		for(int c = 0; c < contours.size(); c++)
		{
			//double currentArea = contourArea(contours[c]);

			if(contourArea(contours[c]) < MIN_AREA) 
			{
				continue;
			}
			// draws contours of random colors
			Scalar color = Scalar((rand()%255)+1, (rand()%255)+1, (rand()%255)+1);
			if (argPassed) {
				drawContours(frame, contours, c, color, 4);
			}

			// finds polygons and draws bounding rectangles
			approxPolyDP(Mat(contours[c]), contours_poly[c], 10, true);
			for (int i=0; i<contours_poly[c].size(); i++)
			{
				if (argPassed) {
					Scalar red = Scalar(255, 0, 0);
					circle(frame, contours_poly[c][i], 3, red, 10);
					Point p = contours_poly[c][i];
					putText(frame, format("(%d, %d)", p.x, p.y), p, FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255));
				}
			}

			boundRect[c] = boundingRect(Mat(contours_poly[c]));
			if (argPassed) {
				rectangle(frame, boundRect[c].tl(), boundRect[c].br(), color);
			}

            // finds largest and second largest contours
			if (largestContour == -1)
			{
				largestContour = c;
				continue;
			}

			if (largestContour2 == -1)
				largestContour2 = c;

			// we have 2 contours to check 
			if(contourArea(contours[c]) > contourArea(contours[largestContour]))
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

		// finds distance only if 2 pieces of tape are detected
		if (hasTwoRects)
		{
            // resets timer because calculating distance to tape again
            duration = 0;
            
			// initializes variables
			float finalDistInInches;
			int rectHeight = boundRect[largestContour].height;
			int rectWidth = boundRect[largestContour].width;

			Rect largestRect = boundRect[largestContour];
			Rect largestRect2 = boundRect[largestContour2];
			float leftCornerDist = abs(largestRect.tl().x - largestRect2.tl().x);
			float rightCornerDist = abs(largestRect.br().x - largestRect2.br().x);

			// distinguishes left and right tape
			Rect leftRect = largestRect, rightRect = largestRect2;
			if (largestRect.tl().x > largestRect2.tl().x)
			{
				leftRect=largestRect2;
				rightRect=largestRect;
			}

			// finds horizontal offset
			float lengthWidth = rightRect.tl().x + rightRect.width - leftRect.tl().x; // length from left edge of left tape to right edge of right tape
			float pixelsToInches = T_INCHES_BOTH_WIDTH / lengthWidth;
			float tapeCenter = leftRect.tl().x + lengthWidth/2;
			float localOffset = (FOV_PIXELS_WIDTH / 2) - tapeCenter;
			float offsetInches = localOffset * pixelsToInches;

			//cout << "widthThreshold: " << widthThreshold << endl;
			
			// checks if tape's height is cut off
			if(lengthWidth < widthThreshold)
			{
				// uses tape height to find distance
				widthThreshold = DEFAULT_WIDTH_THRESHOLD + 5;
				float fovHeight = FOV_PIXELS_HEIGHT * T_INCHES_HEIGHT / rectHeight;
				float fovWidth = fovHeight * frame.size().width / frame.size().height;
				float fovDiagonal = sqrt(pow(fovHeight, 2) + pow(fovWidth, 2));
				//float distanceToTape = fovDiagonal / (2 * tan(theta / 1.15));
				float verticalDistanceToTape = fovHeight / (2 * tan(MEASURED_VERT_FOV));
				finalDistInInches = verticalDistanceToTape;
				//cout << "height" << endl;

			}
			else {
				// uses tape width to find distance
				widthThreshold = DEFAULT_WIDTH_THRESHOLD;
				float fovWidth = FOV_PIXELS_WIDTH * T_INCHES_WIDTH / rectWidth;
				float fovHeight = fovWidth * frame.size().height / frame.size().width;
				float fovDiagonal = sqrt(pow(fovHeight, 2) + pow(fovWidth, 2));
				float distanceToTape = fovDiagonal / (2 * tan(theta / 1.4));

				// getting pixel width using top left corners of both tapes 
				fovWidth = FOV_PIXELS_WIDTH * T_INCHES_LEFT_WIDTH / leftCornerDist;
				float horizDistanceToTapeLeft = fovWidth / (2 * tan(MEASURED_HORIZ_FOV));


				// getting pixel width using bottom right corners of both tapes
				fovWidth = FOV_PIXELS_WIDTH * T_INCHES_LEFT_WIDTH / rightCornerDist;
				float horizDistanceToTapeRight = fovWidth / (2 * tan(MEASURED_HORIZ_FOV));	

				// averaging out left and right distances to be more accurate
				float avgDistToTape = (horizDistanceToTapeLeft + horizDistanceToTapeRight) / 2;
				finalDistInInches = avgDistToTape;
			}
		
			float robotDistance = finalDistInInches - ROBOT_OFFSET;


			
			// sends distance and offset to robot
			vTable->PutNumber("horizontal offset", offsetInches);
			vTable->PutNumber("distance to tape", finalDistInInches);
			vTable->PutNumber("robot distance", robotDistance);

			average[counter] = finalDistInInches;

			counter++;

			if(counter==8)
			{
                lastAverage = findAverage(average);
 //               cout << "averaged distance to tape: " << lastAverage << endl;
				vTable->PutNumber("averaged distance to tape", lastAverage);
				counter=0;
                
			}

            // printing out values
//            cout<< "Horizontal offset: "<<offsetInches<<endl;
//            cout << "rectWidth: " << rectWidth << endl;
//            cout << "lengthWidth: " << lengthWidth << endl;
//            cout << "distance to tape: " << finalDistInInches << endl;
            cout << "robot distance: " << robotDistance << endl;
//            cout<<"average counter: "<<counter<<", average value: "<<average[counter]<<endl;
			cout<<" "<<endl;
		}
        // if not calculating distance to tape
        else {
            // start the clock if not started
            if (duration == 0) {
                auto valueStart = chrono::high_resolution_clock::now();
                duration = .001;
            }
            else {
                // calculate duration
                auto valueEnd = chrono::high_resolution_clock::now();
                auto valueDuration = chrono::duration_cast<chrono::milliseconds>(valueEnd - valueStart);
                duration = valueDuration.count() * 1000;
                
                // send -1 to distance if time not calculating new values is more than 2 seconds
                if (duration >= NO_VALUE_TIME) {
                    vTable->PutNumber("distance to tape", -1);
//                   cout << "averaged distance to tape: -1 " << endl;
                    vTable->PutNumber("averaged distance to tape", -1);
                }
                else {
//                    cout << "averaged distance to tape: " << lastAverage << endl;
                    vTable->PutNumber("averaged distance to tape", lastAverage);
                }
            }
        }
        
        if (argPassed) {
			circle(frame, Point(0, 0), 3, Scalar(255, 0, 0), 10);
			imshow("camera feed", frame);
			imshow("filtered green", green);
		}

		frames++;
		
			auto end = chrono::high_resolution_clock::now();
	auto msec_duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	float fps = (frames / msec_duration.count()) * 1000;

	cout << "msec_duration: " << msec_duration.count() << endl;
	cout << "frames: " << frames << " FPS: " << fps << endl;
	}

	cap.release();

	auto end = chrono::high_resolution_clock::now();
	auto msec_duration = chrono::duration_cast<chrono::milliseconds>(end - start);
	float fps = (frames / msec_duration.count()) * 1000;

	cout << "msec_duration: " << msec_duration.count() << endl;
	cout << "frames: " << frames << " FPS: " << fps << endl;
}
			
