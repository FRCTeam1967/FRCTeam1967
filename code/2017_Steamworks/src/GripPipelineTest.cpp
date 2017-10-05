#include "GripPipelineTest.h"
#include "WPILib.h"

#define X_RESOLUTION 320
#define Y_RESOLUTION 240

namespace grip {

GripPipelineTest::GripPipelineTest() {
}
/**
* Runs an iteration of the pipeline and updates outputs.
*/
void GripPipelineTest::Process(cv::Mat& source0){
	//Step HSV_Threshold0:
	//input
	cv::Mat hsvThresholdInput = source0;
	double hsvThresholdHue[] = {67.98561151079137, 79.15110356536502};
	double hsvThresholdSaturation[] = {255.0, 255.0};
	double hsvThresholdValue[] = {50.44964028776978, 151.0950764006791};
	hsvThreshold(hsvThresholdInput, hsvThresholdHue, hsvThresholdSaturation, hsvThresholdValue, this->hsvThresholdOutput);
	//Step Find_Contours0:
	//input
	cv::Mat findContoursInput = hsvThresholdOutput;
	bool findContoursExternalOnly = false;  // default Boolean
	findContours(findContoursInput, findContoursExternalOnly, this->findContoursOutput);
	//Step Filter_Contours0:
	//input
	std::vector<std::vector<cv::Point> > filterContoursContours = findContoursOutput;
	double filterContoursMinArea = 0.0;  // default Double
	double filterContoursMinPerimeter = 0.0;  // default Double
	double filterContoursMinWidth = 0.0;  // default Double
	double filterContoursMaxWidth = 1000.0;  // default Double
	double filterContoursMinHeight = 0.0;  // default Double
	double filterContoursMaxHeight = 1000.0;  // default Double
	double filterContoursSolidity[] = {0.0, 100};
	double filterContoursMaxVertices = 1000000.0;  // default Double
	double filterContoursMinVertices = 0.0;  // default Double
	double filterContoursMinRatio = 0.2;  // default Double
	double filterContoursMaxRatio = 0.8;  // default Double
	filterContours(filterContoursContours, filterContoursMinArea, filterContoursMinPerimeter, filterContoursMinWidth, filterContoursMaxWidth, filterContoursMinHeight, filterContoursMaxHeight, filterContoursSolidity, filterContoursMaxVertices, filterContoursMinVertices, filterContoursMinRatio, filterContoursMaxRatio, this->filterContoursOutput);
}

/**
 * This method is a generated getter for the output of a HSV_Threshold.
 * @return Mat output from HSV_Threshold.
 */
cv::Mat* GripPipelineTest::GetHsvThresholdOutput(){
	return &(this->hsvThresholdOutput);
}
/**
 * This method is a generated getter for the output of a Find_Contours.
 * @return ContoursReport output from Find_Contours.
 */
std::vector<std::vector<cv::Point> >* GripPipelineTest::GetFindContoursOutput(){
	return &(this->findContoursOutput);
}
/**
 * This method is a generated getter for the output of a Filter_Contours.
 * @return ContoursReport output from Filter_Contours.
 */
std::vector<std::vector<cv::Point> >* GripPipelineTest::GetFilterContoursOutput(){
	return &(this->filterContoursOutput);
}
	/**
	 * Segment an image based on hue, saturation, and value ranges.
	 *
	 * @param input The image on which to perform the HSL threshold.
	 * @param hue The min and max hue.
	 * @param sat The min and max saturation.
	 * @param val The min and max value.
	 * @param output The image in which to store the output.
	 */
	void GripPipelineTest::hsvThreshold(cv::Mat &input, double hue[], double sat[], double val[], cv::Mat &out) {
		cv::cvtColor(input, out, cv::COLOR_BGR2HSV);
		cv::inRange(out,cv::Scalar(hue[0], sat[0], val[0]), cv::Scalar(hue[1], sat[1], val[1]), out);
	}

