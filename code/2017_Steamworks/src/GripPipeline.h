#pragma once
#include "vision/VisionPipeline.h"
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/features2d.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <vector>
#include <string>
#include <math.h>
#include "WPILib.h"

namespace grip
{

class Line
{
public:
	double x1, y1, x2, y2;

	Line(double x1, double y1, double x2, double y2)
	{
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}

	double length()
	{
		return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	}


	double angle()
	{
		return (180*atan2(y2 - y1, x2 - x1)/CV_PI);
	}

	double xVal1()
	{
		return x1;
	}

	double yVal1()
	{
		return y1;
	}

	double xVal2()
	{
		return x2;
	}

	double yVal2()
	{
		return y2;
	}

};

class GripPipeline : public VisionPipeline {
private:
	int difference;
	double distance;
	int xMax;
	int xMin;
	int peg;
	int offset;
	double offsetInches;
	double angle;
	cv::Mat cvResizeOutput;
	cv::Mat hsvThresholdOutput;
	cv::Mat cvErodeOutput;
	std::vector<Line> findLinesOutput;
	std::vector<std::vector<cv::Point> > findContoursOutput;
	std::vector<std::vector<cv::Point> > filterContoursOutput;
	void cvResize(cv::Mat &, cv::Size &, double , double , int , cv::Mat &);
	void hsvThreshold(cv::Mat &, double [], double [], double [], cv::Mat &);
	void findContours(cv::Mat &, bool , std::vector<std::vector<cv::Point> > &);
	void filterContours(std::vector<std::vector<cv::Point> > &, double , double , double , double , double , double , double [], double , double , double , double , std::vector<std::vector<cv::Point> > &);
	void cvErode(cv::Mat &, cv::Mat &, cv::Point &, double , int , cv::Scalar &, cv::Mat &);
	void findLines(cv::Mat &, std::vector<Line> &);
	double findCenter(cv::Mat &input, std::vector<Line> &lineList);
	double findDistance(cv::Mat &input, std::vector<Line> &lineList, int difference);

	// void printLines(cv::Mat &input, std::vector<Line> &lineList);

public:
	GripPipeline();
	void Process(cv::Mat & source0);
	cv::Mat* getcvResizeOutput();
	cv::Mat* gethsvThresholdOutput();
	cv::Mat* getcvErodeOutput();
	std::vector<Line>* getfindLinesOutput();
	std::vector<std::vector<cv::Point> >* getfindContoursOutput();
	std::vector<std::vector<cv::Point> >* getfilterContoursOutput();
	double getDistance();
	int getOffset();

};

}
