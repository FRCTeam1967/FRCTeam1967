#include <chrono>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/photo/photo.hpp>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <cstdlib>

using namespace std;

const int MIN_AREA =900; //pixels
const float T_FEET_HEIGHT = 16;
const float T_FEET_WIDTH = 2;
const int FOV_PIXELS_HEIGHT = 480;
const int FOV_PIXELS_WIDTH = 640;
const float theta = 68.5 * 3.141592653589793238462643383279 / 360; //degrees

int main()
{
    //system("v4l2-ctl -d /dev/video1 --set-fmt-video=width=1280,height=720,pixelformat=BGR3");
//    SetResolution(1280, 720);
//    system("ffprobe -f v4l2 -input_format mjpeg -video_size 1280x720 /dev/video1");
    //system("v4l2-ctl -d /dev/video1 -c exposure_auto=1 -c exposure_absolute=1 -c brightness=30");
	system("v4l2-ctl -d /dev/video1 -c exposure_auto=1 -c exposure_absolute=10 -c brightness=50");
    cv::VideoCapture cap(1);

    if (!cap.isOpened())
	{
            cerr << "Failed to open USB camera" << endl;
            return -1;
	}

    auto start = chrono::high_resolution_clock::now();
    float frames = 0;
	//vector<vector<cv::Point>> contours;
	vector<cv::Point> maxContour;
	vector<vector<cv::Point>> contours;

	//vector<Vec4i> lines;
	double hue[] = {55,85};
	double sat[] = {108,255};
	double val[] = {46,151};

	//double hue[] = {0,180};
	//double sat[] = {0,255};
	//double val[] = {222,255};

     for(;;)
     {
	cv::Mat gray, frame, green, outline;

        cap >> frame; 
//	cout << "width get " << cap.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
//	cout << "height: " << frame.size().height << endl;
//	cout << "width: " << frame.size().width << endl;
	imwrite("/home/nvidia/master-ocv/cv-capture/TapeButton.jpg", frame);
	// convert from brg to hsv
	cv::cvtColor(frame, green, cv::COLOR_BGR2HSV);
	cv::inRange(green,cv::Scalar(hue[0], sat[0], val[0]), cv::Scalar(hue[1], sat[1], val[1]), green); // filter green tape
	cv::GaussianBlur(green, outline, cv::Size(9, 9), 2, 2); // blurs image
	cv::findContours(outline, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0)); // finds contourss
	vector<vector<cv::Point>> contours_poly(contours.size());
	vector<cv::Rect> boundRect(contours.size());
	int largestContour = 0;

	// loops through each contour - does more processing if contour area is greater than MIN_AREA
	for(int c = 0; c < contours.size(); c++) {
		//double currentArea = cv::contourArea(contours[c]);

		if(cv::contourArea(contours[c]) < MIN_AREA) 
		{
//			cout << "skipped" << c <<endl;
			continue;
		}
		// draws contours of random colors
		cv::Scalar color = cv::Scalar((rand()%255)+1, (rand()%255)+1, (rand()%255)+1);
		cv::drawContours(frame, contours, c, color, 4);

		// finds polygons
		cv::approxPolyDP(cv::Mat(contours[c]), contours_poly[c], 10, true);
		for (int i=0; i<contours_poly[c].size(); i++)
		{
			cv::Scalar red = cv::Scalar(255, 0, 0);
			cv::circle(frame, contours_poly[c][i], 3, red, 10);
			cv::Point p = contours_poly[c][i];
			cv::putText(frame, cv::format("(%d, %d)", p.x, p.y), p, cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0, 0, 255));
		}

		boundRect[c] = cv::boundingRect(cv::Mat(contours_poly[c]));
		cv::rectangle(frame, boundRect[c].tl(), boundRect[c].br(), color);

		if(cv::contourArea(contours[c]) > cv::contourArea(contours[largestContour]))
		{
			largestContour = c;
		}
	}
	
	if (contours_poly.size() > 0) {
		cout<<"area of largest contour: - "<<cv::contourArea(contours[largestContour])<<endl;
		cout<<contours_poly[largestContour]<<endl;
		cout<<"Largest rectangle height: "<<boundRect[largestContour].height <<endl;

		// parabolic formula distance
		int rectHeight = boundRect[largestContour].height;
		int rectWidth = boundRect[largestContour].width;
		//float distance = 220-(1.23*rectHeight)+(0.00208*pow(rectHeight, 2));
		cout << "rect height: " << rectHeight << endl;
		cout << "rect width: " << rectWidth << endl;
		//float distance = 220-(​1.23*​rectHeight)+(​0.00208*​pow(rectHeight, 2));
		//cout << "parabolic distance: " << distance << endl;

		if(rectWidth < 33) {
			cout<<"frame width: "<<frame.size().width<<endl;
			cout<<"frame height: "<<frame.size().height<<endl;
			float fovHeight = FOV_PIXELS_HEIGHT * T_FEET_HEIGHT / rectHeight;
			cout<<"Field of view height: "<<fovHeight<<endl;
			float fovWidth = fovHeight * frame.size().width / frame.size().height;
			cout<<"Field of view width: "<<fovWidth<<endl;
			float fovDiagonal = sqrt(pow(fovHeight, 2) + pow(fovWidth, 2));
			cout<<"Field of view diagonal: "<<fovDiagonal<<endl;
			float distanceToTape = fovDiagonal / (2 * tan(theta / 1.15));
			//float distanceToTape = fovDiagonal / (2 * tan(theta));
			cout<<"Distance to tape: "<<distanceToTape<<endl;
		}
		else {
			cout<<"frame width: "<<frame.size().width<<endl;
			cout<<"frame height: "<<frame.size().height<<endl;
			float fovWidth = FOV_PIXELS_WIDTH * T_FEET_WIDTH / rectWidth;
			cout<<"Field of view width: "<< fovWidth <<endl;
			float fovHeight = fovWidth * frame.size().height / frame.size().width;
			cout<<"Field of view height: "<<fovHeight<<endl;
			float fovDiagonal = sqrt(pow(fovHeight, 2) + pow(fovWidth, 2));
			cout<<"Field of view diagonal: "<<fovDiagonal<<endl;
			float distanceToTape = fovDiagonal / (2 * tan(theta / 1.4));
			cout<<"Distance to tape: "<<distanceToTape<<endl;
		}
	}
	// new frame from camera
        // cv::Sobel(frme, gray, CV_8U, 1, 1);
        //cv::cvtColor(frame, gray, cv::COLOR_RGB2GRAY);
	//cv::Canny(gray, frame, 35, 125);
        // cv::fastNlMeansDenoising(gray, frame);

        // cv::fastNlMeansDenoising(gray, frame);
	//cv::findContours(green, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
	//cv::max(contours, contours, maxContour);
	
	//return cv::minAreaRect(contours);

        // cv::imshow("camera feed", gray);
	cv::circle(frame, cv::Point(0, 0), 3, cv::Scalar(255, 0, 0), 10);
	cv::imshow("camera feed", frame);
        cv::imshow("filtered green", green);
	//cv::imshow("blurred image", outline);

#if 1
        // char c = cv::waitKey(30);
        char c = cv::waitKey(1);

        if (c == ' ')
            break;
#else
        char c = cv::waitKey(1);
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