	/**
	 * Finds contours in an image.
	 *
	 * @param input The image to find contours in.
	 * @param externalOnly if only external contours are to be found.
	 * @param contours vector of contours to put contours in.
	 */
	void GripPipelineTest::findContours(cv::Mat &input, bool externalOnly, std::vector<std::vector<cv::Point> > &contours) {
		std::vector<cv::Vec4i> hierarchy;
		contours.clear();
		int mode = externalOnly ? cv::RETR_EXTERNAL : cv::RETR_LIST;
		int method = cv::CHAIN_APPROX_SIMPLE;
		cv::findContours(input, contours, hierarchy, mode, method);
	}


	/**
	 * Filters through contours.
	 * @param inputContours is the input vector of contours.
	 * @param minArea is the minimum area of a contour that will be kept.
	 * @param minPerimeter is the minimum perimeter of a contour that will be kept.
	 * @param minWidth minimum width of a contour.
	 * @param maxWidth maximum width.
	 * @param minHeight minimum height.
	 * @param maxHeight  maximimum height.
	 * @param solidity the minimum and maximum solidity of a contour.
	 * @param minVertexCount minimum vertex Count of the contours.
	 * @param maxVertexCount maximum vertex Count.
	 * @param minRatio minimum ratio of width to height.
	 * @param maxRatio maximum ratio of width to height.
	 * @param output vector of filtered contours.
	 */
	void GripPipelineTest::filterContours(std::vector<std::vector<cv::Point> > &inputContours, double minArea, double minPerimeter, double minWidth, double maxWidth, double minHeight, double maxHeight, double solidity[], double maxVertexCount, double minVertexCount, double minRatio, double maxRatio, std::vector<std::vector<cv::Point> > &output) {
		std::vector<cv::Point> hull;
		output.clear();
		for (std::vector<cv::Point> contour: inputContours) {
			cv::Rect bb = boundingRect(contour);
			if (bb.width < minWidth || bb.width > maxWidth) continue;
			if (bb.height < minHeight || bb.height > maxHeight) continue;
			double area = cv::contourArea(contour);
			if (area < minArea) continue;
			if (arcLength(contour, true) < minPerimeter) continue;
			cv::convexHull(cv::Mat(contour, true), hull);
			double solid = 100 * area / cv::contourArea(hull);
			if (solid < solidity[0] || solid > solidity[1]) continue;
			if (contour.size() < minVertexCount || contour.size() > maxVertexCount)	continue;
			double ratio = (double) bb.width / (double) bb.height;
			if (ratio < minRatio || ratio > maxRatio) continue;
			output.push_back(contour);
		}
	}


//	void GripPipeline::findCenter(cv::Mat &input, std::vector<Line> &lineList)
//	{
//		xMax = 0;
//		xMin = 320;
//
//		printf("lineList.size %d \n",lineList.size());
//
//		for (unsigned int pos = 0; pos < lineList.size(); pos++)
//		{
//			if (xMax < lineList[pos].xVal1())
//				xMax = lineList[pos].xVal1();
//			if (xMax < lineList[pos].xVal2())
//				xMax = lineList[pos].xVal2();
//			if (xMin > lineList[pos].xVal1())
//				xMin = lineList[pos].xVal1();
//			if (xMin > lineList[pos].xVal2())
//				xMin = lineList[pos].xVal2();
//		}
//
//		difference =  xMax-xMin;
//		if (difference < X_RESOLUTION)
//		{
//			peg = (xMax - xMin)/2 + xMin; // the x value of the peg
//			localOffset = ((X_RESOLUTION/2) - peg); // the difference in x between the peg and the center of the screen
//			if (abs(localOffset) < X_RESOLUTION / 2)
//			{
//				offset = localOffset;
//				offsetInches = offset * (10.375/difference); // finding the offset in inches
//				angle = ((atan(offsetInches/distance))*180)/(3.14); // calculates the angle the robot needs to drive (PID input)
//				SmartDashboard::PutNumber("xMin", xMin);
//				SmartDashboard::PutNumber("xMax", xMax);
//				SmartDashboard::PutNumber("Difference", difference);
//				SmartDashboard::PutNumber("Angle", angle);
//				printf ("xMin %d \n", xMin);
//				printf ("xMax %d \n", xMax);
//				printf("Difference: %d   ", difference);
//				printf ("    Angle %f      ", angle);
//				printf("Distance: %f /n", distance);
//			}
//			else
//				badOffset ++;
//		}
//		else
//			badDifference ++;
//
//	}

} // end grip namespace

