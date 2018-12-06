#include "GripPipeline.h"
#include "WPILib.h"
#include "math.h"
#define LSD_REFINE_STD 1
#define X_RESOLUTION 320
#define	Y_RESOLUTION 240

namespace grip {

GripPipeline::GripPipeline() {
}

void GripPipeline::Process(cv::Mat & source0) {

	// Step 1: Resize the mat, or image, taken from the camera

    cv::Mat cvResizeSrc = source0;
    cv::Size cvResizeDsize(0, 0);
    double cvResizeFx = 0.75;  // default Double
    double cvResizeFy = 0.75;  // default Double
    int cvResizeInterpolation = cv::INTER_LINEAR;
    cvResize(cvResizeSrc, cvResizeDsize, cvResizeFx, cvResizeFy, cvResizeInterpolation, this->cvResizeOutput);

    // Step 2: Adjust the Hue, Saturation, and Value of the image
    cv::Mat hsvThresholdInput = cvResizeOutput;
    //source0= hsvThresholdInput;
    double hsvThresholdHue[] = {69, 180};
    //hue ranges from 0-180
    double hsvThresholdSaturation[] = {172, 255.0};
    //saturation ranges from 0-255
    double hsvThresholdValue[] = {112, 255.0};
    //value ranges from 0-255
    hsvThreshold(hsvThresholdInput, hsvThresholdHue, hsvThresholdSaturation, hsvThresholdValue, this->hsvThresholdOutput);

   // Find the contours (we deleted this step)
    cv::Mat findContoursInput = hsvThresholdOutput;
    source0= hsvThresholdOutput;
    //source0= hsvThresholdOutput; //for test purposes
    //bool findContoursExternalOnly = true;  // default Boolean
    // findContours(findContoursInput, findContoursExternalOnly, this->findContoursOutput);
    //Step Filter_Contours0:
    //input

    // Step 3: Erode the image to take out the excess noise in the photo.
    cv::Mat cvErodeSrc = hsvThresholdOutput;
    cv::Mat cvErodeKernel;
    cv::Point cvErodeAnchor(-1, -1);
    double cvErodeIterations = 1;  // default Double
    int cvErodeBordertype = cv::BORDER_CONSTANT;
    cv::Scalar cvErodeBordervalue(-1);
    cvErode(cvErodeSrc, cvErodeKernel, cvErodeAnchor, cvErodeIterations, cvErodeBordertype, cvErodeBordervalue, this->cvErodeOutput);

    // Step 4: Find the lines of the reflective tape.
    cv::Mat findLinesInput = cvErodeOutput;
    findLines(findLinesInput, this->findLinesOutput);

    std::vector<std::vector<cv::Point> > filterContoursContours = findContoursOutput;

/*    double filterContoursMinArea = 500.0;  // default Double
    double filterContoursMinPerimeter = 0.0;  // default Double
    double filterContoursMinWidth = 0.0;  // default Double
    double filterContoursMaxWidth = 1000.0;  // default Double
    double filterContoursMinHeight = 0.0;  // default Double
    double filterContoursMaxHeight = 1000.0;  // default Double
    double filterContoursSolidity[] = {0, 100};
    double filterContoursMaxVertices = 1000000.0;  // default Double
    double filterContoursMinVertices = 0.0;  // default Double
    double filterContoursMinRatio = 0.0;  // default Double
    double filterContoursMaxRatio = 1000.0;  // default Double
    filterContours(filterContoursContours, filterContoursMinArea, filterContoursMinPerimeter, filterContoursMinWidth, filterContoursMaxWidth, filterContoursMinHeight, filterContoursMaxHeight, filterContoursSolidity, filterContoursMaxVertices, filterContoursMinVertices, filterContoursMinRatio, filterContoursMaxRatio, this->filterContoursOutput);
    */

    GripPipeline::printLines(source0,findLinesOutput);
    GripPipeline::findCenter(source0,findLinesOutput);

    // GripPipeline::findDistance(source0,findLinesOutput,difference);


}

cv::Mat* GripPipeline::getcvResizeOutput()
{
    return &(this->cvResizeOutput);
}

cv::Mat* GripPipeline::gethsvThresholdOutput()
{
    return &(this->hsvThresholdOutput);
}

cv::Mat* GripPipeline::getcvErodeOutput()
{
	return &(this->cvErodeOutput);
}

std::vector<Line>* GripPipeline::getfindLinesOutput()
{
	return &(this->findLinesOutput);
}

std::vector<std::vector<cv::Point> >* GripPipeline::getfindContoursOutput()
{
    return &(this->findContoursOutput);
}

std::vector<std::vector<cv::Point> >* GripPipeline::getfilterContoursOutput()
{

    //for(cv::Point contourPt : filterContoursOutput)//nested for loop (add for 2D vector)
        //filterContoursOutput.size();

    SmartDashboard::PutNumber("contourvectorsize",filterContoursOutput.size());
    return &(this->filterContoursOutput);
}

