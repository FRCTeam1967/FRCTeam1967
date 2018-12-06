#include "GripPipeline.h"
#include "WPILib.h"

/**
* Initializes a GripPipeline.
*/

namespace grip {

GripPipeline::GripPipeline() {
}
/**
* Runs an iteration of the Pipeline and updates outputs.
*
* Sources need to be set before calling this method. 
*
*/
void GripPipeline::process(cv::Mat source0){
	//Step CV_resize0:
	//input
	cv::Mat cvResizeSrc = source0;
	cv::Size cvResizeDsize(0, 0);
	double cvResizeFx = 0.25;  // default Double
	double cvResizeFy = 0.25;  // default Double
    int cvResizeInterpolation = cv::INTER_LINEAR;
	cvResize(cvResizeSrc, cvResizeDsize, cvResizeFx, cvResizeFy, cvResizeInterpolation, this->cvResizeOutput);
	//Step HSV_Threshold0:
	//input
	cv::Mat hsvThresholdInput = cvResizeOutput;
	double hsvThresholdHue[] = {42.086330935251794, 86.7911714770798};
	double hsvThresholdSaturation[] = {32.10431654676259, 207.37691001697794};
	double hsvThresholdValue[] = {91.72661870503596, 255.0};
	hsvThreshold(hsvThresholdInput, hsvThresholdHue, hsvThresholdSaturation, hsvThresholdValue, this->hsvThresholdOutput);
	//Step CV_erode0:
	//input
	cv::Mat cvErodeSrc = hsvThresholdOutput;
	cv::Mat cvErodeKernel;
	cv::Point cvErodeAnchor(-1, -1);
	double cvErodeIterations = 1;  // default Double
    int cvErodeBordertype = cv::BORDER_CONSTANT;
	cv::Scalar cvErodeBordervalue(-1);
	cvErode(cvErodeSrc, cvErodeKernel, cvErodeAnchor, cvErodeIterations, cvErodeBordertype, cvErodeBordervalue, this->cvErodeOutput);
	//Step Mask0:
	//input
	cv::Mat maskInput = cvResizeOutput;
	cv::Mat maskMask = cvErodeOutput;
	mask(maskInput, maskMask, this->maskOutput);
	//Step Find_Blobs0:
	//input
	cv::Mat findBlobsInput = maskOutput;
	double findBlobsMinArea = 0.0;  // default Double
	double findBlobsCircularity[] = {0.0, 1.0};
	bool findBlobsDarkBlobs = true;  // default Boolean
	findBlobs(findBlobsInput, findBlobsMinArea, findBlobsCircularity, findBlobsDarkBlobs, this->findBlobsOutput);
}

/**
 * This method is a generated setter for source0.
 * @param source the Mat to set
 */
void GripPipeline::setsource0(cv::Mat &source0){
	source0.copyTo(this->source0);
}
/**
 * This method is a generated getter for the output of a CV_resize.
 * @return Mat output from CV_resize.
 */
cv::Mat* GripPipeline::getcvResizeOutput(){
	return &(this->cvResizeOutput);
}
/**
 * This method is a generated getter for the output of a HSV_Threshold.
 * @return Mat output from HSV_Threshold.
 */
cv::Mat* GripPipeline::gethsvThresholdOutput(){
	return &(this->hsvThresholdOutput);
}
/**
 * This method is a generated getter for the output of a CV_erode.
 * @return Mat output from CV_erode.
 */
cv::Mat* GripPipeline::getcvErodeOutput(){
	return &(this->cvErodeOutput);
}
/**
 * This method is a generated getter for the output of a Mask.
 * @return Mat output from Mask.
 */
cv::Mat* GripPipeline::getmaskOutput(){
	return &(this->maskOutput);
}
/**
 * This method is a generated getter for the output of a Find_Blobs.
 * @return BlobsReport output from Find_Blobs.
 */
std::vector<cv::KeyPoint>* GripPipeline::getfindBlobsOutput(){
	return &(this->findBlobsOutput);
}
	/**
	 * Resizes an Image.
	 * @param src The image to resize.
	 * @param dSize size to set the image.
	 * @param fx scale factor along X axis.
	 * @param fy scale factor along Y axis.
	 * @param interpolation type of interpolation to use.
	 * @param dst output image.
	 */
	void GripPipeline::cvResize(cv::Mat &src, cv::Size &dSize, double fx, double fy, int interpolation, cv::Mat &dst) {
		cv::resize(src, dst, dSize, fx, fy, interpolation);
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
	void GripPipeline::hsvThreshold(cv::Mat &input, double hue[], double sat[], double val[], cv::Mat &out) {
		cv::cvtColor(input, out, cv::COLOR_BGR2HSV);
		cv::inRange(out,cv::Scalar(hue[0], sat[0], val[0]), cv::Scalar(hue[1], sat[1], val[1]), out);
	}

	/**
	 * Expands area of lower value in an image.
	 * @param src the Image to erode.
	 * @param kernel the kernel for erosion.
	 * @param anchor the center of the kernel.
	 * @param iterations the number of times to perform the erosion.
	 * @param borderType pixel extrapolation method.
	 * @param borderValue value to be used for a constant border.
	 * @param dst Output Image.
	 */
	void GripPipeline::cvErode(cv::Mat &src, cv::Mat &kernel, cv::Point &anchor, double iterations, int borderType, cv::Scalar &borderValue, cv::Mat &dst) {
		cv::erode(src, dst, kernel, anchor, (int)iterations, borderType, borderValue);
	}

		/**
		 * Filter out an area of an image using a binary mask.
		 *
		 * @param input The image on which the mask filters.
		 * @param mask The binary image that is used to filter.
		 * @param output The image in which to store the output.
		 */
		void GripPipeline::mask(cv::Mat &input, cv::Mat &mask, cv::Mat &output) {
			mask.convertTo(mask, CV_8UC1);
			cv::bitwise_xor(output, output, output);
			input.copyTo(output, mask);
		}

	/**
	 * Detects groups of pixels in an image.
	 *
	 * @param input The image on which to perform the find blobs.
	 * @param minArea The minimum size of a blob that will be found.
	 * @param circularity The minimum and maximum circularity of blobs that will be found.
	 * @param darkBlobs The boolean that determines if light or dark blobs are found.
	 * @param blobList The output where the MatOfKeyPoint is stored.
	 */
	//void findBlobs(Mat *input, double *minArea, double circularity[2],
		//bool *darkBlobs, vector<KeyPoint> *blobList) {
	void GripPipeline::findBlobs(cv::Mat &input, double minArea, double circularity[], bool darkBlobs, std::vector<cv::KeyPoint> &blobList) {
		blobList.clear();
		cv::SimpleBlobDetector::Params params;
		params.filterByColor = 1;
		params.blobColor = (darkBlobs ? 0 : 255);
		params.minThreshold = 10;
		params.maxThreshold = 220;
		params.filterByArea = true;
		params.minArea = minArea;
		params.filterByCircularity = true;
		params.minCircularity = circularity[0];
		params.maxCircularity = circularity[1];
		params.filterByConvexity = false;
		params.filterByInertia = false;
		cv::Ptr<cv::SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
		detector->detect(input, blobList);
	}



} // end grip namespace
