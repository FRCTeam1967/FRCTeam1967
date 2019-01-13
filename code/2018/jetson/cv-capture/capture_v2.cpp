#include <chrono>
#include <iostream>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/photo/photo.hpp>

int main()
{
    cv::VideoCapture cap(1);

    if (!cap.isOpened()) {
        std::cerr << "Failed to open USB camera" << std::endl;
        return -1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    float frames = 0;

    for(;;) {
        cv::Mat gray, frame;
	std::vector<std::vector<cv::Point>> contours;	


        cap >> frame;   // new frame from camera
        // cv::Sobel(frame, dst, CV_8U, 1, 1);
        cv::cvtColor(frame, gray, cv::COLOR_RGB2GRAY);
	cv::Canny(gray, frame, 35, 125);
	cv::findContours(frame, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
      //  cv::fastNlMeansDenoising(dst, frame);
	//auto d = std::max(cv::contourArea(contours));
	
        // cv::imshow("camera feed", dst);
        cv::imshow("camera feed", frame);

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

    auto end = std::chrono::high_resolution_clock::now();
    auto msec_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    float fps = (frames / msec_duration.count()) * 1000;

    std::cout << "msec_duration: " << msec_duration.count() << std::endl;
    std::cout << "frames: " << frames << " FPS: " << fps << std::endl;
}
