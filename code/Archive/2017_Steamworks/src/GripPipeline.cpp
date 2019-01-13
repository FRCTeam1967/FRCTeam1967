#include "GripPipeline.h"
#include "Math.h"
#define LSD_REFINE_STD 1
#define X_RESOLUTION 320
#define Y_RESOLUTION 240

namespace grip
{

GripPipeline::GripPipeline()
{
	offset=0;
	distance=0;
	badOffset = 0;
	badDifference = 0;

}

void GripPipeline::Process(cv::Mat & source0)
{
	cv::Mat cvResizeSrc = source0;
	cv::Size cvResizeDsize(0, 0);
	double cvResizeFx = 0.75;  // default Double
	double cvResizeFy = 0.75;  // default Double
	int cvResizeInterpolation = cv::INTER_LINEAR;
	cvResize(cvResizeSrc, cvResizeDsize, cvResizeFx, cvResizeFy, cvResizeInterpolation, this->cvResizeOutput);

	cv::Mat hsvThresholdInput = cvResizeOutput;

//	double hsvThresholdHue[] = {69,180};
//	double hsvThresholdSaturation[] = {172,255};
//	double hsvThresholdValue[] = {112,255};
	double hsvThresholdHue[] = {71,91};
	double hsvThresholdSaturation[] = {110,255};
	double hsvThresholdValue[] = {80,255};

	hsvThreshold(hsvThresholdInput, hsvThresholdHue, hsvThresholdSaturation, hsvThresholdValue, this->hsvThresholdOutput);

	cv::Mat findContoursInput = hsvThresholdOutput;
	source0= hsvThresholdOutput;

	std::vector<std::vector<cv::Point> > filterContoursContours = findContoursOutput;

	cv::Mat cvErodeSrc = hsvThresholdOutput;
	cv::Mat cvErodeKernel;
	cv::Point cvErodeAnchor(-1, -1);
	double cvErodeIterations = 4;
	int cvErodeBordertype = cv::BORDER_CONSTANT;
	cv::Scalar cvErodeBordervalue(-1);
	cvErode(cvErodeSrc, cvErodeKernel, cvErodeAnchor, cvErodeIterations, cvErodeBordertype, cvErodeBordervalue, this->cvErodeOutput);

	cv::Mat findLinesInput = cvErodeOutput;
	findLines(findLinesInput, this->findLinesOutput);

	//	print the lines
	//	GripPipeline::printLines(source0, findLinesOutput);

	// find center
	GripPipeline::findCenter(source0, findLinesOutput);

	// find distance
	GripPipeline::findDistance(source0, findLinesOutput, difference);
}

// output of resize
cv::Mat* GripPipeline::getcvResizeOutput()
{
	return &(this->cvResizeOutput);
}

// output of threshold
cv::Mat* GripPipeline::gethsvThresholdOutput()
{
	return &(this->hsvThresholdOutput);
}

// output of erode
cv::Mat* GripPipeline::getcvErodeOutput()
{
	return &(this->cvErodeOutput);
}

// output of finding lines
std::vector<Line>* GripPipeline::getfindLinesOutput()
{
	return &(this->findLinesOutput);
}

// output of find contours
std::vector<std::vector<cv::Point> >* GripPipeline::getfindContoursOutput()
{
	return &(this->findContoursOutput);
}

//output of filter contours
std::vector<std::vector<cv::Point> >* GripPipeline::getfilterContoursOutput()
{
	return &(this->filterContoursOutput);
}

//RESIZE IMAGE
void GripPipeline::cvResize(cv::Mat &src, cv::Size &dSize, double fx, double fy, int interpolation, cv::Mat &dst)
{
	cv::resize(src, dst, dSize, fx, fy, interpolation);
}

//THRESHOLD
void GripPipeline::hsvThreshold(cv::Mat &input, double hue[], double sat[], double val[], cv::Mat &out)
{
	cv::cvtColor(input, out, cv::COLOR_BGR2HSV);
	cv::inRange(out,cv::Scalar(hue[0], sat[0], val[0]), cv::Scalar(hue[1], sat[1], val[1]), out);
}

// ERODE
void GripPipeline::cvErode(cv::Mat &src, cv::Mat &kernel, cv::Point &anchor, double iterations, int borderType, cv::Scalar &borderValue, cv::Mat &dst)
{
	cv::erode(src, dst, kernel, anchor, (int)iterations, borderType, borderValue);
}

// FINDS LINES
void GripPipeline::findLines(cv::Mat &input, std::vector<Line> &lineList)
{
	cv::Ptr<cv::LineSegmentDetector> lsd = cv::createLineSegmentDetector(LSD_REFINE_STD);
	std::vector<cv::Vec4i> lines;
	lineList.clear();
	if (input.channels() == 1)
		lsd->detect(input, lines);
	else
	{
		cv::Mat tmp;
		cv::cvtColor(input, tmp, cv::COLOR_BGR2GRAY);
		lsd->detect(tmp, lines);
	}
	if (!lines.empty())
	{
		for (unsigned int i = 0; i < lines.size(); i++)
		{
			cv::Vec4i line = lines[i];
			lineList.push_back(Line(line[0], line[1], line[2], line[3]));
		}
	}
}

//    	 PRINT LINES
//        void GripPipeline::printLines(cv::Mat &input, std::vector<Line> &lineList)
//        {
//        	for (unsigned int pos = 0; pos < lineList.size(); pos++)
//        	{
//        		//printf("Line, %d, x1, y1, x2, y2:, %f, %f, %f, %f \n", pos, lineList[pos].xVal1(), lineList[pos].yVal1(), lineList[pos].xVal2(), lineList[pos].yVal2());
//        	}
//        }

// FIND CENTER
void GripPipeline::findCenter(cv::Mat &input, std::vector<Line> &lineList)
{
	xMax = 0;
	xMin = 320;

	//printf("lineList.size %d \n",lineList.size());

	for (unsigned int pos = 0; pos < lineList.size(); pos++)
	{
		if (xMax < lineList[pos].xVal1())
			xMax = lineList[pos].xVal1();
		if (xMax < lineList[pos].xVal2())
			xMax = lineList[pos].xVal2();
		if (xMin > lineList[pos].xVal1())
			xMin = lineList[pos].xVal1();
		if (xMin > lineList[pos].xVal2())
			xMin = lineList[pos].xVal2();
	}

	difference =  xMax-xMin;
	if (difference < X_RESOLUTION)
	{
		peg = (xMax - xMin)/2 + xMin; // the x value of the peg
		localOffset = ((X_RESOLUTION/2) - peg); // the difference in x between the peg and the center of the screen
		if (abs(localOffset) < X_RESOLUTION / 2)
		{
			offset = localOffset;
			offsetInches = offset * (10.375/difference); // finding the offset in inches
			angle = ((atan(offsetInches/distance))*180)/(3.14); // calculates the angle the robot needs to drive (PID input)
			//	printf ("xMin %d \n", xMin);
			//	printf ("xMax %d \n", xMax);
			//	printf("Difference: %d   ", difference);
			//	printf ("    Angle %f      ", angle);
		}
		else
			badOffset ++;
	}
	else
		badDifference ++;

}

// FIND DISTANCE
double GripPipeline::findDistance(cv::Mat &input, std::vector<Line> &lineList, int difference)
{
	distance = ( -5.5355*pow(10,-5)*pow(difference,3) + 0.0199*pow(difference,2) - 2.5918*(difference) + 137.8207 );
	//printf ("    Distance: %d \n", distance);
	return distance;
}

int GripPipeline::getBadDifference()
{
	return badDifference;
}

int GripPipeline::getBadOffset()
{
	return badOffset;
}
double GripPipeline::getDistance() {
	return distance;
	//TODO create a running/geometric average
}
int GripPipeline::getOffset(){
	return offset;
	//TODO create a running/geometric average
}


}