    void GripPipeline::cvResize(cv::Mat &src, cv::Size &dSize, double fx, double fy, int interpolation, cv::Mat &dst)
    {
        cv::resize(src, dst, dSize, fx, fy, interpolation);
    }

    void GripPipeline::hsvThreshold(cv::Mat &input, double hue[], double sat[], double val[], cv::Mat &out)
    {
        cv::cvtColor(input, out, cv::COLOR_BGR2HSV);
        cv::inRange(out,cv::Scalar(hue[0], sat[0], val[0]), cv::Scalar(hue[1], sat[1], val[1]), out);
    }

    void GripPipeline::cvErode(cv::Mat &src, cv::Mat &kernel, cv::Point &anchor, double iterations, int borderType, cv::Scalar &borderValue, cv::Mat &dst)
    	{
    		cv::erode(src, dst, kernel, anchor, (int)iterations, borderType, borderValue);
    	}

    void GripPipeline::findLines(cv::Mat &input, std::vector<Line> &lineList)
    {
    	cv::Ptr<cv::LineSegmentDetector> lsd = cv::createLineSegmentDetector(LSD_REFINE_STD);
    	std::vector<cv::Vec4i> lines;
    	lineList.clear();
    		if (input.channels() == 1)
    		{
    			lsd->detect(input, lines);
    		} else
    		{
    			// The line detector works on a single channel.
    			cv::Mat tmp;
    			cv::cvtColor(input, tmp, cv::COLOR_BGR2GRAY);
    			lsd->detect(tmp, lines);
    		}
    		// Store the lines in the LinesReport object
    		if (!lines.empty())
    		{
    			for (unsigned int i = 0; i < lines.size(); i++)
    			{
    				cv::Vec4i line = lines[i];
    				lineList.push_back(Line(line[0], line[1], line[2], line[3]));
    			}
    		}
    	}

    void GripPipeline::printLines(cv::Mat &input, std::vector<Line> &lineList)
            {
            	for (unsigned int pos = 0; pos < lineList.size(); pos++)
            	{
            		//printf("Line, %d, :, %f, %f \n", pos, lineList[pos].xVal1(), lineList[pos].yVal1());
            		//printf("Line, %d, :, %f, %f \n \n", pos, lineList[pos].xVal2(), lineList[pos].yVal2());
            	}
            }
    void GripPipeline::findCenter(cv::Mat &input, std::vector<Line> &lineList)
        		{
    				for (unsigned int i = 0; i < 50; i++){
    					printf("In the delayed loop \n\n");
    				}
//    				printf("Line List %d/n/n", lineList);
    				printf("Line List Size %d\n",lineList.size());
        			xMax = 0;
        			xMin = 320;
        			for (unsigned int pos = 0; pos < lineList.size(); pos++)
        			{
        				if (xMax < lineList[pos].xVal1())
        				{
        					xMax = lineList[pos].xVal1();
        				}
        				if (xMax < lineList[pos].xVal2())
        				{
        				    xMax = lineList[pos].xVal2();
        				}
        				if (xMin > lineList[pos].xVal1())
        				{
        				    xMin = lineList[pos].xVal1();
        				}
        				if (xMin > lineList[pos].xVal2())
        				{
        				    xMin = lineList[pos].xVal2();
        				}
        				//int pegPosition = difference/2 + xMin; //This is the exact x value of the peg.
        				//int offset = X_RESOLUTION - pegPosition; // this is the distance between the peg and the center of the image
        				difference = (xMax - xMin); //distance between the greatest and least X values
        				//printf("Xmin, %d, Xmax %d \n", xMin, xMax);
        				//printf("Difference, %d \n\n", difference);
        				//printf("Offset, %d \n\n", offset);
        				//printf("Xmin, %d, Xmax %d \n", xMin, xMax);
        				distance = (((-2.9041*( pow(10, -5)))*(pow(difference, 3))+(0.0127)*(pow(difference,2))-(1.9164)*difference+118.601) + 3);
        				//int distance = difference*0.3;
        				//printf("Distance, %d \n\n", distance);
        			}

        }

// double GripPipeline::findDistance(cv::Mat &input, std::vector<Line> &lineList, int difference) {
//
//    	// y = (-2.9041E^-5)x^3+(0.0127)x^2-(1.9164)x+118.601
//    	int distance = ((-2.9041*(10^-5))*(difference^3)+(0.0127)*(difference^2)-(1.9164)*difference+118.601);
//    	printf("Distance, %d /n/n", distance);
//    	return distance;
//    }

}
