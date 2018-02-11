#include <chrono>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/photo/photo.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <cstdlib>
//#include <cscore>

using namespace std;
using namespace cv;

const int MIN_AREA =500; //pixels
const float T_INCHES_HEIGHT = 16;
const float T_INCHES_WIDTH = 2;
const float T_INCHES_LEFT_WIDTH = 6;
const int FOV_PIXELS_HEIGHT = 480;
const int FOV_PIXELS_WIDTH = 640;
const float theta = 68.5 * 3.141592653589793238462643383279 / 360; //degrees
const float MEASURED_HORIZ_FOV = 51.80498 * 3.141592653589793238462643383279 / 360;
const float MEASURED_VERT_FOV = 38.3557 * 3.141592653589793238462643383279 / 360;
#define DEFAULT_WIDTH_THRESHOLD 44
int widthThreshold = DEFAULT_WIDTH_THRESHOLD;

int main()
{
	cout << "set fmt before cap(1)" << endl;
    	system("v4l2-ctl -d /dev/video1 --verbose --set-fmt-video=width=1280,height=720,pixelformat=1");
	system("v4l2-ctl -d /dev/video1 -c exposure_auto=1 -c exposure_absolute=1 -c brightness=10"); // KEEP
	cout << "get fmt before cap(1)" << endl;
	system("v4l2-ctl -d /dev/video1 --get-fmt-video");
	cout << "calling cap(1)" << endl;
        VideoCapture cap(1);
	system("v4l2-ctl -d /dev/video1 --verbose --set-fmt-video=width=1280,height=720,pixelformat=BGR3");
	system("v4l2-ctl -d /dev/video1 --get-fmt-video");

	cout << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
	cout << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
	cap.set(CV_CAP_PROP_FRAME_WIDTH, 1280);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
	cout << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
	cout << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
    if (!cap.isOpened())
	{
            cerr << "Failed to open USB camera" << endl;
            return -1;
	}

    auto start = chrono::high_resolution_clock::now();
    float frames = 0;
	vector<Point> maxContour;
	vector<vector<Point>> contours;

	double hue[] = {62,73};
	double sat[] = {174,255};
	double val[] = {64,126};

     for(;;)
     {
	Mat gray, frame, green, outline;
//	cout << "height: " << cap.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;
//	cout << "width: " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
        cap >> frame;
	imwrite("/home/nvidia/master-ocv/cv-capture/TapeWindow.jpg", frame);
	// convert from brg to hsv
	cvtColor(frame, green, COLOR_BGR2HSV);
	inRange(green,Scalar(hue[0], sat[0], val[0]), Scalar(hue[1], sat[1], val[1]), green); // filter green tape
	GaussianBlur(green, outline, Size(9, 9), 2, 2); // blurs image
	findContours(outline, contours, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0)); // finds contourss
	vector<vector<Point>> contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	int largestContour = 0;
	int largestContour2 = 0;

	// loops through each contour - does more processing if contour area is greater than MIN_AREA
	for(int c = 0; c < contours.size(); c++) {
		//double currentArea = contourArea(contours[c]);

		if(contourArea(contours[c]) < MIN_AREA) 
		{
			continue;
		}
		// draws contours of random colors
		Scalar color = Scalar((rand()%255)+1, (rand()%255)+1, (rand()%255)+1);
		drawContours(frame, contours, c, color, 4);

		// finds polygons
		approxPolyDP(Mat(contours[c]), contours_poly[c], 10, true);
		for (int i=0; i<contours_poly[c].size(); i++)
		{
			Scalar red = Scalar(255, 0, 0);
			circle(frame, contours_poly[c][i], 3, red, 10);
			Point p = contours_poly[c][i];
			putText(frame, format("(%d, %d)", p.x, p.y), p, FONT_HERSHEY_PLAIN, 1, Scalar(0, 0, 255));
		}

		boundRect[c] = boundingRect(Mat(contours_poly[c]));
		rectangle(frame, boundRect[c].tl(), boundRect[c].br(), color);

		if(contourArea(contours[c]) > contourArea(contours[largestContour]))
		{
			largestContour = c;
		}
		else if (contourArea(contours[c]) > contourArea(contours[largestContour2])) {
			largestContour2 = c;
		}
	}
		
	if (contours_poly.size() > 0) {
		cout<<"area of largest contour: - "<<contourArea(contours[largestContour])<<endl;
		cout<<contours_poly[largestContour]<<endl;
		cout<<"Largest rectangle height: "<<boundRect[largestContour].height <<endl;

		// parabolic formula distance
		int rectHeight = boundRect[largestContour].height;
		int rectWidth = boundRect[largestContour].width;
		cout << "rect height: " << rectHeight << endl;
		cout << "rect width: " << rectWidth << endl;

		Rect largestRect = boundRect[largestContour];
		Rect largestRect2 = boundRect[largestContour2];
		float leftCornerDist = abs(largestRect.tl().x - largestRect2.tl().x);
		float rightCornerDist = abs(largestRect.br().x - largestRect2.br().x);

		if(rectWidth < widthThreshold) {
			widthThreshold = DEFAULT_WIDTH_THRESHOLD + 5;
			cout<<"frame width: "<<frame.size().width<<endl;
			cout<<"frame height: "<<frame.size().height<<endl;
			float fovHeight = FOV_PIXELS_HEIGHT * T_INCHES_HEIGHT / rectHeight;
			cout<<"Field of view height: "<<fovHeight<<endl;
			float fovWidth = fovHeight * frame.size().width / frame.size().height;
			cout<<"Field of view width: "<<fovWidth<<endl;
			float fovDiagonal = sqrt(pow(fovHeight, 2) + pow(fovWidth, 2));
			cout<<"Field of view diagonal: "<<fovDiagonal<<endl;
			float distanceToTape = fovDiagonal / (2 * tan(theta / 1.15));
			//float distanceToTape = fovDiagonal / (2 * tan(theta));
			cout<<"Diagonal distance to tape: "<<distanceToTape<<endl;
			//float verticalDistanceToTape = (T_INCHES_HEIGHT * fovHeight) / (2 * rectHeight * tan(MEASURED_VERT_FOV));
			float verticalDistanceToTape = fovHeight / (2 * tan(MEASURED_VERT_FOV));
			cout << "Vertical distance to tape: " << verticalDistanceToTape << endl;
		}
		else {
			widthThreshold = DEFAULT_WIDTH_THRESHOLD;
			cout<<"frame width: "<<frame.size().width<<endl;
			cout<<"frame height: "<<frame.size().height<<endl;
			float fovWidth = FOV_PIXELS_WIDTH * T_INCHES_WIDTH / rectWidth;
			cout<<"Field of view width: "<< fovWidth <<endl;
			float fovHeight = fovWidth * frame.size().height / frame.size().width;
			cout<<"Field of view height: "<<fovHeight<<endl;
			float fovDiagonal = sqrt(pow(fovHeight, 2) + pow(fovWidth, 2));
			cout<<"Field of view diagonal: "<<fovDiagonal<<endl;
			float distanceToTape = fovDiagonal / (2 * tan(theta / 1.4));
			cout<<"Distance to tape: "<<distanceToTape<<endl;

			float horizDistanceToTape = (T_INCHES_WIDTH * fovWidth) / (2 * rectWidth * tan(MEASURED_HORIZ_FOV));
			//float horizDistanceToTape = fovWidth / (2 * tan(MEASURED_HORIZ_FOV));
			cout << "Horizontal distance to tape: " << horizDistanceToTape << endl;
			
			fovWidth = FOV_PIXELS_WIDTH * T_INCHES_LEFT_WIDTH / leftCornerDist;
			//float horizDistanceToTapeLeft = (T_INCHES_LEFT_WIDTH * fovWidth) / (2 * rectHeight * tan(MEASURED_HORIZ_FOV));
			float horizDistanceToTapeLeft = fovWidth / (2 * tan(MEASURED_HORIZ_FOV));
			cout << "Horizontal left distance to tape: " << horizDistanceToTapeLeft << endl;

			fovWidth = FOV_PIXELS_WIDTH * T_INCHES_LEFT_WIDTH / rightCornerDist;
			//float horizDistanceToTapeLeft = (T_INCHES_LEFT_WIDTH * fovWidth) / (2 * rectHeight * tan(MEASURED_HORIZ_FOV));
			float horizDistanceToTapeRight = fovWidth / (2 * tan(MEASURED_HORIZ_FOV));
			cout << "Horizontal right distance to tape: " << horizDistanceToTapeRight << endl;

			cout << "Horizontal distance average: " << (horizDistanceToTapeLeft + horizDistanceToTapeRight) / 2 << endl;
		}
	}



	circle(frame, Point(0, 0), 3, Scalar(255, 0, 0), 10);
	imshow("camera feed", frame);
        imshow("filtered green", green);
	//imshow("blurred image", outline);

#if 1
        // char c = waitKey(30);
        char c = waitKey(1);

        if (c == ' ')
            break;
#else
        char c = waitKey(1);
#endif
        frames++;
    }

    cap.release();

    auto end = chrono::high_resolution_clock::now();
    auto msec_duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    float fps = (frames / msec_duration.count()) * 1000;

    cout << "msec_duration: " << msec_duration.count() << endl;
    cout << "frames: " << frames << " FPS: " << fps << endl;

	for (int i=0; i< contours.size(); i++)
	{
		cout << "contour: " << i << ":"<<contours[i]<<endl;
	}
}
